UMODE
ACK_ALARMS
INTEGER(OPMK,"")
INTEGER(AF,"")
FLAG(AOFS,"")
FLAG(CALI,"")
FLAG(CASM,"0- нет каскада, 1- есть каскад")

DOUBLE(PV,"")
DOUBLE(RAW,"")
DOUBLE(SUM,"")
DOUBLE(SV,"")
DOUBLE(CSV,"")
DOUBLE(RSV,"")
DOUBLE(DV,"")
DOUBLE(VN,"")
DOUBLE(MV,"")
DOUBLE(m_mvForOutBySUBSYS,"")//В случае пересчета выхода
DOUBLE(RMV,"")
DOUBLE(RLV1,"Reset limit value 1 Value in the same engineering unit as MV MSL")
DOUBLE(RLV2,"Reset limit value 2 Value in the same engineering unit as MV MSL")
DOUBLE(PVP,"Velocity-Reference Sample Value in the same engineering unit as PV")
FLAG(TSW,"")
FLAG(CSW,"")
INTEGER(PSW,"")// C4-19 1-MV=MSL,2-MV=MSH,3-MV=PMV
FLAG(RSW,"")
FLAG(BSW,"")//computer failure (BSW=ON)
