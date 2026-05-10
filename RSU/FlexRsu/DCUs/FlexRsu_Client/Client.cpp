#include "Client.h"
#include <SetDataTypes.h>
#include <Connect.h>
#include <RsuX.h>
#include <rsuNoNames.h>
#include <DataTypes_.h>
#include "CBsPool.h"
#include <FlexRsuCommon.h>
#include <SysDataTypes.h>
#include <rsuProjectVersion.h>
#include <rsuStr.h>
#include "SharedFlexBlocks.h"

static bool bRegisteredFind = false;
#ifdef _WIN32
bool GetFirstACSObject( );
bool GetNextACSObject( DWORD & Type, void ** pData, char Name[256], char Info[256] );
bool ShowACSObject( void * pInfo, HWND hMainWnd, char * Name, void * pData );
#endif

LPCSTR g_szPntsFilePrefix = "РСУ_FLEXRSU_";

nRSUx::SParamInfo RsuConnectParamInfo( LPCSTR pszPointName, LPSTR pszIOtype, LPCSTR pSuffix );

KNoName* g_SharedMemory;

extern KCBsPool g_Pool;

KClient::KClient( LPSTR _ObjName, rsu_cp::RsuClient* a_cl)
: m_nAcsFindStep (0), //IBaseModel ( _ObjName ), 
    m_proxy(a_cl), m_ObjName (_ObjName)
{
}

void KClient::Finalize()
{
    
}

LPCSTR KClient::ObjName()
{
    return m_ObjName.c_str();
}

LPCSTR KClient::GetName()
{
  return ObjName();
}

int KClient::Init( int )
{
  LPCSTR pszRealName = ObjName();
  size_t prefixLen = strlen(g_szPntsFilePrefix);
  if( !_Kstrnicmp(pszRealName, g_szPntsFilePrefix, prefixLen) )
    pszRealName += prefixLen;

  strcpy_s ( m_szObjName, pszRealName );
  int r = m_proxy.Init(0);
  if( r )
    return 1;

  char szName[64 * 4];
  ns_FlexRsu::GetSharedName( szName );
  g_SharedMemory = KNoName::Create( szName, false );
  KNoName::RegisterClassInNames( SFlex::TypeID, "FlexBlock");
  KNoName::RegisterClassInNames( CFlexSystem::TypeID, "FlexSystem");
  KNoName::RegisterClassInNames( SFlexFutureTrend::TypeID, "FlexFutureTrend");

#ifdef _WIN32
  if( pRegisterFinderACSobj && !bRegisteredFind )
  {
    bRegisteredFind = true;
    pRegisterFinderACSobj( ::GetFirstACSObject, ::GetNextACSObject, ::ShowACSObject);
  }
#endif
  if( pRegisterRsuConnection )
    pRegisterRsuConnection( g_szPntsFilePrefix, RsuConnectParamInfo, NULL );

  g_Pool.RegisterCB( this );

  return 0;
}

template <typename SimObj>
void KClient::SetExchangeParams(SExchange* pRec, SimObj* pObj, char rsuType, bool b2Model) 
{
  if (!pRec || !pObj)
    return;
  pRec->rsuType = rsuType;
  pRec->b2Model = b2Model;
  pRec->pRsuAddr = &pObj->PV;
}


template <typename  SourceType, typename  DestinationType>
void KClient::AssignPointer(void* pSource, void* pDestination)
{
  SourceType sourceValue = *static_cast<SourceType*>(pSource);
  DestinationType destValueTmp = *static_cast<DestinationType*>(pDestination);
  *static_cast<DestinationType*>(pDestination) = static_cast<DestinationType>(sourceValue);
  DestinationType dValueNew = *static_cast<DestinationType*>(pDestination);
  if (destValueTmp != dValueNew)
    KKK();
}


