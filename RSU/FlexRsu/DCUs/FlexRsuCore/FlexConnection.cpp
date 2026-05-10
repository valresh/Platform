#include <crossplatform.h>
#include "FlexConnection.h"
#include <FlexRsuConnect.h>
#include <rsuErr.h>

KFlexConnection::KFlexConnection()
: m_ConInVarType( evtHZ )
, m_ConOutVarType( evtHZ )
{
}


void *KFlexConnection::operator new( size_t size )
{
  return pFlexMemoryAllocator( (DWORD)size ); 
}

void KFlexConnection::operator >> ( tDigital32 &inVar )
{
  m_ConInVarType = evtDigital32;
  m_Dst.pD = &inVar;
}

void KFlexConnection::operator >> ( tAnalog32 &inVar )
{
  m_ConInVarType = evtAnalog32;
  m_Dst.pA = &inVar;
}

void KFlexConnection::operator << ( tDigital32 &outVar )
{
  m_ConOutVarType = evtDigital32;
  m_Src.pD = &outVar;
}

void KFlexConnection::operator << ( tAnalog32 &outVar )
{
  m_ConOutVarType = evtAnalog32;
  m_Src.pA = &outVar;
}

void KFlexConnection::operator = ( tDigital32 &outVar )
{
  m_bAssigned = true;
}

void KFlexConnection::operator = ( tAnalog32 &outVar )
{
  m_bAssigned = true;
}

void KFlexConnection::DataReady2Send()
{
  m_bAssigned = true;
}

eVarType KFlexConnection::GetSourceType()
{
  return m_ConOutVarType;
}

eVarType KFlexConnection::GetSinkType()
{
  return m_ConInVarType;
}

void* KFlexConnection::GetSrcRawMem()
{
  return m_Src.pD;
}
void* KFlexConnection::GetDstRawMem()
{
  return m_Dst.pD;
}

bool KFlexConnection::GetData()
{
  if( !m_bAssigned )
    return false;
  if( !m_Dst.pA || !m_Src.pA )
    return false;
  switch( m_ConInVarType )
  {
  case evtAnalog32:
    switch( m_ConOutVarType )
    {
    case evtAnalog32:
      *m_Dst.pA = *m_Src.pA;
      break;
    case evtDigital32:
      *m_Dst.pA = (tAnalog32)*m_Src.pD;
      break;
    }
    break;
  case evtDigital32:
    switch( m_ConOutVarType )
    {
    case evtAnalog32:
      *m_Dst.pD = (tDigital32)*m_Src.pA;
      break;
    case evtDigital32:
      *m_Dst.pD = *m_Src.pD;
      break;
    }
    break;
  }
  m_bAssigned = false;
  return true;
}
