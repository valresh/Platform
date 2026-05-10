#include "../StdAfx.h"
#include "../ARM.h"
#include "Bin.h"
#include "HTML.h"
#include "Common.h"

CBin::CBin(CDsd& rDsd)
: mHdx( INVALID_HXML )
, mDsd( rDsd )
  {
  }
//
bool CBin::Create( CHTML& html, const char* pszPath, const char* pszFileName )
  {
  //
  CharMP szPath;
  sprintf_s( szPath, SizeMP, "%s%s_files\\bindings.xml", pszPath, pszFileName );
  //
  int nReturn = LoadEx(szPath, pszFileName);
  //
  bool bReturn = false;
  if ( nReturn == 0 )
    {
    mHdx = FindItem( INVALID_HXML, "hdx" );
    if ( mHdx != INVALID_HXML )
      {
      HXML hItem = FindItem( mHdx, "binding" );
      if ( hItem != INVALID_HXML )
       {
       //установка предопределенных в binding параметров
        LPCTSTR ID = Param( hItem, "ID" );
        if(_stricmp(ID, "pageparam") == 0)
          {//данные для DisplayDataRepository
            HXML hPropertyItem;
            hPropertyItem = FindItem( hItem, "property");
            while(hPropertyItem!= INVALID_HXML)
            {
                LPCTSTR sourceproperty = Param( hPropertyItem, "sourceproperty" );
                LPCTSTR custompropertydefaultvalue = Param( hPropertyItem, "custompropertydefaultvalue" );
                if(sourceproperty && custompropertydefaultvalue)
                {
                    if(html.FindInDisplayDataRepository(sourceproperty, true).size()==0)
                      html.DisplayDataRepositorySetValue(sourceproperty, custompropertydefaultvalue);
                }
                hPropertyItem = NextItem( hPropertyItem );
            }
          }

        bReturn = mDsd.Create( html, pszPath, pszFileName );
        }
      }
    }
  return bReturn;
  }
//
HXML CBin::FindBIN( int nID )
  {
  HXML hItem = FindItem( mHdx, "binding" );
  while ( hItem != INVALID_HXML )
    {
    LPCTSTR ID = Param( hItem, "ID" );
    if ( ID && ::IsInt(ID) )
      {
      int id = atoi(ID);
      if ( id == nID )
        return hItem;
      }
    hItem = NextItem( hItem );
    }
  return INVALID_HXML;
  }
//
CDsd* CBin::FindDSO( CHTML& html, int nID, bool &bReadOnly, bool &bParameterFormat)
  {
  HXML hBin = FindBIN( nID );
  if ( hBin == INVALID_HXML )
    return NULL;
  //
  HXML hItem = FindItem( hBin, "dataobject" );
  while ( hItem != INVALID_HXML )
    {
    LPCTSTR ID = Param( hItem, "ID" );
    if ( ID && lstrcmpi( ID, "dso1" ) == 0 )
      {
      LPCTSTR objID = Param( hItem, "objectid" );
      if ( objID && ::IsInt( objID ) )
        {
        int id = atoi(objID);
        CDsd* pDsd = mDsd.FindHDX( id );
        if ( pDsd != NULL )
          {
          bool bIs = pDsd->IsYou(html,"dataobject");
          ASSERT( bIs );
          //
          if(Version >= DOTF_PM)  //пока так
          {
          HXML hItemClass = FindItem( hBin, "class" );
          while ( hItemClass != INVALID_HXML )
            {
              LPCTSTR classID = Param( hItemClass, "ID" );
              if(strcmp(classID, "HSC.Alpha")==0)
               {
                  bReadOnly = false;
                  if(!bParameterFormat)
                    break;
               }
              if(strcmp(classID, "HSC.ParameterFormat")==0)
               {
                  bParameterFormat = true;
                  if(bReadOnly)
                    break;
               }
              hItemClass = NextItem( hItemClass );
            }
          }
          //
          }
        return pDsd;
        }
      }
    hItem = NextItem( hItem );
    }
  return NULL;
  }
//
