#include "Alarms.h"


CAlarmsW::CAlarmsW( LPCTSTR _ObjName ) 
: IBaseModel ((char*)_ObjName)
{
  TypeObj = RSU_Obj;
  m_bInited = m_bStep0 = m_bStep1 = false;
}

CAlarmsW::~CAlarmsW()
{
}

int CAlarmsW::Init( int )
{
  if( !m_bInited )
  {
    m_bInited = true;
    KAlarmKernalHW5xx::InitL();
  }

  return 0;
}

int CAlarmsW::StepT( double dt )
{
  double dtS = dt / 1000.;
  return KAlarmKernalHW5xx::StepTL( dtS );
}
