#pragma once
#include <macros/IntrusivePtr.h>

struct IModelHandler : ref_counted
{
  IModelHandler() : hCmdWnd( 0 )
  {}
  virtual ~IModelHandler(){}
  HWND hCmdWnd;
  virtual LRESULT WndProcImpl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
};

typedef TIntrusivePtr<IModelHandler> tModelHandlerPtr_;
