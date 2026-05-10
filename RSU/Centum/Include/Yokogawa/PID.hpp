UMODE
ACK_ALARMS
INTEGER(OPMK,"")//
//
FLAG(AOFS,"")
FLAG(oldAFLS,"")
FLAG(CALI,"")
FLAG(CASM,"0- нет каскада, 1- есть каскад")
FLAG(Res1,"Резерв")
//
DOUBLE2(PV,Tune_PV,"")
DOUBLE2(SV,Tune_SV,"")
DOUBLE2(MV,Tune_MV,"")
DOUBLE(DV,"")//*
DOUBLE(SUM,"")//*
DOUBLE(DR,"")
DOUBLE(CSV,"")
INTEGER(TSW,"")
INTEGER(PSW,"")// C4-19 1-MV=MSL,2-MV=MSH,3-MV=PMV
DOUBLE(m_mvForOutBySUBSYS,"")//В случае пересчета выхода
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INTEGER(AF,"")
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DOUBLE(RSV,"")
DOUBLE(RMV,"")
DOUBLE(VN,"")// I/O compensated value
FLAG(RSW,"")//Pulse width reset switch
