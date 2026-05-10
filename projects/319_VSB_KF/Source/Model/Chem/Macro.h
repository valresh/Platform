#define EXP_0(Coef,Teta) \
  {\
  double e = exp ( -Teta * t0 );\
  double e_t = -Teta * e;\
  double e_tt = -Teta * e_t;\
  double L = log ( 1. - e );\
  double e1i = 1. / ( 1. - e );\
  double L_t = -e_t * e1i;\
  double L_tt = -e_tt * e1i - L_t * L_t;\
  double V = Coef * L;\
  double V_t = Coef * L_t;\
  double V_tt = Coef * L_tt;\
  F0_x1 += V;\
  F0_x1_t += V_t;\
  F0_x1_tt += V_tt;\
  }

#define POW_0(Coef,Powt) \
  {\
  double f = Powt * log_t0;\
  double f_t = Powt * t0_i;\
  double f_tt = -f_t * t0_i;\
  double V = Coef * exp ( f );\
  double V_t = f_t * V;\
  double V_tt = ( f_tt + f_t * f_t ) * V;\
  F0_x += V;\
  F0_x_t += V_t;\
  F0_x_tt += V_tt;\
  }

#define POW_NH3(Coef,Powt,Powd) \
  {\
  double F = Powd * log_d + Powt * log_t;\
  double F_t = Powt * ti;\
  double F_tt = Powt * ti_t;\
  double F_d = Powd * di;\
  double F_dd = Powd * di_d;\
  double V = Coef * exp ( F );\
  double V_t = F_t * V;\
  double V_tt = ( F_tt + F_t * F_t ) * V;\
  double V_d = F_d * V;\
  double V_dd = ( F_dd + F_d * F_d ) * V;\
  double V_dt = F_d * F_t * V;\
  FNH3 += V;\
  FNH3_d += V_d;\
  FNH3_dd += V_dd;\
  FNH3_dt += V_dt;\
  FNH3_t += V_t;\
  FNH3_tt += V_tt;\
  }

#define EXP_NH3(Coef,Powt,Powd,Pd) \
  {\
  double pow;\
  double pow_d;\
  double pow_dd;\
  if ( Pd == 1 )\
    {\
    pow = d;\
    pow_d = 1.;\
    pow_dd = 0.;\
    }\
  else\
    {\
    if ( Pd == 2 )\
      {\
      pow = d_2;\
      pow_d = 2. * d;\
      pow_dd = 2.;\
      }\
    else\
      {\
      pow = d_3;\
      pow_d = 3. * d_2;\
      pow_dd = 6. * d;\
      }\
    }\
    double F = Powd * log_d + Powt * log_t - pow;\
    double F_t = Powt * ti;\
    double F_tt = Powt * ti_t;\
    double F_d = Powd * di - pow_d;\
    double F_dd = Powd * di_d - pow_dd;\
    double V = Coef * exp ( F );\
    double V_t = F_t * V;\
    double V_tt = ( F_tt + F_t * F_t ) * V;\
    double V_d = F_d * V;\
    double V_dd = ( F_dd + F_d * F_d ) * V;\
    double V_dt = F_d * F_t * V;\
    FNH3 += V;\
    FNH3_d += V_d;\
    FNH3_dd += V_dd;\
    FNH3_dt += V_dt;\
    FNH3_t += V_t;\
    FNH3_tt += V_tt;\
  }

#define POW_DELTA(Coef,Powt,Powd) \
  {\
  double F = Powd * log_d + Powt * log_t;\
  double F_t = Powt * ti;\
  double F_tt = Powt * ti_t;\
  double F_d = Powd * di;\
  double F_dd = Powd * di_d;\
  double V = Coef * exp ( F );\
  double V_t = F_t * V;\
  double V_tt = ( F_tt + F_t * F_t ) * V;\
  double V_d = F_d * V;\
  double V_dd = ( F_dd + F_d * F_d ) * V;\
  double V_dt = F_d * F_t * V;\
  D += V;\
  D_d += V_d;\
  D_dd += V_dd;\
  D_dt += V_dt;\
  D_t += V_t;\
  D_tt += V_tt;\
  }

#define EXP_DELTA(Coef,Powt,Powd,Pd) \
  {\
  double pow;\
  double pow_d;\
  double pow_dd;\
  if ( Pd == 1 )\
    {\
    pow = d;\
    pow_d = 1.;\
    pow_dd = 0.;\
    }\
  else\
    {\
    pow = d_2;\
    pow_d = 2. * d;\
    pow_dd = 2.;\
    }\
    double F = Powd * log_d + Powt * log_t - pow;\
    double F_t = Powt * ti;\
    double F_tt = Powt * ti_t;\
    double F_d = Powd * di - pow_d;\
    double F_dd = Powd * di_d - pow_dd;\
    double V = Coef * exp ( F );\
    double V_t = F_t * V;\
    double V_tt = ( F_tt + F_t * F_t ) * V;\
    double V_d = F_d * V;\
    double V_dd = ( F_dd + F_d * F_d ) * V;\
    double V_dt = F_d * F_t * V;\
    D += V;\
    D_d += V_d;\
    D_dd += V_dd;\
    D_dt += V_dt;\
    D_t += V_t;\
    D_tt += V_tt;\
  }

