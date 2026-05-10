#pragma once

#include "Extensions.h"


struct ADParams
  {
  double T0_kS, K0_kS;
  double T1_kS, K1_kS;
  double A_kS, B_kS;
  double T0_kC, K0_kC;
  double T1_kC, K1_kC;
  double A_kC, B_kC;
  double kC, kS;
  //
  ADParams();
  void CalcCoef();
  void CalcK( double TC );
  };

struct MDEA_Ads : public IReactions
  {
  //
  int ID_M, ID_S, ID_C, ID_MS, ID_MC;
  ADParams * pP;
  //
  MDEA_Ads( ADParams * pP );
  int Init( IBaseModel * pMain );
  void GetParams( IBaseModel * pMain );
  void UpdateParams( struct CParams & Param );
  void Reactions( double dt, IReactionsParam * pParam );
  int SaveState( char * SaveName );
  int RestoreState ( char * SaveName, char * StrName );
  bool ReadData( struct IBaseModel * pModel );
  };
