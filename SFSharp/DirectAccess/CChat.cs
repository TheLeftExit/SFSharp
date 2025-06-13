using System.Diagnostics;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using System.Threading.Tasks.Dataflow;

[UnmanagedFunctionPointer(CallingConvention.FastCall)]
public unsafe delegate void CChatAddEntry(void* thisPtr, int nType, byte* szText, byte* szPrefix, uint textColor, uint prefixColor);

public static unsafe class Hooker
{
    public static void InstallEmptyTrampoline()
    {
        const int overwriteSize = 5;
        var functionPtr = (uint)Win32.GetSampAddress(0x67BE0);
        Win32.VirtualProtect(functionPtr, overwriteSize, PAGE.EXECUTE_READWRITE, out var oldProtect);

        // Initializing the trampoline with the first 5 bytes of the original function
        var trampolinePtr = Win32.VirtualAlloc(0, 10, MEM.COMMIT | MEM.RESERVE, PAGE.EXECUTE_READWRITE);
        for(int i = 0; i < 5; i++) *(byte*)(trampolinePtr + i) = *(byte*)(functionPtr + i);

        // Overwriting the first 5 bytes of the original function with a jump to the trampoline
        *(byte*)(functionPtr) = 0xE9; // JMP instruction
        *(uint*)(functionPtr + 1) = trampolinePtr - (functionPtr + 5);

        // Initializing the second halfof the trampoline to jump back to the original function
        *(byte*)(trampolinePtr + 5) = 0xE9;
        *(uint*)(trampolinePtr + 6) = (functionPtr + 5) - (trampolinePtr + 10);

        Win32.FlushInstructionCache(Process.GetCurrentProcess().Handle, functionPtr, overwriteSize); // Flush the instruction cache to ensure the changes take effect
    }
}

public static unsafe partial class Win32
{
    public static nint GetSampAddress(nint offset) => GetModuleHandle("samp.dll") + offset;

    [LibraryImport("kernel32.dll", EntryPoint = "GetModuleHandleW")]
    internal static partial nint GetModuleHandle([MarshalAs(UnmanagedType.LPWStr)] string lpModuleName);

    [LibraryImport("kernel32.dll")]
    internal static partial uint VirtualAlloc(uint lpAddress, uint dwSize, MEM flAllocationType, PAGE flProtect);

    [LibraryImport("kernel32.dll")]
    [return: MarshalAs(UnmanagedType.Bool)]
    internal static partial bool VirtualProtect(uint lpAddress, uint dwSize, PAGE flNewProtect, out PAGE lpflOldProtect);

    [LibraryImport("kernel32.dll")]
    internal static partial void VirtualFree(uint lpAddress, uint dwSize, MEM dwFreeType);

    [LibraryImport("kernel32.dll")]
    [return: MarshalAs(UnmanagedType.Bool)]
    internal static partial bool FlushInstructionCache(nint hProcess, uint lpBaseAddress, uint dwSize);
}

internal enum MEM : uint
{
    COMMIT = 0x00001000,
    RESERVE = 0x00002000,
    RESET = 0x00080000,
    RESET_UNDO = 0x01000000,

    LARGE_PAGES = 0x20000000,
    PHYSICAL = 0x00400000,
    TOP_DOWN = 0x00100000,
    WRITE_WATCH = 0x00200000,

    DECOMMIT = 0x00004000,
    RELEASE = 0x00008000,

    COALESCE_PLACEHOLDERS = 0x00000001,
    PRESERVE_PLACEHOLDER = 0x00000002
}

internal enum PAGE : uint
{
    EXECUTE = 0x10,
    EXECUTE_READ = 0x20,
    EXECUTE_READWRITE = 0x40,
    EXECUTE_WRITECOPY = 0x80,
    NOACCESS = 0x01,
    READONLY = 0x02,
    READWRITE = 0x04,
    WRITECOPY = 0x08,
    GUARD = 0x100,
    NOCACHE = 0x200,
    WRITECOMBINE = 0x400,
}