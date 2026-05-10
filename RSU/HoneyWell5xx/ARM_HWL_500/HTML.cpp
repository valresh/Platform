//#include "UtilsH/StdAfx.h"
//#include "Tag.h"
//#include "TagDIV.h"
#include "HTML.h"
#include "Bin.h"
#include "Dsd.h"
//#include "../Extensions.h"
//#include "../AplicFrm.h"
//#include <algorithm>
//#include "Scripts127.h"
//#include "../Function.h"

CHTML::CHTML(CWnd* pWnd,KHoneyPipeClient& _pipe)
//: pipe(_pipe), ms_chVer(3)
//, mWnd( pWnd)
//, mPlace(sizeof(SParamsData))
//, mChars(sizeof(char))
//, mStyle(sizeof(CStyle))
//, mDsdPR(sizeof(SBaseDSD))
//, mPipes(sizeof(SParamValueH))
//, m_bPrint(false)
//, m_hHDC(NULL)
//, m_bR300(false)
//, m_bPopup(false)
//, m_bBinData(false)
//, m_nViewNameEu(2)
//, m_bFileLoaded(false)
//, m_tagTargShape140(NULL)
//, m_pAreaPane(NULL)
//, m_bName140(true)
//, m_strPV(NULL)
//, m_strReset(NULL)
//, m_strDesc(NULL)
//, blueRect(NULL)
//, m_bResize(false)
//, m_nTimerCount(0)
//, m_LoadAcceleration(true)
//, m_bNeedPipeEnd(false)
//, m_eShow95(estNone)
//, strCtrlBlockName("")
//, m_pScriptClick(NULL)
  {
  //mStyle.Realloc(2000);
  //memset(m_szTitle, 0, sizeof(m_szTitle));
  //ClearPage();
  }

CHTML::~CHTML(void)
  {
//  Clear();//Блин
//  ClearBuffers();
  }
//
// Проверка наличия файла в данной директории
//bool CHTML::FindFile( const char* pszFolder, char* szPath )
//  {
//  if(m_strFile.GetLength() <_countof(".htm") || stricmp(m_strFile.GetBuffer() + (m_strFile.GetLength() - _countof(".htm") + 1), ".htm") != 0)
//    sprintf_s( szPath, SizeMP, "%s%s\\%s.htm", ::GetBasePath(), pszFolder, m_strFile );
//  else
//  {
//     sprintf_s( szPath, SizeMP, "%s%s\\%s", ::GetBasePath(), pszFolder, m_strFile );
//     m_strFile = m_strFile.Mid(0, m_strFile.GetLength() - _countof(".htm") + 1);
//  }
//  DWORD dwAttr = GetFileAttributes( szPath );
//  if ( dwAttr & FILE_ATTRIBUTE_DIRECTORY ) return false;
//  sprintf_s( szPath, SizeMP, "%s%s\\", ::GetBasePath(), pszFolder );
//  return true;
//  }
//
//std::vector<std::string> CHTML::m_ClietPath;
//bool CHTML::IsTrueMemoname( const char* szFile )
//{
//  for(std::vector<std::string>::iterator it = m_ClietPath.begin(); it!=m_ClietPath.end(); it++) 
//  {
//  char szPath[512];
//  sprintf_s( szPath, "%s%s\\%s.htm", ::GetBasePath(), (*it).c_str(), szFile );
//  DWORD dwAttr = GetFileAttributes( szPath );
//  if ( dwAttr != -1 )
//      return true;
//  else 
//    {
//    // Иногда файл уже идет с расширением
//    sprintf_s( szPath, "%s%s\\%s", ::GetBasePath(), (*it).c_str(), szFile );
//    dwAttr = GetFileAttributes( szPath );
//    if ( dwAttr != -1 ) 
//        return true; 
//    }
//  }
//  return false;
//}
////
//bool CHTML::FindName( char* szPath )
//  {
//  if(m_ClietPath.size()==0)
//  {
//      std::string file(::GetBasePath());
//      file += "Abstract\\Station\\Default.stn";
//      if (!theApp.FileExists(file))
//      {
//          m_ClietPath.push_back("Abstract");
//          m_ClietPath.push_back(R300); 
//          m_ClietPath.push_back("Abstract\\Popup"); 
//          m_ClietPath.push_back("Abstract\\Popups");
//          m_ClietPath.push_back("Abstract\\Faceplates"); 
//      }
//      else
//      {
//          char  path[512];
//          for(int ii=0; ii<16; ii++)
//          {
//            char param[16];
//            sprintf_s(param, "PATH%d", ii);
//            DWORD size = ::GetPrivateProfileStringA("Paths", param, "", path, _countof(path)-1, file.c_str());
//            if(size > 0)
//            {
//             char *pp = strstr(path, "\\Client\\");
//             if(pp)
//             {
//                m_ClietPath.push_back(pp+_countof("\\Client\\")-1);
//             }
//            }
//          }
//          m_ClietPath.push_back(R300); 
//      }
//  }
//
//  bool bFound = false;
//  for(std::vector<std::string>::iterator it = m_ClietPath.begin(); it!=m_ClietPath.end(); it++) 
//  {
//      if ( FindFile( (*it).c_str(), szPath ) )
//      {
//          bFound = true;
//          if((*it) == R300)
//             m_bR300=true;
//          break; 
//      }
//  }
//
//  if(!bFound)    
//  {
//      CString filename(m_strFile);
//      CString path(::GetBasePath());
//      path += "Abstract\\";
//      if(m_strFile.GetLength() >=_countof(".htm") && stricmp(m_strFile.GetBuffer() + (m_strFile.GetLength() - _countof(".htm") + 1), ".htm") != 0)
//          filename += ".htm";
//      else m_strFile = m_strFile.Mid(0, m_strFile.GetLength() - _countof(".htm") + 1);
//      std::string name = ::FindFile(path, filename);
//      if(name.size())
//      {
//          name = name.substr(0, name.size() - filename.GetLength());
//          strcpy_s(szPath, _MAX_PATH, name.c_str());
//      }
//  }
//
//  return true;
//  }

