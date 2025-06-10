using System.Runtime.CompilerServices;

namespace SFSharp;

public record SFChatEntry(string? Text, uint TextColor, uint Timestamp);

public static partial class SF
{
    private static uint _lastTimestamp = 0;
    private static readonly Stack<SFChatEntry> _entryStack = new();
    private static readonly List<Queue<SFChatEntry>> _queues = new();

    private static unsafe SFChatEntry DecodeEntry(ChatEntry entry)
    {
        return new SFChatEntry(AnsiString.Decode(entry._text), entry._textColor, entry._systemTime);
    }

    internal static async void StartLoop()
    {
        while (true)
        {
            var chat = SFCore.GetChat();
            var newLastTimestamp = chat[99]._systemTime;
            for (int i = 99; i > 0 && chat[i]._systemTime != _lastTimestamp; i--)
            {
                var entry = DecodeEntry(chat[i]);
                _entryStack.Push(entry);
            }

            while (_entryStack.Count > 0)
            {
                var entry = _entryStack.Pop();
                foreach (var queue in _queues)
                {
                    queue.Enqueue(entry);
                }
            }
            _lastTimestamp = newLastTimestamp;
            await Task.Yield();
        }
    }

    public static async IAsyncEnumerable<SFChatEntry> StreamChatEntries([EnumeratorCancellation] CancellationToken token = default)
    {
        var queue = new Queue<SFChatEntry>();
        _queues.Add(queue);
        try
        {
            while (true)
            {
                token.ThrowIfCancellationRequested();
                while (queue.TryDequeue(out var entry))
                {
                    yield return entry;
                }
                await Task.Yield();
            }
        }
        finally
        {
            _queues.Remove(queue);
        }
    }
}