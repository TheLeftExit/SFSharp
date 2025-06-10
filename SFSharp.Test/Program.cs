using System.Drawing;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using SFSharp;

public static class Program
{
    [UnmanagedCallersOnly(EntryPoint = "SFSharpMain", CallConvs = [typeof(CallConvStdcall)])]
    public static unsafe int SFSharpMain(CSharpExports* exports) => SFCore.Init(exports, Main);

    public static async void Main()
    {
        SF.AddChatMessage("SFSharp initialized!");

        var container = new SFSharpModuleContainer();
        container.RegisterModule<BrightBinder>();
        container.RegisterModule<LicenseShooter>();
        container.RegisterModule<NodShaker>();
        container.RegisterModule<CountryCapitalHelper>(false);

        await container.RunAllAsync(CancellationToken.None);
    }
}