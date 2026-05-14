#define COMMONOBJ_EXPORTS
#include "CommProc.h"
#include "BaseModel.h"
#include "Defects.h"
#include "Err.h"
#include "Extern_Pnt.h"
#include "ACS_Def.h"
#include "MM.h"
// #include "math.h"
// #include "err.h"
// #include "Names.h"
// #include "stdlib.h"
// #include "stdio.h"
// #include "Param.h"
// //#include "CommProc.h"
// #include "Extern_Pnt.h"
// #include "SetData.h"
//#include "Defects.h"

#define CParams QParams

IBaseModel* g_pAlarmFile = NULL;
IBaseModel* g_pMessageOG = NULL;

IBaseModel * IBaseModel::pFirst = NULL;
IBaseModel * IBaseModel::pLast = NULL;
bool IBaseModel::NotConnect = false;
tSysMsg IBaseModel::pSysMsg = NULL;

IBaseModel ** IBaseModel::AllObjects = NULL;
int IBaseModel::kObjects = 0;

IBaseModel ** IUniModel::UniObjects = NULL;
int IUniModel::kUniObjects = 0;

int ComparePoints( void * TestKey, void * NodeKey, int Type );

ISet* ISet::pFirstGlobalISet;
ISet* ISet::pLastGlobalISet;

ISet::ISet( IBaseModel * _pModel )
{
  pGlobalNextISet = NULL;
  pModel = _pModel;
  if ( pFirstGlobalISet == NULL )
    pFirstGlobalISet = this;
  if ( pLastGlobalISet )
    pLastGlobalISet->pGlobalNextISet = this;
  pLastGlobalISet = this;
  Descr = NULL;
  Objname = NULL;
}

ISet::ISet(  )
{
  pGlobalNextISet = NULL;
  pModel = NULL;
  if ( pFirstGlobalISet == NULL )
    pFirstGlobalISet = this;
  if ( pLastGlobalISet )
    pLastGlobalISet->pGlobalNextISet = this;
  pLastGlobalISet = this;
  Descr = NULL;
  Objname = NULL;
}

IBaseModel::IBaseModel()
{
  ClearAndConnect( );
}

IBaseModel::~IBaseModel()
{
};

void IBaseModel::ClearAndConnect( )
{
  ModelGroup = -1;
  ModelFlags = 0;
  NewData = 0;
  //
  nThread = 0;
  kGroups = 0;
  nGroup = 0;
  Counter = 0;
  RootNode = 0;
  pMain = NULL;
  pFirstChild = NULL;
  pNextChild = NULL;
  pPoolCtrl = NULL;
  pUniModel = NULL;
  pFirstProp = NULL;
  pGetProps = NULL;
  PropsWasRead = false;
  pFirstParams = NULL;
  pLastParams = NULL;
  pFirstDefect = NULL;
  pLastDefect = NULL;
  pFirstISet = NULL;
  pLastISet = NULL;
  pModel_Pnt = NULL;
  TypeObj = None;
  ClassID = 0;
  ID_NAME = 0;
  bp = false;
  PathLabel = 0;
  PathTempLabel = false;
  //  MaxPathTempLabel = 1;
  BreakCount = 0;
  HaveProperties = false;
  HaveExtensions = false;
  //
  if ( NotConnect )
  {
    pNext = NULL;
    pPrev = NULL;
    return;
  }
  pNext = NULL;
  pPrev = pLast;
  IBaseModel * pF = pFirst;
  if ( pFirst == NULL )
    pFirst = this;
  if ( pLast )
    pLast -> pNext = this;
  pLast = this;
}

IBaseModel::IBaseModel( bool )
{
  ModelGroup = 0;
  ModelFlags = 0;
  NewData = 0;
  //
  Counter = 0;
  RootNode = 0;
  pMain = NULL;
  pFirstChild = NULL;
  pNextChild = NULL;
  //	pDbgModel = NULL;
  //	pFirstSubModel = NULL;
  //	pFirstSubGydroModel = NULL;
  TypeObj = None;
  ID_NAME = 0;
  ClassID = 0;
  bp = false;
  BreakCount = 0;
  pPoolCtrl = NULL;
  pUniModel = NULL;
  pModel_Pnt = NULL;
  //
  pNext = NULL;
  pPrev = NULL;
  pFirstProp = NULL;
  pGetProps = NULL;
  PropsWasRead = false;
  pFirstParams = NULL;
  pLastParams = NULL;
  PathLabel = 0;
  PathTempLabel = false;
  HaveProperties = false;
  HaveExtensions = false;
}