#define POW_H2O(Powd,Powt,Coef) \
  {\
  double F = Powd * log_d + Powt * log_t;\
  double F_t = Powt * ti;\
  double F_tt = Powt * ti_t;\
  double F_d = Powd * di;\
  double F_dd = Powd * di_d;\
  double V = Coef * exp ( F );\
  double V_t = F_t * V;\
  double V_tt = ( F_tt + F_t * F_t ) * V;\
  double V_d = F_d * V;\
  double V_dd = ( F_dd + F_d * F_d ) * V;\
  double V_dt = F_d * F_t * V;\
  FH2O += V;\
  FH2O_d += V_d;\
  FH2O_dd += V_dd;\
  FH2O_dt += V_dt;\
  FH2O_t += V_t;\
  FH2O_tt += V_tt;\
  }

#define EXP_H2O(Cd,Powd,Powt,Coef) \
  {\
  double pow = d_##Cd;\
  double pow_d = Cd##. * pow * di;\
  double pow_dd = ( Cd##. - 1. ) * pow_d * di;\
  double F = Powd * log_d + Powt * log_t - pow;\
  double F_t = Powt * ti;\
  double F_tt = Powt * ti_t;\
  double F_d = Powd * di - pow_d;\
  double F_dd = Powd * di_d - pow_dd;\
  double V = Coef * exp ( F );\
  double V_t = F_t * V;\
  double V_tt = ( F_tt + F_t * F_t ) * V;\
  double V_d = F_d * V;\
  double V_dd = ( F_dd + F_d * F_d ) * V;\
  double V_dt = F_d * F_t * V;\
  FH2O += V;\
  FH2O_d += V_d;\
  FH2O_dd += V_dd;\
  FH2O_dt += V_dt;\
  FH2O_t += V_t;\
  FH2O_tt += V_tt;\
  }

#define EXP2_H2O(Powd,Powt,Coef,Alfa,Beta,Gamma,Eps) \
  {\
  double d_eps = d - Eps;\
  double t_gamma = t - Gamma;\
  double d2 = Alfa * d_eps * d_eps;\
  double d2_d = 2. * Alfa * d_eps;\
  double d2_dd = 2. * Alfa;\
  double t2 = Beta * t_gamma * t_gamma;\
  double t2_t = 2. * Beta * t_gamma;\
  double t2_tt = 2. * Beta;\
  double F = Powd * log_d + Powt * log_t - d2 - t2;\
  double F_t = Powt * ti - t2_t;\
  double F_tt = Powt * ti_t - t2_tt;\
  double F_d = Powd * di - d2_d;\
  double F_dd = Powd * di_d - d2_dd;\
  double V = Coef * exp ( F );\
  double V_t = F_t * V;\
  double V_tt = ( F_tt + F_t * F_t ) * V;\
  double V_d = F_d * V;\
  double V_dd = ( F_dd + F_d * F_d ) * V;\
  double V_dt = F_d * F_t * V;\
  FH2O += V;\
  FH2O_d += V_d;\
  FH2O_dd += V_dd;\
  FH2O_dt += V_dt;\
  FH2O_t += V_t;\
  FH2O_tt += V_tt;\
  }

#define POW_H \
  {\
  double Powd = Coef[N].Powd;\
  double Powt = Coef[N].Powt;\
  double F = Powd * log_d + Powt * log_t;\
  double F_t = Powt * ti;\
  double F_tt = Powt * ti_t;\
  double F_d = Powd * di;\
  double F_dd = Powd * di_d;\
  double V = Cfr[N][Numb] * exp ( F );\
  double V_t = F_t * V;\
  double V_tt = ( F_tt + F_t * F_t ) * V;\
  double V_d = F_d * V;\
  double V_dd = ( F_dd + F_d * F_d ) * V;\
  double V_dt = F_d * F_t * V;\
  Fr += V;\
  Fr_d += V_d;\
  Fr_dd += V_dd;\
  Fr_dt += V_dt;\
  Fr_t += V_t;\
  Fr_tt += V_tt;\
  N++;\
  }

#define EXP_H \
  {\
  double Powd = Coef[N].Powd;\
  double Powt = Coef[N].Powt;\
  int Pd = Coef[N].Pd;\
  double pow;\
  double pow_d;\
  double pow_dd;\
  if ( Pd == 1 )\
    {\
    pow = d;\
    pow_d = 1.;\
    pow_dd = 0.;\
    }\
  else\
    {\
    if ( Pd == 2 )\
      {\
      pow = d_2;\
      pow_d = 2. * d;\
      pow_dd = 2.;\
      }\
    else\
      {\
      pow = d_3;\
      pow_d = 3. * d_2;\
      pow_dd = 6. * d;\
      }\
    }\
    double F = Powd * log_d + Powt * log_t - pow;\
    double F_t = Powt * ti;\
    double F_tt = Powt * ti_t;\
    double F_d = Powd * di - pow_d;\
    double F_dd = Powd * di_d - pow_dd;\
    double V = Cfr[N][Numb] * exp ( F );\
    double V_t = F_t * V;\
    double V_tt = ( F_tt + F_t * F_t ) * V;\
    double V_d = F_d * V;\
    double V_dd = ( F_dd + F_d * F_d ) * V;\
    double V_dt = F_d * F_t * V;\
    Fr += V;\
    Fr_d += V_d;\
    Fr_dd += V_dd;\
    Fr_dt += V_dt;\
    Fr_t += V_t;\
    Fr_tt += V_tt;\
    N++;\
  }