void KClient::TransferData()
{
  for( size_t i=0, E(m_RsuModelExchanges.size()); i<E; ++i )
  {
    SExchange* pExchange = &m_RsuModelExchanges[i];

    // by default:  rsu -> model
    void* pSource = pExchange->pRsuAddr;
    void* pDestination = pExchange->pModelAddr;
    char sourceType = pExchange->rsuType;
    char destinationType = pExchange->modelType;
    // if !2Model:  rsu <- model
    if (!pExchange->b2Model)
    {
      std::swap(pSource, pDestination);
      std::swap(sourceType, destinationType);
    }

    switch ( sourceType )
    {
      case 'L':
      case 'I':
        if (destinationType == 'I' || destinationType == 'L')
          AssignPointer<int, int>(pSource, pDestination);
        else if (destinationType == 'D')
          AssignPointer<int, double>(pSource, pDestination);
        else if (destinationType == 'B')
          AssignPointer<int, BYTE>(pSource, pDestination);
        else if (destinationType == 'F')
          AssignPointer<int, float>(pSource, pDestination);
        break;
      case 'D':
        if (destinationType == 'I' || destinationType == 'L')
          AssignPointer<double, int>(pSource, pDestination);
        else if (destinationType == 'D')
          AssignPointer<double, double>(pSource, pDestination);
        else if (destinationType == 'B')
          AssignPointer<double, BYTE>(pSource, pDestination);
        else if (destinationType == 'F')
          AssignPointer<double, float>(pSource, pDestination);
        break;
      case 'B':
        if (destinationType == 'I' || destinationType == 'L')
          AssignPointer<BYTE, int>(pSource, pDestination);
        else if (destinationType == 'D')
          AssignPointer<BYTE, double>(pSource, pDestination);
        else if (destinationType == 'B')
          AssignPointer<BYTE, BYTE>(pSource, pDestination);
        else if (destinationType == 'F')
          AssignPointer<BYTE, float>(pSource, pDestination);
        break;
      case 'F':
        if (destinationType == 'I' || destinationType == 'L')
          AssignPointer<float, int>(pSource, pDestination);
        else if (destinationType == 'D')
          AssignPointer<float, double>(pSource, pDestination);
        else if (destinationType == 'B')
          AssignPointer<float, BYTE>(pSource, pDestination);
        else if (destinationType == 'F')
          AssignPointer<float, float>(pSource, pDestination);
        break;
      default:
        ASSD(0);
    }
  }
}

int KClient::Step0()
{
  CBase *pBase = NULL;
  LPCSTR pszName = NULL;
  int nFcsNumb = 0;
  DWORD ID = 0;
  while( g_SharedMemory->WhileBase( ID, -1, &pBase, &pszName, &nFcsNumb ) )
  {
    if( SFlex::TypeID==pBase->ID_CLASS )
    {
      SFlex* pB = (SFlex*)pBase;
      switch( pB->TypeFlex )
      {
      case S_SimAI::TypeID:
        {
          S_SimAI *pObj = (S_SimAI*)pB->ObjectMem;
          SExchange* pRec = AddLink( pObj->Model, pObj->Path );
          SetExchangeParams(pRec, pObj, 'F', false);
          break;
        }
      case S_SimAO::TypeID:
        {
          S_SimAO *pObj = (S_SimAO*)pB->ObjectMem;
          SExchange* pRec = AddLink( pObj->Model, pObj->Path );
          SetExchangeParams(pRec, pObj, 'F', true);
          break;
        }
      case S_SimDI::TypeID:
        {
          S_SimDI *pObj = (S_SimDI*)pB->ObjectMem;
          SExchange* pRec = AddLink( pObj->Model, pObj->Path );
          SetExchangeParams(pRec, pObj, 'I', false);
          break;
        }
      case S_SimDO::TypeID:
        {
          S_SimDO *pObj = (S_SimDO*)pB->ObjectMem;
          SExchange* pRec = AddLink( pObj->Model, pObj->Path );
          SetExchangeParams(pRec, pObj, 'I', true);
          break;
        }
      default:
        continue;
      }
    }
  }

    return m_proxy.Step0();
}

KClient::SExchange* KClient::AddLink( LPSTR pszModel, LPCSTR pszPath )
{
  SExchange* pLast = NULL;
  IBaseModel *pModel = IBaseModel::Find( pszModel );
  if( !pModel )
  {
    OutputDebugString( "нет модели: "); OutputDebugString( pszModel ); OutputDebugString("\n");
    return NULL;
  }
  CParams::LockParams();
  pModel->GetParams(NULL);
  int paramsFilled = CParams::kParams;
  bool isPathFound = false;
  for (int i = 0; i < paramsFilled; ++i) {
    CParams &param = CParams::Params[i];
    if( !strcmp(pszPath, param.ParamName) )
    {
      SExchange rec;
      rec.modelType = param.Type;
      rec.pModelAddr = param.Addr;
      m_RsuModelExchanges.push_back( rec );
      pLast = &m_RsuModelExchanges.back();
      break;
    }
  }
  CParams::FreeParams();
  return pLast;
}


int KClient::Step1()
{
    return m_proxy.Step1();
}

int KClient::StepT(double dt)
{
  //send data from sim to model
  TransferData();
  double dtMs = dt * 3600. * 1000;
    return m_proxy.StepT(dt);
}

int KClient::SetData( int TypeData, const char* pData )
{
    return m_proxy.SetDataCmd(TypeData, pData);  
}
