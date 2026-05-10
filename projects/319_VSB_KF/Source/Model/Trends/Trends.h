#pragma once
#include "BaseInt.h"
#include "DataTypes.h"
#include "SysDataTypes.h"
#include "SR.h"
#include "CommProc.h"

class CTrendsW : public IBaseModel
{
public:
  CTrendsW( LPCTSTR ObjName );
  virtual ~CTrendsW();
  //  Èםעונפויס IBaseModel
  int Init( int );
  int Step0();
  int Step1();
  int PreStepT();
  int StepT(double dt);
  int GetParams( char * StrName );
  int UpdateParam(struct CParams & Param);
  int SaveState();
  int RestoreState( char * StrName );
  int SetData( int TypeData, void * pData );
  int OnDefect( class CDefect * pDefect ) NOT_USED;
  //int GetConnections ( int & Cnt, struct CConnection Connections[/*100*/]) NOT_USED;
  void * ConnectM( char * Pnt, char * Type );
  void * ConnectR( char * Pnt, char * Type );
//
  double Interval;
  int kStep;
  void AddVars();
  void AddVars( char * PathCSV );
  void AddRegs( );
private:
};

