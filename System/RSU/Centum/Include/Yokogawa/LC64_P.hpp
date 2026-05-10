YPARM_S(Comment,64,"ETCM")
YPARM_D_A(ST,64,"P")
YPARM_I(AlarmLevel,3,"EALA")
//
YPARM_B(BAD_connection_alarm,false,"ILCN")

SKIP("TENK")// ??

YPARM_I(TMNL_hz,1,"")
YPARM_L(TMNL_StartTiming,'T',"","")//%{T;O;I;B} см. enum eTMNL_StartTiming
YPARM_I(TMNL_ControlPeriod,1,"")
YPARM_I(TMNL_ControlPhase,0,"")
