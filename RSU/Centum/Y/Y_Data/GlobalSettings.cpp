#include "stdafx.h"
#include "GlobalSettings.h"
#include <macros/rsuINI_C.h>

KGlobalSettings::KGlobalSettings()
: m_PV_LIMIT_FROM_INI( -200 )
, m_ExtendParam2State( -200 )
{
}

void KGlobalSettings::Init()
{
  char res[16] = { 0 };
 
  if( -200==m_PV_LIMIT_FROM_INI )
  {
    rsuINI_C( "PV_LIMIT", "-1", res );
    m_PV_LIMIT_FROM_INI = atoi( res );
  }
  if( -200==m_ExtendParam2State )
  {
    rsuINI_C( "EXTEND_STATE", "-1", res );
    m_ExtendParam2State = atoi( res );
  }
}

int KGlobalSettings::PV_LIMIT()
{
  Init();
  return m_PV_LIMIT_FROM_INI;
}

int KGlobalSettings::ExtendState()
{
  Init();
  return m_ExtendParam2State;
}

KGlobalSettings& KGlobalSettings::Instance()
{
  static KGlobalSettings s_Instance;
  return s_Instance;
}
