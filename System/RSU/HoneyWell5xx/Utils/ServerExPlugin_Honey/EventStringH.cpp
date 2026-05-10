#include "EventStringH.h"
#include <BaseType_.h>
#include <UtilsH.h>
#include <HoneywellStruct.h>

KEventString::KEventString(void)
{
}

KEventString::~KEventString(void)
{
}

bool KEventString::Dbl( SSendToModel& send, double* v, SValueDef* def, LPCTSTR szName, LPCTSTR szComm, CBase* pBase )
{
  *v = send.dNew;
  if ( !(send.bMessage && send.dOld != send.dNew) ) return true;
  //
#if 0
  char szFiltr[128 * 4];
  sprintf_s( szFiltr, sizeof(szFiltr), "'%%s.%s' изменилось с %%s до %%s", send.szValue );
  //
  Double2( STDARG, send.dOld, send.dNew );
  //Event  ( STDARG, send.dOld, send.dNew );
#else
  int    Z = 5;
  double F = fabs(send.dNew-send.dOld);
  if ( F > 10.00 ) Z = 0;
  else if ( F > 1.000 ) Z = 1;
  else if ( F > 0.100 ) Z = 2;
  else if ( F > 0.010 ) Z = 3;
  else if ( F > 0.001 ) Z = 4;
  char szD1[64 * 4]; DoubleToStr( szD1, send.dNew, Z );
  char szD2[64 * 4]; DoubleToStr( szD2, send.dOld, Z );
  //
  static char* szFiltr = "%-17s%-25s%-4s old=%s";
  Event( szFiltr, szName, szComm, szD1, szD2 );
#endif
  return true;
}
/*
struct SModeNames
{
  LPCSTR pName;
  SMode::tMode val;
};

const SModeNames g_Modes[] =
{
  { "O/S", SMode::OS },
  { "AUT", SMode::AUT },
  { "MAN", SMode::MAN },
  { "CAS", SMode::CAS },
  { "PRD", SMode::PRD },
  { "RCAS", SMode::RCAS },
  { "ROUT", SMode::ROUT },
  { NULL, 0 },
};

LPCSTR GetModeName( int val )
{
  const SModeNames *modes = &g_Modes[0];
  for( ; modes->pName ; modes++ )
  {
    if( modes->val==val )
      return modes->pName;
  }

  return NULL;
}
*/
bool KEventString::Int( SSendToModel& send, int* v, SValueDef* def, LPCTSTR szName, LPCTSTR szComm, CBase* pBase )
  {
  *v  = send.nNew;
  if ( !(send.bMessage && send.nOld != send.nNew) )
    return true;
  
  /*const char *New, *Old;
  if ( def->nEnum != 0 &&  !_strcmpi( def->name, "MODE" ))
  {
    New = GetModeName( send.nNew );
    Old = GetModeName( send.nOld );
    if( New && Old )
    {
      static char* szFiltr = "%-17s%-25s%-4s old=%s";
      Event( szFiltr, szName, szComm, New, Old );
      return true;
    }
  }*/
  //
#if 0
  static char* szFiltr = "'%s.%s' изменилось с %d до %d";
  Event( szFiltr, send.szName, send.szValue, send.nOld, send.nNew );
#else
  static char* szFiltr = "%-17s%-25s%-4d old=%d";
  Event( szFiltr, szName, szComm, send.nNew, send.nOld );
#endif
  return true;
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool KEventString::Chr( SSendToModel& send, char* v, SValueDef* def, LPCTSTR szName, LPCTSTR szComm, CBase* pBase )
{
  *v = send.cNew;
  //
  if ( !(send.bMessage && send.cOld != send.cNew) ) 
    return true;
  //
  const char *New, *Old;
  if ( def->nEnum != 0 )
    {
    New = EnumStrH( *def, send.cNew );
    Old = EnumStrH( *def, send.cOld );
    }
  else
    {
    static char a[16];
    static char b[16];
    sprintf_s( a, 16, "%d", send.cNew );New = a; 
    sprintf_s( b, 16, "%d", send.cOld );Old = b;
    }
  //
  static char* szFiltr = "%-17s%-25s%-4s old=%s";
  Event( szFiltr, szName, szComm, New, Old );
  return true;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool KEventString::Str( SSendToModel& send, char* v, SValueDef* def, LPCTSTR szName, LPCTSTR szComm, CBase* pBase )
  {
  char New[_countof(send.sNew)] = {0}, Old[_countof(send.sOld)] = {0};
  memcpy( New, send.sNew, sizeof(New) );New[_countof(New)-1] = 0;
  memcpy( Old, send.sOld, sizeof(Old) );Old[_countof(Old)-1] = 0;
  if( def->dwSize > 0 )
    strncpy( v, New, def->dwSize );
  else
    strcpy( v, New );
  //
  if ( !(send.bMessage && lstrcmp(New,Old) != 0 ) ) return true;
  //
  static char* szFiltr = "%-17s%-25s%-4s old=%s";
  Event( szFiltr, szName, szComm, New, Old );
  return true;
  }

//
SValueDef* GetDef( SSendToModel& send )
{
  SValueDef* def = NameToValueH( send.eData, send.szValue );
  if( def == NULL && enumValueUnk!=send.def.eVal )
  {
    def = &send.def;
    /*SMessage* mess = Mess(send);
    if ( mess != NULL )
      def = mess->def;*/
  }
  return def;
}

bool KEventString::Put( SSendToModel& send, CBase* pBase )
  {
  SValueDef* def = GetDef( send );
  if ( def == NULL )
    {
    static char* szFiltr = "Не найдено сообщение '%s'.'%s'";
    Event( szFiltr, send.szName, send.szValue );
    return false;
    }
  //
  if ( def->eVal != send.eType )
    {
    static char* szFiltr = "Не совпадают типы переменной '%s'.'%s'";
    Event( szFiltr, send.szName, send.szValue );
    return false;
    }
  //
  LPCTSTR szComm = "";
  static char* noTag = "Без комментария";

  char szName[64 * 4];
  sprintf_s( szName, sizeof(szName), "%s.%s", send.szName, send.szValue );
  //
  void* v = (void*)((char*)(pBase) + def->dwShift);
  switch ( send.eType )
    {
    case enumValueDbl: return Dbl(send,(double*)v,def,szName,szComm,pBase);
    case enumValueInt: return Int(send,(int   *)v,def,szName,szComm,pBase);
    case enumValueChr: return Chr(send,(char  *)v,def,szName,szComm,pBase);
    case enumValueStr: return Str(send,(char  *)v,def,szName,szComm,pBase);
    };
  static char* szFiltr = "Что-то непонятное '%s'.'%s'";
  Event( szFiltr, send.szName, send.szValue );
  send.bMessage = false;
  return true;
  }
//
