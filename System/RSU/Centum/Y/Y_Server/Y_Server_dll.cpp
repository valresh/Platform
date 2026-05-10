#include "stdafx.h"
#include "Y_Server.h"
#include <rsuNoNames2D.h>
#include <rsuMemoryManager.h>
#include <ConnectY.h>

#ifdef LINUX
#include "LogFile.h"
#endif


#ifdef _MANAGED
#pragma managed(push, off)
#endif

HINSTANCE g_hInst = 0;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
  switch (ul_reason_for_call)
  {
  case DLL_PROCESS_ATTACH:
  {
#ifdef LINUX
	CLogFile::Initialize ("YServerDll");
	CLogFile::Log ("YServerDll started");
#endif // LINUX
    g_hInst = hModule;
  }
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
  case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

static KRsuMemoryManager s_Ymemory;

static void* _YObjsMemAllocator( size_t size )
{
  void* p = s_Ymemory.AllocMemory( (DWORD)size );
  ASS( p );
  return p;
}

KNoName2D *g_yokoObjs, *g_yokoIOs;
static CBase* _ObjsStructAllocatorImpl( LPCSTR ObjName, DWORD TypeID, int number, int domain )
{
  return g_yokoObjs->CreateStruct( ObjName, TypeID, number, domain );
}

static CBase* _IOsStructAllocatorImpl( LPCSTR ObjName, DWORD TypeID, int number, int domain )
{
  return g_yokoIOs->CreateStruct( ObjName, TypeID, number, domain );
}
static int _CreateRefToIoData( LPCSTR ObjName, struct CBase *pBase )
{
  return g_yokoIOs->CreateRefToData( ObjName, pBase );
}

static void _StructsCleaner()
{
  g_yokoObjs->Reset();
  g_yokoIOs->Reset();
}
//////////////////////////////////////////////////////////////////////////
extern "C" __declspec(dllexport) IRsuModel* CreateServer(LPCSTR ObjName )
{
#ifdef LINUX
	CLogFile::Log ("CreateServer_1");
#endif // LINUX

  size_t ms = s_Ymemory.Create( "Y_Mem", 128 );
#ifdef LINUX
  CLogFile::Log ("CreateServer_2: ms %d", (int)ms);
#endif // LINUX

  g_yokoObjs = KNoName2D::Create( "Y_Objs", true, 20000 );
#ifdef LINUX
	CLogFile::Log ("CreateServer_3");
#endif // LINUX

  g_yokoIOs = KNoName2D::Create( "Y_IOs", true, 300000 );
#ifdef LINUX
	CLogFile::Log ("CreateServer_4");
#endif // LINUX

#ifndef _DEBUG
  g_yokoObjs.Reset();
#ifdef LINUX
	CLogFile::Log ("CreateServer_40");
#endif // LINUX

  g_yokoIOs.Reset();
#endif

#ifdef LINUX
	CLogFile::Log ("CreateServer_41");
#endif // LINUX

  KMemoryServiceY::Instance().InitCentumMemAllocator( _YObjsMemAllocator );
#ifdef LINUX
	CLogFile::Log ("CreateServer_5");
#endif // LINUX

  KMemoryServiceY::Instance().InitObjectsStructAllocator( _ObjsStructAllocatorImpl, NULL );
#ifdef LINUX
	CLogFile::Log ("CreateServer_6");
#endif // LINUX

  KMemoryServiceY::Instance().InitIOsStructAllocator( _IOsStructAllocatorImpl );
#ifdef LINUX
	CLogFile::Log ("CreateServer_7");
#endif // LINUX

  KMemoryServiceY::Instance().InitCreateRefToIOsStruct( _CreateRefToIoData );
#ifdef LINUX
	CLogFile::Log ("CreateServer_8");
#endif // LINUX

  KMemoryServiceY::Instance().InitStructCleaner( _StructsCleaner );
  
  IRsuModel *pModel = new KServer( ObjName );
#ifdef LINUX
	CLogFile::Log ("CreateServer_9");
#endif // LINUX

  return pModel;
}
