using System.Runtime.CompilerServices;

namespace SFSharp;

public record SFChatEntry(string? Text, uint TextColor, uint Timestamp);

public static partial class SF
{
    private static readonly Stack<SFChatEntry> _entryStack = new();
    private static readonly List<Queue<SFChatEntry>> _queues = new();

    private static unsafe SFChatEntry DecodeEntry(ChatEntry entry)
    {
        return new SFChatEntry(AnsiString.Decode(entry._text), entry._textColor, entry._systemTime);
    }

    internal static async void StartChatLoop()
    {
        var chat = new ChatEntry[100];
        var lastEntry = default(ChatEntry);
        while (true)
        {
            SFCore.GetChat().CopyTo(chat);
            for (int i = 99; i > 0 && !chat[i].Equals(lastEntry); i--)
            {
                var entry = DecodeEntry(chat[i]);
                _entryStack.Push(entry);
            }

            while (_entryStack.Count > 0)
            {
                var entry = _entryStack.Pop();
                SFDebug.Log(entry.Text ?? "<empty>");
                foreach (var queue in _queues)
                {
                    queue.Enqueue(entry);
                }
            }
            lastEntry = chat[99];
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
