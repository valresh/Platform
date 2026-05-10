UMODE
ACK_ALARMS
INTEGER(OPMK,"")//
//
FLAG(AOFS   ,"")
ENUM(BSTS   ,"",STOP,PAUS,RUN)
ENUM(PREBSTS,"",STOP,PAUS,RUN)
//
STRING(AFhz  ,8,"")//наверно ошибка
STRING(SEMA  ,8,"")
STRING(PHASE ,8,"")
STRING(IPHASE,8,"")
//
INTEGER(ERRC   ,"")
INTEGER(ERRE   ,"")
INTEGER(ERRL   ,"")
INTEGER(ERRF   ,"")
INTEGER(ERRS   ,"")
INTEGER(ERRP   ,"")
//
INTEGER(STEPNO ,"")
INTEGER(PHASENO ,"")
INTEGER(SAID   ,"")
INTEGER(UAID   ,"")
//
INTEGER_A(SWCR,5,"")//цвета кнопок 
INTEGER_A(SWST,5,"")//моргание ламп	(1)
INTEGER_A(SWOP,5,"")//отключение > 0
//
STRING_A(AKLB,5,16,"")//подтверждение
STRING_A(SWLB,5,16,"")//метки кнопок(char[8]??)
//
DOUBLE(PV ,"")
DOUBLE(SV ,"")
DOUBLE(MV ,"")
//Total size of all data items exceeds specified data size.: maxSize=8192
STRING_skipSave(USER,8192*2,"")
