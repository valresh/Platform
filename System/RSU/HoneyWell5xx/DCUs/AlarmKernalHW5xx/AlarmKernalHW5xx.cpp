#include "AlarmKernalHW5xx.h"
#include <CommonH.h>
#include <rsuNoNames.h>
#include <ConnectH.h>

extern KNoName *g_Objs, *g_qbIOs;

KAlarmKernalHW5xx* g_pAlarmFile = NULL;
void fnAlarm( LPCSTR PointName, int Alarm, BYTE Prty, bool ON, double Value, void * bMod, BYTE qbaN, LPCSTR pszValue )
{
  if( !PointName || !*PointName )
    return;

  CAlarmEntry a;
  a.btEHLN = Prty;// Уровень сигнализации (enum STATUS)
  a.btType = BYTE(Alarm);// Тип сигнализации MESS
  a.btOnOf = BYTE(ON);
  a.fltValue = (float)Value;
  a.qbaN = qbaN;
  if( pszValue )
    strncpy_s( a.szValue, pszValue, _countof(a.szValue)-1 );
  // Чтобы не выйти за пределы
  a.btType = std::min<BYTE>(a.btType,63);
  g_pAlarmFile->WriteAlarms( PointName, a );
}

KAlarmKernalHW5xx::KAlarmKernalHW5xx()
: m_bChangeFile(false)
, file0(0)
, file1(1)
, m_pCBSystem( NULL )
{
}

KAlarmKernalHW5xx::~KAlarmKernalHW5xx()
{
}

int KAlarmKernalHW5xx::InitL()
{
  m_pCBSystem = GetControlBuilderHSystemData();
  if( m_pCBSystem )
  {
    m_pCBSystem->nAlarms0 = 0;
    m_pCBSystem->nAlarms1 = 0;
    m_pCBSystem->nAlarmXY = 0;
    m_pCBSystem->bSound = false;
    m_pCBSystem->nStep = 0;
  }

  if( !file0.Init() )
    return 1;
  if( !file1.Init() )
    return 1;

  BuildAll();
  UINT nCountO  = mMnemo.Count();
  TMnemoSxem* mnemo = mMnemo.Items();
  for ( UINT n = 0; n < nCountO; n++ )
  {
    const char* name = mMnemo.Name( n );
    mnemo[n].pMnemo = (COneMnemoHoney*)g_Objs->FindStruct( name, COneMnemoHoney::TypeID, 0 );
    if( !mnemo[n].pMnemo )
      mnemo[n].pMnemo = (COneMnemoHoney*)g_Objs->CreateStruct( name, COneMnemoHoney::TypeID, 0 );
    mnemo[n].Reset();
  }
  //
  UINT nCountA = mAlmGr.Count();
  TAlmGr* almgr = mAlmGr.Items();
  for ( UINT n = 0; n < nCountA; n++ )
  {
    const char* name = mAlmGr.Name( n );
    almgr[n].pAlmGr = (COneAlmGrHoney*)g_Objs->FindStruct( name, COneAlmGrHoney::TypeID, 0 );
    if( !almgr[n].pAlmGr )
    {
      almgr[n].pAlmGr = (COneAlmGrHoney*)g_Objs->CreateStruct( name, COneAlmGrHoney::TypeID, 0 );
      memcpy(almgr[n].pAlmGr->description, almgr[n].description, sizeof(almgr[n].pAlmGr->description) );
    }
    almgr[n].Reset();
  }  
  UINT nCountT  = mHoney.Count();
  THoneyWell* items = mHoney.Items();
  for ( UINT n = 0; n < nCountT; n++ )
  {
    const char* name = mHoney.Name( n );
    ASS( !items[n].m_pObj );
    items[n].m_pObj = (CAlarmBase*)g_Objs->FindStruct( name, items[n].ID_CLASS, 0 );
    if( !items[n].m_pObj )
      items[n].m_pObj = (CAlarmBase*)g_qbIOs->FindStruct( name, items[n].ID_CLASS, 0 );
    if( !items[n].m_pObj )
      continue;
    items[n].m_pObj->notACK = 0;
    items[n].m_pObj->setACK = 0;
    items[n].m_pObj->Shelved = 0;
    items[n].m_pObj->btEHLN = 0;
    items[n].m_pObj->btType = 0;
    ZeroMemory( items[n].prio, sizeof(items[n].prio) );
    items[n].int64 = 0;
  }

  for ( UINT k = 0; k < nCountO; k++ )
  {
    LPCSTR name1 = mMnemo.Name( k );
    LPCSTR name2 = mMnemo.mChar.Name( mnemo[k].m_nName );
    UINT N = mnemo[k].m_nPlace;
    for ( UINT n = 0; n < mnemo[k].m_nCount; n++, N++ )
    {
      THoneyWell& obj = mHoney.Item(N);
      if( obj.m_pObj == NULL )
        continue;
      LPCSTR pName = NULL;
      LPCSTR pType = NULL;
      g_Objs->GetNameEx( obj.m_pObj->ID_PNT_NAME, &pType, &pName );
      KKK();
    }

  }

  for ( UINT k = 0; k < nCountA; k++ ) //AlmGr
  {
    LPCSTR name1 = mAlmGr.Name( k );
    LPCSTR name2 = mAlmGr.mChar.Name( almgr[k].m_nName );
    UINT N = almgr[k].m_nPlace;
    for ( UINT n = 0; n < almgr[k].m_nCount; n++, N++ )
    {
      THoneyWell& obj = mHoney.Item(N);
      if( obj.m_pObj == NULL )
        continue;
      LPCSTR pName = NULL;
      LPCSTR pType = NULL;
      g_Objs->GetNameEx( obj.m_pObj->ID_PNT_NAME, &pType, &pName );
      KKK();
    }
  }
  
  g_pAlarmFile = this;
  pAlarm = fnAlarm;

  return 0;
}

