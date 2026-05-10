#include "Y_MainLogic.h"
#include <float.h>

static LPCSTR s_GAIN_NAMES[] = { "Data_GAINin", "Data_GAIN", "GAIN" };
template<size_t _S>
void ExtructGain( char (&szGain)[_S], CY_Base *pObj )
{
  szGain[0] = 0;
  if( !pObj )
    return;
  void *pVar = NULL;
  CY_Base::eVarType Work = CY_Base::evtP;
  char varType = ' ';
  for( int i=0; i<_countof(s_GAIN_NAMES); ++i )
  {
    if( !pObj->GetVar( (char*)s_GAIN_NAMES[i], &pVar, NULL, &varType, NULL ) )
      continue;
    if( 'D'!=varType )
    {
      ASSD(0);
      continue;
    }
    double dblGain = *(double*)pVar;
    if( 0.==dblGain || 1.==dblGain || DBL_MAX==dblGain )
      continue;
    sprintf_s( szGain, "GAIN=%f", dblGain );
    for( size_t j=strlen( szGain )-1; j>0; --j )
    {
      if( '.'==szGain[j-1] )
        break;
      if( '0'==szGain[j] )
        szGain[j] = 0;
      else
        break;
    }
    break;
  }

}

//#define GET_GAINS
//#define GET_EQUIPMENT
//#define GET_MCX_TYPEOUT
//#define HARDWARE_NAMES

