#include <rsuErr.h>
#include "ControlBuilder.h"
#include <locale.h>
#include "../CnfReader/CnfReader.h"
#include <H_Data.h>
#include <macros/AutoCloser.h>
#include <algorithm>
#include <CommonH.h>
#include <rsuProjectVersion.h>
#include <macros/StrHelps.h>
#include <crossplatform.h>
#include <chrono>
#ifdef _WIN32
#include <io.h>
#endif

using namespace std;

LPCSTR g_szPntsFilePrefix = "РСУ_CBH";

LPCSTR KControlBuilder::GetCsvPrefix()
{
  return g_szPntsFilePrefix;
}

int KControlBuilder::ReadProject( SH_Module *&pRoot )
{
  char* Q = setlocale( LC_ALL, "Russian" );
  char *pdecpoint = localeconv()->decimal_point;
  if( pdecpoint && *pdecpoint!='.' )
    setlocale( LC_NUMERIC, "C");
  pdecpoint = localeconv()->decimal_point;

  char szPath[_MAX_PATH];
  GetPathToCnfFile( szPath, NULL, NULL );
  if( access( szPath, 0) )
    return -2;

  GetPathToCnfFile( szPath, "*", "cnf.xml" );
  KCnfReader reader;
  OutputDebugString( "Чтение проекта "); OutputDebugString( m_szEntrySystem ); OutputDebugString( "\n" );
  reader.ReadCnf( szPath, IBlock::emrParameters );
  std::string ctrl( m_szEntrySystem );
  OutputDebugString( "Получение входного модуля "); OutputDebugString( m_szEntrySystem ); OutputDebugString( "\n" );
  tBlockPtr_ entry = reader.SortFor( ctrl );
  if( !entry )
    return -1;
  SH_Block *pB = NULL;
  OutputDebugString( "Наполнение блоками "); OutputDebugString( m_szEntrySystem ); OutputDebugString( "\n" );
  int o = Create( entry->BlockName.c_str(), entry->EntityName.c_str(), "", entry->tnClass.c_str(), &pRoot, &pB );
  ASS( 1==o );
  if( 1==o && pRoot )
  {
    AssignParams( pRoot, entry );
    AppendTo( pRoot, entry );
  }
  OutputDebugString( "Подготовка выражений "); OutputDebugString( m_szEntrySystem ); OutputDebugString( "\n" );
  pRoot->OnReadProject( pRoot, NULL );
  SaveProject( pRoot );
  OutputDebugString( "Сохранён "); OutputDebugString( m_szEntrySystem ); OutputDebugString( "\n" );

  m_readNewProject = 0x00123400;
  return 0;
}

int KControlBuilder::InitL()
{
  OutputDebugString( "KControlBuilder::Init " );
  OutputDebugString( m_szPrjName );
  OutputDebugString( " ...\n" );
  auto t = std::chrono::high_resolution_clock::now();

  CBase* m_pCBSystem = CreateControlBuilderHSystemData();

  SH_Block::s_bProjectLoaded = false;
  e_RestResult res = RestoreProject( m_entryModule );
  switch( res )
  {
    case err_DatVerGreaterSrcVer:
      {
#ifdef _WIN32
        if( IDYES!=MessageBox( NULL, "Использовать текущую версию будете на свой страх и риск.\nПродолжить?"
                                   , "Устаревшая версия ControlBuilder5XX. Обновите ControlBuilder5XX.", MB_TOPMOST|MB_YESNO|MB_ICONWARNING|MB_DEFBUTTON2) )
        {
          return 0;
        }
#else
        return 3;
#endif
      }
    case err_WrongKey:
    case err_WrongVer:
    case err_CantOpen:
      {
        int r = ReadProject(m_entryModule);
        if( -2==r )
          break;
        if( r )
          return 2;
      }
      break;
    default:
    case err_CommonErr:
      return 1;
    case err_Ok:
      break;
  }
  SH_Block::s_bProjectLoaded = true;

  auto durationms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t).count();
  char szDebug[64*4];
  sprintf_s( szDebug, "время инициализации ControlBuilder %lld ms\n", durationms);
  OutputDebugString( szDebug );

  return 0;
}

