Struct_BEI(BADPVALM,FL,ALPRIOR,PR,SV)
Boolean(BYPASS,"Permissive and interlock bypass",false)
Boolean(BYPPERM,"Override and Permissive Bypass Permit",false)
Struct_BEIIa(CMDDISALM,FL,ALPRIOR,PR,SV,TM,2)
Struct_BEIIa(CMDFALALM,FL,ALPRIOR,PR,SV,TM,2)
String_K(DESC,"Block Descriptor",132*4)
String_A(DIPVMAP,"Actual State Definitions",16 * 4,15)
String_K(EUDESC,"Block Engineering Units Descriptor",24*4)
Enumeration(HOLDOPT,"The Hold State Option",None,Man,Auto,Cas,FixedOP,HoldPv,FixedSP,RampedSP,Def=None)
Enumeration(INITOPOPT,"(OP) Initialization Option",NORMALOPT,HOLDOPOPT,SAFEOPOPT,Def=NORMALOPT)
Boolean(MAINTOPT,"Maintenance Statistics Option",false)
Real_A(MAXTIME,"Maximum Duration per State",2,0)
Real_A(MAXTRANS,"Maximum Transitions per State",2,0)
Enumeration(MODETRACK,"Mode Tracking Option",None,OneShot,ContRtn,SemiCont,Cont,Def=OneShot)
Enumeration(MOMSTATE,"Momentary Output States",None,STATE_0,STATE_1,STATE_0AND1,STATE_2,STATE_0AND2,STATE_1AND2,Def=None)
String_K(NAME,"",32*4)
Enumeration(NORMMODE,"",Man,Auto,Cas,Bcas,None,Def=Man)
Enumeration(NORMMODEATTR,"",None,Operator,Program,Def=None)
Integer(NUMDINPTS,"Number of Digital Inputs",0)
Integer(NUMDOUTS,"Number of Digital Outputs",0)
Integer(NUMSTATES,"Number of Settable States",2)
Struct_BBEI(OFFNRMALM,FL,OPT,ALPRIOR,PR,SV)
StructA_BBEI(OIALM,2,FL,OPT,ALPRIOR,PR,SV)
Boolean_AA(OPDOMAP,"Output (OP) to Digital Output Values (DO)",3,3,false)// Map of Settable State Assignments.
Integer(ORDER,"Execution Order within containing Control Module",10)
//Integer(ORDERINCM,"Execution order",10)// of a component block relative to other component blocks within a Control Module (CM) function block
Boolean_A(POCONNECTED,"Pulse Connection Flag",3,false)
Real_A(PULSEWIDTH,"Output Pulsewidth",3,1.)
Enumeration(PVSOURCE,"Identifies the source of the PV input",Sub,Man,Auto,Track,Def=Auto)
Enumeration(PVSRCOPT,"PV Source Option",OnlyAuto,All,Def=OnlyAuto)//Identifies the PV source selection options available to this function block.
Enumeration(RESTARTOPT,"Restart Option",None,LastReq,Def=None)//Indicates activity for device when ControlModule.SCMSTATE returns to Restarting or Running
Enumeration(SAFEOP,"",S0=4,S1=5,S2=6,Def=S0)//Safe Output State S0(4): State0, State1, State2
                                                              //S1(5): State1, State0, State2
                                                              //S2(6): State2, State0, State1
Boolean(SAFEREDTAG,"Safe State Before RedTag",false)// - Requires that Output (OP) be Safe before RedTag may be set.
Boolean(SEALOPT,"Seal-in Option",false)
Boolean(SI,"Safety Override Interlock",false)
Struct_BBEI(SIALM,FL,OPT,ALPRIOR,PR,SV)
Enumeration(STARTOPT,"Starting State Option",None,Man,Auto,Cas,FixedOP,HoldPv,FixedSP,RampedSP,Def=None)
String_A(STATETEXT,"Text for Defined States",12*4,6)
Enumeration(STOPOPT,"Fail State Option",None,Man,Auto,Cas,FixedOP,HoldPv,FixedSP,RampedSP,Def=None)
Boolean_A(DIXCONNECTED,"DIX Connection Flag",4,0)
Enumeration(INITOPAFTLD,"Init OP After Load",Default,State0, State1, State2,Def=Default)
Enumeration(LEGACYINITOPT,"Legacy OP Initialization Option",Disable,Enable,Def=Disable)
String_K(OPERTAGDESC,"Operator Tag Descriptor",15*4)

Struct_EIE(HIALM,ALPRIOR,PR,SV,DEVHIALM,TYPE)
Struct_EII(CHGOFSTALM,ALPRIOR,PR,SV,TM)
Boolean(CHGOFSTALMENB,"COS Alarm Enable option ",false)
