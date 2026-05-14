Boolean(BADPVFL,"Bad PV Flag",false)// - Flag that indicates that an error response was received for the last request sent by this request block. Note that this flag is set to TRUE for both local errors (communication errors that prevented the request from being sent out on the network, for example) and remote errors (the target device could not process the request and so sent an explicit failure status). For alarming on errors, connect the ERRFL to a Flag Block and configure the Flag Block to send an Alarm when ERRFL is TRUE.
Enumeration(CHANSTS,"Channel Status",NotUsed,NoData,Idle,CEEIdle,Simulate,Fail,Run,Def=NotUsed)
Boolean(DONEFL,"Flag indicating a successful response",false)// was received for the last request sent by this request block.
Enumeration16(ERRCODE,"Error Status on the last request operation",OK,Def=OK)
Boolean(ERRFL,"",false);//Flag that indicates that an error response was received for the last request sent by this request block. Note that this flag is set to TRUE for both local errors (communication errors that prevented the request from being sent out on the network, for example) and remote errors (the target device could not process the request and so sent an explicit failure status). For alarming on errors, connect the ERRFL to a Flag Block and configure the Flag Block to send an Alarm when ERRFL is TRUE.
Enumeration(IOCSTATE,"PCDI Channel State",INACTIVE,OK,UNAVAIL,INITREQLATCH,NOSOURCE,BADSOURCE,Def=INACTIVE)
Time_A(LASTERRTIME,"Time of Last Error",1,0)
Integer(MASTERID,"Connected Master Block Key",0)
Boolean_A(PV,"Process Variable",1968,0)
Boolean(PVSTS,"",false)
Boolean(RDYFL,"Ready Flag",false)
Enumeration(REQSTATE,"Current Request State",Idle,ReqAlloc,Adding,AddFail,Ready,Sending,Sent,Freeing,Complete,Failed,Cancel,Def=Idle)
Boolean(SENDFL,"",false)//The flag used as a trigger for the request block to send a request to the target. A request is sent tothe target when this flag transitions from False to True if the READYFL parameter was previously True. For periodic reads or writes, the READYFL must be connected to the SENDFL.
Boolean_A(SIMVALUE,"Simulation Value",1968,0)
Enumeration(IOMCONN,"Currently Assigned Channel",INCONN,OUTCONN,PCONN,Def=INCONN)
Boolean_A(PVFL,"Actual State Flag",99,false)