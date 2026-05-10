#include "stdafx.h"
#define ALL_DATA 
#include "Data.h"
#include "CommProc.h"
#include "SR.h"


//#define TRACE_REST

extern CBaseStruct * BaseStruct[]; 
extern int kBaseStruct;
int kErr;

#undef SAVE_VAR
#define SAVE_VAR(V) { if ( !pSRFile->WF( &V, sizeof( V ))) kErr++; }
#undef SAVE_STR
#define SAVE_STR(V) { if ( !pSRFile->WS( V )) kErr++; }
#undef SAVE_DATA
#define SAVE_DATA(V,L) { if ( !pSRFile->WF( V, L )) kErr++; }
#undef SAVE_TXT
#define SAVE_TXT(V,L) { if ( !pSRFile->WF( V, L, true )) kErr++; }

#undef REST_VAR
#undef REST_DATA
#undef REST_TXT
#undef REST_STR
#undef SKIP

#define REST_VAR(Var)  if ( !pSRFile->RF( &Var, sizeof( Var )) ) kErr++;
#define REST_DATA(Addr,Size)  if ( !pSRFile->RF( Addr, Size ) ) kErr++;
#define REST_TXT(Addr,Size)  if ( !pSRFile->RF( Addr, Size, true ) ) kErr++;
#define REST_STR(Str)  if ( !pSRFile->RS( Str ) ) kErr++;
#define SKIP(Size)  pSRFile->Skip( Size );

void CBaseStruct::SaveBase()
{
#ifdef LINUX
  if ( strstr ( Name, "C-3001S"))
    KKK();
  char Txt[256];
  int L = UtfToChar( (unsigned char *)Txt,256,Name );
  if ( strstr ( Txt, "C-3001S") && L == 47 )
    KKK();
  if ( L == 47 )
    KKK();
  SAVE_VAR( L );
  SAVE_DATA( Txt, L )
#else
  int L = lstrlen ( Name );
  SAVE_VAR( L );
  SAVE_DATA( Name, L )
#endif
  SAVE_VAR ( Type );
  SAVE_VAR ( Numb );
}

void CBaseStruct::RestoreBase()
{
  int L = 0;
  REST_VAR( L );
  char Txt[256];
  ASS(L<=255)
  REST_DATA( Txt, L )
  Txt[L] = 0;
#ifdef LINUX
  ToUTF8( Txt, 256 );
#endif
  Name = Txt;
  REST_VAR ( Type );
  REST_VAR ( Numb );
}

void CComp::Save()
{
  SaveBase();
  SAVE_DATA( static_cast<CComp_W*>(this), sizeof ( CComp_W ));
  if ( SpecialType )
  {
    SpecialSize = 0;
    DWORD AddrConst = pSRFile->m_dwCurr;
    pSRFile->m_dwCurr += 8; // Запас на SpecialType, SpecialSize
    if ( pSpecial )
      SpecialSize = pSpecial->Save();
    *(DWORD*)( pSRFile->m_szBuff + AddrConst ) = SpecialType;
    AddrConst += 4;
    *(DWORD*)( pSRFile->m_szBuff + AddrConst ) = SpecialSize;
  }
}

void CComp::Restore()
{
  // RestoreBase() вызывается сначала для распознания типа
  int L = sizeof ( CComp_W );
  REST_DATA( static_cast<CComp_W*>(this), L );
  ////////Далее стандартно 0x55555555
  DWORD Key = *(DWORD*)( pSRFile->m_szBuff + pSRFile->m_dwCurr );
  if ( Key == 0x55555555 || Key == 0x77777777)
    return;
  REST_VAR ( SpecialType );
  REST_VAR ( SpecialSize );
  if ( pSpecial )
    pSpecial->Restore( 0x498 );
  else
    pSRFile->Skip( 0x498 );
  Key = *(DWORD*)( pSRFile->m_szBuff + pSRFile->m_dwCurr);
  ASS( Key == 0x55555555 )
}

void CFlow::Save()
  {
  SaveBase();
  SAVE_DATA( static_cast<CFlow_Data*>(this), sizeof ( CFlow_Data ));
  if ( ID_COMP == 0 )
    {
    SAVE_VAR( ID_COMP );
    return;
    }
  CComp * pComp = Get_Comp( );
  int L = lstrlen ( pComp->Name );
#ifdef LINUX
  char Txt[256];
  L = UtfToChar((unsigned char *)Txt, 256, pComp->Name );
  ASS( L < 256 )
  SAVE_VAR( L );
  SAVE_DATA( Txt, L )
#else
  ASS( L < 256 )
  SAVE_VAR( L );
  SAVE_DATA( pComp->Name, L )
#endif
  SAVE_VAR ( pComp->Type );
  SAVE_VAR ( pComp->Numb );
  }