//std::string CHTML::GetSafeviewCategory(const char* name)
//{
//  char szPath[MAX_PATH];
//  std::string SafeviewCategory;
//  CString strFile(name);
//  if(stricmp(name + strlen(name)-_countof(".htm")+1, ".htm")==0)
//    strFile = strFile.Mid(0, int(strlen(name)-_countof(".htm")+1));
//  for(std::vector<std::string>::iterator it = m_ClietPath.begin(); it!=m_ClietPath.end(); it++) 
//  {
//    sprintf_s( szPath, "%s%s\\%s.htm", ::GetBasePath(), (*it).c_str(), strFile );
//    DWORD dwAttr = GetFileAttributes( szPath );
//    if ( dwAttr != -1 && !(dwAttr & FILE_ATTRIBUTE_DIRECTORY))
//    { //нашли
//      char szDsdPath[MAX_PATH];
//      sprintf_s( szDsdPath, "%s%s\\%s_files\\DS_datasource1.dsd", ::GetBasePath(), (*it).c_str(), strFile );
//      DWORD dwSize;
//      char* szBuffer = ::FileToBuff( szDsdPath, dwSize );
//      if ( szBuffer  )
//      {
//       char* pp1 = strstr(szBuffer, "/>");
//       if(pp1)
//       {
//         *pp1=0;
//         char* pp2 = strstr(szBuffer, "safeviewCategory=");
//         if(pp2)
//         {
//             pp2 += _countof("safeviewCategory=") -1;
//             SkipSpace(pp2);
//             if(*pp2=='"')
//             {
//                 char* pp3 = strchr(pp2+1, '"');
//                 if(pp3)
//                 {
//                     *pp3=0;
//                     SafeviewCategory = pp2+1;
//                 }
//             }
//         }
//       }
//       ::BuffFree( szBuffer );
//      }
//      break;
//    }
//  }
//  return SafeviewCategory;
//}
//
//void CHTML::setPPVals(const char* pv, const char * rst, const char * desc ){
//	m_strPV = pv;
//	m_strReset = rst;
//	m_strDesc = desc;
//}
//bool CHTML::LoadHTM( HWND hWnd, LPCTSTR pszName, LPCTSTR szTagGrpIn )
bool CHTML::LoadHTM(const char* htmlData, const char* bindingData, const char* dsdData)
  {
  //m_strFile  = pszName;
  //m_bFileLoaded = true;
  //char szTagGrp[1024] = "";
  //if(szTagGrpIn)
  //  strcpy_s(szTagGrp, szTagGrpIn);
  //
  //ClearPage();
  //
  //bool bSetGroupRarameters = false;
  CharMP szPath,szFile;
  if(szTagGrp[0] != '&')
  {
   m_strGroup = szTagGrp;
   if(char *ra = strchr(szTagGrp, '&'))
   {
       strcpy_s( szTagGrp, ra);
       m_strGroup = m_strGroup.Mid(0, (int)(ra-szTagGrp));
       int nn = m_strGroup.Find('='); 
       std::string paramname;
       if(nn != -1)
       {
          paramname = m_strGroup.Mid(0, nn).GetBuffer();
          m_strGroup = m_strGroup.Mid(nn+1);
       }
       DisplayDataRepositorySetValue("CurrentPoint", m_strGroup.GetBuffer());
       if(paramname.size()>0)
          DisplayDataRepositorySetValue(paramname, m_strGroup.GetBuffer());
       bSetGroupRarameters = true;
   }
  }
  else
     bSetGroupRarameters = true;
  m_strFull  = "";
  // Ищем местоположение файла
  /*if ( !FindName( szPath ) )
    {
    m_bR300 = true;
    return false;
    }*/

  //size_t namelen = strlen(pszName);
  //if(namelen > 3 && _strnicmp(pszName + namelen - _countof("_fp")+1, "_fp", _countof("_fp")-1) == 0)
  //  m_bR300=true; //для AnalizSpecific

  // Изменения в Exe-файле могут повлиять
  //::GetModuleFileName( NULL, szFile, SizeMP );
  //if ( !::GetAttr( szFile, wfad[0] ) )return false;
  //// Проверяем наличие обязательных файлов
  //sprintf_s( szFile, SizeMP, "%s%s.htm", szPath, m_strFile );
  //if ( !::GetAttr( szFile, wfad[1] ) )return false;
  ////
  //sprintf_s( szFile, SizeMP, "%s%s_files\\bindings.xml", szPath, m_strFile );
  //if ( !::GetAttr( szFile, wfad[2] ) )return false;
  ////
  //sprintf_s( szFile, SizeMP, "%s%s_files\\DS_datasource1.dsd", szPath, m_strFile );
  //if ( !::GetAttr( szFile, wfad[3] ) )return false;
  //
  // Пытаемся прочитать бинарный файл
  //m_strFull.Format( "%s%s.htm", szPath, m_strFile );
  /*if ( Buffer() )
    return true;*/
  //
  //if(bSetGroupRarameters)
  //{
  //    LPCTSTR pp1 = szTagGrp;
  //    while(pp1[0] == '&')
  //    {
  //        char param[128];
  //        char value[128];
  //        LPCTSTR pp2 = strchr(++pp1, '=');
  //        if(pp2==NULL)
  //          break;
  //        strncpy_s(param, pp1, pp2-pp1);
  //        pp1 = strchr(++pp2, '&');
  //        if(pp1==NULL)
  //           pp1 = pp2 + strlen(pp2);         
  //       strncpy_s(value, pp2, pp1-pp2);
  //       DisplayDataRepositorySetValue(param, value);
  //    }
  //}
  //else
  //{
  //if(Version == DOTF_PM)
  //  {
  //  m_bResize = strstr(szPath, "\\R410") == 0;
  //  if(m_strFile.CompareNoCase("MN14") == 0 || m_strFile.CompareNoCase("MN62") == 0 || m_strFile.CompareNoCase("MN47") == 0)
  //     m_bResize = false;
  //  }
  ////
  //if(Version == LG35_8_KF || Version == L35_11600_KF)
  //{
  //   if(m_strFile.CollateNoCase("sncdtlCPCa_fp")==0)
  //   {
  //   DisplayDataRepositorySetValue("Sel", "LSel");
  //   DisplayDataRepositorySetValue("Seq", "Seq");
  //   DisplayDataRepositorySetValue("L1", "L1");
  //   DisplayDataRepositorySetValue("L2", "L2");
  //   DisplayDataRepositorySetValue("L3", "L3");
  //   DisplayDataRepositorySetValue("CtrlBlockSelected", "LSel");
  //   DisplayDataRepositorySetValue("CtrlBlockPVSPSelected", "L1");
  //   }
  //   else 
  //   if(m_strFile.CollateNoCase("sncdtlSPCMa_fp")==0)
  //   {
  //   DisplayDataRepositorySetValue("Sel", "SelH");
  //   DisplayDataRepositorySetValue("L1", "AS");
  //   DisplayDataRepositorySetValue("L2", "Lim1");
  //   DisplayDataRepositorySetValue("L3", "Lim2");
  //   DisplayDataRepositorySetValue("CtrlBlockSelected", "SelH");
  //   DisplayDataRepositorySetValue("CtrlBlockPVSPSelected", "AS");
  //   }
  //}
  //else
  //{
  //   if(Version == DOTF_PM)
  //    {
  //      //if(m_strFile.CollateNoCase("sysdtlpida_hs_cl0")==0)
  //      if((m_strFull.Find("_fp.htm") != std::string::npos) || (m_strFull.Find("_fp_switcha.htm") != std::string::npos))
  //      {   //нужно читать из BLKCOMMENT1 
  //        CShCONTROLMODULE sh;
  //        if(pipe.GetData( id_CONTROLMODULE, m_strGroup, &sh ))
  //          {
  //          if(sh.BLCKCOMMENT1[0]) 
  //            DisplayDataRepositorySetValue("SECPNTNAME", sh.BLCKCOMMENT1);
  //          }
  //      }
  //   }
  //   else
  //   if(Version == BERTH_ONPZ)
  //   {
  //       if(m_strFile.CompareNoCase("sysdtldevctla_1_fp") == 0)
  //       {
  //            DisplayDataRepositorySetValue("strCtlBlockName", "DEVCTLA");
  //       }
  //       else
  //       if(m_strFile.CompareNoCase("sysdtlSMdevctla_3SVLV_fp") == 0 || m_strFile.CompareNoCase("sysdtlSMdevctla_PUMP_fp") == 0)
  //       {
  //            DisplayDataRepositorySetValue("strCtlGOP", (m_strObj+".GOP.PV").GetBuffer());
  //            DisplayDataRepositorySetValue("strCtlGPV", (m_strObj+".GPV.PV").GetBuffer());
  //            DisplayDataRepositorySetValue("strCtlOI", (m_strObj+".OI.PV").GetBuffer()); 
  //            DisplayDataRepositorySetValue("strCtlPI", (m_strObj+".PI.PV").GetBuffer()); 
  //            DisplayDataRepositorySetValue("strCtlSt0Cmd", (m_strObj+".{%BlkCMD4%}.PVFL").GetBuffer()); 
  //            DisplayDataRepositorySetValue("strCtlSt1Cmd", (m_strObj+".{%BlkCMD5%}.PVFL").GetBuffer()); 
  //            DisplayDataRepositorySetValue("strCtlSt2Cmd", (m_strObj+".{%BlkCMD6%}.PVFL").GetBuffer()); 
  //            DisplayDataRepositorySetValue("strCtlLocalman", (m_strObj+".{%BlkLocalman%}.PV").GetBuffer()); 
  //       }
  //   }
  //}
  //}
  //

  bool bReturn = false;
  char* szBuffer = (char*)htmlData;/*::FileToBuff( m_strFull )*/;
  //if ( szBuffer != NULL )
    {
    CHTML_Path path(m_strFull);
    char* ptr = szBuffer;
    //if ( !m_bBinData )
      {
      // В начале должен быть NULL
      mChars.Realloc(1);
      mChars.m_nCurrent++;
      
      CDsd dsd;
      // !! дошли до этого места !!
      CBin bin(dsd);
      if ( bin.Create( *this, szPath, m_strFile ) )
      bReturn = Parser( *this, szBuffer, ptr, &bin );
      }
    // Нужен кэш или нет, надо определиться
    /*else
      {
      CStyle& s = Style(0);
      ptr = szBuffer + s.m_dwShiftTxt;
      if ( s.m_dwShiftTxt ) AddStyle();
      bReturn = Parser( *this, szBuffer, ptr, NULL );
      }*/
    //
    INT_PTR H = FullCount();
    if ( bReturn )
      {
       pagecomplete();

    	INT_PTR nCount = GetSize();
      CTag**     obj = GetData();
    	for ( INT_PTR n = 0; n < nCount; n++ )
      obj[n]->Kerdyk( hWnd, *this );
    	for ( INT_PTR n = 0; n < nCount; n++ )
      obj[n]->Script( hWnd, *this );
      }
    ::BuffFree( szBuffer );
    }
  //
#ifdef _DEBUG
  DWORD Time = GetTickCount() -  starttime;
  char buf[512];
  sprintf_s(buf, "%s Время загрузки = %d\n", m_strFile.GetBuffer(), Time); 
  OutputDebugString(buf);
#endif
  //
  m_bFileLoaded = false;
  //
  if(Version == LG35_8_KF)
   {//регистрациия в pipe нужных точек (их нет в мнемосхеме)
    if(m_strFile.CompareNoCase("M01")==0)
       SParamValueH pv = ::FindExperion(*this, "SBDA6100", "FLAGB.PVFL"); //нужно для "pushbutton005"
   }

   if(Version == L35_11600_KF)
   {
     if(m_strFile.CollateNoCase("sncdtlSPCMa_fp")==0)
     if(m_strObj == "FIC6112" || m_strObj == "FIC7112")
     {
      if(CTag *pgr = FindTagInBodyById(*this, "grButL2" ))
         pgr->SetVisibility(*this, enumHidden);
      if(CTag *pgr = FindTagInBodyById(*this, "grButL3" ))
         pgr->SetVisibility(*this, enumHidden);
      if(CTag *pgr = FindTagInBodyById(*this, "grIndL2" ))
         pgr->SetVisibility(*this, enumHidden);
      if(CTag *pgr = FindTagInBodyById(*this, "grIndL3" ))
         pgr->SetVisibility(*this, enumHidden);
      if(CTag *pgr = FindTagInBodyById(*this, "GRP_Arrows" ))
         pgr->SetVisibility(*this, enumHidden);
     }
   }

  //
  ShowOrHide( hWnd, m_nViewNameEu );
  return bReturn;
  }

