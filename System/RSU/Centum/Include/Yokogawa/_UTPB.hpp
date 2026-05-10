UMODE
ACK_ALARMS
INTEGER(OPMK,"")//

ENUM(BSTS   ,"",STOP,PAUS,RUN)
ENUM(PREBSTS,"",STOP,PAUS,RUN)
//
FLAG(_AF,"")
FLAG(AOFS   ,"")

INTEGER(STEPNO ,"")
INTEGER(SAID   ,"")
INTEGER(UAID   ,"")

INTEGER_A(SWCR,5,"")//цвета кнопок 0 15
STRING_A(SWLB,5,16,"")//метки кнопок(char[8])
INTEGER_A(SWST,5,"")//моргание ламп	(0,1)
INTEGER_A(SWOP,5,"")//отключение -15 15

//The total number of data may be arrayed is 10000 or less.
STRING_skipSave(USER,10000*2,"")
INTEGER(AF,"")