void KControlBuilder::AppendTo( SH_Module *module, tBlockPtr_ &ptr )
{
  strcpy_s( module->szFile, ptr->szXmlFile.c_str() );
  vector<SH_Module*> vModules;
  vector<SH_Block*> vBlocks;
  for( tBlocksCont_::iterator it=ptr->m_EmbBlocks.begin(), end(ptr->m_EmbBlocks.end()); it!=end; ++it )
  {
    tConnectionsCont_ conIn;//, conOut;

    tBlockPtr_ bl = *it;
    string &szBlkName = bl->BlockName;
    for( tConnectionsCont_::iterator ic=bl->Connections.begin(), endC(bl->Connections.end()); ic!=endC; ++ic )
    {
      string &szIn = ic->InputEnd;
      string &szOut = ic->OutputEnd;
      size_t lb = szBlkName.length();

      if( szIn.length() > lb )
      {
        if( '.'==szIn.at( lb ) && !_strnicmp( szIn.c_str(), szBlkName.c_str(), lb ) )
        {
          char blkSplt[3][32*4] = {};
          int C = SplitString( szBlkName.c_str(), blkSplt, '.', false );
          ASSD(2==C);

          char outSplt[5][32*4] = {};
          C = SplitString( szOut.c_str(), outSplt, '.', false );
          ASSD(C>=2 && C<_countof(outSplt) );
          char nszOut[64*4] = {};
          switch( C )
          {
          case 2:
            {
              for( tBlocksCont_::iterator it2=ptr->m_EmbBlocks.begin(), end2(ptr->m_EmbBlocks.end()); it2!=end2; ++it2 )
              {
                tBlockPtr_ bl2 = *it2;
                string &szBlkName2 = bl2->BlockName;
                C = SplitString( szBlkName2.c_str(), blkSplt, '.', false );
                ASSD(2==C);
                if( !strcmp(blkSplt[1], outSplt[0]) )
                {
                  ASSD( !nszOut[0] );
                  sprintf_s( nszOut, "%s.%s.%s", blkSplt[0], outSplt[0], outSplt[1] );
                }
              }
              //sprintf_s( nszOut, "%s.%s.%s", blkSplt[0], outSplt[0], outSplt[1] );
            }
            break;
          }
          if( nszOut[0] )
            szOut = nszOut;

          conIn.push_back( *ic );
          continue;
        }
        else if( '.'==szOut.at( lb ) && !_strnicmp( szOut.c_str(), szBlkName.c_str(), lb ) )
        {
          //(*ic).bForwardConnection = false;
          conIn.push_back( *ic );
          continue;
        }
        else
        {
          KKK();
        }
      }
      else if( szOut.length()>lb )
      {
        if( '.'==szOut.at( lb ) && !_strnicmp( szOut.c_str(), szBlkName.c_str(), lb ) )
        {
          //(*ic).bForwardConnection = false;
          conIn.push_back( *ic );
          continue;
        }
        else
        {
          KKK();
        }
      }

      char blkSplt[2][32*4] = {};
      int C = SplitString( szBlkName.c_str(), blkSplt, '.', false );
      ASSD(2==C);
      char inSplt[4][32*4] = {};
      C = SplitString( szIn.c_str(), inSplt, '.', false );
      ASSD(C>=2 && C<4);
      ASSD( !strcmp(blkSplt[1],inSplt[0]) );
      char nszIn[64*4] = {};
      switch( C )
      {
      case 2:
        sprintf_s( nszIn, "%s.%s.%s", blkSplt[0], inSplt[0], inSplt[1] );
        break;
      case 3:
        sprintf_s( nszIn, "%s.%s.%s.%s", blkSplt[0], inSplt[0], inSplt[1], inSplt[2] );
        break;
      }
      szIn = nszIn;
      if( szIn.length() > lb )
      {
        if( '.'==szIn.at( lb ) && !_strnicmp( szIn.c_str(), szBlkName.c_str(), lb ) )
          conIn.push_back( *ic );
        else if( '.'==szOut.at( lb ) && !_strnicmp( szOut.c_str(), szBlkName.c_str(), lb ) )
          conIn.push_back( *ic );
        else
        {
          ASSD(0);
        }
      }
      else
      {
        ASSD(0);
      }
    }

    SH_Block *pB = NULL;
    SH_Module *pM = NULL;

    char AssignedTo[KBmBase::sizeofAssignedTo];
    char Container[KBmBase::sizeofContainer];
    char ref2Block[KBmBase::sizeofBlockName] = {};
    strcpy_s( AssignedTo, bl->AssignedTo.c_str() );
    strcpy_s( Container, bl->Container.c_str() );
    if( Container[0] && strcmp(AssignedTo, Container) )
    {
      char splt[5][KBmBase::sizeofBlockName] = {};
      int C = SplitString( bl->BlockName.c_str(), splt, '.' );
      ASS( C<=_countof(splt) );
      if( !strcmp(splt[0], Container) )
      {
        strcpy_s( ref2Block, AssignedTo );
        strcat_s( ref2Block, "." );
        strcat_s( ref2Block, bl->BlockName.c_str() );
      }
    }

    int o = Create( bl->BlockName.c_str(), bl->EntityName.c_str(), ref2Block, bl->tnClass.c_str(), &pM, &pB );
    switch( o )
    {
    case 1:
      AssignParams( pM, bl );
      vModules.push_back( pM );
      AppendTo( pM, bl);
      break;
    case 2:
      AssignParams( pB, bl );
      vBlocks.push_back( pB );
      break;
    default:
      if( ns_rsu::L35_11600_KF==rsuProjectVersion() )
      {
        if( bl->tnNamespace=="SNCLIB")
          continue;
      }
      if( ns_rsu::LG35_8_KF==rsuProjectVersion() )
      {
        if( bl->tnNamespace=="SNCLIB")
          continue;
      }
      ASSD(0);
      continue;
    }    
    if( !conIn.empty() )
    {
      SConnectionsCont *pCon = NULL;
      if( pB )
      {
        pCon = pB;
        ASS( !pM );
      }
      if( pM )
      {
        pCon = pM;
        ASS( !pB );
      }
      pCon->inConsC = (DWORD)conIn.size();
      size_t memSize = pCon->inConsC * sizeof(*pCon->pInConns);
      pCon->pInConns = (SConnectionMB*)KMemoryServiceH5xx::Instance().NewMem(memSize);
      int i = 0;
      for( tConnectionsCont_::iterator ic=conIn.begin(), endC(conIn.end()); ic!=endC; ++ic, ++i )
      {
        tConnectionsCont_::value_type &con = *ic;
        pCon->pInConns[i].g_throughNumb = g_throughNumb++;
        if( !con.bForwardConnection )
          pCon->pInConns[i].direction = SConnectionMB::eBackward;
        size_t cI =std::count( con.InputEnd.begin(), con.InputEnd.end(), '.' );
        size_t cO =std::count( con.OutputEnd.begin(), con.OutputEnd.end(), '.' );

        string::size_type pI = con.InputEnd.rfind('.');
        size_t cPointsI = std::count( con.InputEnd.begin(), con.InputEnd.end(), '.' );
        if( 3==cI )
          pI = con.InputEnd.rfind('.', pI-1 );
        string::size_type pO = con.OutputEnd.rfind('.');
        size_t cPointsO = std::count( con.OutputEnd.begin(), con.OutputEnd.end(), '.' );
        if( 3==cO )
          pO = con.OutputEnd.rfind('.', pO-1);
        ASS( pI );
        ASS( pO );
        if( 1==cPointsI )
          sprintf_s( pCon->pInConns[i].szInObj, "%s.", module->BlockName );
        strncpy_s( pCon->pInConns[i].szInObj, con.InputEnd.c_str(), pI );
        strcpy_s( pCon->pInConns[i].szInFld, con.InputEnd.c_str()+pI+1 );
        strncat( pCon->pInConns[i].szOutObj, con.OutputEnd.c_str(), pO );
        strcpy_s( pCon->pInConns[i].szOutFld, con.OutputEnd.c_str()+pO+1 );
      }
    }
  }
  
  if( !vModules.empty() )
  {
    module->nSubModules = vModules.size();
    size_t memSize = module->nSubModules * sizeof(SH_Module*);
    module->ppSubModules = (SH_Module**)KMemoryServiceH5xx::Instance().NewMem( memSize );
    ZeroMemory( module->ppSubModules, memSize );
    for( size_t i=0; i<module->nSubModules; ++i )
    {
      module->ppSubModules[i] = vModules[i];
    }
  }
  if( !vBlocks.empty() )
  {
    module->nBlocks = vBlocks.size();
    size_t memSize = module->nBlocks * sizeof(SH_Block*);
    module->ppBlocks = (SH_Block**)KMemoryServiceH5xx::Instance().NewMem( memSize );
    ZeroMemory( module->ppBlocks, memSize );
    for( size_t i=0; i<module->nBlocks; ++i )
    {
      module->ppBlocks[i] = vBlocks[i];
    }
  }  
}

