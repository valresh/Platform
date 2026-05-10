#include "CommProc.h"
#include "Defects.h"
#include "BaseModel.h"
#include "Err.h"

CDefHead * pHead = NULL;

const DWORD MAX_DEFECTS = 80000;
const DWORD MAX_PARAMS = 40000;
const DWORD MEMORY_REQUESTED = MAX_DEFECTS * sizeof ( CDef ) + MAX_PARAMS * sizeof ( CDefParam ) + sizeof ( CDefHead );

//??mio::mmap_sink DefectsMap;

BYTE * pDefectsMem = NULL;
static bool CanNew;
bool SimDef = false;
int NumbProject = 0;

CDefHead* GetDefectHead();

bool ValidDefectAddr ( DWORD Addr )
  {
  if ( Addr < pHead->PosDefect )
    return false;
  if ( Addr > pHead->PosDefect + pHead->kDefects * sizeof ( CDef ) )
    return false;
  return true;
  }


BYTE * GetDefectMemPtr()
	{
	return pDefectsMem;
	}

CDefHead * GetDefectHead()
	{
	return (CDefHead*)pDefectsMem;
	}

CDef * GetDefect( DWORD Addr )
	{
    ASS( ValidDefectAddr ( Addr ))
	return (CDef*)( pDefectsMem + Addr );
	}

CDef * GetFirstDefect( )
  {
	return (CDef*)( pDefectsMem + pHead->PosDefect );
  }


DWORD NewDefect( )
	{
	if( pHead == NULL )
	  CreateDefectMem();
	ASS(pHead->kDefects < MAX_DEFECTS)
    if(pHead->kDefects >= MAX_DEFECTS)
      KKK();
    DWORD Addr = pHead->PosDefect + pHead->kDefects * sizeof ( CDef );
	CDef * pDef = (CDef*)( pDefectsMem + Addr );
	pDef -> Init();
    pDef -> NumbProject = NumbProject;
	pHead->kDefects++;
	return Addr;
	}

void PrtDefects( )
  {
  // DWORD Addr = pHead->PosDefect;
  // void * F = Fopen ( "Defects.txt", "wt" );
  // for ( int n = 0; n < pHead->kDefects; n++ )
  //   {
  //   CDef * pDef = (CDef*)( pDefectsMem + Addr );
  //   Fprintf ( F, "%d;%s;\n", n, pDef->Name );
  //   Addr += sizeof ( CDef );
  //   }
  // Fclose(F);
  }

void AddDefectPram( CDef * pDef, CDefParam * pDefParam )
  {
  DWORD Addr = (DWORD)((BYTE*)pDefParam - pDefectsMem );
  if ( pDef ->Param == 0 )
    {
    pDef ->Param = Addr;
    return;
    }
  CDefParam * pParm = (CDefParam*)( pDefectsMem + pDef ->Param );
  while ( 1 )
    {
    if ( pParm->NextParam == 0 )
      {
      pParm->NextParam = Addr;
      break;
      }
    pParm = (CDefParam*)( pDefectsMem + pParm->NextParam );
    }
  }

CDefParam * NewDefectPram( DWORD Defect )
	{
    ASS( ValidDefectAddr ( Defect ))
	ASS(pHead->kParams < MAX_PARAMS)
	DWORD Addr = pHead->PosParam + pHead->kParams * sizeof ( CDefParam );
	CDefParam * pDefParam = (CDefParam*)( pDefectsMem + Addr );
	pDefParam -> Init();
	pHead->kParams++;
//
  CDef * pDef = (CDef*)( pDefectsMem + Defect );
  if ( pDef ->Param == 0 )
    {
    pDef ->Param = Addr;
    return (CDefParam*)( pDefectsMem + Addr );
    }
  CDefParam * pParm = (CDefParam*)( pDefectsMem + pDef ->Param );
  while ( 1 )
    {
    if ( pParm->NextParam == 0 )
      {
      pParm->NextParam = Addr;
      break;
      }
    pParm = (CDefParam*)( pDefectsMem + pParm->NextParam );
    }
  return (CDefParam*)( pDefectsMem + Addr );
	}

