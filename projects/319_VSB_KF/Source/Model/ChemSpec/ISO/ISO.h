#pragma once

#define K_BASE 21

struct CISO
  {
  double X[K_BASE];
  double C[58];
  double P, T, Tau, DeltaN, M, Z, F1, F2;
  double FiS, T_dFdT, T2_dFdTT;
  double Delta, D, Ro, U, H, S, Cp, Cv, Mu;
  int SetComp( CComp * pComp );
  int SetComp( bool IsComp[], double Cmol[] );
  void SetPT( double P, double TC );
  //
  double R, F, Q, G, V, K, K3, B;
  double Bnij( int n, int i, int j );
  double Bn( int n );
  double Press( );
  //
  CISO();
  void CalcF1();
  void CalcF2();
  double F0( int i );
  double dF0dTau( int i );
  double dF0dTauTau( int i );
  double F0( );
  double dF0dTau( );
  double dF0dTauTau( );
  void CalcFi();
  void Tau_dFdTau( );
  void Tau2_dFdTauTau( );
//////////////////////////////////////////
  bool WasCalcFi;
  bool WasCalcF1;
  bool WasCalcF2;
  bool WasCalcDelta;
  bool WasCalcTau_dFdTau;
  bool WasCalcTau2_dFdTauTau;
  bool WasCalcUHS;
  bool WasCalcCpCv;
  bool WasCalcMu;
// Delta, M, D, Ro, Z
  void CalcDelta();
//////////////////////////////////////////
  void CalcUHS();
  void CalcCpCv();
  void CalcMu();
  };

void InitISO();
