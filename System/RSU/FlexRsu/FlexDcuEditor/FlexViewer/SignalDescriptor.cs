using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FlexViewer
{
  public class SSignalMemDescr
  {
    public UInt32 memAddr;
    //public eVarType type;
    public IntPtr ptrToMem;
  }
  public class SSignalDescriptor
  {
    public SSignalMemDescr srcMem;
    public SSignalMemDescr destMem;
  }
}