CDefParam * NewDefectPram( )
  {
	ASS(pHead->kParams < MAX_PARAMS)
	DWORD Addr = pHead->PosParam + pHead->kParams * sizeof ( CDefParam );
	CDefParam * pDefParam = (CDefParam*)( pDefectsMem + Addr );
	pDefParam -> Init();
	pHead->kParams++;
  return pDefParam;
  }

CDefParam * GetDefectParam( DWORD Addr )
	{
	return (CDefParam*)( pDefectsMem + Addr );
	}

//void * NewDefectMem( DWORD size )
//	{
//	ASS ( CanNew )
//	ASS ( pHead->PosDefect + size < pHead->LenDefect )
//	BYTE * Addr = pDefectsMem + pHead->PosDefect;
//	pHead->PosDefect += size;
//	return Addr;
//	}

DWORD DefectAddr( void * pAddr )
	{
	return (DWORD)((BYTE*)pAddr - pDefectsMem );
	}

/*
static mio::mmap_sink CreateMem(DWORD Size, char* _FileName)
{
	CanNew = true;
	bool NewMem = false;
	char Path[_MAX_PATH];
	lstrcpy(Path, GetMemoryPath());
	//
	char FileName[256]; 
      snprintf(FileName, 256, "%s_%s", _FileName, CURR_PROJECT);
	std::error_code error;
	lstrcat(Path, FileName);
	lstrcat(Path, ".datx");
	mio::mmap_sink m_hMap;
	m_hMap.map(Path, 0, Size, error);
	NewMem = m_hMap.size() < Size;
	if (error)
	{
		NewMem = true;
		m_hMap.map(Path, 0, Size, error, true);
	}
	if (error)
		return std::move(m_hMap);
	BYTE* pVar = (BYTE*)m_hMap.data();
	if (NewMem)
		memset(pVar, 0, Size);
	//
	return std::move(m_hMap);
}

static mio::mmap_sink UseCommonDefects( DWORD Size )
  {
	std::error_code error;
	mio::mmap_sink m_hMap;
	m_hMap.map("Defects", 0, mio::map_entire_file, error);
	return std::move(m_hMap);
  }

static mio::mmap_sink OpenMem(char* _FileName)
{
	CanNew = false;
	char Path[_MAX_PATH];
	lstrcpy(Path, GetMemoryPath());
	//
	char FileName[256];
	// VM: (19.06.2020). Несовпадение имен файлов при создании и открытии отказов
	snprintf(FileName, sizeof(FileName), "%s_%s", _FileName, CURR_PROJECT);
	lstrcat(Path, FileName);
	lstrcat(Path, ".datx");
	std::error_code error;
	mio::mmap_sink m_hMap;
    m_hMap.map(Path, 0, mio::map_entire_file, error);
	if (!m_hMap.is_mapped())
	{
		return std::move(m_hMap);
	}
	return std::move(m_hMap);
}
*/

static DWORD RootDefects;
static DWORD RootResource;

static DWORD RootEvents;
static DWORD RootParams;


DWORD GetRootDefects( )
	{
	if ( pHead == NULL )
		CreateDefectMem();
    return RootDefects;
	}

DWORD GetRootEvents( )
	{
	if ( pHead == NULL )
		CreateDefectMem();
	return RootEvents;
	}

DWORD GetRootResource( )
	{
	if ( pHead == NULL )
		CreateDefectMem();
  return RootResource;
	}

DWORD GetRootParams( )
	{
	if ( pHead == NULL )
		CreateDefectMem();
	return RootParams;
	}

bool FireDefect( CDef * pDef, BYTE Mask )
	{
	if ( SimDef )
		{
		ASS(pDef)
		}
  if ( pDef == NULL )
    return false;
  if ( pDef->Fire & 0x01 )
    return false;
  pHead->kFire++;
  pDef->Fire = 0x01 | Mask;
  return true;
	}

bool FireDefect( int kDefects, CDef * pDef[] )
	{
  for ( int n = 0; n < kDefects; n++ )
    {
		if ( SimDef )
			{
			ASS(pDef[n])
			continue;
			}
    if ( pDef[n]->Fire != 0x01 )
      {
      pDef[n]->Fire = 0x01;
      pHead->kFire++;
      }
    }
  return true;
	}

void DefectsCreated( )
  {
  if( pHead )
    pHead->Flags &= ~CDefHead::Created; 
  }

//  DWORD Addr = pHead->PosDefect + pHead->kDefects * sizeof ( CDef );

