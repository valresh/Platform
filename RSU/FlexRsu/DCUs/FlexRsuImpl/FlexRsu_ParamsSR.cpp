#include <crossplatform.h>
#include <rsus/rsuPaths.h>
#include "FlexRsu.h"
#include <macros/AutoRestore.h>
#include <FlexRsuCommon.h>
#include <float.h>

using namespace tinyxml2;
using namespace TINYXMLHELPER;

class KPrjReplacer : protected KTinyXmlReader<KPrjReplacer>
{
  GUID activeITEM;
  KFlexDR *m_pDR;
public:
  KPrjReplacer( XMLElementPtr root, KFlexDR *pDR ) : m_pDR( pDR )
  {
    memset( &activeITEM, 0, sizeof(activeITEM) );
    tHandlersCont_ cont;
    cont["Items"] = &KPrjReplacer::OnItems;
    KTinyXmlReader<KPrjReplacer>::Read( root, cont, false );
  }
protected:
  void OnItems( tinyxml2::XMLElementPtr &root )
  {
    tHandlersCont_ cont;
    cont["Item"] = &KPrjReplacer::OnItem;
    KTinyXmlReader<KPrjReplacer>::Read( root, cont );
  }
  void OnItem( tinyxml2::XMLElementPtr &root )
  {
    TAutoRestore<GUID> arID( activeITEM );
    XMLElementPtr findItem = root->FirstChildElement("ID");
    ASSD( findItem );
    if( !findItem )
      return;
    GetXMLData( findItem, activeITEM );
    findItem = root->FirstChildElement("HiddenProperties");
    if( !findItem )
      return;
    KFlexBlockBase *pBlock = m_pDR->GetBlock( activeITEM );
    if( !pBlock )
      return;
    for( tinyxml2::XMLNode *node = findItem->FirstChild(); node != NULL; node = node->NextSibling())
    {
      tinyxml2::XMLElementPtr item = node->ToElement();
      LPCSTR pszName = item->Name();
      LPCSTR pszCurValue = item->GetText();
      //if( !pszCurValue )continue;
      eVarType type = evtHZ;
      BYTE *pMem = pBlock->GetField( pszName, &type );
      ASSD( pMem );
      if( !pMem )
        continue;
      char szNewVal[512] = { 0 };
      if( !pszCurValue )
        pszCurValue = szNewVal;
      switch( type )
      {
      case evtPrivConstString:
        {
          LPCSTR pszT = *(tPrivConstDynString*)pMem;
          if( strcmp(pszT, pszCurValue) )
            strcpy_s( szNewVal, pszT );
        }
        break;
      case evtChars:
        {
          LPCSTR pszT = (LPCSTR)pMem;
          if( strcmp(pszT, pszCurValue) )
            strcpy_s( szNewVal, pszT );
        }
        break;
      case evtAnalog32:
        {
          tAnalog32 val = *(tAnalog32*)pMem;
          if( FLT_MAX==val || (-FLT_MAX)==val )
            continue;
          tAnalog32 curVal = (tAnalog32)_atof( pszCurValue );
          if( curVal==val )
          {
            strcpy_s( szNewVal, pszCurValue );
            break;
          }
          sprintf_s( szNewVal, "%f", val );
          size_t l = strlen(szNewVal);
          if( l )
          {
            for( LPSTR pC = &szNewVal[l-1]; pC>=szNewVal; --pC )
            {
              bool bA = true;
              switch( *pC )
              {
              case '0':
                bA = false;
              case '.':
              case ',':
                *pC = 0;
                break;
              }
              if( bA )
                break;
            }
          }
        }
        break;
      case evtAnalog64:
        {
          tAnalog64 val = *(tAnalog64*)pMem;
          tAnalog64 curVal = _atof( pszCurValue );
          if( curVal==val )
          {
            strcpy_s( szNewVal, pszCurValue );
            break;
          }
          sprintf_s( szNewVal, "%f", val );
          size_t l = strlen(szNewVal);
          if( l )
          {
            for( LPSTR pC = &szNewVal[l-1]; pC>=szNewVal; --pC )
            {
              bool bA = true;
              switch( *pC )
              {
              case '0':
                bA = false;
              case '.':
              case ',':
                *pC = 0;
                break;
              }
              if( bA )
                break;
            }
          }
        }
        break;
      case evtDigital32:
        sprintf( szNewVal, "%d", *(tDigital32*)pMem );
        break;
      }
      if( !szNewVal[0] )
        continue;
      item->SetText( szNewVal );
    }
  }
};

int KFlexRsu::SaveParamToDRs()
{
  for( DWORD n=0; n<m_nDR; ++n )
  {
    std::string v_ext = std::string(".") + ns_FlexRsu::GetLogicFileExtension();
    std::string v_ffn = rsu_cp::makepath_with_ext_cp( m_szDirPrj, m_ppDR[n]->m_szFileName, v_ext).generic_string();
    tinyxml2::XMLDocument doc;
    XMLError err = doc.LoadFile( v_ffn.c_str() );
    if( XML_SUCCESS!=err )
      continue;

    KPrjReplacer repl( doc.RootElement(), m_ppDR[n] );
    err = doc.SaveFile(v_ffn.c_str(), false, true );
  }
  return 0;
}
