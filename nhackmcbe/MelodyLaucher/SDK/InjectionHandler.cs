using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Security.AccessControl;
using System.Security.Principal;
using System.Text;
using System.Runtime.Remoting.Metadata.W3cXsd2001;
using System;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Net;
using System.Security;
using System.Security.Permissions;
using System.Text;
internal class InjectionHandler
{
    [DllImport("kernel32.dll", SetLastError = true)]
    private static extern IntPtr OpenProcess(ProcessAccessFlags processAccess, bool bInheritHandle, int processId);

    [DllImport("kernel32.dll", CharSet = CharSet.Auto)]
    private static extern IntPtr GetModuleHandle(string lpModuleName);

    [DllImport("kernel32.dll", SetLastError = true)]
    private static extern IntPtr GetProcAddress(IntPtr hModule, string procName);

    [DllImport("kernel32.dll", SetLastError = true)]
    private static extern IntPtr VirtualAllocEx(IntPtr hProcess, IntPtr lpAddress, uint dwSize, AllocationType flAllocationType, MemoryProtection flProtect);

    [DllImport("kernel32.dll", SetLastError = true)]
    private static extern bool WriteProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, byte[] lpBuffer, uint nSize, out IntPtr lpNumberOfBytesWritten);

    [DllImport("kernel32.dll")]
    private static extern IntPtr CreateRemoteThread(IntPtr hProcess, IntPtr lpThreadAttributes, uint dwStackSize, IntPtr lpStartAddress, IntPtr lpParameter, uint dwCreationFlags, IntPtr lpThreadId);

    public static void InjectDLL(string path)
    {
        foreach (Process process in Process.GetProcessesByName("Minecraft.Windows"))
        {
            ApplyAccess(path);

            IntPtr hProcess = OpenProcess(ProcessAccessFlags.All, false, process.Id);
            if (hProcess == IntPtr.Zero)
            {
                throw new ApplicationException("Failed to open process.");
            }

            IntPtr procAddress = GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
            if (procAddress == IntPtr.Zero)
            {
                throw new ApplicationException("Failed to retrieve address of LoadLibraryA.");
            }

            IntPtr intPtr = VirtualAllocEx(hProcess, IntPtr.Zero, (uint)((path.Length + 1) * Marshal.SizeOf(typeof(char))), AllocationType.Commit | AllocationType.Reserve, MemoryProtection.ReadWrite);
            if (intPtr == IntPtr.Zero)
            {
                throw new ApplicationException("Failed to allocate memory in the remote process.");
            }

            IntPtr bytesWritten;
            if (!WriteProcessMemory(hProcess, intPtr, Encoding.Default.GetBytes(path), (uint)((path.Length + 1) * Marshal.SizeOf(typeof(char))), out bytesWritten))
            {
                throw new ApplicationException("Failed to write to remote process memory.");
            }

            IntPtr hThread = CreateRemoteThread(hProcess, IntPtr.Zero, 0U, procAddress, intPtr, 0U, IntPtr.Zero);
            if (hThread == IntPtr.Zero)
            {
                throw new ApplicationException("Failed to create remote thread.");
            }
        }
    }

    private static void ApplyAccess(string path)
    {
        FileInfo fileInfo = new FileInfo(path);
        FileSecurity accessControl = fileInfo.GetAccessControl();
        accessControl.AddAccessRule(new FileSystemAccessRule(new SecurityIdentifier("S-1-15-2-1"), FileSystemRights.FullControl, AccessControlType.Allow));
        fileInfo.SetAccessControl(accessControl);
    }
}

[Flags]
internal enum ProcessAccessFlags : uint
{
    All = 0x001F0FFF,
    Terminate = 0x00000001,
    CreateThread = 0x00000002,
    VirtualMemoryOperation = 0x00000008,
    VirtualMemoryRead = 0x00000010,
    VirtualMemoryWrite = 0x00000020,
    DuplicateHandle = 0x00000040,
    CreateProcess = 0x000000080,
    SetQuota = 0x00000100,
    SetInformation = 0x00000200,
    QueryInformation = 0x00000400,
    QueryLimitedInformation = 0x00001000,
    Synchronize = 0x00100000
}

[Flags]
internal enum AllocationType
{
    Commit = 0x1000,
    Reserve = 0x2000,
    Decommit = 0x4000,
    Release = 0x8000,
    Reset = 0x80000,
    Physical = 0x400000,
    TopDown = 0x100000,
    WriteWatch = 0x200000,
    LargePages = 0x20000000
}

[Flags]
internal enum MemoryProtection
{
    Execute = 0x10,
    ExecuteRead = 0x20,
    ExecuteReadWrite = 0x40,
    ExecuteWriteCopy = 0x80,
    NoAccess = 0x01,
    ReadOnly = 0x02,
    ReadWrite = 0x04,
    WriteCopy = 0x08,
    GuardModifierflag = 0x100,
    NoCacheModifierflag = 0x200,
    WriteCombineModifierflag = 0x400
}