int KControlBuilder::Create( LPCSTR dcuName, LPCSTR hmiName, LPCSTR pszRef2RealBlock, LPCSTR pType, SH_Module **pM, SH_Block **pB )
{
  ASS( hmiName && *hmiName );
  if( !hmiName || !*hmiName )
    hmiName = dcuName;

  if( strcmp( dcuName, hmiName) )
  {
    char msg[256];
    sprintf_s( msg, "!!!!! %s != %s (HMI)\n", dcuName, hmiName );
    OutputDebugString( msg );
  }
#pragma message( "!!!!!  пока будет одно и то же имя\n")
  hmiName = dcuName;

  LPCSTR pTypeFull = pType;
  int r = 0;
  LPCSTR p = strchr( pType, '.');
  if( p )
    pType = p+1;
  if( pM )
  {
    *pM = NULL;
    *pM = SModuleCreate::Create( dcuName, hmiName, pType );
    if( !*pM )
      *pM = SModuleCreate::Create( dcuName, hmiName, pTypeFull );
    if( *pM )
    {
      r = 1;
      (*pM)->SetParmList();
    }
  }
  if( pB && !*pM )
  {
    *pB = SBlockCreate::Create( dcuName, hmiName, pszRef2RealBlock, pType );
    if( !*pB )
      *pB = SBlockCreate::Create( dcuName, hmiName, pszRef2RealBlock, pTypeFull );
    if( *pB )
    {
      r = 2;
      (*pB)->SetParmList();
    }
  }
  //ASSD(r);
  if( !r )
  {
    OutputDebugString("Не создан блок   ");OutputDebugString( dcuName );
    OutputDebugString("   типа   ");OutputDebugString( pType ); OutputDebugString("\n");
  }
  return r;
}

void KControlBuilder::AssignParams( KBmBase *obj, tBlockPtr_ &ptr )
{
  strcpy_s( obj->AssignedTo, ptr->AssignedTo.c_str() );
  strcpy_s( obj->Container, ptr->Container.c_str() );
  obj->SetParmList();
  char tmp[_countof(obj->BlockName)];
  strcpy_s( tmp, obj->BlockName );
  _Ktoupper(tmp);
  LPSTR p = tmp;
  p = strchr( tmp, '.' );
  if( !p )
    p = tmp;
  else
    p++;
  obj->SetValue("NAME", p );
  obj->SetValue("ITEMNAME", p );
  for( tParamsCont_::iterator it=ptr->Parameters.begin(), end(ptr->Parameters.end()); it!=end; ++it )
  {
    LPCSTR pVal = it->second.c_str();
    if( !*pVal )
      continue;
    LPCSTR pField = it->first.c_str();
    obj->SetValue( pField, pVal );
    KKK();
  }
}