bool CreateDefectMem( )
{
    // DefectsMap = CreateMem(MEMORY_REQUESTED, "Defects");
    // if (!DefectsMap.is_mapped())
    // 	return false;
    // pDefectsMem = (BYTE*)DefectsMap.data();
    pDefectsMem = new BYTE[sizeof( CDefHead ) + MAX_DEFECTS * sizeof ( CDef ) + MAX_PARAMS * sizeof ( CDefParam )];
	pHead = GetDefectHead();
	pHead->Flags = CDefHead::Created;
    pHead->Version = 1;
	pHead->PosDefect = sizeof ( CDefHead );
    pHead->PosParam = pHead->PosDefect + MAX_DEFECTS * sizeof ( CDef );
	//	pHead->LenDefect = MEMORY_REQUESTED;
	//	pHead->FirstDefect = 0;
	pHead->kDefects = 0;
	pHead->kParams = 0;
	pHead->kFire = 0;
	pHead->NumbProject = 0;
	//
	RootDefects = NewDefect();
	CDef * pDef = GetDefect ( RootDefects );
	lstrcpy ( pDef ->Name, "Отказы" );
	pDef->Flags = CDef::Defects|CDef::Node|CDef::Active;
	pDef->NumbProject = 0;
	//
	RootResource = NewDefect();
	pDef = GetDefect ( RootResource );
	lstrcpy ( pDef ->Name, "Ресурсы" );
	pDef->Flags = CDef::Resource|CDef::Node|CDef::Active;
	pDef->NumbProject = 0;
	//
	RootEvents = NewDefect();
	pDef = GetDefect ( RootEvents );
	lstrcpy ( pDef ->Name, "События" );
	pDef->Flags = CDef::Events|CDef::Node|CDef::Active;
	pDef->NumbProject = 0;
	//
	RootParams = NewDefect();
	pDef = GetDefect ( RootParams );
	lstrcpy ( pDef ->Name, "Параметры" );
	pDef->Flags = CDef::Params|CDef::Node|CDef::Active;
	pDef->NumbProject = 0;
	return true;
}


DWORD GiveClassRoot( char * Name, DWORD Type )
{
	if ( pHead == NULL )
		CreateDefectMem();
	DWORD Addr = pHead->PosDefect;
	CDef * pDef = (CDef*)( pDefectsMem + Addr );
	int K = pHead->kDefects;
	DWORD Flag = Type|CDef::Node|CDef::Active;;
	for ( int n = 0; n < K; n++ )
	{
		if (( pDef->Flags & Flag ) != Flag )
		{
			pDef++;
			Addr += sizeof ( CDef );
			continue;
		}
		if ( pDef->NumbProject == 0 || pDef->NumbProject == NumbProject )
		{
			if ( lstrcmp ( pDef->Name, Name ) == 0 )
				return Addr;
		}
		pDef++;
		Addr += sizeof ( CDef );
	}
	DWORD ClassRootNode = NewDefect( );
	pDef = GetDefect( ClassRootNode );
	lstrcpy ( pDef->Name, Name );
	pDef->Flags = Type|CDef::Node|CDef::Active;
	pDef->NumbProject = NumbProject;
	switch ( Type )
	{
	case CDef::Defects:
		pDef->Parent = RootDefects;
		break;
	case CDef::Events:
		pDef->Parent = RootEvents;
		break;
	case CDef::Resource:
		pDef->Parent = RootResource;
		break;
	case CDef::Params:
		pDef->Parent = RootParams;
		break;
	}
	return ClassRootNode;
}

CDef * GetDefect( const char * ObjName, const char * DefectName )
  {
	DWORD Addr = pHead->PosDefect;
	CDef * pDef = (CDef*)( pDefectsMem + Addr );
  int K = pHead->kDefects;
  DWORD Flag = CDef::Defect|CDef::Active;;
  for ( int n = 0; n < K; n++ )
    {
    if (( pDef->Flags & Flag ) != Flag )
      {
      pDef++;
      continue;
      }
    if ( lstrcmp ( pDef->Name, DefectName ) == 0 &&
         lstrcmp ( pDef->ObjName, ObjName ) == 0 )
      {
      return pDef;
      }
    pDef++;
    }
  return NULL;
  }