void KAlarmKernalHW5xx::Reset()
{
  if( m_pCBSystem )
  {
    m_pCBSystem->nAlarms0 = 0;
    m_pCBSystem->nAlarms1 = 0;
    m_pCBSystem->nAlarmXY = 0;
    m_pCBSystem->bSound = false;
    m_pCBSystem->nStep = 0;
  }
  file0.Init();
  file1.Init();
  UINT nMnemo = mMnemo.Count();
  TMnemoSxem* mnemo =  mMnemo.Items();
  for ( UINT n = 0; n < nMnemo; n++ )
  {
    mnemo[n].Reset();
  }
  UINT nCountT  = mHoney.Count();
  THoneyWell* items = mHoney.Items();
  for ( UINT n = 0; n < nCountT; n++ )
  {
    if( !items[n].m_pObj )
      continue;
    items[n].m_pObj->notACK = 0;
    items[n].m_pObj->setACK = 0;
    items[n].m_pObj->Shelved = 0;
    items[n].m_pObj->btEHLN = 0;
    items[n].m_pObj->btType = 0;
    ZeroMemory( items[n].prio, sizeof(items[n].prio) );
    items[n].int64 = 0;
  }
}

int KAlarmKernalHW5xx::StepTL( double dtS )
{
  LOCK( m_csOnContour );
  double dt_sec = dtS;
  int ms = DWORD(dt_sec * 1000);
  UINT nMnemo = mMnemo.Count();
  TMnemoSxem* mnemo =  mMnemo.Items();
  int nNotAckedMnemos = 0;
  for ( UINT n = 0; n < nMnemo; n++ )
  {
    if ( mnemo[n].pMnemo->setACK != 0 )
    {
      LPCSTR name = mMnemo.mChar.Name( mnemo[n].m_nName );
      OutputDebugString( "квитирование объектов на схеме ");
      OutputDebugString( name ); OutputDebugString("\n");
      mnemo[n].ACK(mHoney,m_bChangeFile);// Произошло квитирование
      ++nNotAckedMnemos;
    }
    else
      mnemo[n].STP(mHoney,m_bChangeFile);// Квитировались все объекты на мнемосхеме
  }
  /*if( 0==nNotAckedMnemos && m_pCBSystem->nStep > file1.eMagicStep )
  {
    m_pCBSystem->bSound = false;
  }*/
  //
  if( m_pCBSystem && m_pCBSystem->nStep <= 10 )
    return 0;
  //
  UINT nCount = mHoney.Count();
  THoneyWell* honey = mHoney.Items();
  //
  for( UINT n = 0; n < nCount; n++ )
  {
    THoneyWell &tag = honey[n];
      //  char* name = mName.Name( obj.m_nName );
    if( tag.m_pObj == NULL )
        continue;
    if( tag.m_pObj->Shelved > 0 )
    {
      tag.m_pObj->Shelved -= ms;
      if( tag.m_pObj->Shelved < 0 )
        tag.m_pObj->Shelved = 0;
    }
      //
      // Оператор квитировал объект
      // Это повод для анализа списка алармов
    if( tag.m_pObj->setACK )
        m_bChangeFile = true;
    }
    // Ничего не поменялось - сваливаем.
  if ( !m_bChangeFile )
    return 0;

  m_bChangeFile = false;
  //
  int nFileA = m_pCBSystem->FileA();
  int nNotAckedTags = 0;
  //
  CAlarmsFile& src =  ( nFileA ) ? file1 : file0;
  CAlarmsFile& dst =  ( nFileA ) ? file0 : file1;
  //
  dst.Init();
  int  nSrc =  ( nFileA ) ? m_pCBSystem->nAlarms1 : m_pCBSystem->nAlarms0;
  int& nDst =  ( nFileA ) ? m_pCBSystem->nAlarms0 : m_pCBSystem->nAlarms1;
  //
  std::ifstream hFile(src.FileName());
  if (hFile.is_open())// Открываем файл
  {
    DWORD dwSize = nSrc*sizeof(CAlarmEntry);
    std::vector<CAlarmEntry> data;
    data.resize(nSrc);
    if (!data.empty())
    {
        DWORD dwRead = 0;
        hFile.read((char*)&data.at(0), dwSize);
        BOOL fSuccess = false;
        if (hFile)// Читаем данные
        {
            nDst = 0;// Дошли до анализа
            nNotAckedTags += Analiz(dst, nDst, nSrc, &data.at(0));
            m_pCBSystem->nAlarmXY++;
        }
    }
    hFile.close();
  }
  if( 0==nNotAckedTags && m_pCBSystem->nStep > file1.eMagicStep )
  {
    m_pCBSystem->bSound = false;
  }
  // Сбрасываем флаги
  /*for( UINT n = 0; n < nCount; n++ )
    {
      if ( honey[n].m_pObj == NULL )
        continue;
      honey[n].m_pObj->setACK = 0;
  }*/
    //
    return 0;
}
//
int KAlarmKernalHW5xx::Analiz( CAlarmsFile& file, int& nAlarm, int nCount, CAlarmEntry* data )
{
  int nNotAcked = 0;
  for ( int n = 0; n < nCount; n++ )
  {
    THoneyWell* obj = mHoney.Data( data[n].dwLocl );
    // Этого не может быть, но - на всякий случай
    if ( obj == NULL )
      continue;
    // Этого не может быть, но - на всякий случай
    if ( obj->m_pObj == NULL )
      continue;
    //
    if ( obj->m_pObj->setACK == 1 )
    {
      obj->m_pObj->setACK = 0;
      obj->m_pObj->notACK = 0;
      data[n].btKvit = 1;
	  /////////////////
      UINT hFind = mHoney.Find( data[n].pntFullName );
      if ( hFind == INVALID_UNI )// Нет в списке
        ASS(0);
      ////////////////	  
      for( int j = n; j < nCount; ++j )
      {
        if( data[n].dwLocl != data[j].dwLocl )
          continue;
        data[j].btKvit = 1;
      }
	  ///////////////////////////////
      UINT nAlmGr = mAlmGr.Count();
      TAlmGr* almgr = mAlmGr.Items();
      //
      THoneyWell& rHoney = *mHoney.Data( hFind );
      CAlarmBase* pAlarm = rHoney.m_pObj;
      int nCount = rHoney.m_nCountAlmGr;
      int nPlace = rHoney.m_nPlace;
      for ( int n = 0; n < nCount; n++, nPlace++ )
      {
        TAlmGr& rAlmGr = mAlmGr.Item(nPlace);
        //LPCSTR name = mAlmGr.mChar.Name( rAlmGr.m_nName );
        //rAlmGr.ACK(mHoney,m_bChangeFile);
        if ( rAlmGr.pAlmGr->notACK == 1 )
          rAlmGr.STP(mHoney,m_bChangeFile);// Квитировались все объекты в аларменной группе
      }
      /////////////////////////////////	  
    }
    if ( data[n].btKvit & 0x01 )
    {
      if ( data[n].btOnOf == 0 )
        continue;
    }
    bool notSave = false;
    for( int j=n+1; j<nCount; ++j )
    {
      if( data[n].dwLocl != data[j].dwLocl )
        continue;
      if( data[n].btType != data[j].btType )
        continue;
      if( data[j].btOnOf )
      {
        notSave = true;
        break;
      }
    }
    if( notSave )
      continue;
    if( obj->m_pObj->notACK )
      ++nNotAcked;
    //
    file.SaveAlarm( data[n], nAlarm, NULL, m_pCBSystem );
  }
  //
  return nNotAcked;
}
//
// Запись в файл сигнализаций
void KAlarmKernalHW5xx::WriteAlarms( LPCTSTR pszPoint, CAlarmEntry& rAlarm )
{
  LOCK( m_csOnContour );
  strcpy_s( rAlarm.pntFullName, pszPoint );
  UINT hFind = mHoney.Find( pszPoint );
  if ( hFind == INVALID_UNI )// Нет в списке - до свидания
  {
    ASSD(0);
    return;
  }
  //
  THoneyWell& rHoney = *mHoney.Data( hFind );
  CAlarmBase* pAlarm = rHoney.m_pObj;
  if ( pAlarm == NULL )// Это нонсенс
    return;
  //
  rAlarm.dwLocl = UINT(hFind);
  rAlarm.dwType = pAlarm->ID_CLASS;
  rAlarm.dwName = pAlarm->ID_PNT_NAME;
  //
  if ( rAlarm.btOnOf )
  {
    uint64_t int64 = 1 << rAlarm.btType;
    if ( (rHoney.int64 & int64) != 0 )
    {
      // Повтор сигнализации - этого не должно быть
    } // Запоминаем код сигнализации
    else rHoney.int64 |= int64;
    // Запоминаем приоритет сигнализации
    // для возможного восстановления
    rHoney.prio[rAlarm.btType] = rAlarm.btEHLN;
    // Устанавливаем в модель более приоритетную сигнализацию
    if ( !pAlarm->btType || pAlarm->btType > rAlarm.btType )
    {
      pAlarm->btEHLN = rAlarm.btEHLN;
      pAlarm->btType = rAlarm.btType;
      HIALM( pAlarm, rAlarm.btEHLN, rAlarm.btType );
    }
  }
  else
  {
    rAlarm.btEHLN = pAlarm->btEHLN;
    //
    uint64_t int64 = 1 << rAlarm.btType;
    if ( (rHoney.int64 & int64) == 0 )
    {
      // Окончание незарегистрированной сигнализации - этого не должно быть
    }// Снимаем код сигнализации
    else
      rHoney.int64 ^= int64;
    //
    // Устанавливаем предыдущую сигнализацию
	
	FindAlmGr( rAlarm, rHoney );// Уведомляем аларменные группы о сигнализации
	
    BYTE nEHLN = 0;
    BYTE nType = 0;
    if ( rHoney.int64 != 0 )
    {
      uint64_t M = 1;
      for ( int n = 0; n < 64; n++, M <<= 1 )
        if  ( rHoney.int64 & M )
        {
          nEHLN = rHoney.prio[n];
          nType = BYTE(n);
          break;
        }
    }
    pAlarm->btEHLN = nEHLN;
    pAlarm->btType = nType;
    HIALM( pAlarm, nEHLN, nType );
  }
  //
  if( IsAcy(rAlarm.dwType) )
  {
    if( rAlarm.btEHLN == CAlarmEntry::None ) 
      return;
    if( rAlarm.btEHLN == CAlarmEntry::Journal ) 
      return;
  }
  else if( IsQBAcy( rAlarm.dwType) )
  {
    if( 0==rAlarm.btEHLN )
      return;
  }
  else
    return;
  //
  // Остальные пишутся в журнал
  if( rAlarm.btOnOf )
  {
    if( !pAlarm->Shelved )
      AlarmOn  ( rAlarm, rHoney );
    FindMnemo( rAlarm, rHoney );// Уведомляем мнемосхемы о сигнализации
	FindAlmGr( rAlarm, rHoney );// Уведомляем аларменные группы о сигнализации
  }
  else
  {
    FindMnemo( rAlarm, rHoney );// Уведомляем мнемосхемы о сигнализации
    if( !pAlarm->Shelved )
      AlarmOff ( rAlarm, rHoney );
  }
  //
#undef  EnumMESS
#define EnumMESS( Name, Number, Text ) #Text,
  static char* Mess[] =
  {
#include <_AlarmMess.hpp>
  };
  //
#undef  EnumSTATUS
#define EnumSTATUS( Name, Number, Text ) #Name,
  static char* Status[] =
  {
#include <_AlarmStatus.hpp>
  };
  //
  char szText[256];
  BYTE btEHLN_Index = std::min<BYTE>( rAlarm.btEHLN, _countof(Status)-1 );
  if ( rAlarm.btOnOf )
    sprintf_s( szText, ("Сигнализация %s (%s - %s)"), pszPoint, Status[btEHLN_Index], Mess[rAlarm.btType] );
  else
    sprintf_s( szText, ("Конец сигнализации %s (%s - %s)"), pszPoint, Status[btEHLN_Index], Mess[rAlarm.btType] );
  //
  // Посылаем сообщение в Event,dat
  /*CEventEntry str;
  str.nTime = rAlarm.nTimeS;
  str.nMess = CEventLogType::etAlarm;
  lstrcpyn( str.szUser, _T("Система"), sizeof(str.szUser) );
  lstrcpyn( str.szText, szText, sizeof(str.szText) );
  ::WriteEvents( str.szUser, str.nMess, str.szText );*/
  //
}
//
void KAlarmKernalHW5xx::AlarmOn ( CAlarmEntry& rAlarm, THoneyWell& rHoney )
{
  // Запись сигнализации в файл
  if ( m_pCBSystem->FileA() )
    file1.SaveAlarm( rAlarm, m_pCBSystem->nAlarms1, &rHoney, m_pCBSystem );
  else
    file0.SaveAlarm( rAlarm, m_pCBSystem->nAlarms0, &rHoney, m_pCBSystem );
  // Включение звуковой сигнализации
  if( m_pCBSystem->nStep > file1.eMagicStep )
    m_pCBSystem->bSound = true;
}
//
void KAlarmKernalHW5xx::AlarmOff( CAlarmEntry& rAlarm, THoneyWell& rHoney )
{
  m_bChangeFile = true;
  //
  // Ищем в массиве начало сигнализации
  // Устанавливаем ему признак конца сигнализации
  if ( m_pCBSystem->FileA() )
    file1.FindAlarm( rAlarm, m_pCBSystem->nAlarms1, rHoney );
  else
    file0.FindAlarm( rAlarm, m_pCBSystem->nAlarms0, rHoney );
}
//
void KAlarmKernalHW5xx::FindMnemo(CAlarmEntry& rAlarm, THoneyWell& rHoney)
{
  int nCount = rHoney.m_nCount;
  int nPlace = rHoney.m_nPlace;
  for ( int n = 0; n < nCount; n++, nPlace++ )
  {
    TMnemoSxem& rMnemo = mMnemo.Item(nPlace);
    LPCSTR name = mMnemo.mChar.Name( rMnemo.m_nName );
    //if ( rHoney.m_pObj )//&& rHoney.m_pObj->mUnit < 20 )
    rMnemo.AlarmIncDec( rAlarm, rHoney.m_pObj );
  }
  //
}

void KAlarmKernalHW5xx::FindAlmGr(CAlarmEntry& rAlarm, THoneyWell& rHoney)
{
  int nCount = rHoney.m_nCountAlmGr;
  //int nPlace = rHoney.m_nPlace;
  for ( int n = 0; n < nCount; n++ )
  {
    TAlmGr& rAlmGr = mAlmGr.Item(rHoney.indexplace[n]);
    LPCSTR name = mAlmGr.mChar.Name( rAlmGr.m_nName );
    rAlmGr.AlarmIncDec( rAlarm, rHoney.m_pObj );
  }
}