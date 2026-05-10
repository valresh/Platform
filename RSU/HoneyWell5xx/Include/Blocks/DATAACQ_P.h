Real(ALMDB,"Overall Deadband for all analog alarms",0.)
Enumeration(ALMDBU,"Overall Deadband unit for all analog alarms",Percent,EU,Def=Percent)
Integer(ALMTM,"Filter Time for all alarms",0)
String_K(DESC,"Block Descriptor",132*4)
String_K(EUDESC,"Block Engineering Units Descriptor",24*4)
Struct_BEI(BADPVALM,FL,ALPRIOR,PR,SV)
Integer_A(INSBLOCK,"Insert Block",10,0)//- Identifies name of CAB instance associated with insertion type. 
Struct_EI(INSFAIL,ALPRIOR,PR,SV)
Real(LOCUTOFF,"Low Signal Cutoff",NaN)
String_K(NAME,"",32*4)
//Integer(ORDERINCM,"Execution order",10)// of a component block relative to other component blocks within a Control Module (CM) function
Integer(NUMINSERT,"",0)
Enumeration(P1CLAMPOPT,"Process Input Value #1 (P1) Clamping Option",Disable,Enable,Def=Disable)
Real(P1FILTTIME,"(P1) Filter Time (in minutes)",0.)
Enumeration(PVCHAR,"PV Characterization Option",None,Linear,SquareRoot,Def=Linear)//Defines the characterization to be used for characterizing the PI value.
Real(PVEUHI,"PV High Range",100.)//(in Engineering Units) - This value corresponds to 100% of full scale, Greater-than PVEULO and less-than or equal to PVEXHILM.
Real(PVEULO,"PV Low Range",0.)//(in Engineering Units) - This value corresponds to 0% of full scale, Less-than PVEUHI and Greater-than or equal to PVEXLOLM.
Real(PVEXHILM,"(PV) High Limit",102.9)//(in Engineering Units) - Used to detect and clamp a bad Process Variable (PV) value, Greater-than or equal to PVEUHI.
Real(PVEXLOLM,"(PV) Low Limit",-2.9)//(in Engineering Units) - Used to detect and clamp a bad PV value, Less than or equal to PVEULO
Enumeration(PVFORMAT,"Process Variable (PV) Display Format",D0,D1,D2,D3,Def=D1)//D0 (-XXXXXX.) D1 (-XXXXX.X) D2 (-XXXX.XX) D3 (XXX.XXX)
Struct_REBEIIR(PVHHALM,DB,ALMDBU,DBU,FL,ALPRIOR,PR,SV,TM,TP)
Struct_REBEIIR(PVHIALM,DB,ALMDBU,DBU,FL,ALPRIOR,PR,SV,TM,TP)
Struct_IR(PVHISIGCHG,CT,TP,NaN)
Struct_REBEIIR(PVLLALM,DB,ALMDBU,DBU,FL,ALPRIOR,PR,SV,TM,TP)
Struct_REBEIIR(PVLOALM,DB,ALMDBU,DBU,FL,ALPRIOR,PR,SV,TM,TP)
Struct_IR(PVLOSIGCHG,CT,TP,NaN)
Enumeration(PVSOURCE,"Identifies the source of the PV input",Sub,Man,Auto,Def=Auto)
Enumeration(PVSRCOPT,"PV Source Option",OnlyAuto,All,Def=OnlyAuto)//Identifies the PV source selection options available to this function block.

Enumeration(ПолучениеP1EU,"",Брать_с_P1,Честно,Def=Брать_с_P1)
Boolean(INALM,"Function Block In Alarm Flag",false)// - Indicates if an alarm condition has been detected with this function block.
Struct_EIE(HIALM,ALPRIOR,PR,SV,DACALMTYPE,TYPE)

Boolean(GOODPVHHFL,"",false);
Boolean(GOODPVHIFL,"",false);
Boolean(GOODPVLOFL,"",false);
Boolean(GOODPVLLFL,"",false);

Struct_EII(CHGOFSTALM,ALPRIOR,PR,SV,TM)
Boolean(CHGOFSTALMENB,"COS Alarm Enable option ",false)