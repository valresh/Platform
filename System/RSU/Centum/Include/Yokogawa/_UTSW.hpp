UMODE
ACK_ALARMS
INTEGER(OPMK,"")//

ENUM(BSTS   ,"",STOP,PAUS,RUN)
ENUM(PREBSTS,"",STOP,PAUS,RUN)
//
FLAG(AOFS   ,"")

STRING(UBSC,16,"")
//The total number of data may be arrayed is 10000 or less.
STRING_skipSave(USER,10000*2,"")
