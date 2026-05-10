#include "stdafx.h"
#define EXT_EXP
#include "Extensions.h"

struct CExtInfo
  {
  char * Group;
  char * Name;
  char * Dll;
  char * Section;
  bool Default;
  };


CExtInfo ExtInfo[] = {
"Форма объекта", "НЕ задана",                       "Lev_123", "PROJECT", 0, 
"Форма объекта", "Вертикальный цилиндр",            "Lev_123", "PROJECT", 1,
"Форма объекта", "Горизонтальный цилиндр",          "Lev_123", "PROJECT", 0,
"Форма объекта", "Однородный вертикальный цилиндр", "Lev_123", "PROJECT", 0,
"Форма объекта", "С отстойником",                   "Lev_123", "PROJECT", 0,
//================================
//"Тип фазы 1", "НЕ задан", IGamma::ID, "Gamma", "PROJECT", 0, 
"Тип фазы 1", "Углеводороды",                     "Gamma", "PROJECT", 1,
"Тип фазы 1", "Универсальный абсорбент",          "Gamma", "PROJECT", 0, 
"Тип фазы 1", "Газы в воде",                      "Gamma", "PROJECT", 0, 
"Тип фазы 1", "Не задается",                      "Gamma", "PROJECT", 0, 
"Тип фазы 1", "Полимер в углеводородах",          "Gamma", "PROJECT", 0,
//================================
"Тип фазы 2", "НЕ задан",                         "Gamma", "PROJECT", 0, 
"Тип фазы 2", "Универсальный абсорбент",          "Gamma", "PROJECT", 0, 
"Тип фазы 2", "Вода",                             "Gamma", "PROJECT", 1,
"Тип фазы 2", "Газы в воде",                      "Gamma", "PROJECT", 0, 
"Тип фазы 2", "Не задается",                      "Gamma", "PROJECT", 0, 
"Тип фазы 2", "Углеводороды в полимере",          "Gamma", "PROJECT", 1,
"Тип фазы 2", "Нет фазы 2",                       "Gamma", "PROJECT", 1,
//================================
"Процессы на тарелках", "НЕ задан",                 "Gamma", "PROJECT", 1, 
"Процессы на тарелках", "Ректификация у/в",         "Gamma", "PROJECT", 0, 
"Процессы на тарелках", "Универсальный абсорбент",  "Gamma", "PROJECT", 0, 
"Процессы на тарелках", "Газы в воде",              "Gamma", "PROJECT", 0, 
//================================
"Абсорбция", "Нет",                     "Gamma", "PROJECT", 1,
"Абсорбция", "Универсальный абсорбент", "Gamma", "PROJECT", 0,
"Абсорбция", "H2S+MEA",                 "Gamma", "PROJECT", 0,
"Абсорбция", "CO2+MEA",                 "Gamma", "PROJECT", 0,
"Абсорбция", "CO2+Карсол",              "Gamma", "PROJECT", 0,
"Абсорбция", "Ароматика+ТЭГ",           "Gamma", "PROJECT", 0,
//=Расширения для вешеств ===============================
"Взаимодействие", "Нет",               "ChemNH3", "PROJECT", 1,
"Взаимодействие", "Аммиак+Вода",       "ChemNH3", "PROJECT", 0,
//"Расширение состава", "НЕ задано",                  IExtChem::ID, "ChemH2SO4", "PROJECT", 1, 
//"Расширение состава", "Сера",                       IExtChem::ID, "ChemH2SO4", "PROJECT", 1, 
//=Химия ===============================
//"Полипропилен", "Полимеризация",  IReactions::ID, "PPChem", "PP_TSK", 1,
//"Парафины", "Осаждение",          IParaffineРrecipitation::ID, "DeparafinChem", "C400_YAR", 1,
//=Управление ===============================
"Управление", "НЕ задано", "IntegralCtrl", "PROJECT", 1,
"Управление", "Регулятор уровня", "IntegralCtrl", "PROJECT", 0,
//=Общие Gamma ===============================
"Gamma", "Аммиак+Вода",   "ChemNH3", "PROJECT", 0, 
//= ===============================
"Рабочие", "Эталонный состав",   "EtalonComp", "PROJECT", 0, 
"Реакции", "НЕ заданы",   "ReactionsMTBE", "PROJECT", 1, 
"Реакции", "МТБЭ",   "ReactionsMTBE", "PROJECT", 0, 
//= ===============================
"Gamma", "H2ONH3CO2",   "H2ONH3CO2", "PROJECT", 0, 
};

int kExtInfo = sizeof ( ExtInfo ) / sizeof ( CExtInfo );


bool GetPropList( CProp_L & Prop, char * Type, char PropList[256] )
  {
  int Nb = -1;
  for ( int n = 0; n < kExtInfo; n++ )
    {
    if ( lstrcmp ( ExtInfo[n].Group, Type ) == 0 )
      {
      Nb = n;
      break;
      }
    }
  if ( Nb < 0 )
    return false;
  memset ( PropList, 0, 256 );
  while( Nb < kExtInfo )
    {
    if ( lstrcmp ( ExtInfo[Nb].Group, Type ) != 0 )
      break;
    lstrcat ( PropList, ExtInfo[Nb].Name );
    lstrcat ( PropList, ";" );
    Nb++;
    }
  return true;
  }

