#include "Prop.h"
#include "PropsLoader.h"

CProperties PROPS;
CModelInfo Info;
bool UseAlt_CompNames = false;

static CPropsLoader PropLoader;

CProperties::CProperties()
{
  WasLoad = false;
}

CProperties::~CProperties()
{
}

bool CProperties::ReadProp()
  {
  if ( WasLoad )
    return true;
  bool bResult = PropLoader.Load ();
  return bResult;
  }

bool CProperties::Get ( const char * Name, double & Val )
  {
  return PropLoader.GetDouble (Name, Val) ? true : false;
  }

bool CProperties::Get ( const char * Name, int & Val )
  {
  return PropLoader.GetInt (Name, Val) ? true : false;
  }

bool CProperties::Get ( const char * Name, bool & Val )
  {
  return PropLoader.GetBool (Name, Val);
  }

bool CProperties::Get ( const char * Name, char Val[256] )
  {
  return PropLoader.GetString (Name, Val);
  }

bool CProperties::LoadProp()
  {
  if ( WasLoad )
    return true;
  if ( !ReadProp())
    return false;
  WasLoad = true;
  return true;
  }

char * CProperties::GetErrorMsg ()
  {
  return PropLoader.GetErrorMsg ();
  }

