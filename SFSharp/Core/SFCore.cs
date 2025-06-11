using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace SFSharp;

public unsafe static partial class SFCore
{
    public static int Init(nint exportsPtr, Action mainMethod)
    {
        var exports = (CSharpExports*)exportsPtr;
        _exports = *exports;
        if(_exports.Version != sizeof(CSharpExports))
        {
            return 1;
        }

        SynchronizationContext.SetSynchronizationContext(new SFSynchronizationContext());
        exports->MainLoop = &MainLoop;

        RegisterDialogCallback(&SFDialog.DialogCallback);
        SF.StartChatLoop();

        try { mainMethod(); } catch(Exception ex) { LogException(ex); }
        return 0;
    }

    [UnmanagedCallersOnly(CallConvs = [typeof(CallConvStdcall)])]
    private static void MainLoop()
    {
        SFSynchronizationContext.LoopProc();
    }

    internal static void LogException(Exception ex)
    {
        LogToChat($"{ex.GetType()}: {ex.Message}");
    }
}