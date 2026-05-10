#include "stdafx.h"
#include "FlexRsuDebugBridgeToGUI.h"
#include "memoryDescriptor.h"

#include <msclr\auto_gcroot.h>
#using "FlexViewer.dll"
#using "FlexDesigner.dll"

using namespace System;
using namespace System::Reflection;

namespace ns_local
{
  LPCSTR GetExecutionPath()
  {
    static char EXE_PATH[_MAX_PATH] = { 0 };
    if( !EXE_PATH[0] )
    {
      char szDrv[_MAX_DRIVE],szDir[_MAX_DIR];
      int nLen = ::GetModuleFileName( GetModuleHandle("FlexRsuDebugBridgeToGUI.dll"),EXE_PATH,_countof(EXE_PATH));
      _strupr_s( EXE_PATH );
      _splitpath_s(EXE_PATH,szDrv,_countof(szDrv),szDir,_countof(szDir),NULL,0,NULL,0);
      _makepath_s (EXE_PATH,_countof(EXE_PATH),szDrv,szDir,NULL,NULL);
    }
    return EXE_PATH;
  }
}

static Assembly^ MyResolver(Object^ sender, ResolveEventArgs^ args)
{
  AssemblyName^ askedAssembly = gcnew AssemblyName(args->Name);
  array<System::String^>^ fields = args->Name->Split(',');
  String^ name = fields[0];
  String^ culture = fields[2];

  if (name->EndsWith(".resources") && !culture->EndsWith("neutral"))
    return nullptr;
  String^ finalPath = nullptr;

  finalPath = gcnew String(ns_local::GetExecutionPath()) + name + ".dll";
  Assembly^ retval = Assembly::LoadFrom(finalPath);
  return retval;
}

static bool bAssemblyResolved = false;

extern "C" FLEXRSUDEBUGBRIDGETOGUI_API int OpenDebugGUI(LPCSTR pszXamlFullPath, LPCSTR pszName )
{
  if (!bAssemblyResolved)
  {
    bAssemblyResolved = true;
    AppDomain^ currentDomain = AppDomain::CurrentDomain;
    currentDomain->AssemblyResolve += gcnew ResolveEventHandler(MyResolver);
  }

  System::String^ prov;
  if( pszXamlFullPath )
    prov = gcnew System::String( pszXamlFullPath );
  else
    return -1;

  FlexViewer::MainWindow^ window1 = gcnew FlexViewer::MainWindow;
  window1->ExePath = gcnew String(ns_local::GetExecutionPath());
  window1->Show();
  if( pszName )
    window1->Title = gcnew System::String( pszName );
  window1->OpenWindow( prov );
  return 0;
}

int OpenDebugGuiWithInfoImpl(LPCSTR pszXamlFullPath, LPCSTR pszName, n_GUI::SSignalDescriptor *pSignals, DWORD sigCount )
{
  System::String^ prov;
  if( pszXamlFullPath )
    prov = gcnew System::String( pszXamlFullPath );
  else
    return -1;

  FlexViewer::MainWindow^ window1 = gcnew FlexViewer::MainWindow;
  window1->ExePath = gcnew String(ns_local::GetExecutionPath());

  if( !window1->memories )
    window1->memories = gcnew System::Collections::Generic::List<FlexDesigner::SSignalDescriptor^>();

  for( DWORD n=0; n<sigCount; ++n )
  {
    n_GUI::SSignalDescriptor &src = pSignals[n];

    FlexDesigner::SSignalDescriptor^ sig = gcnew FlexDesigner::SSignalDescriptor();

    sig->src.ptrToMem = System::IntPtr(src.srcMem.memAddr);
    sig->src.objID = System::Guid(src.srcMem.objID.Data1, src.srcMem.objID.Data2, src.srcMem.objID.Data3, src.srcMem.objID.Data4[0], src.srcMem.objID.Data4[1], src.srcMem.objID.Data4[2], src.srcMem.objID.Data4[3], src.srcMem.objID.Data4[4], src.srcMem.objID.Data4[5], src.srcMem.objID.Data4[6], src.srcMem.objID.Data4[7] );
    sig->src.type = (FlexDesigner::eVarType)src.srcMem.type;
    sig->src.szPntName = gcnew System::String(src.srcMem.pszFieldName);

    sig->dst.ptrToMem = System::IntPtr(src.destMem.memAddr);
    sig->dst.objID = System::Guid(src.destMem.objID.Data1, src.destMem.objID.Data2, src.destMem.objID.Data3, src.destMem.objID.Data4[0], src.destMem.objID.Data4[1], src.destMem.objID.Data4[2], src.destMem.objID.Data4[3], src.destMem.objID.Data4[4], src.destMem.objID.Data4[5], src.destMem.objID.Data4[6], src.destMem.objID.Data4[7]);
    sig->dst.type = (FlexDesigner::eVarType)src.destMem.type;
    sig->dst.szPntName = gcnew System::String(src.destMem.pszFieldName);
    
    window1->memories->Add( sig );
  }
  window1->Show();
  if( pszName )
    window1->Title = gcnew System::String( pszName );
  window1->OpenWindow( prov );
  return 0;
}

extern "C" FLEXRSUDEBUGBRIDGETOGUI_API int OpenDebugGuiWithInfo(LPCSTR pszXamlFullPath, LPCSTR pszName, n_GUI::SSignalDescriptor *pSignals, DWORD sigCount )
{
  if (!bAssemblyResolved)
  {
    bAssemblyResolved = true;
    AppDomain^ currentDomain = AppDomain::CurrentDomain;
    currentDomain->AssemblyResolve += gcnew ResolveEventHandler(MyResolver);
  }
  return OpenDebugGuiWithInfoImpl( pszXamlFullPath, pszName, pSignals, sigCount );
}

extern "C" FLEXRSUDEBUGBRIDGETOGUI_API void CloseAllWindows()
{
  FlexViewer::MainWindow::CloseOpenedWindows();
}