IBaseModel::IBaseModel( char * _Name ) :
  Points ( &RootPoint, ComparePoints, NULL, 0 )
{
  ClearAndConnect( );
  TypeObj = MainModel;
  ObjName = _Name;
}

IBaseModel::IBaseModel( char * _Name, IBaseModel * _pMain ) :
  Points ( &RootPoint, ComparePoints, NULL, 0 )
{
  ClearAndConnect( );
  pMain = _pMain;
  if ( pMain )
  {
    pNextChild = pMain->pFirstChild;
    pMain->pFirstChild = this;
    TypeObj = SubModel;
  }
  else
    TypeObj = MainModel;
  ObjName = _Name;
}

void IBaseModel::SetName( char * _Name, IBaseModel * _pMain )
{
  pMain = _pMain;
  TypeObj = SubModel;
  ObjName = _Name;
}

#define MAX_LEVEL 20
void IBaseModel::InitName( )
{
  //	ASS( ObjName != NULL && ObjName[0] != 0 )
  if ( ObjName == NULL || ObjName.Str == NULL || ObjName[0] == 0 )
  {
    ObjName = "XXX";
    //		return;
  }
  if ( pMain )
  {
    IBaseModel * Obj[MAX_LEVEL];
    int kLev = 0;
    Obj[kLev++] = this;
    IBaseModel * pM = pMain;
    int l_Name = strlen ( ObjName );
    while ( pM )
    {
      ASS(kLev < MAX_LEVEL)
      Obj[kLev++] = pM;
      l_Name += strlen ( pM->ObjName ) + 1;
      if ( strchr ( pM->ObjName, '/' ) != NULL )
        break;
      pM = pM -> pMain;
    }
    char TempName[256];
    ASS( l_Name <= 255 );
    strcpy ( TempName, Obj[kLev-1]->ObjName );
    for ( int n = kLev - 2; n >= 0; n-- )
    {
      strcat ( TempName, "/" );
      strcat ( TempName, Obj[n]->ObjName );
    }
    ObjName = TempName;
  }
  ID_NAME = 0;//GetID ( "Model", ObjName );
  //void * pOld = GetNameData ( ID_NAME );
  //ASS( pOld == NULL )
  //Sergej SetNameData ( ID_NAME, this );
}

char * IBaseModel::GetModelName()
{
  if ( Model.Str == 0 )
    return "";
  return (char*)Model.Str;
}

static IBaseModel Key ( false );
static IBaseModel * pKey = &Key;
int CompModels ( const void *p1, const void *p2 )
{
  IBaseModel ** m1 = (IBaseModel **) p1;
  IBaseModel ** m2 = (IBaseModel **) p2;
  return strcmp ( (*m1) ->ObjName, (*m2) ->ObjName );
}

int FindModels ( const void *p1, const void *p2 )
{
  char* m1 = (char*)p1;
  IBaseModel ** m2 = (IBaseModel **) p2;
  int res = strcmp (m1, (*m2)->ObjName);
  return res;
}

IBaseModel * IBaseModel::Find (const char * Name )
{
  if ( !Name )
    return NULL;
  if ( *Name == '#' )
    Name++;
  Key.ObjName = Name;
  IBaseModel ** m = (IBaseModel **)bsearch( Name, AllObjects, kObjects, sizeof ( IBaseModel *), FindModels );
  if ( m )
    return *m;
  return NULL;
}

bool IBaseModel::Find(char * Object, char * Param, char & Type, void ** pAddr, void ** pParam)
{
  IBaseModel * pModel = Find(Object);
  if(pModel == NULL) return false;
  if(Param [0] == '@')
    {
    // Ищем среди точек
    CExtern_Pnt * pPnt = (CExtern_Pnt*)::Find( &pModel->Points, Param + 1, false );
    if(pPnt == NULL ) return false;
    if(pPnt->Type[0] == 'A')
    {
      CAnalog * pA = (CAnalog*)pPnt;
      Type = 'D';
      *pAddr = &pA->Value;
      return true;
    }
    if(pPnt->Type[0] == 'D')
    {
      CDiscrete * pD = (CDiscrete*)pPnt;
      Type = 'I';
      *pAddr = &pD->Value;
      return true;
    }
    return false;
  }
  // Ищем среди параметров
  CLockParams Lock;
  pModel->GetParams( "" );
  char szParam[256];
  strcpy(szParam, Param);
  int n = 0;
  while(n < CParams::kParams)
    {
    char* pChar = strchr(szParam, '\\');
    if( pChar )
      *pChar = '\0';
    for(; n < CParams::kParams; n++)
      {
      CParams & P = CParams::Params[n];
      if(strcmp(P.ParamName, szParam) != 0) continue;
      if(pChar)
        {
        pChar++;
        strcpy(szParam, pChar);
        break;
        }
      if(pParam)
        *pParam = &P;
      Type = P.Type;
      *pAddr = P.Addr;
      return true;
      }
    }
  return false;
  }


