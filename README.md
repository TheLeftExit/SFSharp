# SFSharp

SFSharp is a C# plugin library and loader for SAMPFUNCS, another library/loader that provides C++ bindings for GTA San Andreas and SAMP.

## Game build prerequisites
- GTA San Andreas (1.0 US)
- [SAMP](https://www.sa-mp.mp/downloads/) 0.3.7-R5
- [CLEO 4](https://github.com/cleolibrary/CLEO4)
- [SAMPFUNCS](https://www.blast.hk/threads/17/) 5.7.1

## Getting started

### For plugin users

1. Download and copy `SFSharpLoader.sf` from the Releases page to the `SAMPFUNCS` folder in your GTA SA installation directory.
2. Create a folder named `SFSharp` in your GTA SA installation directory.
3. Copy `.dll` plugins that you were supplied to the `SFSharp` folder.

### For plugin writers

1. Download and copy `SFSharpLoader.sf` from the Releases page to the `SAMPFUNCS` folder in the game directory.
2. Create a folder named `SFSharp` in the game directory.
3. Create a new .NET 9+ C# class library project with NativeAOT support.
4. Add a reference to the [SFSharp](https://www.nuget.org/packages/SFSharp/0.1.0) NuGet package.
5. Add the following code:
```cs
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using SFSharp;

public static class Program
{
    [UnmanagedCallersOnly(EntryPoint = "SFSharpMain", CallConvs = [typeof(CallConvStdcall)])]
    public static unsafe int SFSharpMain(CSharpExports* exports) => SFCore.Init(exports, Main);

    public static async void Main()
    {
        
    }
}
```
6. Go to town. Run a loop, use the `SFSharp.SF` class to do stuff, and `await Task.Yield()`/`await Task.Delay(...)` to keep the game responsive.
7. When you're done, publish your project as a `win-x86` NativeAOT library, to the `SFSharp` folder in the game directory.

### Rebuilding from source

1. Build `SFSharpLoader` and copy the output to the `SAMPFUNCS` folder in the game directory.
2. Create a new .NET 9+ C# class library project with NativeAOT support **in the same solution**.
3. Add a reference to the `SFSharp` project in the solution.
4. Continue from step 5 in the previous section.