CDef * GiveDefect( const char * ClassName, const char * ObjName, const char * DefectName )
  {
  if ( pHead == NULL )
    return NULL;
	DWORD Addr = pHead->PosDefect;
	CDef * pDef = (CDef*)( pDefectsMem + Addr );
  int K = pHead->kDefects;
  for ( int n = 0; n < K; n++ )
    {
    if ( lstrcmp ( pDef->Name, DefectName ) == 0 &&
         lstrcmp ( pDef->ObjName, ObjName ) == 0 )
      {
      return pDef;
      }
    pDef++;
    }
	pDef = (CDef*)( pDefectsMem + Addr );
  DWORD Node = CDef::Defects|CDef::Node;
  for ( int n = 0; n < K; n++ )
    {
    if ( ( pDef-> Flags & Node ) == Node && lstrcmp ( pDef->Name, ObjName ) == 0 )
      {
      DWORD RootNode = (BYTE*)pDef - pDefectsMem;
// К существующему объекту
 	    DWORD LastDefect = NewDefect( );
	    CDef * DefectVar = GetDefect( LastDefect );
	    DefectVar->Parent = RootNode;
	    lstrcpy ( DefectVar->Name, DefectName );
	    lstrcpy ( DefectVar->ObjName, ObjName );
	    DefectVar->Flags = CDef::Defects|CDef::Defect|CDef::Active;
      DefectVar->ScriptID = 1;
      DefectVar->pModel = NULL;
      CDef * pRoot = GetDefect ( pDef->Parent );
      return DefectVar;
      }
    pDef++;
    }
// Создание нового
	DWORD ClassRootNode = GiveClassRoot( (char*)ClassName );
	DWORD RootNode = NewDefect( );
	CDef * ObjRoot = GetDefect( RootNode );
	ObjRoot->Parent = ClassRootNode;
	lstrcpy ( ObjRoot->Name, ObjName );
  ObjRoot->Show = true;
	ObjRoot->Flags = CDef::Defects|CDef::Node|CDef::Active;
	DWORD LastDefect = NewDefect( );
	CDef * DefectVar = GetDefect( LastDefect );
	DefectVar->Parent = RootNode;
	lstrcpy ( DefectVar->Name, DefectName );
	lstrcpy ( DefectVar->ObjName, ObjName );
	DefectVar->Flags = CDef::Defects|CDef::Defect|CDef::Active;
  DefectVar->ScriptID = 2;
  DefectVar->pModel = NULL;
  return DefectVar;
  }

bool FireDefect( const char * ObjName, const char * DefectName,  CDef * pSrcParams )
  {
  CDef * pDef = GetDefect( ObjName, DefectName );
  if ( pDef == NULL )
		{
		if ( SimDef )
			{
			ASS(pDef)
			}
		return false;
		}
  if ( pSrcParams == NULL)   
    return FireDefect( pDef, 0x04 );

  CDefParam * SrcParm[20];
  int kSrc = 0;
  if ( pSrcParams->Param )
    {
    SrcParm[kSrc] = GetDefectParam( pSrcParams->Param );
    kSrc++;
    while ( kSrc < 20 )
      {
      if ( SrcParm[kSrc-1]->NextParam == 0 )
        break;
      SrcParm[kSrc] = GetDefectParam( SrcParm[kSrc-1]->NextParam );
      kSrc++;
      }
    }
  CDefParam * DstParm[20];
  int kDst = 0;
  if ( pDef->Param )
    {
    DstParm[kDst] = GetDefectParam( pDef->Param );
    kDst++;
    while ( kDst < 20 )
      {
      if ( DstParm[kDst-1]->NextParam == 0 )
        break;
      DstParm[kDst] = GetDefectParam( DstParm[kDst-1]->NextParam );
      kDst++;
      }
    }
  for ( int s = 0; s < kSrc; s++ )
    {
    for ( int d = 0; d < kDst; d++ )
      {
      if ( lstrcmp ( SrcParm[s]->Name, DstParm[d]->Name ) != 0 )
        continue;
      if ( SrcParm[s]->Type != DstParm[d]->Type )
        continue;
      switch ( DstParm[s]->Type )
        {
        case CDefParam::tDbl:
          DstParm[d]->Dbl = SrcParm[d]->Dbl;
        break;
        case CDefParam::tInt:
          DstParm[d]->Int = SrcParm[d]->Int;
        break;
        case CDefParam::tBool:
          DstParm[d]->Bool = SrcParm[d]->Bool;
        break;
        }
      }
    }
  FireDefect( pDef, 0x04 );
  return true;
  }