int IBaseModel::Init(int)
{
  //	ASS(FALSE);
  return 0;
}

int IBaseModel::Step0()
{
  //	ASS(FALSE);
  return 0;
}

int IBaseModel::Step1()
{
  //	ASS(FALSE);
  return 0;
}
// Вызывается в потоке StepT
int IBaseModel::PreStepT()
{
  return 0;
}

int IBaseModel::StepT(double dt)
{
  //	ASS(FALSE);
  ModelFlags |= NO_STEP_T;//!!
  return 0;
}

int IBaseModel::GetParams( char * StrName )
{
  //
  // IBaseModel * pModel = IBaseModel::pFirst;
  // while ( pModel )
  // {
  //   if ( pModel->pMain != this )
  //   {
  //     pModel = pModel->pNext;
  //     continue;
  //   }
  //   CParams & __P = CParams::Get();
  //   __P.SetDefaultValue(NULL);
  //   __P.Set( pModel, (char*)(pModel->ObjName), 0, 4 );
  //   __P.pDefValue = NULL;
  //   __P.Type = 'R';
  //   pModel = pModel->pNext;
  // }
  // return 0;
}

int IBaseModel::UpdateParam( struct CParams & Param )
{
  return 1;
}

int IBaseModel::SaveState ( )
{
  return 0;
}

int IBaseModel::RestoreState ( char * StrName )
{
  return 1;
}

int IBaseModel::SetData( int TypeData, void * pData )
{
  return 0;
}

int IBaseModel::SetProp ( struct CObjProps * pProp )
{
  return 0;
}

int IBaseModel::GetProp( )
{
  return 0;
}

int IBaseModel::GetConnections ( int & Cnt, struct CConnection Connections[/*100*/])
{
  Cnt = 0;
  return 0;
}

int IBaseModel::OnDefect( CDef * pDefect )
{
  if(pDefect == pRecoveryDef)
  {
    IBaseModel* pChild = pFirstChild;
    while(pChild)
    {
      pChild->OnDefect(pDefect);
      pChild = pChild->pNextChild;
    }
    pDefect->Fire = 0;
    return 1;
  }
  return 0;
}

#ifdef _WIN32
#include "ShowParams.h"
int IBaseModel::ShowParams( DWORD DataType, struct CShowData * pSD )
{
  switch ( DataType )
  {
    case sp_Actions:
      return 1;
  }
  return 0;
}
#endif

void IBaseModel::ShowAddParams(  char * TabName )
{
  if ( pFirstParams == NULL )
    return;
  {
    static int Show = 0;
    static int64_t Level = 5;
    CParams & __P = CParams::Get();
    __P.SetDefaultValue(NULL);
    __P.Set( this, TabName, Show, 4 );
    __P.pDefValue = (void*)Level;
    __P.Type = 'T';
  }
  CParams * pP = pFirstParams;
  while ( pP )
  {
    {
      CParams & __P = CParams::Get();\
        __P = *pP;
    }
    pP = pP->pNextParam;
  }
}

void IBaseModel::ShowAddPoints( char * TabName )
{
  //  IN_DLL CSortTree * GetFirst( CSortTreeInfo * Info );
  //  IN_DLL CSortTree * GetNext( );
  CExtern_Pnt * pPnt = (CExtern_Pnt*)Points.GetFirst(  );
  if ( pPnt == NULL )
    return;
  {
    static int Show = 0;
    static int64_t Level = 5;
    CParams & __P = CParams::Get();
    __P.SetDefaultValue(NULL);
    __P.Set( this, TabName, Show, 4 );
    __P.pDefValue = (void*)Level;
    __P.Type = 'T';
  }
}

