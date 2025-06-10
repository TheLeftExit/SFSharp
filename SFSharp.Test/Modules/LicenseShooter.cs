using SFSharp;
using System.Reflection.PortableExecutable;

public class LicenseShooter : ISFSharpModule
{
    public async Task RunAsync(CancellationToken token)
    {
        var path = Path.Combine(SF.SFSharpDirectory, "earnings.txt");
        var earnings = File.Exists(path) ? int.Parse(File.ReadAllText(path)) : 0;
        SF.AddChatMessage("[SFSharp] Starting earnings: " + earnings);

        var lastEarnings = 0;

        await foreach(var entry in SF.StreamChatEntries(token))
        {
            if (entry.Text is not string text) continue;
            text = text.Trim();

            if (entry.TextColor != 0xFF6495ED) continue;

            if (text.EndsWith("вирт"))
            {
                var valueText = text.Split(' ')[^2];
                if (!int.TryParse(valueText, out var value)) continue;
                lastEarnings = value;
                continue;
            }

            if (!text.EndsWith("купил лицензию")) continue;

            
            var buyer = text.Substring(0, text.IndexOf(' '));
            /*
            if (buyer == SF.GetPlayerName(SF.GetLocalPlayerId()))
            {
                SF.AddChatMessage("[SFSharp] License sale to self detected, skipping screenshot.");
                continue;
            }
            */
            var dialogResult = await new SFDialog
            {
                Style = DialogStyle.MsgBox,
                Items = [$"License sale to {buyer} detected. Proceed with recording?"],
                Title = "LicenseShooter",
                AcceptButton = "OK",
                CancelButton = "Cancel"
            }.ShowAsync();
            if (dialogResult is not { Button: DialogButton.Accept }) continue;

            SF.AddChatMessage($"[SFSharp] License sale to {buyer} recorded. Earnings: {earnings} + {lastEarnings} = {earnings + lastEarnings}");
            earnings += lastEarnings;
            File.WriteAllText(path, earnings.ToString());
            await Task.Delay(500);

            SF.AddChatMessage("/time");
            await Task.Delay(500);

            var lastScreenshot = await SF.TakeScreenshotAsync();

            var folder = Path.GetDirectoryName(lastScreenshot)!;
            var fileName = Path.GetFileName(lastScreenshot)!;
            int screenshotNumber = 0;
            while (File.Exists(Path.Combine(folder, $"LicenseSale_{screenshotNumber:D3}.png")))
            {
                screenshotNumber++;
            }
            File.Move(Path.Combine(folder, fileName), Path.Combine(folder, $"LicenseSale_{screenshotNumber:D3}.png"));
        }
    }
}