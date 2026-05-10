UMODE
ACK_ALARMS
INTEGER(OPMK,"")//
//
FLAG(ANCK,"The answerback check status (Ждет исполнения команды)")
FLAG(AOFS,"")
FLAG(CALI,"")
FLAG(Res1,"")
ENUM(BSTS   ,"",NR,SIM,ANCK)
FLAG(Res2,"")
FLAG(Res3,"")
FLAG(Res4,"")
//
INTEGER(PV,"")// 0,1,2
INTEGER(MV,"")// 0 или 2 ( если 1, то идёт в 2 )
INTEGER(CSV,"")
//
INTEGER(SIMM,"")//0-1
INTEGER(BPSW,"")//0-1
//
INTEGER(TSW,"")
INTEGER(CAS,"")
INTEGER(AF,"")