void IBaseModel::SetObjectsList( )
{
  //IBaseModel * IBaseModel::AllObjects[MAX_OBJECTS];
  kObjects = 0;
  IBaseModel * pModel = IBaseModel::pFirst;
  while ( pModel )
  {
    kObjects++;
   pModel = pModel ->pNext;
  }
  //AllObjects = new IBaseModel*[kObjects];
  AllObjects = (IBaseModel**)NewMem (sizeof(IBaseModel*) * kObjects);
  pModel = IBaseModel::pFirst;
  int n = 0;
  while ( pModel )
  {
    AllObjects[n++] = pModel;
    pModel->InitName();
    pModel = pModel ->pNext;
  }
  //
  qsort( AllObjects, kObjects, sizeof ( IBaseModel *), CompModels );
  /*
    FILE * F = fopen ( "X:\\Models.txt", "wt" );
    for ( int n = 0; n < kObjects; n++ )
        {
        fprintf ( F, "[%s]\n", AllObjects[n]->ObjName );
        }
    fclose ( F );
*/
}

int ComparePoints( void * TestKey, void * NodeKey, int Type )
{
  char *s1 = (char *)TestKey;
  while (s1 && *s1) {
    if (*s1 == '|') {
      *s1 = 0;
      break;
    }
    s1++;
    if (*s1 == 0) {
      s1 = nullptr;
      break;
    }
  }
  char* s2 = (char*)NodeKey;
  while (s2 && *s2) {
    if (*s2 == '|') {
      *s2 = 0;
      break;
    }
    s2++;
    if (*s2 == 0) {
      s2 = nullptr;
      break;
    }
  }

  int res = strcmp((char*)TestKey, (char*)NodeKey);
  if (s1 != nullptr) {
    *s1 = '|';
  }
  if (s2 != nullptr) {
    *s2 = '|';
  }
  return res;
}

bool IBaseModel::AddParam( struct CParams * pNewParam )
{
  // if ( pFirstParams == NULL )
  //   pFirstParams = pNewParam;
  // if ( pLastParams )
  //   pLastParams->pNextParam = pNewParam;
  // pLastParams = pNewParam;
  // pNewParam->pNextParam = NULL;
  // pNewParam->pModel = this;
  return true;
}

CParams * IBaseModel::GetParam( const char * ParamName )
{
  // CParams * pRes = NULL;
  // CParams::LockParams( );
  // SetData(sd_ParamsWillShow,NULL);
  // GetParams( NULL );
  // //  SetSubModelParmList( this );
  // for ( int n = 0; n < CParams::kParams; n++ )
  // {
  //   CParams & Parm = CParams::Params[n];
  //   const char * No = strchr ( "TER", Parm.Type );
  //   if ( No )
  //     continue;
  //   if ( strcmp ( Parm.ParamName, ParamName ) == 0 )
  //   {
  //     pRes = &Parm;
  //     break;
  //   }
  // }
  // CParams::FreeParams( );
  //return pRes;
  return NULL;
}

bool IBaseModel::GetParamsList( struct CParams ** pParams, int * kParams )
{
  return false;
}

void IBaseModel::SetParamsList( DWORD SizeOfThis, struct CParams ** ppParams, int * kParams )
{
  // if ( *kParams >= 0 )
  //   return;
  // CParams::LockParams( );
  // GetParams( NULL );
  // int K = 0;
  // for ( int n = 0; n < CParams::kParams; n++ )
  // {
  //   CParams & Parm = CParams::Params[n];
  //   if ( Parm.IsWrite())
  //     K++;
  // }
  // if ( K <= 0 )
  // {
  //   *kParams = 0;
  //   *ppParams = NULL;
  //   CParams::FreeParams( );
  //   return;
  // }
  // *kParams = K;
  // CParams * pParams = new CParams[K];
  // *ppParams = pParams;
  // int N = 0;
  // for ( int n = 0; n < CParams::kParams; n++ )
  // {
  //   CParams & Parm = CParams::Params[n];
  //   if ( Parm.IsWrite())
  //   {
  //     pParams[N] = Parm;
  //     BYTE * Addr = (BYTE*)pParams[N].Addr;
  //     BYTE * Min = (BYTE*)this;
  //     BYTE * Max = Min + SizeOfThis;
  //     if ( Addr < Min || Addr > Max )
  //     {
  //       ASS(pParams[N].AbsAddr)
  //     }
  //     if ( !pParams[N].AbsAddr )
  //     {
  //       pParams[N].Addr = (void*)( Addr - Min );
  //     }
  //     N++;
  //   }
  // }
  // CParams::FreeParams( );
  // ASS( N == K )
}