//void CHTML::NotFoundM(bool bWrite)
//  {
//  _static int nCount = 0;
//  _static CString nameT[MAX_QUQU];
//  NotFound( *this, nameT, nCount );
//  if ( !bWrite )
//    return;
//  HFILE hFile =  _lcreat("c:/ForDebug/NotTegs.scn",0);
//  if ( hFile != HFILE_ERROR )
//    {
//    CString str;
//    for ( int n = 0; n < nCount; n++ )
//      {
//      str.Format( "<html=\"%s\">\r\n", nameT[n] );
//      _hwrite( hFile, str, lstrlen(str) );
//      }
//    _lclose( hFile );
//    }
//  }
//
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//void CTag::SetMnemo( CHTML& html, const char* mnemo, SMnemo* list, int& nCounts )
//  {
//  //для ассоциативных дисплеев исключаем блокировочные схемы
//  if (Version==LG35_8_KF)
//  if ((strstr(mnemo,"BL")!=NULL)||(strstr(mnemo,"bl")!=NULL))
//	  return;
//	INT_PTR nCount = GetSize();
//  CTag**     obj = GetData();
//	for ( INT_PTR n = 0; n < nCount; n++ )
//    obj[n]->SetMnemo( html, mnemo, list, nCounts );
//  //
////  SBaseDSD* dsd = Dsd(html);
////  if ( !( dsd && dsd->m_nCountPip ) )
////    return;
//  //SParamValueH* var  = dsd->Value(html);
//  //const char* name = html.pipe.GetMnemoName( var->nNumber );
//  //
//  if ( mVal.nNumber == -1 )
//    return;
//  const char* name1 = html.pipe.GetMnemoName( mVal.nNumber );
//  if (strcmpi("Фёдор", name1) == 0) return;
//  char name[128];
//  sprintf(name, "%s.%s", name1, mVal.def->name);
//  //
//  for ( int n = 0; n < nCounts; n++ )
//  if ( lstrcmpi( list[n].name, name ) == 0 )
//    {
//    int m = 0;
//    for ( ; m < list[n].nCount; m++ )
//    if ( lstrcmpi( list[n].list[m], mnemo ) == 0 )
//      break;
//    if ( m == list[n].nCount )
//      {
//      ASSERT(m < 128);
//      lstrcpy( list[n].list[m], mnemo );
//      list[n].nCount++;
//      }
//    return;
//    }
//  //ASSERT( nCounts < MAX_QUQU-1 );
//  if ( !(nCounts < MAX_QUQU-1) )
//    {
//    ASSUNO
//    return;
//    }
//  list[nCounts].name = name;
//  lstrcpy( list[nCounts].list[0], mnemo );
//  list[nCounts].nCount++;
//  nCounts++;
//  //
//  }
//
//void CHTML::SetMnemoM(bool bWrite)
//  {
//  _static int nCount = 0;
//  _static SMnemo list[MAX_QUQU];
//  SetMnemo( *this, m_strFile, list, nCount );
//  if ( !bWrite )
//    return;
//  HFILE hFile =  _lcreat("c:/ForDebug/FoundTegsOnShemas.csv",0);
//  CString str;
//  _hwrite( hFile, str, lstrlen(str) );
//  for ( int n = 0; n < nCount; n++ )
//    {
//    //if ( !list[n].bInvert ) continue;
//
//    str.Format( "%s;%s", list[n].name, list[n].list[0] );
//    for ( int m = 1; m < list[n].nCount; m++ )
//      {
//      str += ",";
//      str += list[n].list[m];
//      }
//    //if ( list[n].bInvert )
//    //  str += " Finvert=4";
//    str += "\r\n";
//    _hwrite( hFile, str, lstrlen(str) );
//    }
//  _hwrite( hFile, str, lstrlen(str) );
//  _lclose( hFile );
//  }
//
//void CHTML::Print( bool bWrite )
//  {
//  //NotFoundM(bWrite);
//  SetMnemoM(bWrite);
//  if ( m_bPrint ) return;
//  m_bPrint = true;
//  //
//  UINT n = 0;
//  for ( ; n < pipe.Count(); n++ )
//    {
//    CShBase* sh = pipe.Sh(n);
//    if ( !IsTypeOk ) break;
//    }
//  if ( n == pipe.Count() ) return;
//  //
//  HFILE hFile = _lopen ( "c:/ForDebug/NotModel.txt", OF_WRITE );
//  if (  hFile == HFILE_ERROR )
//        hFile = _lcreat( "c:/ForDebug/NotModel.txt", 0 );
//  if (  hFile == HFILE_ERROR ) return;
//  _llseek( hFile, 0, SEEK_END );
//  CString str;
//  str.Format( "Мнемосхема %s\r\n", m_strFile );
//  _hwrite( hFile, str, lstrlen(str) );
//  //
//  n = 0;
//  for ( ; n < pipe.Count(); n++ )
//    {
//    CShBase* sh = pipe.Sh(n);
//    if ( IsTypeOk ) continue;
//    //
//	//MIHAIL_HWL добавил GetAt к CPipeClient
//    SSterver& st = pipe.GetAt(n);
//    if ( lstrcmpi( st.szName+1, st.szServ+1 ) == 0 )
//      str.Format( "%s %d\r\n", st.szName+1, st.nType );
//    else
//      str.Format( "%s %s %d\r\n", st.szName+1, st.szServ+1, st.nType );
//    _hwrite( hFile, str, lstrlen(str) );
//    }
//  str = "\r\n";
//  _hwrite( hFile, str, lstrlen(str) );
//  _lclose( hFile );
//  }
////
////void CHTML::ClearPage()
////  {
////  Clear();
////  m_bR300  = false;
////  m_bPrint = false;
////  m_pageSize.cx = 1;
////  m_pageSize.cy = 1;
////  m_pageColor  = ::GetSysColor( COLOR_WINDOW );
////  m_strTitle = "";
////  mBody = NULL;
////  m_nTimerCount = 0;
////  m_strGroup = "";
////  m_bNeedPipeEnd = false;
////  m_Subroutines.clear();
////  m_pScriptClick = NULL;
////  m_psrcelement = NULL;
////  m_eventpoint.x = m_eventpoint.y = -1;
////  WINDOWS_EXTERNAL_CURRENTPAGE[0] = 0;
////  }
////
//void CHTML::ClearBuffers()
//  {
//  mPlace.Clear();
//  mChars.Clear();
//  mStyle.Clear();
//  mDsdPR.Clear();
//  
//  SParamValueH* val = Value();
//  for(UINT i = 0; i < mPipes.Count(); i++)
//	{if(val) val->freeMulti(); val++;}
//  mPipes.Clear();
//  }
////
//bool CHTML::Buffer()
//  {
//  m_bBinData = false;
//  //
//  ClearBuffers();
//  //
//  #define Return { ::BuffFree(szBuffer); return false;}
//  CharMP szFile;
//  ::GetBinFile( szFile, m_strFull, ::IsMnemoStyle(MNEMO_EDIT) ? "bin" : "user" );
//  DWORD dwSize;
//  char* szBuffer = ::FileToBuff( szFile, dwSize );
//  if  ( szBuffer == NULL ) return false;
//  char* ptr = szBuffer;
//  if ( *ptr != ms_chVer ) Return;
//  ptr++;
//  //
//  FILETIME* F = (FILETIME*)ptr;
//  for ( int n = 0; n < 4; n++ )
//    if ( !EqAttr( wfad[n], F[n] ) ) Return;
//  ptr += 4*sizeof(FILETIME);
//  //
//  mPlace.Read( ptr );
//  mChars.Read( ptr );
//  mStyle.Read( ptr );
//  mDsdPR.Read( ptr );
//  mPipes.Read( ptr );
//  //
//  m_bBinData = true;
//  //
//  return false;
//  }
////
//void CHTML::SaveBin()
//  {
////  if ( m_bR300 || m_bBinData )
//    return;
//  //
//  CharMP szFile;
//  ::GetBinFile( szFile, m_strFull, ::IsMnemoStyle(MNEMO_EDIT) ? "bin" : "user" );
//  HFILE hFile = _lcreat( szFile, 0 );
//  if ( hFile != HFILE_ERROR )
//    {
//    _hwrite( hFile, &ms_chVer, 1 );
//    _hwrite( hFile, (char*)&wfad[0].ftLastWriteTime, sizeof(FILETIME) );
//    _hwrite( hFile, (char*)&wfad[1].ftLastWriteTime, sizeof(FILETIME) );
//    _hwrite( hFile, (char*)&wfad[2].ftLastWriteTime, sizeof(FILETIME) );
//    _hwrite( hFile, (char*)&wfad[3].ftLastWriteTime, sizeof(FILETIME) );
//    mPlace.Write( hFile );
//    mChars.Write( hFile );
//    mStyle.Write( hFile );
//    mDsdPR.Write( hFile );
//    mPipes.Write( hFile );
//    _lclose( hFile );
//    }
//  }
////
//void CHTML::Paint( HDC hDC )
//  {
//  // Во время загрузки файла отсекаем отрисовку,
//  // так как при ASSERT-ах всё может ебануться!
//  if ( m_bFileLoaded ) return;
//  //
//	INT_PTR nCount = GetSize();
//  CTag**     obj = GetData();
//	for ( INT_PTR n = 0; n < nCount; n++ )
//    obj[n]->PaintThis( hDC, *this );
//  }
////
//CTag* CHTML::PtInTag( POINT& pt, bool bAll )
//  {
//  if ( mBody == NULL ) return NULL;
//  return mBody->PtInTag( *this, pt, bAll );
//  }
////
//CTag* CHTML::PtInBoundTag( POINT& pt )
//  {
//  if ( mBody == NULL ) return NULL;
//  return mBody->PtInBoundTag( *this, pt );
//  }
//
////поиск всех видимых тегов (кроме "Page") под курсором
//int CHTML::FindTags(POINT pt, std::vector<CTag*>& tags)
//{
//   if ( mBody == NULL ) 
//       return 0;
//   return mBody->FindTags( *this, pt, tags );
//}
////
//bool CHTML::FindTitle( POINT& pt, LPSTR title )
//{
//  if ( mBody == NULL ) 
//     return false;
//  return mBody->FindTitle( *this, pt, title );  
//}
////
//CTag* CHTML::TabIndex( POINT& pt )
//  {
//  if ( mBody == NULL ) return NULL;
//  return mBody->TabIndex( *this, pt );
//  }
////
//void CHTML::ShowOrHide( HWND hWnd, int N )
//  {
//	INT_PTR nCount = GetSize();
//  CTag**     obj = GetData();
//	for ( INT_PTR n = 0; n < nCount; n++ )
//    {
//    obj[n]->ShowOrHide( *this, "TxtEUDesc", N == 1 );
//    obj[n]->ShowOrHide( *this, "TxtName"  , N == 2 );
//    }
//  if ( m_nViewNameEu != N )
//  ::InvalidateRect( hWnd, NULL, FALSE );
//  m_nViewNameEu = N;
//  }
////
//void CHTML::Dlg(class CDlgDebug& dlg, POINT& pt, int nPaint )
//  {
//	INT_PTR nCount = GetSize();
//  CTag**     obj = GetData();
//	for ( INT_PTR n = 0; n < nCount; n++ )
//    obj[n]->DlgDebug( dlg, pt, nPaint );
//  }
////
//void CHTML::ApplyLayout( float fScale, SIZE sz )
//  {
//  //sub ApplyLayout()
//  //' This routine enables the 'smart sizing' of faceplates
//  //
//  //  on error resume next
//  //  GrpLowerPanel.style.top = ""
//  //  GrpLowerPanel.style.bottom = "2px"
//  //  PicBackground.style.pixelWidth  = Page.style.pixelWidth
//  //  PicBackground.style.pixelHeight = Page.style.pixelHeight
//  //
//  //  GrpCenterPanel.style.top = ""
//  //  if (Page.style.pixelHeight - GrpLowerPanel.style.pixelHeight - GrpUpperPanel.style.pixelHeight - 10) > 153 then
//  //    GrpCenterPanel.style.bottom = (GrpLowerPanel.style.pixelHeight + 15) & "px"
//  //  else
//  //    GrpCenterPanel.style.bottom = (GrpLowerPanel.style.pixelHeight + 2) & "px"
//  //  end if
//  //
//  //end sub
//  //
//  if ( mBody == NULL ) return;
//  CTag* page = mBody->LinkScript( *this, "Page" );
//  CTag* back = mBody->LinkScript( *this, "PicBackground" );
//  //
//  CTag* upper = mBody->LinkScript( *this, "GrpUpperPanel"  );
//  CTag* centr = mBody->LinkScript( *this, "GrpCenterPanel" );
//  CTag* lower = mBody->LinkScript( *this, "GrpLowerPanel"  );
//  //
//  if ( !(upper && centr && lower) )
//    return;
//  //
//  upper->mVal.nNumber = pipe.GetActive();
//  //
//  SIZE size= Size();
//  float cy = float(sz.cy) / fScale;
//  float dy = cy-size.cy;
//  size.cy += dy*.75;
//  Size(size);
//  if ( page && back )
//    {
//    CStyle& pageS = page->Style( *this );
//    CStyle& backS = back->Style( *this );
//    pageS.mRect.Height = cy;
//    backS.mRect.Width  = pageS.mRect.Width;
//    backS.mRect.Height = pageS.mRect.Height;
//    }
//  // Смешение элементов GrpLowerPanel
//  lower->ShiftDY( *this, dy );
//  // Изменение координат GrpCenterPanel
//  CStyle& style = centr->Style( *this );
//  float N = style.mRect.Height+dy;
//  if ( N < 100 )
//    {
//    centr->Disable( *this );
//    if ( N < 0 )
//    upper->Disable( *this );
//    }
//  else
//    {
//    const char* face = m_strFile; face += 6;// "sysdtl"
//    //
//    CTag* arrows = NULL;
//    if ( (lstrcmpi(face, "CPCa_fp") == 0) || (lstrcmpi(face, "SPCMa_fp") ==0) )
//      {//масштабирование стрелок по Y
//      float s = N/style.mRect.Height;
//      if(arrows =mBody->LinkScript( *this, "GRP_Arrows"))
//				arrows->ScaleDY( *this, style.mRect.Y, s );
//      }
//    if ( !(lstrcmpi( face, "pida_fp" ) == 0
//    ||     lstrcmpi( face, "daca_fp" ) == 0
//    ||     lstrcmpi( face, "daca_fp" ) == 0 ) )
//    if (!arrows)
//    return;
//    //
//    float s = N/style.mRect.Height;
//		centr->ScaleDY( *this, style.mRect.Y, s );
//    //
//    CTag* tag = mBody->LinkScript( *this, "indicator_PV"  );
//    if  ( tag == NULL )
//          tag = mBody->LinkScript( *this, "indicator_OP" );
//    CTag* hi  = mBody->LinkScript( *this, "textHiLimit"  );
//    CTag* lo  = mBody->LinkScript( *this, "textLoLimit"  );
//    if ( hi && lo && tag )
//      {
//      CStyle& hiS = hi ->Style( *this );
//      CStyle& loS = lo ->Style( *this );
//      CStyle& taS = tag->Style( *this );
//      hiS.mRect.Y = taS.mRect.Y;
//      loS.mRect.Height = (taS.mRect.Y+taS.mRect.Height)-loS.mRect.Y;
//      loS.align |= DT_BOTTOM;
//      }
//    //CTag* tag = mBody->LinkScript( *this, "indicator_PV"  );
//    }
//  }
////
//std::string CHTML::FindInDisplayDataRepository(std::string key, bool bCheck)
//{
//    std::map<std::string, std::string>::iterator it;
//    if( (it = m_DisplayDataRepository.find(key)) != m_DisplayDataRepository.end() )
//      {//бывает имя ссылаестся на другое ссылчное имя
//          std::map<std::string, std::string>::iterator itt;
//          if( (itt = m_DisplayDataRepository.find(it->second)) != m_DisplayDataRepository.end() )
//             return itt->second;
//          else return it->second;
//      }
//    if(bCheck)
//      return "";
//    return key;
//}
////
void CHTML::DisplayDataRepositorySetValue(std::string key, std::string val)
{
     m_DisplayDataRepository[key] = val;
}
////
//const O_VAL* CHTML::GetCommomValue(std::string key)
//{
//    std::map<std::string, O_VAL>::iterator it = m_CommomValue.find(key);
//    if(it !=  m_CommomValue.end())
//        return &it->second;
//    return NULL;
//}
////
//CTag* CHTML::FindTagInBodyById(CHTML& html, const char *nameid )
//{
//  if ( mBody == NULL ) 
//      return NULL;
//  return mBody->FindTagById( *this, nameid);
//}
////
//void CHTML::CtrlBlockSet140(char *id)
//{
//    CTag *pTagScript = dynamic_cast<CTagSCRIPT*>(FindTagById(*this, id));
//    if(pTagScript)
//      {
//      CStyle& style = pTagScript->Style(*this);
//      const char* script  = style.GetValue(*this);
//      if(script)
//        {
//        const char *call = strstr(script, "CtrlBlockSet(me, DisplayDataRepository.GetValue");
//        if(call)
//         {
//          const char *ref1 = strchr(call + sizeof("CtrlBlockSet(me, DisplayDataRepository.GetValue"), '"');
//          if(ref1)
//           {
//           const char *ref2 = strchr( ref1+1, '"');
//           if(ref2)
//            {
//            char strCtrlBlockName[32];
//            strncpy_s(strCtrlBlockName, ref1+1, ref2-ref1-1); 
//            DisplayDataRepositorySetValue("CtrlBlockSelected", strCtrlBlockName);
//            const char *bPVSP = strstr( ref1+1, "true");
//            CTag *pTagt = NULL;
//            if(bPVSP)
//             {        
//                  DisplayDataRepositorySetValue("CtrlBlockPVSPSelected", strCtrlBlockName);
//                  if(pTagt = FindTagInBodyById(*this, "grpPVSP"))
//                     pTagt->SetVisibility(*this, enumVisible);
//                  if(pTagt = FindTagInBodyById(*this, "grpState"))
//                     pTagt->SetVisibility(*this, enumHidden);                                        
//             }
//            else
//             {
//                  if(pTagt = FindTagInBodyById(*this, "grpPVSP"))
//                     pTagt->SetVisibility(*this, enumHidden);
//                  if(pTagt = FindTagInBodyById(*this, "grpState"))
//                    pTagt->SetVisibility(*this, enumVisible);                                        
//             }
//
///*   
//Public Sub SimStateUpdate()
//    TxtSIM.Style.visibility = "hidden"
//    EnumSimulationState.Style.visibility = "Hidden"
//    if grpState.style.visibility = "visible" then
//		if sd.DataValue("currentpoint.SIMSTATE") = 2 then
//	        'SIMSTATE is SIMFREEZE
//	        TxtSIM.Style.visibility = "Hidden": EnumSimulationState.Style.visibility = "visible"
//	    Elseif sd.DataValue("currentpoint.SIMSTATE") = 1 then
//	        'SIMSTATE is SIMRUN
//	        TxtSIM.Style.visibility = "visible": EnumSimulationState.Style.visibility = "Hidden"
//	    End If
//    End If
//End Sub
//*/           
//             if(pTagt = FindTagInBodyById(*this, "TxtSIM"))
//               pTagt->SetVisibility(*this, enumHidden);
//             if(pTagt = FindTagInBodyById(*this, "EnumSimulationState"))
//               pTagt->SetVisibility(*this, enumHidden);
//             if(pTagt = FindTagInBodyById(*this, "grpState"))
//               {
//                if(pTagt->Style(*this).m_eVisible == enumVisible)
//                 {
//		           SParamValueH val = FindExperion(*this, m_strObj, "SIMSTATE");
//		           char v = pipe.ValueB_H(val);
//                   switch(v)
//                   {
//                   case 1:
//                    if(pTagt = FindTagInBodyById(*this, "TxtSIM"))
//                      pTagt->SetVisibility(*this, enumVisible);
//                    if(pTagt = FindTagInBodyById(*this, "EnumSimulationState"))
//                      pTagt->SetVisibility(*this, enumHidden);
//                    break;
//                   case 2:
//                    if(pTagt = FindTagInBodyById(*this, "TxtSIM"))
//                      pTagt->SetVisibility(*this, enumHidden);
//                    if(pTagt = FindTagInBodyById(*this, "EnumSimulationState"))
//                      pTagt->SetVisibility(*this, enumVisible);
//                    break;
//                   }
//                   KKK();
//                 }
//               }
//            }
//          }
//         }
//       }
//      }
//}
////
//void CHTML::AckAll()
//{ //квитирование всего на мемосхеме
//  SValueDef* def = ::NameToValueEx( id_AICHANNEL, "UnackAlarmExists" );
//  SParamValueH var;
//  //
//  SSendToModel send;
//  BYTE bOld = 0;
//  BYTE bNew = 1;
//  send.Set( enumValueChr, &bOld, &bNew );
//  lstrcpy( send.szValue, "Квитирование" );
//  //
//  int nCount = (int)pipe.Count();
//  for ( int n = 0; n < nCount; n++ )
//    {
//    var.nNumber = n;
//    var.def     = def;
//    BYTE noAck = pipe.ValueB_H( var, 0 );
//    pipe.SendData( n, send, noAck != 0 );
//    }
//}
////
//void CHTML::GetSubroutines0(std::map<std::string, SUB_STRUCT> &_Subroutines, LPCSTR vv, char *sbegin, char *send, bool bFunction)
//{
//    std::string lowervvstr(vv);
//    _strlwr((char*)lowervvstr.c_str());
//    char *lowervv = (char*)lowervvstr.c_str();
//
//    LPSTR pp = (char*)vv;
//    LPSTR lowerpp = (char*)lowervv;
//    //while(LPSTR pp1 = strstr(pp, sbegin))
//    while(LPSTR lowerpp1 = strstr(lowerpp, sbegin))
//     {
//       if(lowerpp1>lowervvstr.c_str() && lowerpp1[-1] == '\'')
//       {
//           lowerpp = lowerpp1 + strlen(sbegin);
//           continue;
//       }
//       LPSTR pp1 = (char*)vv + (lowerpp1 - lowervv);
//       pp1 += strlen(sbegin);
//       pp = pp1;
//       SkipSpace(pp1);
//       //char* pp2 = strstr(pp1, send);
//       SkipSpace(lowerpp1);
//       char* lowerpp2 = strstr(lowerpp1, send);
//       if(strcmp(send, "\nend sub")==0)
//       {
//           char* lowerpp3 = strstr(lowerpp1,  "\n end sub");
//           if(lowerpp3 && lowerpp3 < lowerpp2)
//             lowerpp2 = lowerpp3;
//       }
//       if(lowerpp2)
//       {
//         lowerpp = lowerpp2 + strlen( send );
//         char* pp2 = (char*)vv + (lowerpp2 - lowervv);
//         pp = pp2 + strlen( send );
//         char ch = *pp2;
//         *pp2=0;
//         char* pp3 = pp1+1;
//         while(*pp3 != ' ' && *pp3 != '(' && *pp3 != '\xA' && *pp3 != '\xD')
//           pp3++;
//         char subname[256]="";
//         strncpy_s(subname, pp1, pp3-pp1);
//         SkipSpace(pp3);
//         SUB_STRUCT sub;
//         sub.bFunction = bFunction;
//         
//         if( strstr(pp3, "next\n") && strstr(pp3, " for ") )
//         {
//            //*pp2=ch;
//            //continue;  //пока исключаю скрипты с циклами for
//         AAA();
//         }
//#ifdef _DEBUG
//         if(strcmp(subname, "PIUpdate")==0)
//           AAA();
//#endif 
//         if(*pp3!='(')
//           {//подпрограмма без параметров  
//              std::string script(pp3);
//              sub.body = script + "\n";
//              _Subroutines[subname] = sub;
//           }
//         else
//         { //подпрограмма с параметрами
//           char* pp4 = strchr(pp3+1, ')');
//           if(pp4)
//           {
//              std::vector<std::string> params;
//              SkipSpace( pp1 );
//              char *tt1 = pp3+1;
//              *pp4=0;
//              while(*tt1)
//              {
//                char *tt2 = strchr(tt1, ',');
//                if(tt2)
//                  {
//                   char par[256]="";
//                   strncpy_s(par, tt1, tt2-tt1);
//                   params.push_back(par);
//                   tt1 = tt2+1;
//                  }
//                else
//                  {
//                    params.push_back(tt1);
//                    break;
//                  }
//              }
//              *pp4++=')';
//              SkipSpace( pp4);
//              std::string script(pp4);
//              sub.body = script + "\n";
//              sub.params = params;
//              _Subroutines[subname] = sub;
//           }
//           KKK();
//         }
//       *pp2=ch;
//       }
//       else break;
//     }
//}
////
//void CHTML::GetSubroutines(LPCSTR vv)
//{
//    {
//      char *vvv = (char*)vv;
//      SkipSpace(vvv);
//      bool bFind=true;
//      while(bFind)
//      {
//       if(_strnicmp(vvv, "const", _countof("const")-1)==0)
//       {
//         std::string cname, cval;
//         vvv += _countof("const")-1;
//         SkipSpace(vvv);
//         while( IsV(*vvv) )
//            cname +=  *vvv++; 
//         SkipSpace(vvv);
//         if(*vvv++ == '=')
//         {
//            SkipSpace(vvv);
//            while( IsV(*vvv) )
//              cval +=  *vvv++; 
//            if(cval.size())
//            {
//             O_VAL o_val;
//             if(cval.size()>1 && cval[0]=='"')
//             {
//                o_val.eType = enumValueStr;
//                strncpy_s(o_val.ostr, cval.c_str()+1, cval.size()-2);
//             }
//             else
//             {
//                 o_val.eType = enumValueInt;
//                 o_val.oint = atoi(cval.c_str());
//             }
//            SetCommomValue(cname, o_val);
//            SkipSpace(vvv);
//            }
//         }
//       }
//       else
//       if(_strnicmp(vvv, "dim", _countof("dim")-1)==0)
//       {
//           char* v1 = strchr(vvv, '\n');
//           if(v1)
//           {
//             vvv = v1;
//             SkipSpace(vvv);
//           }
//           else break;
//       }
//       else break;
//      }
//    }
//    size_t len1= strlen(vv);
//    GetSubroutines0(m_Subroutines, vv, "sub ", "\nend sub", false);
//    size_t len2= strlen(vv);
//    //GetSubroutines0(m_Subroutines, vv, "function ", "\nend Function", true);
//    GetSubroutines0(m_Subroutines, vv, "function ", "\nend function", true);
//}
//
////typedef void (CTag::*pFnChange)(CHTML& html);
//void ChangestyleClass_Recframe(CHTML& html, CTag *pTag)
//{
//    CStyle& style = pTag->Style(html);
//    //if(shape_property_not_empty(html, pTag, "Tagname"))
//    {
//      const char* id = style.NameID(html);
//      if(strstr(id, "_RecFrame"))
//      if(_strnicmp(style.m_sStyleClass, "Recframe_", _countof("Recframe_")-1)==0)
//        strcpy_s(style.m_sStyleClass, "RecFrame");
//      else
//      if(_strnicmp(style.m_sStyleClass, "recframeequipment_", _countof("recframeequipment_")-1)==0)
//        strcpy_s(style.m_sStyleClass, "RecFrameEquipment");
//    }
//}
//
////перепесанные скриптыты page_pagecomplete
//void CHTML::pagecomplete()
//{
//   if(BERTH_ONPZ == Version)
//   {
///*
//       	for each oObj in page.all
//		'UI Toolkit initialization
//		if instr(ucase(oObj.classname),"HSC.ACTIVEXCONTROL.1")<>0 then
//			sPCObject = page.getattribute("ProcedureControlObject")
//			if sPCObject <> "" then exit for
//			if (instr(oObj,"ProcedureControl") <> 0) or (instr(oObj,"ProcedureListControl") <> 0) or (instr(oObj,"ProcedurePanelControl") <> 0) and sPCObject="" then
//				oObj.initialize()
//				page.setattribute "ProcedureControlObject", oObj.id
//			end if
//		'SP Tagname / EU initialization
//		elseif instr(ucase(oObj.classname),"HSC.SHAPE.1")<>0 then
//			'if typename(oObj.objects("TxtName"))<>"Nothing" then call Butt_Page_OnShapeLoad(oObj,sShow)
//			if Not IsNull(oObj.GetCustomProperty("Point","Tagname")) then call Butt_Page_OnShapeLoad(oObj,sShow)
//			if typename(oObj.objects("RecFrame"))<>"Nothing" then
//				sCls = lcase(oObj.objects("RecFrame").styleClass)
//				if instr(sCls, "recframe_") then
//					sCls = "RecFrame"
//				elseif instr(sCls, "recframeequipment_") then
//					sCls = "RecFrameEquipment"
//				end if
//				oObj.setAttribute "RecFrame_Class", sCls
//				oObj.objects("RecFrame").styleClass = sCls
//			end if
//		end if
//	next
//*/
//  if ( mBody != NULL )
//     mBody->ChangeTags(*this, &ChangestyleClass_Recframe);
//   }
//}
////
//void CTag::ChangeTags(CHTML &html, PFnChange pFnChange)
//{
//  pFnChange(html, this);
//  INT_PTR nCount = GetSize();
//  CTag**     obj = GetData();
//  for ( INT_PTR n = 0; n < nCount; n++ )
//      obj[n]->ChangeTags(html, pFnChange);
//}
////
//void CTag::ShiftDY( CHTML& html, float dy )
//  {
//  CStyle& style = Style( html );
//  style.mRect.Y += dy;
//  //
//  POINT* pt = style.PT(html);
//  for ( int n = 0; n < style.m_nPointPT; n++ )
//    pt[n].y += int(dy);
//  //
//	INT_PTR nCount = GetSize();
//  CTag**     obj = GetData();
//	for ( INT_PTR n = 0; n < nCount; n++ )
//    obj[n]->ShiftDY( html, dy );
//  }
////
//void CTag::ScaleDY( CHTML& html, float dy, float scale )
//  {
//  CStyle& style = Style( html );
//  float shift = (style.mRect.Y-dy)*scale;
//  style.mRect.Y = dy+shift;
//  style.mRect.Height *= scale;
//  //
//  POINT* pt = style.PT(html);
//  for ( int n = 0; n < style.m_nPointPT; n++ )
//    {
//    float shift = (pt[n].y-dy)*scale;
//    pt[n].y = int(dy+shift);
//    }
//  //
//	INT_PTR nCount = GetSize();
//  CTag**     obj = GetData();
//	for ( INT_PTR n = 0; n < nCount; n++ )
//    obj[n]->ScaleDY( html, dy, scale );
//  }
////
//void CTag::Disable( CHTML& html )
//  {
//  CStyle& style = Style( html );
//  style.m_eVisible = enumHidden;
//  //
//	INT_PTR nCount = GetSize();
//  CTag**     obj = GetData();
//	for ( INT_PTR n = 0; n < nCount; n++ )
//    obj[n]->Disable( html );
//  }
////
//void  CTag::NaviGo( CHTML& html, HWND hWnd, const char* mnemoname)
//{  
//   if(theApp.bUseWDL())
//   {
//       std::string SafeviewCategory = CHTML::GetSafeviewCategory(mnemoname);
//       CAmmFrm* pafrm = theApp.GetCategotyFrm(SafeviewCategory);
//       if(pafrm)
//         pafrm->GoToPage(mnemoname, html.Group());
//   }
//   else
//   {
//   if(!theApp.IsAllInOne() || html.m_strFile.CompareNoCase("nav") != 0)
//       GetAppWnd()->GiveAmm(hWnd)->GoToPage(mnemoname, html.Group());
//   else GetAppWnd()->GiveRegularAmm(hWnd)->GoToPage( mnemoname, html.Group());
//   }
//}
//

