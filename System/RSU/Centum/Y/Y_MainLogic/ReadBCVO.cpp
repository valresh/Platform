#include "Y_MainLogic.h"

struct SFieldNumb
{
  LPCSTR pszF;
  int index;
};
static const SFieldNumb s_Fields[] = 
{ 
  {"MH", 0 },
  {"ML", 1 },
  {"PH", 0 },
  {"PL", 1 },
  {"HH", 0 },
  {"LL", 1 },
};
void SetParamsFromELMD( CY_Base *pB, SELMD *p )
{
  for( int i=0; i<_countof(s_Fields); ++i )
  {
    void *pVar = NULL;
    char varType = ' ';
    if( !pB->GetVar( (char*)s_Fields[i].pszF, &pVar, NULL, &varType, NULL ) )
      break;
    switch( varType )
    {
    case 'D':
      *(double*)pVar = atof( p->szNumbs[s_Fields[i].index] );
      break;
    case 'I':
      *(int*)pVar = atof( p->szNumbs[s_Fields[i].index] );
      break;
    default:
      ASSD(0);
      break;
    }
  }
}

void CY_MainLogic::ReadBCVO( LPCSTR pszDir, LPCSTR pszName )
{
  ASS( CY_MainLogic::kDR<_countof(DR) )
  CY_DR * pDR = new CY_DR;
  CY_MainLogic::DR[CY_MainLogic::kDR++] = pDR;
  strcpy_s( pDR->FCS, pszName );
  pDR->NumbFCS = m_NumbFCS;
  pDR->domain = m_domain;

  char szRdb[_MAX_PATH];
  _makepath_s( szRdb, NULL, pszDir, "ETC\\FcsRef", "rdb");
  std::vector<BYTE> btBuf;
  GetProjectFile( szRdb, btBuf );
  if( btBuf.empty() )
    return;
  BYTE * pData = &btBuf[0];

  SYokogawa200* hdr = (SYokogawa200*)pData;
  SYokogawaID*  pELMD = FindYokoID( hdr, "ELMD" );
  if( !pELMD )
    return;

  char name[24] = {}, number[24] = {};
  pDR->nYObjects = 0;
  for( DWORD i=0; i<pELMD->dwCount; ++i )
  {
    SELMD *p = (SELMD*)pELMD->Item( pData, i );
    if( !p->tagName[0] )
      continue;
    switch( p->dwType )
    {
    case id_BOPCSYS:
      continue;
    case id_BOPCCHR:
    case id_BOPCBOL:
    case id_BOPCI16:
    case id_BOPCU16:
    case id_BOPCI32:
    case id_BOPCU32:
    case id_BOPCF32:
    case id_BOPCF64:
    case id_SR_PID:
      ++pDR->nYObjects;
      break;
    case id_SR_FNCVR:
      continue;
    }
  }

  int n = 1;
  for( DWORD i=0; i<pELMD->dwCount; ++i )
  {
    SELMD *p = (SELMD*)pELMD->Item( pData, i );
    if( !p->tagName[0] )
      continue;
    MoveMemory( name, p->tagName, _countof(p->tagName) );
    name[_countof(p->tagName)] = '\0';
    MoveMemory( number, p->szType_Number, _countof(p->szType_Number) );
    number[_countof(p->szType_Number)] = '\0';

    CY_Base *pB = NULL;

    switch( p->dwType )
    {
    case id__SW:
      AddSystemSW( m_NumbFCS, m_domain, atoi( number+3 ), p->comment );
      continue;
    case id_BOPCSYS:
      continue;
    case id_BOPCCHR:
    case id_BOPCBOL:
    case id_BOPCI16:
    case id_BOPCU16:
    case id_BOPCI32:
    case id_BOPCU32:
    case id_BOPCF32:
    case id_BOPCF64:
    case id_SR_PID:
      {
        pB = CY_Create::Create( p->blkType, name, pDR->NumbFCS, pDR->domain );
        if( pB == NULL )
        {
          OutputDebugString( "!!! нет модели ");
          OutputDebugString( p->blkType );
          OutputDebugString( ", блока \"");
          OutputDebugString( name );
          OutputDebugString( "\"\n");
          continue;
        }
        pB->sequenceNumberInDR = n++;
        pB->SetParmList();
        strcpy_s( pB->szNumber, number );
        pB->pDR = pDR;
        pDR->AddYBlk( pB );
        CY_MainLogic::AddYObj ( pB );
        strcpy_s( pB->Model, p->blkType );
        pB->SetObjName();
        SetParamsFromELMD( pB, p );
      }
      break;
    case id_SR_FNCVR:
      continue;
    default:
      ASSD(0);
      continue;
    }
  }
}