bool IBaseModel::AddDefect( struct CDef * pNewDefect )
{
  if ( pFirstDefect == NULL )
    pFirstDefect = pNewDefect;
  if ( pLastDefect )
    pLastDefect->pNextDefect = pNewDefect;
  pLastDefect = pNewDefect;
  pNewDefect->pNextDefect = NULL;
  return true;
}


bool IBaseModel::AddPoint(struct CExtern_Pnt * pNewItem)
{
  // CSortTreeInfo * Info = pMain ? &pMain->Points : &Points;
  // bool Res = Add( Info, pNewItem->Point, pNewItem);
  // if(!Res) return false;
  return true;
}

// void IBaseModel::PrtNoUsed(void * pData)
// {
//   std::ofstream* hFile = (std::ofstream*)pData;
//   //_HANDLE hFile = *(_HANDLE*)pData;
//   Char<256> Name;
//   int L = Name.Prt("%s;\x0D\x0A", (char*)ObjName);
//   hFile->write( Name, L );
// }

CExtern_Pnt * IBaseModel::FindPoint(const char * PntName)
{
  //CExtern_Pnt * pPnt = (CExtern_Pnt*)::Find(&Points, (char*)PntName, false);
  return NULL;//pPnt;
}

CExtern_Pnt * IBaseModel::GetPoint(const char * PntName)
{
  //CExtern_Pnt * pPnt = (CExtern_Pnt*)::Find(&Points, (char*)PntName, false);
  CExtern_Pnt * pPnt = pModel_Pnt;
  while ( pPnt )
  {
      if ( strcmp ( pPnt->Point.Str, PntName ) == 0 )
          return pPnt;
      pPnt = pPnt->pNext_Model_Pnt;
  }
  return NULL;//pPnt;
}


// Обращение к источнику сигнала
int IBaseModel::SrcMM(char * DataName, struct IMM * pMM)
{
    CExtern_Pnt * pPnt = pModel_Pnt;
    while ( pPnt )
    {
        if ( strcmp ( pPnt->Point.Str, DataName ) == 0 )
        {
            pMM->ID = 1;// ACS !!!
            pMM->pData = pPnt;
            return 0;
        }
        pPnt = pPnt->pNext_Model_Pnt;
    }
  //CExtern_Pnt * pPnt = (CExtern_Pnt*)::Find( &Points, DataName, false );
  //if(pPnt == NULL) return 1;
  //pMM->ID = 1;// ACS !!!
  //pMM->pData = pPnt;
  return 1;
}

int IBaseModel::DstMM(char * DataName, struct IMM * pMM)// Вызов получателя сигнала
{
    CExtern_Pnt * pPnt = pModel_Pnt;
    while ( pPnt )
    {
        if ( strcmp ( pPnt->Point.Str, DataName ) == 0 )
        {
            pMM->ID = 1;// ACS !!!
            pMM->pData = pPnt;
            return 0;
        }
        pPnt = pPnt->pNext_Model_Pnt;
    }
  return 1;
}

#ifdef _WIN32
void IBaseModel::DrawObj(struct CDrawObjData * pDraw)
{
}
#endif

double * IBaseModel::GetValueAddr()
{
  return NULL;
};


IBaseModel * FindBaseModel(char * Name)
{
  // CSortModel * pRes = (CSortModel*)Find( &CSortModel::TreeInfo, Name, false );
  // if ( pRes != NULL )
  //   return (IBaseModel*)(pRes->Key);
  return NULL;
}

bool SaveBaseModel(IBaseModel * pModel)
{
  // CSortModel * pSort = new CSortModel(pModel);
  // bool Res = Add(&CSortModel::TreeInfo, pModel->ObjName, pSort);
  return NULL;//Res;
}

bool IBaseModel::AddSet( struct ISet * pNewSet )
{
  if(pFirstISet == NULL) pFirstISet = pNewSet;
  if(pLastISet) pLastISet->pModelNextISet = pNewSet;
  pLastISet = pNewSet;
  pNewSet->pModelNextISet = NULL;
  pNewSet->pModel->pMain = this;
  return true;
}

