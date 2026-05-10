YPARM_S(Comment,64,"ETCM")
YPARM_D(MTM,4.,"answerback check suppression timer")
YPARM_L(Answer_Direction, 0, "ABID", "%{DIRECT;REVERSE;INVERTDIR;INVERTREV}" )
YPARM_L(Answerback_Check,3, "ABCH", "%{NO;OPEN;CLOSE;BOTH}")
YPARM_L(Out_Action_Direct,1,"OADR", "%{REVERSE;DIRECT;INVERTDIR;INVERTREV}")
YPARM_L(Label,0,"LBNO", "%{DIR;REV}")
YPARM_B(Output_Open_alarm,true,"ALOP") 
YPARM_I(Inhibit_PERR,0,"ABMK");
YPARM_L(Ans_Abnorm_Alarm,0,"ANSS","%{NO;ON;OFF;BOTH}")
YPARM_B(Answerback_Trk, true, "ABTK")
//
YPARM_I(AlarmLevel,12,"EALA")
YPARM_B(BAD_connection_alarm,false,"ILCN" )// BAD connection alarm
YPARM_L(Input_Open_alarm,0,"INOP","%{NO;H;L;HL}")
//