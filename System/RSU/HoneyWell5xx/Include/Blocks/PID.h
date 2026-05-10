Boolean(ADVDEVOPT,"Advisory Deviation Option",false)
Real(ADVSP,"Advisory Setpoint Value (in Engineering Units)",0)
Real(ADVSPP,"Advisory Setpoint Value (Real(ADVSP,"Advisory Setpoint Value (in Engineering Units)",0))",0)
Integer(ASTEPID,"",0)
Struct_BEI(BADCTLALM,FL,ALPRIOR,PR,SV)
Boolean(BADCTLFL,"if a Bad Control condition exists",false)
Boolean(CASREQFL,"Cascade Request Flag",false)
Boolean(COMPUTEARW,"Compute Anti-Rest Windup (ARW)",false)// - Indicates that the windup status has to be recomputed
Boolean(CTLINIT,"Control Initialization Request Flag",false)// - Used to request one-shot initialization of a Regulatory Control block. May be written to by a user program or another function.
Enumeration(CTLSTATE,"Control State",FWD,Hold,INIT,MAN,Def=FWD)// - Shows the control state of the block.
Real(CV,"Calculated Value",NaN)//The result (calculated value) of the function blocks. The value may be in Percent or Engineering Units, depending on the function block
Real(DELCV,"Incremental Change in CV",NaN)//in Percent.
Real(DEV,"",NaN)//- The difference between the Process Variable (PV) and Set Point (SP).
Struct4B(ESWFL,"",AUTO,BCAS,CAS,MAN)
Enumeration(FBORSTS,"Override Status",NotCon,Sel,NotSel,Def=NotCon)
Boolean(INITMAN,"Initialization Manual Flag",false)//When On, it indicates that the function block is in Initialization Manual. INITMAN goes On when either a cascade is broken (i.e., a block's mode is not Cascade), or initialization is requested by a secondary
Boolean_A(INITREQ,"Initialization Request Flags",8,false)
Struct_BEI(INSFAILALM,FL,ALPRIOR,PR,SV)//Indicates if an alarm condition has been detected for an insertion failure
Boolean(INSFAILFL,"Insertion Fail Flag",false)// - Indicates if an alarm condition has been detected for an insertion failure
Real(LASTGOODPV,"",NaN)
Enumeration(LASTMODEREQ,"Last value of MODEREQ",Man,Auto,Cas,None,Def=None)//when SCMMODE is Running.
Real(LASTOPREQ,"Last value of OPREQ ",0)//SCMMODE is Running
Enumeration(LASTOPTYPE,"Last OPTYPE",Null,Set,NotStored,Default,Def=Null)
Real(LASTRATEREQ,"Last value of SPRATEREQ",0.)
Real(LASTSPREQ,"Last value of SPREQ",0)
Real(LASTSPTVREQ,"Last Requested Set Point Target Value (SPTVREQ)",0)
String_K(LASTSTEP,"",8*4)
Enumeration(MODE,"",Man,Auto,Cas,BCas,Normal,None,Def=Man)//BackupCascade
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
Boolean(OPEXHIFL,"OP Extended High Limit Flag",false)
Boolean(OPEXLOFL,"OP Extended Low Limit Flag",false)
Boolean(OPHIFL,"OP High Limit Flag",false)//Indicates if OP has reached the high limit, as specified by OPHILM
Boolean(OPLOFL,"OP Low Limit Flag",false)//Indicates if OP has reached its low limit, as specified by OPLOLM
Real(OPREQ,"",0)
Boolean(OPROCNEGFL,"OP Negative Rate of Change Flag",false)
Boolean(OPROCPOSFL,"OP Positive Rate of Change Flag",false)
Enumeration(OPTYPE,"SCM Control Output Request Type Option",Null,Set,NotStored,Default,Def=Null)//Sequential Control Module 
Real(PV,"",NaN)
Real(PVP,"PV in Percent",NaN)
Enumeration(PVSTS,"Process Variable (PV) Status",Bad,Uncertain,Normal,Manual,Def=Bad)
Struct4B(PVSTSFL,"",BAD,MAN,NORM,UNCER)
Boolean(REDTAG,"Red Tag",false)// - Allows the user to set the FB as being "out-of-service"; this indicates that the FB or the associated control strategy needs repair, or is being repaired
Real(SAFEOP,"in Percent",NaN)
Boolean(SIFL,"Safety Interlock Flag",false)
Real(SP,"",0)
Boolean(SPHIFL,"SP High Limit Flag",false)//Indicates if SP has exceeded its upper limit, as specified by the Set Point High Limit (SPHILM).
Boolean(SPLOFL,"SP Low Limit Flag",false)//Indicates if SP has exceeded its lower limit, as specified by the Set Point Low Limit (SPLOLM).
Real(SPP,"SP (in Percent)",NaN)
Real(SPRATEREQ,"Requested SP ramp rate ",0)
Real(SPREQ,"The requested SP value",0)//The SCM CONTROLREQ wants a step-change in the regulatory control block's SP
Real(SPTV,"SP Target Value (in Engineering Units)",0)//Contains the target SP value when SP ramping is being used
Boolean(SPTVDEVFL,"SP Target Value Deviation Flag",false)
Enumeration(SPTVOPT,"SP Target Value Option",Disable,Enable,Def=Disable)//Indicates if SP target value processing is requested
Real(SPTVP,"SP Target Value (in Percent)",0)
Real(SPTVRATE,"SP Target Value Ramp Rate (calculated)",0)
Real(SPTVREQ,"Requested SP target value",0)
Real(SPTVTIME,"SP Target Value Ramp Time",0)
Real(STARTRATE,"Starting State SP Ramp Rate",NaN)
Enumeration(SPTVSTATE,"SP Target Value Processor State",Off,Preset,Run,Def=Off)
Boolean(TMOUTFL,"Timeout Flag",false)// - Indicates if an initializable input has timed-out.
//////////////////////////////////////////////////////////////////////////
Real(SP_OLD,"",0)
Real(PVPm,"PV in Percent",0)
Real(SPPm,"SP (in Percent)",0)
//////////////////////////////////////////////////////////////////////////
Enumeration(EXECSTATE,"Execution state of the block",Inactive,Active,Def=Active)

Real(BACKCALCIN,"",NaN)
Real(BACKCALCOUT,"",NaN)
