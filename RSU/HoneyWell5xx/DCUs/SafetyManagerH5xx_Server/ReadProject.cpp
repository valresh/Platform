#include "SafetyManager.h"
#ifdef _WIN32
#include <io.h>
#endif
#include <macros/AutoCloser.h>
#include <macros/NamedCsv.h>
#include <macros/StrHelps.h>
#include <crossplatform.h>

using namespace std;

extern int g_chVer;

int CompInfo( const void * p1, const void * p2 )
{
  CFSC_Info * pI1 = (CFSC_Info*)p1;
  CFSC_Info * pI2 = (CFSC_Info*)p2;
  int res1 = _stricmp( pI1->NAME, pI2->NAME );
  if( res1 )
    return res1;

  if( pI1->PointType==CFSC_Info::hz || pI2->PointType==CFSC_Info::hz )
    return res1;

  if( pI1->PointType < pI2->PointType )
    return -1;
  if( pI1->PointType > pI2->PointType )
    return 1;
  return 0;
}

struct SNameWithDispls
{
  std::string name;
  std::string PointDetailDisplay;
  std::string GroupDetailDisplay;
  std::string AssociatedDisplay;
};

int KSafetyManager::ReadProject( LPCSTR pszPrj )
{
  char szPath[_MAX_PATH] = {};

  typedef std::map<int, SNameWithDispls> tNameByNumb_;
  tNameByNumb_ fldNames;
  {
    char tmp[64*4];
    sprintf_s( tmp, "%s_FLDProperties", pszPrj );
    GetPathToSmFile( szPath, pszPrj, tmp, ".csv" );
    FILE *pFileIn = NULL;
    bool res = fopen_s( &pFileIn, szPath, "rt" );
    if( res )
      return -10;
    KAutoCloser<FILE*> file( pFileIn, fclose );
    char szLine[2048] = {};
    while( fgets( szLine, _countof(szLine)-1, pFileIn ) )
    {
      szLine[strcspn(szLine, "\r\n")] = 0;
      //LPSTR end = strrchr( szLine, '\n' );
      //if( end )
      //  *end = 0;
      char row[18][128] = { };
      int C = SplitString( szLine, row, ';', false );
      ASSD( C < _countof(row) );
      tNameByNumb_ fld;
      int nFld = atoi( row[0] );
      if( !nFld )
        continue;
      SNameWithDispls nwd;
      nwd.name = row[1];
      if( nwd.name.empty() )
        continue;
      nwd.PointDetailDisplay = row[14];
      nwd.GroupDetailDisplay = row[15];
      nwd.AssociatedDisplay = row[16];
      fldNames[nFld] = nwd;
    }
  }


  GetPathToSmFile( szPath, pszPrj, pszPrj, ".csv" );

  int Res = _access( szPath, 4 );
  ASSD( !Res );
  if( Res )
    return -1;

  FILE *pFileIn = NULL;
  bool res = fopen_s( &pFileIn, szPath, "rt" );
  if( res )
    return -2;
  KAutoCloser<FILE*> file( pFileIn, fclose );
  char szLine[2256] = {};
  fgets( szLine, _countof(szLine)-1, pFileIn );
  szLine[strcspn(szLine, "\r\n")] = 0;
  //LPSTR end = strrchr( szLine, '\n' );
  //if( end )
  //  *end = 0;

  aCSVRow_ row(';');
  {
    stringstream lineStr;
    lineStr << szLine;
    lineStr >> row;
  }
  KNamedCsv header( row.m_data );

  int countComAllocationType = 0;
  {
    size_t L = strlen( "ComAllocationType" );
    for( int i=0, e(header.GetCountNames()); i<e; ++i )
    {
      LPCSTR pszName = header.GetName4( i );
      if( strncmp(pszName, "ComAllocationType", L ) )
        continue;
      int n = atoi( pszName+L );
      countComAllocationType = std::max<int>( countComAllocationType, n );
    }
  }


  GetInMemFilePath( szPath, pszPrj, GetCsvPrefix(), "csv" );
  KAutoCloser<FILE*> fAI( fopen ( szPath, "wt" ), fclose);

  /*GetInMemFilePath( szPath, pszPrj, "Tags", "csv" );
  KAutoCloser<FILE*> _F( fopen ( szPath, "wt" ), fclose);*/

  if( NULL!=fAI )
    fprintf( fAI, "ED#Точка;ED#Alt;TypeDV#Тип;ST#Описание;LE,Objects#Модель;LS,Type#ТипМодели;Points#ACS;ED#Формула;ST#EngUnit;\n" );

  vector<CFSC_Info> FSC_Info;
  FSC_Info.resize(CFSC_Info::MAX_FSC_INFO);
  int kFSC_Info = 0;

  int nLine = 1;

  while( fgets( szLine, _countof(szLine)-1, pFileIn ) )
  {
    szLine[strcspn(szLine, "\r\n")] = 0;
    //LPSTR end = strrchr( szLine, '\n' );
    //if( end )
    //  *end = 0;
    ++nLine;
    {
      stringstream lineStr;
      lineStr << szLine;
      lineStr >> row;
    }

    ASSD( kFSC_Info < CFSC_Info::MAX_FSC_INFO );
    CFSC_Info * pI = &FSC_Info[kFSC_Info];
    ZeroMemory( pI, sizeof(*pI) );

    strcpy_s( pI->NAME, row[header.GetIndex("TagNumber")].c_str() );
    strcpy_s( pI->szPowerUpValue, row[header.GetIndex("PowerUpValue")].c_str() );
    if( '\0'==pI->szPowerUpValue[1] && '0'==pI->szPowerUpValue[0] )
      pI->szPowerUpValue[0] = '\0';

    strcpy_s( pI->szFLDNumber, row[header.GetIndex("FLDNumber")].c_str() );
    pI->nFLDNumber = atoi(pI->szFLDNumber);
    tNameByNumb_::iterator itFN = fldNames.find( pI->nFLDNumber );
    if( fldNames.end()!=itFN )
    {
      tNameByNumb_::mapped_type &nwd = itFN->second;
      strcpy_s( pI->szFldName, nwd.name.c_str() );
      strcpy_s( pI->szPointDetailDisplay, nwd.PointDetailDisplay.c_str() );
      strcpy_s( pI->szGroupDetailDisplay, nwd.GroupDetailDisplay.c_str() );
      strcpy_s( pI->szAssociatedDisplay, nwd.AssociatedDisplay.c_str() );
    }
    else
    {
      ASSD( !pI->szFLDNumber[0] || '0'==pI->szFLDNumber[0] );
    }


    LPCSTR pState0Text = row[header.GetIndex("State0Text")].c_str();
    LPCSTR pState1Text = row[header.GetIndex("State1Text")].c_str();
    LPCSTR pNormalState = row[header.GetIndex("NormalState")].c_str();

    strcpy_s( pI->szStatesText[0], pState0Text );
    strcpy_s( pI->szStatesText[1], pState1Text );
    strcpy_s( pI->szNormalState, pNormalState );
    strcpy_s( pI->szEngineeringUnits, row[header.GetIndex("EngineeringUnits")].c_str() );
    strcpy_s( pI->szBottomScale, row[header.GetIndex("BottomScale")].c_str() );
    strcpy_s( pI->szTopScale, row[header.GetIndex("TopScale")].c_str() );

    strcpy_s( pI->szAlarmLimitHighHigh, row[header.GetIndex("AlarmLimitHighHigh")].c_str() );
    strcpy_s( pI->szAlarmLimitHigh, row[header.GetIndex("AlarmLimitHigh")].c_str() );
    strcpy_s( pI->szAlarmLimitLow, row[header.GetIndex("AlarmLimitLow")].c_str() );
    strcpy_s( pI->szAlarmLimitLowLow, row[header.GetIndex("AlarmLimitLowLow")].c_str() );

    if( *pState0Text && *pState1Text )
      snprintf( pI->DESC2, _countof(pI->DESC2)-1, "%s/%s", pState0Text, pState1Text );
    else if( *pState0Text )
      snprintf( pI->DESC2, _countof(pI->DESC2)-1, "%s", pState0Text );
    else if( *pState1Text )
      snprintf( pI->DESC2, _countof(pI->DESC2)-1, "%s", pState1Text );

    char Name[256] = {};
    {
      char szCATMS[64]={};
      for( int cat=1; cat<=countComAllocationType; ++ cat )
      {
        sprintf_s( szCATMS, "ComAllocationType%d", cat );
        strcpy_s( Name, row[header.GetIndex(szCATMS)].c_str() );
        if( !Name[0] )
          continue;
        /*if( CFSC_Info::catHZ!=pI->ComAllocationType )
        {
          if( strcmp( Name, "Input" ) == 0 )
          {
            ASSD( pI->ComAllocationType == CFSC_Info::catInput );
          }
          else if( strcmp( Name, "Output" ) == 0 )
          {
            ASSD( pI->ComAllocationType == CFSC_Info::catOutput );
          }
          else
          {
            ASSD(0);
          }
        }
        else */
        {
          if( strcmp( Name, "Input" ) == 0 )
            *(BYTE*)&pI->ComAllocationType[cat - 1] |= CFSC_Info::catInput;
          else if( strcmp( Name, "Output" ) == 0 )
            *(BYTE*)&pI->ComAllocationType[cat - 1] |= CFSC_Info::catOutput;
          else
          {
            ASSD(0);
          }
        }
        sprintf_s( szCATMS, "Master%d", cat );
        strcpy_s( pI->szMaster[cat - 1], row[header.GetIndex(szCATMS)].c_str() );
        sprintf_s( szCATMS, "Slave%d", cat );
        strcpy_s( pI->szSlave[cat - 1], row[header.GetIndex(szCATMS)].c_str() );
        sprintf_s( szCATMS, "PLCAddress%d", cat );
        strcpy_s( pI->szPLCADDR[cat - 1], row[header.GetIndex(szCATMS)].c_str() );
      }
    }

    for ( int i=0; i<6; ++i )
    {
		pI->PLCADDR[i] = atoi( pI->szPLCADDR[i] );
		if( pI->PLCADDR[i] < 0 )
		{
		  pI->PLCADDR[i] = 0;
		  pI->szPLCADDR[i][0] = 0;
		}
	}
    LPCSTR pLoc = row[header.GetIndex("Location")].c_str();
    if( fAI && *pLoc )
    {
      if( !(!strcmp(pLoc,"COM") || !strcmp(pLoc,"SYS")) )
      {
        //if( pI->PLCADDR )
        fprintf( fAI, "%s;PLC=%d/FLD=%s(%s);%s;%s;\n", row[header.GetIndex("TagNumber")].c_str(), pI->PLCADDR[0], pI->szFLDNumber, pLoc, row[0].c_str(), row[header.GetIndex("Description")].c_str() );
      }
    }
    /*fprintf ( _F, "0x%04X;%s;", nLine, pI->NAME );
    fprintf ( _F, "%s;", row[header.GetIndex("Unit")].c_str() );
    fprintf ( _F, "%s;", row[header.GetIndex("SubUnit")].c_str() );*/
    strcpy_s ( pI->DESC, row[header.GetIndex("Description")].c_str() );

    strcpy_s( Name, row[header.GetIndex("Location")].c_str() );
    bool NoType = false;
    if( lstrcmp ( Name, "COM" ) == 0 )
      pI->Type = CFSC_Info::COM;
    else
    {
      if( lstrcmp ( Name, "FLD" ) == 0 )
        pI->Type = CFSC_Info::FLD;
      else
      {
        NoType = true;
        //strcpy_s ( pI->DESC2, sizeof( pI->DESC2 ), Name );
      }
    }
    strcpy_s( pI->szTypeLocation, Name );
    if( !pI->szTypeLocation[0] )
    {}
    else if( !strcmp(pI->szTypeLocation, "SYS") )
      pI->Type = CFSC_Info::SYS;
    else if( !strcmp(pI->szTypeLocation, "COM") )
      pI->Type = CFSC_Info::COM;
    else if( !strcmp(pI->szTypeLocation, "FLD") )
      pI->Type = CFSC_Info::FLD;
    else if( !strcmp(pI->szTypeLocation, "CAB") )
      pI->Type = CFSC_Info::CAB;
    /*else if( !strcmp(pI->szTypeLocation, "FFB") )
      pI->Type = CFSC_Info::FFB;*/
    else if( !strcmp(pI->szTypeLocation, "FSC") )
      pI->Type = CFSC_Info::FSC;
    else if( !strcmp(pI->szTypeLocation, "PNL") )
      pI->Type = CFSC_Info::PNL;
    else if( !strcmp(pI->szTypeLocation, "MOV") )
      pI->Type = CFSC_Info::MOV;
    else if( !strcmp(pI->szTypeLocation, "BNS") )
      pI->Type = CFSC_Info::BNS;
    else if( !strcmp(pI->szTypeLocation, "MCC") )
      pI->Type = CFSC_Info::MCC;
    else
    {
      pI->Type = CFSC_Info::Other;
      //ASSD(0);
    }

    strcpy_s( pI->szDataType, row[header.GetIndex("DataType")].c_str() );
    strcpy_s( pI->szBlockName, row[header.GetIndex("BlockName")].c_str() );

    strcpy_s( pI->szPointType, row[header.GetIndex("PointType")].c_str() );
    switch( pI->szPointType[0] )
    {
    default:
      ASSD(0);
      break;
    case 'M':
      pI->PointType = CFSC_Info::M;
      switch( pI->szPointType[1] )
      {
      default:
        ASSD(0);
        break;
      case 0:
        break;
      case 'S':
        pI->PointType = CFSC_Info::MS;
        break;
      }
      break;
    case 'D':
      pI->DA = CFSC_Info::Discrete;
      switch( pI->szPointType[1] )
      {
      case 'I':
        pI->PointType = CFSC_Info::DI;
        pI->IO = CFSC_Info::In;
        break;
      case 'O':
        pI->PointType = CFSC_Info::DO;
        pI->IO = CFSC_Info::Out;
        break;
      }
      break;
    case 'A':
      pI->DA = CFSC_Info::Analog;
      switch( pI->szPointType[1] )
      {
      case 'I':
        pI->PointType = CFSC_Info::AI;
        pI->IO = CFSC_Info::In;
        break;
      case 'O':
        pI->PointType = CFSC_Info::AO;
        pI->IO = CFSC_Info::Out;
        break;
      default:
        ASSD(0);
      }
      break;
    case 'T':
      pI->PointType = CFSC_Info::T;
      break;
    case 'R':
      pI->PointType = CFSC_Info::R;
      break;
    case 'B':
      switch( pI->szPointType[1] )
      {
      case 'I':
        pI->PointType = CFSC_Info::BI;
        pI->IO = CFSC_Info::In;
        break;
      case 'O':
        pI->PointType = CFSC_Info::BO;
        pI->IO = CFSC_Info::Out;
        break;
      default:
        ASSD(0);
      }
      break;
    case 'C':
      pI->PointType = CFSC_Info::C;
      ASSD( !pI->szPointType[1] );
      break;
    }

    ++kFSC_Info;
  }

  GetDatFilePath( szPath, pszPrj );

  std::ofstream hFile(szPath, std::ios::binary | std::ios::out | std::ios::trunc);

  //KAutoCloser<HANDLE> hFile( CreateFile( szPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL ),
  //                           CloseHandle  );

  if( !hFile.is_open() )
  {
    ASSD(0)
    return -5;
  }

  qsort( &FSC_Info[0], kFSC_Info, sizeof( CFSC_Info ), CompInfo );
  for( int n = 0; n < kFSC_Info; n++ )
    FSC_Info[n].NUMB = n;

  m_Ready = FALSE;
  hFile.write( (const char*)& g_chVer, sizeof(g_chVer));
  hFile.write( (const char*)&kFSC_Info, sizeof(kFSC_Info) );
  hFile.write( (const char*)&m_Ready, sizeof(m_Ready) );
  hFile.write( (const char*)&FSC_Info[0], kFSC_Info * sizeof( CFSC_Info ) );

  return 0;
}