bool FireDefect( const char * ObjName, const char * DefectName,  CDefParam * pDefParam )
  {
  CDef * pDef = GetDefect( ObjName, DefectName );
  if ( pDef == NULL )
		{
		if ( SimDef )
			{
			ASS(FALSE);
			}
		return false;
		}
  if ( pDefParam == NULL)   
    return FireDefect( pDef, 0x04 );
  if ( pDef->Param )
    {
    CDefParam * pP = GetDefectParam( pDef->Param );
    while ( 1 )
      {
      if ( lstrcmp ( pDefParam->Name, pP->Name ) == 0 &&
           pDefParam->Type == pP->Type )
        {
        switch ( pP->Type )
          {
          case CDefParam::tDbl:
            pP->Dbl = pDefParam->Dbl;
          break;
          case CDefParam::tInt:
            pP->Int = pDefParam->Int;
          break;
          case CDefParam::tBool:
            pP->Bool = pDefParam->Bool;
          break;
          }
        break;
        }
      if ( pP->NextParam == 0 )
        break;
      pP = GetDefectParam( pP->NextParam );
      }
    }
  FireDefect( pDef, 0x04 );
  return true;
  }

bool SetParam ( CDefParam * pParam, double & Value )
  {	  
  if ( pParam->Type == CDefParam::tDbl )
    {
    pParam->Dbl = Value;
	  pParam->Flags |= CDefParam::Change;
    return true;
    }
  if ( pParam->Type == CDefParam::tInt )
    {
    pParam->Int = (int)Value;
		pParam->Flags |= CDefParam::Change;
    return true;
    }
  if ( pParam->Type == CDefParam::tBool )
    {
		pParam->Flags |= CDefParam::Change;
    if ( Value )
      pParam->Bool = true;
    else
      pParam->Bool = false;
    return true;
    }
  return false;
  }

bool SetDefectParam( CDef * pDef, const char * ParamName, double & Value )
  {
  if ( pDef->Param )
    {
    CDefParam * pParam = GetDefectParam( pDef->Param );
    if ( pParam->NextParam == 0 )
      {
// Единственный параметр - имя не проверяем
      if ( SimDef )
				return true;
			return SetParam ( pParam, Value );
      }
    while ( 1 )
      {
      if ( lstrcmp ( pParam->Name, ParamName ) == 0 )
				{
	      if ( SimDef )
					return true;
        return SetParam ( pParam, Value );
				}
      if ( pParam->NextParam == 0 )
        return false;
      pParam = GetDefectParam( pParam->NextParam );
      }
    }
  return false;
  }

bool SetParam ( CDefParam * pParam, int & Value )
  {
  if ( pParam->Type == CDefParam::tDbl )
    {
    pParam->Dbl = Value;
    return true;
    }
  if ( pParam->Type == CDefParam::tInt )
    {
    pParam->Int = Value;
    return true;
    }
  if ( pParam->Type == CDefParam::tBool )
    {
    if ( Value )
      pParam->Bool = true;
    else
      pParam->Bool = false;
    return true;
    }
  return false;
  }

bool SetDefectParam( CDef * pDef, const char * ParamName, int & Value )
  {
  if ( pDef->Param )
    {
    CDefParam * pParam = GetDefectParam( pDef->Param );
    if ( pParam->NextParam == 0 )
      {
// Единственный параметр - имя не проверяем
      return SetParam ( pParam, Value );
      }
    while ( 1 )
      {
      if ( lstrcmp ( pParam->Name, ParamName ) == 0 ) 
        return SetParam ( pParam, Value );
      if ( pParam->NextParam == 0 )
        return false;
      pParam = GetDefectParam( pParam->NextParam );
      }
    }
  return false;
  }