CY_Base * CY_MainLogic::FormCsvRecord( FILE *F, CY_DI *pY, int &bNeedStationNumb, int n, bool bOutIfEmpty, LPCSTR pszObj, LPCSTR pExComment, int &line )
{
  if( pY->bWrote2Csv )
    return NULL;
  char Model[256] = { 0 };
  char Point[256] = { 0 };
  char cMes[64] = { 0 };
#ifdef GET_GAINS
  char szGain[128] = {};
#endif

  char szObjComment[128] = { 0 };
  char szPointComment[128] = { 0 };

  int numbFcs = pY->NumbFCS;
  if( m_pCentumSystem->domains[1] )
    numbFcs += pY->domain * CY_DR::s_dwFcsNumModulus;

  if ( pY->Y_Type == id__Z )
  {
    switch( pY->IOZ )
    {
    case 'I':
    case 'O':
      break;
    default: 
      return NULL;
    }

    CY_Base *pObj = SetModel( pY->Name, pY->modelTypeZ, pY->IOZ, Model, Point, cMes, pY->NumbFCS, pY->domain );
#ifdef GET_GAINS
    ExtructGain( szGain, pObj );
#endif
    if( pObj && pObj->pComm )
      strcpy_s( szObjComment, pObj->pComm );
    CorrectComment( szObjComment );

#ifndef HARDWARE_NAMES
    if( (n+1) < kD_Obj )
    {
      if( !strcmp(pY->Name, pD_Obj[n+1]->Name ) && (CY_Base::ectRSU==pD_Obj[n+1]->controllerType) )
      {
        if( bNeedStationNumb <= 0 )
          bNeedStationNumb = 2;
        else
          ++bNeedStationNumb;
      }
    }

    if( (pY->NumbFCS>=0 && !strncmp(pY->Name,"%Z", 2)) || bNeedStationNumb>0 )
    {
      fprintf( F, "%d:", numbFcs );
      --bNeedStationNumb;
    }
    fprintf ( F, "%s;", pY->Name);
#else
    fprintf ( F, "%d:%s;", pY->NumbFCS, pY->szNumber);
#endif
    ++line;
    pY->bWrote2Csv = true;

#ifdef GET_GAINS
    if( szGain[0] )
      fprintf( F, szGain );
    fprintf ( F, ";" );
#endif
#ifdef GET_EQUIPMENT
    if( pObj )
      fprintf ( F, "%s", pObj->szUPPL);
    fprintf ( F, ";" );
#endif
#ifdef GET_MCX_TYPEOUT
    if( pObj )
    {
      if( !strncmp( pObj->Model, "MC-", 3) )
        fprintf ( F, "%s", ((CY_MC_NE*)pObj)->TypeOut );
    }
    fprintf ( F, ";" );
#endif

    if( pY->pComm )
      strcpy_s( szPointComment, pY->pComm );
    CorrectComment( szPointComment );
    if( pObj )
      fprintf ( F, "%s", pObj->Name);
    if( pszObj )
      fprintf ( F, "%s", pszObj);

    fprintf ( F, ";%c%c;", (char)pY->modelTypeZ, pY->IOZ );

    int o = 0;
    if( szPointComment[0] )
    {
      fprintf ( F, "%s", szPointComment );
      ++o;
    }
    if( pExComment && *pExComment )
    {
      fprintf ( F, "%s", pExComment );
      ++o;
    }
    if( szObjComment[0] && _strcmpi(szPointComment, szObjComment) )
      fprintf ( F, !o ? "%s" : "(%s)", szObjComment );
    fprintf ( F, ";%s;;%s;;%s\n", Model, Point,cMes );

    return pObj;
  }
  else if ( pY->Y_Type == id__Y )
  {
    switch( pY->IOZ )
    {
    case 'I':
    case 'O':
      break;
    default: 
      return NULL;
    }

    CY_Base *pObj = SetModel( pY->Name, pY->modelTypeZ, pY->IOZ, Model, Point, cMes, pY->NumbFCS, pY->domain );
#ifdef GET_GAINS
    ExtructGain( szGain, pObj );
#endif
    if( pObj && pObj->pComm )
      strcpy_s( szObjComment, pObj->pComm );
    CorrectComment( szObjComment );

#ifndef HARDWARE_NAMES
    if( (n+1) < kD_Obj )
    {
      if( !strcmp(pY->Name, pD_Obj[n+1]->Name ) && (CY_Base::ectRSU==pD_Obj[n+1]->controllerType) )
      {
        if( bNeedStationNumb <= 0 )
          bNeedStationNumb = 2;
        else
          ++bNeedStationNumb;
      }
    }

    if( (pY->NumbFCS>=0 && !strncmp(pY->Name,"%Y", 2)) || bNeedStationNumb>0 )
    {
      fprintf( F, "%d:", numbFcs );
      --bNeedStationNumb;
    }
    fprintf ( F, "%s;", pY->Name);
#else
    fprintf ( F, "%d:%s;", pY->NumbFCS, pY->szNumber);
#endif
    ++line;
    pY->bWrote2Csv = true;

#ifdef GET_GAINS
    if( szGain[0] )
      fprintf( F, szGain );
    fprintf ( F, ";" );
#endif
#ifdef GET_EQUIPMENT
    if( pObj )
      fprintf ( F, "%s", pObj->szUPPL);
    fprintf ( F, ";" );
#endif
#ifdef GET_MCX_TYPEOUT
    if( pObj )
    {
      if( !strncmp( pObj->Model, "MC-", 3) )
        fprintf ( F, "%s", ((CY_MC_NE*)pObj)->TypeOut );
    }
    fprintf ( F, ";" );
#endif

    if( pY->pComm )
      strcpy_s( szPointComment, pY->pComm );
    CorrectComment( szPointComment );
    if( pObj )
      fprintf ( F, "%s", pObj->Name);
    if( pszObj )
      fprintf ( F, "%s", pszObj);

    fprintf ( F, ";%c%c;", (char)pY->modelTypeZ, pY->IOZ );

    int o = 0;
    if( szPointComment[0] )
    {
      fprintf ( F, "%s", szPointComment );
      ++o;
    }
    if( pExComment && *pExComment )
    {
      fprintf ( F, "%s", pExComment );
      ++o;
    }
    if( szObjComment[0] && _strcmpi(szPointComment, szObjComment) )
      fprintf ( F, !o ? "%s" : "(%s)", szObjComment );
    fprintf ( F, ";%s;;%s;;%s\n", Model, Point,cMes );

    return pObj;
  }
  else if ( pY->Y_Type == id__WW || pY->Y_Type == id__XW )
  {
#ifndef HARDWARE_NAMES
    if( (n+1) < kD_Obj )
    {
      if( !strcmp(pY->Name, pD_Obj[n+1]->Name ) && (CY_Base::ectRSU==pD_Obj[n+1]->controllerType) )
      {
        if( bNeedStationNumb <= 0 )
          bNeedStationNumb = 2;
        else
          ++bNeedStationNumb;
      }
    }
#endif
    pY->bWrote2Csv = true;
    ++line;
    CY_Base *pObj = SetModel( pY->Name, pY->modelTypeZ, pY->IOZ, Model, Point, cMes, pY->NumbFCS, pY->domain );
#ifdef GET_GAINS
    ExtructGain( szGain, pObj );
#endif
    if( pObj && pObj->pComm )
      strcpy_s( szObjComment, pObj->pComm );
    CorrectComment( szObjComment );
#ifndef HARDWARE_NAMES
    if( (pY->NumbFCS>=0 && (!strncmp(pY->Name,"%WW", 3) || !strncmp(pY->Name,"%XW", 3)) ) || bNeedStationNumb>0 )
    {
      fprintf( F, "%d:", numbFcs );
      --bNeedStationNumb;
    }
    fprintf ( F, "%s;", pY->Name);
#else
    fprintf ( F, "%d:%s;", pY->NumbFCS, pY->szNumber);
#endif

#ifdef GET_GAINS
    if( szGain[0] )
      fprintf( F, szGain );
    fprintf ( F, ";" );
#endif
#ifdef GET_EQUIPMENT
    if( pObj )
      fprintf ( F, "%s", pObj->szUPPL);
    fprintf ( F, ";" );
#endif
#ifdef GET_MCX_TYPEOUT
    if( pObj )
    {
      if( !strncmp( pObj->Model, "MC-", 3) )
        fprintf ( F, "%s", ((CY_MC_NE*)pObj)->TypeOut );
    }
    fprintf ( F, ";" );
#endif

    if( pY->pComm )
      strcpy_s( szPointComment, pY->pComm );
    CorrectComment( szPointComment );
    if( pY->modelTypeZ!=pY->pointTypeZ )
      fprintf( F, "(%c)", (char)pY->pointTypeZ );
    if( pObj )
      fprintf ( F, "%s", pObj->Name);

    fprintf ( F, ";%c%c;", (char)pY->modelTypeZ, pY->IOZ );
    if( szPointComment[0] )
      fprintf ( F, "%s", szPointComment );
    if( szObjComment[0] && _strcmpi(szPointComment, szObjComment) )
      fprintf ( F, szPointComment[0] ? "(%s)" : "%s", szObjComment );
    fprintf ( F, ";%s;;%s;;%s\n", Model, Point,cMes );
    return pObj;
  }
  else if ( pY->Y_Type == id__WB )
  {
#ifndef HARDWARE_NAMES
    if( (n+1) < kD_Obj )
    {
      if( !strcmp(pY->Name, pD_Obj[n+1]->Name ) && (CY_Base::ectRSU==pD_Obj[n+1]->controllerType) )
      {
        if( bNeedStationNumb <= 0 )
          bNeedStationNumb = 2;
        else
          ++bNeedStationNumb;
      }
    }
#endif
    pY->bWrote2Csv = true;
    ++line;
    CY_Base *pObj = SetModel( pY->Name, pY->modelTypeZ, pY->IOZ, Model, Point, cMes, pY->NumbFCS, pY->domain );
#ifdef GET_GAINS
    ExtructGain( szGain, pObj );
#endif
    if( pObj && pObj->pComm )
      strcpy_s( szObjComment, pObj->pComm );
    CorrectComment( szObjComment );
#ifndef HARDWARE_NAMES
    if( (pY->NumbFCS>=0 && !strncmp(pY->Name,"%WB", 3)) || bNeedStationNumb>0 )
    {
      fprintf( F, "%d:", numbFcs );
      --bNeedStationNumb;
    }
    fprintf ( F, "%s;", pY->Name);
#else
    fprintf ( F, "%d:%s;", pY->NumbFCS, pY->szNumber);
#endif

#ifdef GET_GAINS
    if( szGain[0] )
      fprintf( F, szGain );
    fprintf ( F, ";" );
#endif
#ifdef GET_EQUIPMENT
    if( pObj )
      fprintf ( F, "%s", pObj->szUPPL);
    fprintf ( F, ";" );
#endif
#ifdef GET_MCX_TYPEOUT
    if( pObj )
    {
      if( !strncmp( pObj->Model, "MC-", 3) )
        fprintf ( F, "%s", ((CY_MC_NE*)pObj)->TypeOut );
    }
    fprintf ( F, ";" );
#endif

    if( pY->pComm )
      strcpy_s( szPointComment, pY->pComm );
    CorrectComment( szPointComment );
    if( pObj )
      fprintf ( F, "%s", pObj->Name);

    if( CY_DI::eptUnknown!=pY->modelTypeZ )
      fprintf( F, ";%c", (char)pY->modelTypeZ );
    else if( CY_DI::eptUnknown!=pY->pointTypeZ )
      fprintf( F, ";%c", (char)pY->pointTypeZ );
    else
      fprintf( F, "; " );
    fprintf ( F, "%c;", pY->IOZ );

    if( szPointComment[0] )
      fprintf ( F, "%s", szPointComment );
    if( szObjComment[0] && _strcmpi(szPointComment, szObjComment) )
      fprintf ( F, szPointComment[0] ? "(%s)" : "%s", szObjComment );
    fprintf ( F, ";%s;;%s;;%s\n", Model, Point,cMes );
    return pObj;
  }
  return NULL;
}
