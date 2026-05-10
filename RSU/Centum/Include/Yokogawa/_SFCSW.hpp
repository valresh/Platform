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
//Total size of all data items exceeds specified data size.: maxSize=8192
STRING_skipSave(USER,8192*2,"")
