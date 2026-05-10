#include "BaseModel.h"
#include "CommProc.h"
#include "Err.h"
#include "WinFiles.h"
#include <QLibrary>
#include <dlfcn.h>

// tCreateObject pCreate[2] = { NULL, NULL };
struct ModelInfo
  {
  CStr ModelName;
  tCreateObject pCreate;
  void * handle;
  ModelInfo()
  {
  pCreate = nullptr;
  handle = nullptr;
  }
  };

#define MAX_MODEL 100
static ModelInfo Modeles[MAX_MODEL]; //[MAX_MODEL];
static int kModeles = 0;

int FindDll( const char * Dll, tCreateObject *pCreate, void ** handle )
{
  for ( int n = 0; n < kModeles; n++ )
    {
    if ( strcmp ( Modeles[n].ModelName, Dll ) == 0 )
      {
      *pCreate = Modeles[n].pCreate;
      *handle = Modeles[n].handle;
      return n;
      }
    }
  return -1;
  }

IBaseModel * Load_Obj ( const char * ObjName, const char * Model )
{
  char * DLL = NULL;
  char * DIR = NULL;
  char * SubType = NULL;
  char ** pRes = NULL;
  char Path[1024];
  if ( kModeles == 0 )
    {
    sprintf ( Path, "%slibTemp.so", EXE_PATH );
    void * handle = dlopen( Path, RTLD_LAZY);
    tCreateObject pCreate = (tCreateObject)dlsym(handle, "CreateObject");
    Modeles[kModeles].ModelName = "Temp";
    Modeles[kModeles].pCreate = pCreate;
    Modeles[kModeles].handle = handle;
    kModeles++;
    }

  if ( lstrcmp ( Model, "Pipe" ) == 0 )
    {
    pRes = pModelDLL->Find( (char*)Model );////MODEL;PROJECT;DLL;DESCR
    if(pRes)
      {
      DIR = pRes[2];
      DLL = pRes[3];
      SubType = pRes[1];
      }
    else
      {
      DIR = "SYSTEM";
      DLL = "Pipe";
      SubType = "";
      }
    //     if ( PathDllCat[0] && ( ObjName[0] == 'c' || ObjName[0] == 'C' ))
    //       {
    //       DIR = PathDllCat;
    //       DLL = "PipeCat";
    //       }
  }
  else
    if ( lstrcmp ( Model, "Signal" ) == 0 )
    {
      DIR = "SYSTEM";
      DLL = "Signal";
      SubType = "";
    }
    else
      {
      pRes = pModelDLL->Find( (char*)Model );////MODEL;PROJECT;DLL;DESCR
      if ( pRes == NULL )
        {
//        SysMSG("Не найдена модель '%s", Model );
        //        return NULL;
        DIR = "SYSTEM";
        if ( strstr ( Model, "Электрооборудование" ) )
          DLL = "Electro";//???
        else
          DLL = (char*)Model;
        SubType = "";
        }
      else
        {
        DIR = pRes[2];
        DLL = pRes[3];
        SubType = pRes[1];
        }
      }
   // if ( !CanLoad ( DLL ))
   //   {
   //   qDebug() << DLL;
   //   DLL = "Temp";
   //   Model = "Temp";
   //   }
  int K = -1;
  tCreateObject pCreate = NULL;
  void * handle = nullptr;
  K =  FindDll( DLL, &pCreate, &handle );
  if ( pCreate == NULL )
    {
    sprintf ( Path, "%slib%s.so", EXE_PATH, DLL );
    void * handle = dlopen( Path, RTLD_NOW);//RTLD_LAZY);
    if ( strstr ( DLL, "CtrlConn"))
      KKK();
    if ( !handle )
      {
       const char * err = dlerror();
      MsgErr( NULL, "DLL '%s' не загрузилась '%s'", DLL, err );
      handle = Modeles[0].handle;
      pCreate = Modeles[0].pCreate;
      Modeles[kModeles].ModelName = DLL;
      Modeles[kModeles].pCreate = pCreate;
      Modeles[kModeles].handle = handle;
      kModeles++;
      DLL = "Temp";
      Model = "Temp";
      }
    else
      {
      pCreate = (tCreateObject)dlsym(handle, "CreateObject");
      if ( pCreate == NULL )
        {
        const char * err = dlerror();
        SysMSG("Ошибочная структура DLL '%s'", DLL );
        return NULL;
        }
      Modeles[kModeles].ModelName = DLL;
      Modeles[kModeles].pCreate = pCreate;
      Modeles[kModeles].handle = handle;
      kModeles++;
      }
    }
  else
  {
    ASS(handle)
    KKK();
   }
  if ( handle == Modeles[0].handle )
      Model = "Temp";
   static IBaseModel * pModel = NULL;
    //if ( pModel == NULL )
    pModel = (*pCreate) ( ObjName, SubType );
    if ( pModel == NULL )
      {
      SysMSG("Ошибка создания объекта '%s'('%s')", ObjName, Model );
      return NULL;
      }
   pModel->Model = Model;
  return pModel;
}

_HANDLE Load_Dll ( const char * DllName, const char * DllPath )
{
  tCreateObject pCreate = NULL;
  void * handle = nullptr;
  for ( int n = 0; n < kModeles; n++ )
  {
    if ( strcmp ( Modeles[n].ModelName, DllName ) == 0 )
    {
      pCreate = Modeles[n].pCreate;
      handle = Modeles[n].handle;
      break;
    }
  }
  char Path[1024];
  if ( pCreate == NULL )
    {
    sprintf ( Path, "%slib%s.so", EXE_PATH, DllName );
    handle = dlopen( Path, RTLD_LAZY);
    if (!handle)
      {
      SysMSG("Ошибка открытия библиотеки '%s'", DllName );
      return NULL;
      }
    //    QLibrary Lib;
    pCreate = (tCreateObject)dlsym(handle, "CreateObject");
    if ( pCreate == NULL )
      {
      SysMSG("Ошибочная структура DLL '%s'", DllName );
      return NULL;
      }
    Modeles[kModeles].ModelName = DllName;
    Modeles[kModeles].pCreate = pCreate;
    Modeles[kModeles].handle = handle;
    kModeles++;
    static int L = 0;
    L++;
    KKK();
    }
  else
    {
    ASS(handle)
    }
    return (QFile*)handle;
  }

void * GetProcAddress( HMODULE hLib, const char * FuncName )
  {
  void * pFunc = dlsym(hLib, FuncName);
    return pFunc;
  }

IBaseModel * Load_Object( const char * DllName, const char * ObjName, const char *DllPath )
{
  return Load_Obj( ObjName, DllName );
}
