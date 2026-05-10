UMODE
ACK_ALARMS
INTEGER(OPMK,"")//
//
FLAG(AOFS,"")
FLAG(BSTS,"")//?????
//
DOUBLE(PV,"")//*
DOUBLE(SV,"")
DOUBLE(MV,"")
INTEGER(PV01,"")
INTEGER(PV02,"")
INTEGER(PV03,"")
INTEGER(PV04,"")
INTEGER(PV05,"")
//
INTEGER(MV01,"")
INTEGER(MV02,"")
INTEGER(MV03,"")
INTEGER(MV04,"")
INTEGER(MV05,"")
//
INTEGER_A(SWCR,5,"")//цвета кнопок 
INTEGER_A(SWST,5,"")//моргание ламп	(1)
INTEGER_A(SWOP,5,"")//отключение > 0
//
STRING_A(AKLB,5,16,"")//подтверждение
STRING_A(SWLB,5,16,"")//метки кнопок(char[8]??)
//
DOUBLE_A(MBOTs,5,"")
