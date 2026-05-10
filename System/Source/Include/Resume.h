#pragma once


#include <QtCore/qglobal.h>
#ifdef UTILS_EXPORTS
#define IN_DLL Q_DECL_EXPORT
#else
#define IN_DLL Q_DECL_IMPORT
#endif

extern int IN_DLL kCriticalErrors;
extern bool IN_DLL WriteResume;

int IN_DLL StartRecord();
int IN_DLL Record( char * Fmt, ... );
void IN_DLL SysRecord( char * Fmt, ... );
int IN_DLL Critical( char * Fmt, ... );


#define MAX_CTRL_NAME 64

struct IN_DLL CStateCtrl
  {
  static CStateCtrl * pFirst;
  CStateCtrl * pNext;
  char Name[MAX_CTRL_NAME];
  int State;
  CStateCtrl();
  virtual int Test() = 0;
  static void TestAll();
  static void ReadAll();
  };

struct IN_DLL CAnalogCtrl : public CStateCtrl
  {
  double * pValue;
  double Nominal;
  double Delta;
  CAnalogCtrl();
  virtual int Test();
  };

struct IN_DLL CDiscreteCtrl : public CStateCtrl
  {
  int * pValue;
  int Nominal;
  CDiscreteCtrl();
  virtual int Test();
  };

