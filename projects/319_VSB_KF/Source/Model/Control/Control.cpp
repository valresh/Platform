#include "stdafx.h"
#define DLL_Control
#include "Control.h"
#include "Err.h"
#include "SetData.h"
#include "Connect.h"

IBaseControl::IBaseControl( char * _ControlName, struct IBaseModel * _pObject )
  {
  ControlName = _ControlName;
  pObject = _pObject;
  NewEvent = false;
  On = true;
  Use = true;
  pNextControl = NULL;
  if ( pFirstControl == NULL )
    pFirstControl = this;
  if ( pLastControl )
    pLastControl->pNextControl = this;
  pLastControl = this;
  }