void CFlow::Restore()
  {
  // RestoreBase() вызывается сначала для распознания типа
  int L = sizeof ( CFlow_Data );
  REST_DATA( static_cast<CFlow_Data*>(this), L );
  char Name[256];
  int Type;
  int Numb;
  L = 0;
  REST_VAR( L );
  if ( L == 0 )
    return;
  ASS( L <= 255 )
  REST_DATA( Name, L )
  Name[L] = 0;
#ifdef LINUX
  ToUTF8(Name,256);
#endif
  REST_VAR ( Type );
  REST_VAR ( Numb );
  ASS( Type == CComp::TypeID )
  pComp = (CComp*)Create ( Name, CComp::TypeID, Numb );
  ID_COMP = pComp->ID;
}

void CSignalData::Save()
{
  SaveBase();
  SAVE_DATA( static_cast<CSignalData_W*>(this), sizeof ( CSignalData_W ));
}

void CSignalData::Restore()
{
  // RestoreBase() вызывается сначала для распознания типа
  int L = sizeof ( CSignalData_W );
  REST_DATA( static_cast<CSignalData_W*>(this), L );
}

void CContact::Save()
{
  SaveBase();
  SAVE_DATA( static_cast<CContact_W*>(this), sizeof ( CContact_W ));
}

void CContact::Restore()
{
  // RestoreBase() вызывается сначала для распознания типа
  int L = sizeof ( CContact_W );
  REST_DATA( static_cast<CContact_W*>(this), L );
}

void CPortion::Save()
{
  SaveBase();
  SAVE_DATA( static_cast<CPortion_W*>(this), sizeof ( CPortion_W ));
}

void CPortion::Restore()
{
  // RestoreBase() вызывается сначала для распознания типа
  int L = sizeof ( CPortion_W );
  REST_DATA( static_cast<CPortion_W*>(this), L );
}

char * DataName = "Составы и потоки";
void SaveData( )
{
  int Key = 0xE1320724;
  SAVE_VAR( Key );
#ifdef LINUX
  char Txt[256];
  UtfToChar((unsigned char*)Txt,256,DataName);
  SAVE_STR( Txt );
#else
  SAVE_STR( DataName );
#endif
  int Len = 0;// Условно
  SAVE_VAR( Len );
  /////////////////////////////////////////////////////////////////////
  int kSavedComp = 0;
  for ( int n = 1; n < kBaseStruct; n++ )
  {
    int Key = 0x55555555;
    SAVE_VAR( Key );
    BaseStruct[n]->Save();
  }
  Key = 0x77777777;
  SAVE_VAR( Key );
}


void RestData( )
{
  int kRest = 0;
  while ( kRest++ < 65536 )
  {
    int Key = 0;
    REST_VAR( Key );
    if ( Key == 0x77777777 )
      break;
    ASS( Key == 0x55555555 )
    ///////////////////////////////////////
    char Name[256];
    int Type;
    int Numb;
    int L = 0;
    REST_VAR( L );
    ASS(L<=255)
    REST_DATA( Name, L )
    Name[L] = 0;
#ifdef LINUX
    ToUTF8 (Name, 256);
#endif
    REST_VAR ( Type );
    REST_VAR ( Numb );
    switch ( Type )
      {
      case CComp::TypeID:
        {
        CComp * pComp = (CComp*)Create ( Name, CComp::TypeID, Numb );
        pComp->Restore();
        }
      break;
      case CFlow::TypeID:
        {
        CFlow * pFlow = (CFlow*)Create ( Name, CFlow::TypeID, Numb );
        pFlow->Restore();
        }
      break;
      case 3: //Удалить этот тип после сохранений состояния во всех проектах
        {
        struct CMotorData_W
          {
              bool On;
              double RelOborot; // 0 - 1
              double AbsOborot; // об/мин
              double Power;     // кВт
              double Угол_атаки;
              CMotorData_W()
              {
                memset ( this, 0, sizeof ( *this ));
              }
          }Temp;
          int L = sizeof ( CMotorData_W );
          REST_DATA( static_cast<CMotorData_W*>(&Temp), L );
        }
      break;
      case CPortion::TypeID:
        {
          CPortion * pP = (CPortion*)Create ( Name, CPortion::TypeID, Numb );
          pP->Restore();
        }
        break;
      case CSignalData::TypeID:
        {
          CSignalData * pSig = (CSignalData*)Create ( Name, CSignalData::TypeID, Numb );
          pSig->Restore();
        }
        break;
      case CContact::TypeID:
        {
          CContact * pContact = (CContact*)Create ( Name, CContact::TypeID, Numb );
          pContact->Restore();
        }
        break;
      default:
        ASS(0)
        break;
    }
  }
}

