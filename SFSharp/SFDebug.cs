namespace SFSharp;

public static class SFDebug
{
    private static Queue<string> _messages = new();
    public static void Log(string message)
    {
        _messages.Enqueue($"{{AAAAAA}}[{TimeOnly.FromDateTime(DateTime.Now):T}] {{FFFFFF}}{message}");
        if (_messages.Count > 50) _messages.Dequeue();
    }

    public static void ShowDialog()
    {
        new SFDialog
        {
            Style = DialogStyle.MsgBox,
            Title = "SFSharp Debug Log",
            Items = _messages.ToArray(),
        }.Show();
    }
}