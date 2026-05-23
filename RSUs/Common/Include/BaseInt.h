#ifndef BASEINT_H
#define BASEINT_H
// #include <Common.h>
// #include "Err.h"
// #include "Param.h"
// #include "stdio.h"
#include "BaseModel.h"
// #include "Names.h"

#define PURE = 0
#undef IN_DLL
#ifdef UTILS_EXPORTS
#define IN_DLL Q_DECL_EXPORT
#else
#define IN_DLL Q_DECL_IMPORT
#endif

struct IN_DLL IPoolCtrlModel
{
  virtual int InitPool ( struct CVarPool * pPool, struct CGroupObject * pPoolObject ) = 0;
};

#define POOLCTRL pPoolCtrl = static_cast<IPoolCtrlModel*>(this);

//
struct IN_DLL IUniModel
{
  static IBaseModel ** UniObjects;
  static int kUniObjects;
  virtual int SetMatr( void * pExternals, int PntGroup,  double dt,
                      double A[/* kExkV */], double B[ /*kE*/ ] ) = 0;   // A * X = B
  virtual int SetVar( void * pExternals, int PntGroup, double dt,
                     // Переменные на входах
                     double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] ) = 0;
  virtual int TestNodes( int kNodes, struct CObjectPoint ** ppNodes ) = 0;
};

#define UNIMODEL pUniModel = static_cast<IUniModel*>(this);

struct IN_DLL IElectroModel
{
  virtual void ElectroOn() = 0;
  virtual void ElectroOff() = 0;
};



#ifndef _MAX_PATH //Utils не компилируется
#include <stdlib.h>
#endif
#include <time.h>
#pragma warning ( disable : 4275 )

#endif // BASEINT_H
