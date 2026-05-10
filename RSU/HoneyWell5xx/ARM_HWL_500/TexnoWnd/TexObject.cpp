#include "stdafx.h"
#include "../ARM.h"
#include "TexnoFrm.h"
#include "BaseType.h"
#include "Queue.h"
#include "Resource.h"
//
#include "BaseWnd.h"
#include "RegWnd.h"

#include "LogFile.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Поиск уже существующего окна
CBaseWnd* CTexChild::FindItem(EDataTypes eType, const char* pszName,CWnd* pWnd)
  {
  //
  pWnd = pWnd->GetWindow( GW_HWNDFIRST );
  while ( pWnd )
    {
    CRuntimeClass* cls = pWnd->GetRuntimeClass();
	  if ( cls != NULL )
      {
      #ifdef _AFXDLL
      CRuntimeClass* bas = cls->m_pfnGetBaseClass();
      #else
      CRuntimeClass* bas = cls->m_pBaseClass;
      #endif
      if ( bas != NULL )
      if ( strcmp( bas->m_lpszClassName, "CBaseWnd" ) == 0 )
        {
        CBaseWnd* pBas = (CBaseWnd*)pWnd;
        if ( pBas->m_eType == eType )
        if ( pBas->m_strName == pszName )
          return pBas;
        }
      }
    pWnd = pWnd->GetNextWindow();
    }
  return 0;
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Поиск уже существующего окна с проходом по дереву
CBaseWnd* CTexChild::FindItem(EDataTypes eType, const char* pszName, bool bAviFors)
  {
  char szName[256];
  lstrcpy( szName, pszName );
  if ( bAviFors ) lstrcat( szName, " " );
  //
  CWnd* pWnd = this;
  while ( pWnd )
    {
    CBaseWnd* pFind = FindItem( eType, szName, pWnd );
    if ( pFind != NULL )
      return pFind;
    pWnd = pWnd->GetParent();
    }
  return NULL;
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Создание окна с объектом
void CTexChild::CreateObject( EDataTypes eType, UINT eSxema, const char* pszName, const char* pszComm, bool bAviFors )
  {
  BOOL bChild = ::IsWindow(m_hParentUSB);
  //
  if ( !bChild )
    {
    // Ищем среди существующих
    CBaseWnd* pFind = FindItem( eType, pszName,bAviFors );
    if ( pFind != NULL )
      {
      pFind->SetForegroundWindow();
	  pFind->ShowWindow(SW_SHOW);
      return;
      }
    }
  //
  CBaseWnd* obj = BaseWnd( eType, eSxema, pszName, pszComm );
  if ( obj == NULL ) return;
  //
  if ( bChild && ( !(eType == id_Sensor || eType == id_GasAnalyz) ) )
    {
    HWND hOld = ::GetWindow( m_hParentUSB, GW_CHILD );
    if ( !obj->MyCreate( m_hParentUSB ) )
      delete obj;
    else
      ::SendMessage( m_hParentUSB, nPressQueue, QUEUE_CHILD_NEW, (LPARAM) hOld );
    }
  else
    {
    if ( !obj->MyCreate( this, bAviFors ) )
      delete obj;
    }
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CBaseWnd* CTexChild::BaseWnd( EDataTypes eType, UINT eSxema, const char* pszName, const char* pszComm )
  {
  CBaseWnd* obj = NULL;
  //
  switch ( eType )
    {
    case id_Zadv      : obj = new CZadvWnd (eSxema); break;
    case id_KVO       : obj = new CKvoWnd  (eSxema); break;
	case id_PHS       : obj = new CPolyHSWnd  (eSxema); break;
    case id_Pump      : obj = new CPumpWnd (eSxema); break;
    case id_Forsunka  : obj = new CForsWnd (eSxema); break;
    case id_Sensor    : obj = new CSensWnd (eSxema); break;
    case id_GasAnalyz : obj = new CGasWnd  (eSxema); break;
    case id_Klapan    : obj = new CKlapWnd (eSxema); break;
    case id_HS        : obj = new CHsWnd   (eSxema); break;
    case id_Value     : obj = new CValueWnd(eSxema); break;
	case id_ModelData : obj = new CModelWnd   (eSxema); break;
//  case id_Reg       : obj = new CRegWnd (eSxema); break;
    default:
    if ( ::IsMnemoStyle(MNEMO_EDIT) )
      {
      CString strMess;
      strMess.Format( "%s\n%s\nНеподдерживаемый тип '%d'", pszName, pszComm, eType );
      AfxMessageBox(strMess);
      }
    };
  //
  if ( obj != NULL )
    obj->m_strName = pszName;
  return obj;
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
