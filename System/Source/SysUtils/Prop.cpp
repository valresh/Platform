#include "Prop.h"
#include "PropsLoader.h"

CPropertiesM PROPS;
CModelInfo Info;
bool UseAlt_CompNames = false;

static CPropsLoaderM PropLoader;

CPropertiesM::CPropertiesM()
{
  WasLoad = false;
}

CPropertiesM::~CPropertiesM()
{
}

bool CPropertiesM::ReadProp()
  {
  if ( WasLoad )
    return true;
  bool bResult = PropLoader.Load ();
  return bResult;
  }

bool CPropertiesM::Get ( const char * Name, double & Val )
  {
  return PropLoader.GetDouble (Name, Val) ? true : false;
  }

bool CPropertiesM::Get ( const char * Name, int & Val )
  {
  return PropLoader.GetInt (Name, Val) ? true : false;
  }

bool CPropertiesM::Get ( const char * Name, bool & Val )
  {
  return PropLoader.GetBool (Name, Val);
  }

bool CPropertiesM::Get ( const char * Name, char Val[256] )
  {
  return PropLoader.GetString (Name, Val);
  }

bool CPropertiesM::LoadProp()
  {
  if ( WasLoad )
    return true;
  if ( !ReadProp())
    return false;
  WasLoad = true;
  return true;
  }

char * CPropertiesM::GetErrorMsg ()
  {
  return PropLoader.GetErrorMsg ();
  }