CDefParam * GetDefectParam( CDef * pDef, const char * ParamName )
{
	DWORD ParamID = pDef->Param;
	while ( ParamID > 0 )
	{
		CDefParam * pParam = GetDefectParam( ParamID );
		if ( lstrcmp ( pParam->Name, ParamName ) == 0 ) 
			return pParam;

		ParamID = pParam->NextParam;
    }

	return NULL;
}
// VM: (01.07.2020) Очистка вирт. памяти отказов
/*
void ClearDefects(mio::mmap_sink &hVarMapping)
{
	pDefectsMem = NULL;
	pHead = NULL;
	NumbProject = 0;
	hVarMapping.unmap();
}

bool GetDefectMem(mio::mmap_sink& phVarMapping)
{
	pDefectsMem = NULL;
	auto mm = OpenMem("Defects");
	phVarMapping = std::move(mm);
	if (!phVarMapping.is_mapped())
		return false;
	pDefectsMem = (BYTE*)phVarMapping.data();
	pHead = GetDefectHead();
	return true;
}
*/

CDefHead * GetDefHead()
  {
  return pHead;
  }

void Defect_Params(IBaseModel* pModel)
{
    // if(pModel->pMain)
    // 	return;
    // CDef *pDef = GetDefect(pModel->RootNode);
    // pDef++;
    // DWORD nParent = pDef->Parent;
    // char* szTabDefect = "Отказы";
    // bool bFillParams = !CParams::SaveAllParams && !CParams::ReadProps;
    // static int Show = 0;
    // if (bFillParams)
    // {
    // 	CParams & __P1 = CParams::Get();
    // 	__P1.SetDefaultValue(NULL);
    // 	__P1.Set( pModel, szTabDefect, Show, 4 );
    // 	__P1.Type = 'T';
    // 	__P1.Class = 'X';
    // 	CDef* pDef = pModel->pFirstDefect;
    // 	while(pDef)
    // 	{
    // 		if ( pDef->Flags & CDef::Defect)
    // 		{
    // 			if ( pDef && !CParams::SaveAllParams )
    // 			{
    // 				CParams & P = CParams::Get();
    // 				P.SetDefaultValue(NULL);
    // 				char ParmName[514];
    // 				snprintf ( ParmName, sizeof(ParmName), "#%s", pDef->Name );
    // 				P.SetDefect( pModel, ParmName, pDef );
    // 				P.dwFlags = CParams::FLAG_PARAM;
    // 			}
    // 			DWORD nParam = pDef->Param;
    // 			while(nParam)
    // 			{
    // 				CDefParam* pDefParam = GetDefectParam(nParam);
    // 				CParams & P = CParams::Get();
    // 				P.SetDefaultValue(&pDefParam->__Dbl);
    // 				P.m_bNoSave = true;
    // 				P.Set( pModel, pDefParam->Name, pDefParam->Dbl, sizeof ( pDefParam->Dbl ) );
    // 				P.Class = 'D';
    // 				P.dwFlags = CParams::FLAG_PARAM;
    // 				nParam = pDefParam->NextParam;
    // 			}
    // 		}
    // 		pDef = pDef->pNextDefect;
    // 	}
    // 	CParams & __P2 = CParams::Get();
    // 	__P2.SetDefaultValue(NULL);
    // 	__P2.Set( pModel, szTabDefect, Show, 4 );
    // 	__P2.Type = 'E';
    // 	__P2.Class = 'X';
    // }
}

/**/
//hh:mm:ss
//mm:ss
//ss
// выход - c
#if 0
int ConvTime ( char * Time )
  {
  char T[256];
  lstrcpy ( T, Time );
  int V = 0;
  char * P = T;
  V = atol ( P );
  P = strchr ( P, ':' );
  if ( P == NULL )
    return V;
  V = V * 60 + atol ( P + 1 );
  P = strchr ( P + 1, ':' );
  if ( P == NULL )
    return V;
  V = V * 60 + atol ( P + 1 );
  return V;
  }

int Select ( char * Name, char * List[] )
  {
  int L = 1;
  while ( 1 )
    {
    if ( Name[L] == 0 )
      return -2;// Имя не однозначно
    int k = 0;
    int n = 0;
    int last = -1;
    while ( 1 )
      {
      if ( List[n] == NULL )
        break;
      if ( List[n][0] == 0 )
        break;
      if ( strnicmp ( Name, List[n], L ) == 0 )
        {
        k++;
        last = n;
        }
      n++;
      }
    if ( k == 0 )
      return -1;
    if ( k == 1 )
      return last;
    L++;
    }
  return -2;
  }
#endif

