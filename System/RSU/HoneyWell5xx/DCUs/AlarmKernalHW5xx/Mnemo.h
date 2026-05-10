#pragma once
#include <DataTypes_.h>
#include <QbDataTypes_.h>
#include <_alarmentry.h>
#include <UniBufferT.h>
#include <rsuErr.h>
//
struct THoneyWell : public ns_US::SLocal
{
  DWORD ID_CLASS;
  UINT m_nCount;// Количество мнемосхем
  UINT m_nCountAlmGr;// Количество аларменных групп
  UINT m_nPlace;// Место в буфере индексов
  CAlarmBase* m_pObj;// Указатель на модельный объект
  //
  uint64_t int64;//Наличие сигнализаций
  unsigned char prio[64];//Приоритеты сигнализаций
  //
  UINT indexplace[64];
};
//
struct TMnemoSxem : public ns_US::SLocal
  {
  UINT m_nCount;// Количество тэгов
  UINT m_nPlace;// Место в буфере индексов
  COneMnemoHoney* pMnemo;
  //
  void AlarmIncDec( CAlarmEntry& data,CAlarmBase* obj );
  void ACK( struct SUniHoney& tag, bool& bAnaliz );
  void STP( struct SUniHoney& tag, bool& bAnaliz );
  void Reset();
	};
//
struct SUniIndex : public SUniBufferT<UINT>
{
  SUniIndex()
  {};
  //
  UINT Add( UINT uint )
    {
    ASS( Realloc(1) );
    UINT* u = (UINT*)m_szBuffer;
    UINT n = AddCurr();
    u[n] = uint;
    return n;
    }
  UINT W( int n )
  {
    UINT* list = (UINT*)m_szBuffer;
    return list[n];
  }
};
//
struct SUniHoney : public SUniSortT<THoneyWell>
{
  SUniHoney(SUniIndex& w,SUniCharT& n)
  : SUniSortT(n), rWord(w)
  {
    m_nSort = 2;
  };
  //
  SUniIndex& rWord;
  THoneyWell* Items()
  {
    return (THoneyWell*)Obj(0); 
  }
  THoneyWell* Data(UINT n)
  {
    return (THoneyWell*)Obj(n); 
  }
  THoneyWell& Item(int  n)
  {
    THoneyWell* lst = Items();
    return lst[ rWord.W(n)];
  }
};
//
struct SUniMnemo : public SUniSortT<TMnemoSxem>
  {
  SUniMnemo(SUniIndex& w,SUniCharT& n)
  : SUniSortT(n), rWord(w){};
  //
  SUniIndex& rWord;
  TMnemoSxem* Items(){ return (TMnemoSxem*)Obj(0); }
  TMnemoSxem* Data(UINT n){ return (TMnemoSxem*)Obj(n); }
  TMnemoSxem& Item(int  n){TMnemoSxem* lst = Items();return lst[rWord.W(n)];}
  };
//