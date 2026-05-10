#include "Mnemo.h"
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TMnemoSxem::AlarmIncDec( CAlarmEntry& data, CAlarmBase* obj )
{
  BYTE unit = obj->mUnit;
  BYTE ehln = obj->btEHLN;
  if ( data.btOnOf == 0 )// Конец сигнализации
  {
    if ( ehln == 2 )//LOW
    {
      int n = pMnemo->count1[unit]-1;
      if ( n < 0 ) n = 0;// Этого не должно быть
      pMnemo->count1[unit] = n;
    }
    else if ( ehln == 4 )//Emergency
    {
      int n = pMnemo->count2[unit]-1;
      if ( n < 0 ) n = 0;// Этого не должно быть
      pMnemo->count2[unit] = n;
    }
    else if ( ehln == 3 )//High
    {
      int n = pMnemo->count3[unit]-1;
      if ( n < 0 ) n = 0;// Этого не должно быть
      pMnemo->count3[unit] = n;
    }
  }
  else
  {
    if ( ehln == 2 )//LOW
    {
      if ( data.btKvit == 0 )
        pMnemo->state1[unit] = true;
      pMnemo->count1[unit]++;
      pMnemo->notACK = data.btKvit ? 0 : 1;
    }
    else if ( ehln == 4 )//Emergency
    {
      if ( data.btKvit == 0 )
        pMnemo->state2[unit] = true;
      pMnemo->count2[unit]++;
      pMnemo->notACK = data.btKvit ? 0 : 1;
    }
    else if ( ehln == 3 )//High
    {
      if ( data.btKvit == 0 )
        pMnemo->state3[unit] = true;
      pMnemo->count3[unit]++;
      pMnemo->notACK = data.btKvit ? 0 : 1;
    }
  }
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Квитирование всех объектов, которые сидят на мнемосхеме
void TMnemoSxem::ACK( SUniHoney& tag, bool& bAnaliz )
  {
  BYTE ack = pMnemo->setACK-1;
  if ( ack < 4 )
    {
    UINT N = m_nPlace;
    for ( UINT n = 0; n < m_nCount; n++, N++ )
      {
      THoneyWell& obj = tag.Item(N);
      // Этого, вообще-то, не должно быть
      if ( obj.m_pObj == NULL )
        continue;
      //
      if ( obj.m_pObj->notACK )
//Sergej if ( TrueUNIT( obj.m_pObj->mUnit ) )
        {
        bAnaliz = true;
        obj.m_pObj->setACK = 1;
        }
      }
    }
  pMnemo->notACK = 0;
  pMnemo->setACK = 0;
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TMnemoSxem::STP( SUniHoney& tag, bool& bAnaliz )
  {
  int N = m_nPlace;
  for ( UINT n = 0; n < m_nCount; n++, N++ )
    {
    THoneyWell& obj = tag.Item(N);
    // Этого, вообще-то, не должно быть
    if ( obj.m_pObj == NULL )
      continue;
    // Нашёлся неквитированный объект
    if ( obj.m_pObj->notACK )
      return;
    }
  // Все объекты квитированы ( nCount не может быть меньше нуля )
  memset( pMnemo->state1, 0, sizeof(pMnemo->state1) );
  memset( pMnemo->state2, 0, sizeof(pMnemo->state2) );
  memset( pMnemo->state3, 0, sizeof(pMnemo->state3) );
  pMnemo->notACK = 0;
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TMnemoSxem::Reset()
{
  if( !pMnemo )
    return;
  memset( pMnemo->state1, 0, sizeof(pMnemo->state1) );
  memset( pMnemo->state2, 0, sizeof(pMnemo->state2) );
  memset( pMnemo->state3, 0, sizeof(pMnemo->state3) );
  memset( pMnemo->count1, 0, sizeof(pMnemo->count1) );
  memset( pMnemo->count2, 0, sizeof(pMnemo->count2) );
  memset( pMnemo->count3, 0, sizeof(pMnemo->count3) );
  pMnemo->notACK = 0;
  pMnemo->setACK = 0;
  pMnemo->Shelved = 0;
  pMnemo->btEHLN = 0;
  pMnemo->btType = 0;
}
