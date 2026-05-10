using System.Reflection;
using System.Resources;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

[assembly: AssemblyCompany("T-Soft")]
#if DEBUG
[assembly: AssemblyDescription("DCU (Debug)")]
#else
[assembly: AssemblyDescription("DCU (Retail)")]
#endif
[assembly: AssemblyProduct("FlexDCU")]
[assembly: AssemblyCopyright("Copyright © 2025 T-Soft")]
[assembly: AssemblyTrademark("T-Soft 2025")]

#if DEBUG
[assembly: AssemblyConfiguration("Debug")]
#else
[assembly: AssemblyConfiguration("Retail")]
#endif

// Version information for an assembly consists of the following four values:
//
//      Major Version
//      Minor Version 
//      Build Number
//      Revision
//
// You can specify all the values or you can default the Build and Revision Numbers 
// by using the '*' as shown below:
//[assembly: AssemblyVersion("1.1.*")]
[assembly: AssemblyVersion("1.1.143.640")]
[assembly: AssemblyFileVersion("1.1.143.640")]
