using System;
using System.Runtime.InteropServices;

namespace FlexDesigner
{
  public enum eVarType
  {
    evtHZ = 0,
    evtDigital32 = 1,
    evtAnalog32 = 2,
    evtChars = 3,
    evtDigital64 = 4,
    evt_LastSimple,
  }

  // [StructLayout(LayoutKind.Sequential)]
  public struct SSignalMemDescr
  {
    public eVarType type;
    public IntPtr ptrToMem;
    public Guid objID;
    //[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 16)]
    public String szPntName;
  }
  public class SSignalDescriptor
  {
    public SSignalMemDescr src;
    public SSignalMemDescr dst;
  }
}
