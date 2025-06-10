using System.Diagnostics.CodeAnalysis;
using System.Net.Sockets;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace SFSharp;

public record DialogResult(DialogButton Button, int SelectedItemIndex, string? InputText);

public enum DialogButton
{
    Cancel,
    Accept
};

public class SFDialog
{
    public required DialogStyle Style { get; set; }
    public required string Title { get; set; }
    public required string[] Items { get; set; }
    public string? Header { get; set; }
    public string AcceptButton { get; set; } = "OK";
    public string CancelButton { get; set; } = "";

    private const int _dialogId = 0x0083;
    private static uint _dialogCount = 0;
    private static DialogResult? _lastResult;

    public async Task<DialogResult?> ShowAsync()
    {
        var dialogNumber = ++_dialogCount;
        Show();
        while (_lastResult is null && SFCore.IsDialogOpen(_dialogId) && _dialogCount == dialogNumber) await Task.Yield();
        var result = _lastResult;
        _lastResult = null;
        return result;
    }

    public void Show()
    {
        var content = (Header is null || Style != DialogStyle.TabListHeaders) ? string.Join("\r\n", Items) : $"{Header}\r\n{string.Join("\r\n", Items)}";
        SFCore.ShowDialog(_dialogId, Style, Title, content, AcceptButton, CancelButton);
    }

    [UnmanagedCallersOnly(CallConvs = [typeof(CallConvStdcall)])]
    internal static unsafe void DialogCallback(int dialogId, int buttonId, int listItem, byte* input)
    {
        if (dialogId != _dialogId) return;
        _lastResult = new DialogResult((DialogButton)buttonId, listItem, AnsiString.Decode(input));
    }
}