int GetIntList( char * Group, int MaxNames, char * Names[] )
  {
  int kNames = 0;
  for ( int n = 0; n < kExtInfo; n++ )
    {
    if ( lstrcmp ( ExtInfo[n].Group, Group ) == 0 )
      {
      if ( kNames < MaxNames )
        Names[kNames++] = ExtInfo[n].Name;
      else
        {
        ASS(0)
        break;
        }
      }
    }
  return kNames;
  }


void GetDefault( CProp_L & Prop, char * Type )
  {
  int N = 0;
  int Nb = -1;
  for ( int n = 0; n < kExtInfo; n++ )
    {
    if ( lstrcmp ( ExtInfo[n].Group, Type ) == 0 )
      {
      if ( Nb < 0 )
        Nb = n;
      if ( ExtInfo[n].Default )
        break;
      N++;
      }
    }
  ASS( Nb >= 0 )
  Prop.NumbFirstInExtInfo = Nb;
  Prop.NumbSelect = N;
  }

typedef 
void * (*tGetExtension) ( const char * Group, const char * Name );

#if 1
void * GetProp( CProp_L & Prop )
  {
  int N = Prop.NumbFirstInExtInfo + Prop.NumbSelect;
  if ( N < 0 || N >= kExtInfo )
    return NULL;
//
  // HMODULE hLib = NULL;
  // if( ! strcmp( ExtInfo[N].Section, "SYSTEM"))
  //   hLib = (HMODULE)Load_Dll( ExtInfo[N].Dll );
  // else
  //   hLib = (HMODULE)Load_Dll( ExtInfo[N].Dll, ExtInfo[N].Section );
  // if ( hLib == NULL )
  //   return NULL;

  // tGetExtension pGetExtension = (tGetExtension)GetProcAddress ( hLib, "GetExtension" );
  // if ( pGetExtension == NULL )
  //   return NULL;
  // void * pExt = (*pGetExtension) ( ExtInfo[N].Group, ExtInfo[N].Name );
  // return pExt;
  return NULL;
  }
#endif

void * GetProp( char * Group, char * Interface )
  {
  // int N = -1;
  // for ( int n = 0; n < kExtInfo; n++ )
  //   {
  //   if ( lstrcmp ( ExtInfo[n].Group, Group ) == 0 && lstrcmp ( ExtInfo[n].Name, Interface ) == 0 )
  //     {
  //     N = n;
  //     break;
  //     }
  //   }
  // if ( N < 0 || N >= kExtInfo )
  //   return NULL;
  // //
  // HMODULE hLib = NULL;
  // if( ! strcmp( ExtInfo[N].Section, "SYSTEM"))
  //   hLib = (HMODULE)Load_Dll( ExtInfo[N].Dll );
  // else
  //   hLib = (HMODULE)Load_Dll( ExtInfo[N].Dll, ExtInfo[N].Section );
  // if ( hLib == NULL )
  //   return NULL;
  // tGetExtension pGetExtension = (tGetExtension)GetProcAddress ( hLib, "GetExtension" );
  // if ( pGetExtension == NULL )
  //   return NULL;
  // void * pExt = (*pGetExtension) ( ExtInfo[N].Group, ExtInfo[N].Name );
  // return pExt;
  return NULL;
  }

void * GetExt( char * Group, char * Interface )
  {
  int N = -1;
  for ( int n = 0; n < kExtInfo; n++ )
    {
    if ( lstrcmp ( ExtInfo[n].Group, Group ) == 0 && lstrcmp ( ExtInfo[n].Name, Interface ) == 0 )
      {
      N = n;
      break;
      }
    }
  if ( N < 0 || N >= kExtInfo )
    return NULL;
  //
  // HMODULE hLib = NULL;
  // if( ! strcmp( ExtInfo[N].Section, "SYSTEM"))
  //   hLib = (HMODULE)Load_Dll( ExtInfo[N].Dll );
  // else
  //   hLib = (HMODULE)Load_Dll( ExtInfo[N].Dll, ExtInfo[N].Section );
  // if ( hLib == NULL )
  //   return NULL;
  // tGetExtension pGetExtension = (tGetExtension)GetProcAddress ( hLib, "GetExtension" );
  // if ( pGetExtension == NULL )
  //   return NULL;
  // void * pExt = (*pGetExtension) ( ExtInfo[N].Group, ExtInfo[N].Name );
  // return pExt;
  return NULL;
  }

typedef IExtension * (*tGetIExtension) ( const char * Name );

IExtension * GetExtension ( char * Param ) // "Путь DLL|имя расширения"
  {
  char Parms[1024];
  lstrcpy ( Parms, Param );
  char * DLL = Parms;
  char * Name = strchr ( DLL, '|' );
  if ( Name == NULL )
    Name = "";
  else
    {
    *Name = 0;
    Name++;
    }
  // HMODULE hLib = (HMODULE)Load_Dll( DLL, "PROJECT" );
  // if ( hLib == NULL )
  //   return NULL;
  // tGetIExtension pGetIExtension = (tGetIExtension)GetProcAddress ( hLib, "GetExtension" );
  // if ( pGetIExtension == NULL )
  //   return NULL;
  // IExtension * pExt = (*pGetIExtension) ( Name );
  // return pExt;
  return NULL;
  }
