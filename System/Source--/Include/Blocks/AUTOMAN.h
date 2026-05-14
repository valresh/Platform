Enumeration_A(ARWNET,"Network Windup Status",8,Normal,Hi,Lo,HiLo,Def=Normal)
Enumeration(ARWNETIN,"Anti-Reset Windup Network Status Input",Normal,Hi,Lo,HiLo,Def=Normal)
Enumeration(ARWOP,"Output Anti-Reset Windup Status",Normal,Hi,Lo,HiLo,Def=Normal)
Enumeration(ARWOPIN,"Output Anti-Reset Windup Output (OP) Input",Normal,Hi,Lo,HiLo,Def=Normal)
Integer(ASTEPID,"",0)
Struct_BEI(BADCTLALM,FL,ALPRIOR,PR,SV)
Boolean(BADCTLFL,"if a Bad Control condition exists",false)
Boolean(COMPUTEARW,"Compute Anti-Rest Windup (ARW)",false)// - Indicates that the windup status has to be recomputed
Boolean(CTLINIT,"Control Initialization Request Flag",false)// - Used to request one-shot initialization of a Regulatory Control block. May be written to by a user program or another function.
Enumeration(CTLSTATE,"Control State",FWD,Hold,INIT,MAN,Def=FWD)// - Shows the control state of the block.
Real(CV,"Calculated Value",NaN)//The result (calculated value) of the function blocks. The value may be in Percent or Engineering Units, depending on the function block
Struct4B(ESWFL,"",AUTO,BCAS,CAS,MAN)
Enumeration(FBORSTS,"Override Status",NotCon,Sel,NotSel,Def=NotCon)
Real(HOLDRATE,"",NaN)
Real(HOLDVAL,"",NaN)
Boolean(INITMAN,"Initialization Manual Flag",false)//When On, it indicates that the function block is in Initialization Manual. INITMAN goes On when either a cascade is broken (i.e., a block's mode is not Cascade), or initialization is requested by a secondary
Boolean_A(INITREQ,"Initialization Request Flags",8,false)
Struct_BEI(INSFAILALM,FL,ALPRIOR,PR,SV)//Indicates if an alarm condition has been detected for an insertion failure
Boolean(INSFAILFL,"Insertion Fail Flag",false)// - Indicates if an alarm condition has been detected for an insertion failure
Enumeration(LASTMODEREQ,"Last value of MODEREQ",Man,Auto,Cas,None,Def=None)//when SCMMODE is Running.
Real(LASTOPREQ,"Last value of OPREQ ",0)//SCMMODE is Running
Enumeration(LASTOPTYPE,"Last OPTYPE",Null,Set,NotStored,Default,Def=Null)
Boolean(LASTREQFL,"Last Requested Parameters Flag",0)
String_K(LASTSTEP,"",8*4)
Enumeration(MODE,"",Man,Auto,Cas,BackupCascade,Normal,None,Def=Man)
Boolean_A(MODEAPPL,"Mode Applicability - Defines changes for Regulatory",4,0)
Enumeration(MODEATTR,"",Operator=1,Program,Def=Operator)
Struct3B(MODEATTRFL,"",NORM,OPER,PROG)
Boolean(MODECHANGE,"Mode Change",false)//- Indicates the first time mode changes to Auto or Cascade.
Struct5B(MODEFL,"",AUTO,BCAS,CAS,MAN,NORM)
Enumeration(MODEREQ,"The requested MODE",Man,Auto,Cas,None,Def=None)//The requested MODE
Enumeration(MODETRACK,"Mode Tracking Option",None,OneShot,ContRtn,SemiCont,Cont,Def=OneShot)
Integer(NUMONESHOT,"",0)
Integer(NUMPROPREQ,"",0)
Integer(NUMSEC,"",0)
Real(OP,"in Percent",0)
Real(OPBIAs,"Total Output Bias (in Engineering Units)",0)//is the sum of two components: a user-specified fixed bias (OPBIAS.FIX) and a calculated floating bias (OPBIAS.FLOAT)
Real(OPEU,"in Engineering Units",0)
Boolean(OPEXHIFL,"OP High Limit Flag",false)//Indicates if OP has reached the high limit, as specified by OPHILM
Boolean(OPEXLOFL,"OP Low Limit Flag",false)//Indicates if OP has reached its low limit, as specified by OPLOLM
Boolean(OPHIFL,"OP High Limit Flag",false)//Indicates if OP has reached the high limit, as specified by OPHILM
Boolean(OPLOFL,"OP Low Limit Flag",false)//Indicates if OP has reached its low limit, as specified by OPLOLM
Real(OPREQ,"",0)
Boolean(OPROCNEGFL,"OP Negative Rate of Change Flag",false)
Boolean(OPROCPOSFL,"OP Positive Rate of Change Flag",false)
Enumeration(OPTYPE,"SCM Control Output Request Type Option",Null,Set,NotStored,Default,Def=Null)//Sequential Control Module 
Boolean(REDTAG,"Red Tag",false)// - Allows the user to set the FB as being "out-of-service"; this indicates that the FB or the associated control strategy needs repair, or is being repaired
Real(SAFEOP,"in Percent",NaN)
Boolean(SIFL,"Safety Interlock Flag",false)
Real(X1,"",NaN)
Real(X1P,"X in Percent",NaN)
Enumeration(X1STS,"Process Variable (PV) Status",Bad,Uncertain,Normal,Manual,Def=Bad)

Real(BACKCALCIN,"",NaN)
Real(BACKCALCOUT,"",NaN)
