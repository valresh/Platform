String_K(DESC,"Block Descriptor",132*4)
//Integer(ORDERINCM,"Execution order",10)
String_K(EUDESC,"Block Engineering Units Descriptor",24*4)
Integer(CHANNUM,"CHANNUM",1)
String_K(CONTAINEDIN,"Contained in",16*4)
String_K(DEVICELOCATION,"Associated Device Location",64*4)
Enumeration(FAULTOPT,"Fault Option",Hold,Unpower,Power,UseFaultValue,Def=Unpower)
Real(FAULTVALUE,"Fault Value",-6.9)
Boolean(HALARMENABLE,"Enable HART Alarms and Events",false)
Integer(HARTVERSION,"Supported HART Version",0)
Integer(HCFGDEV,"",0)
String_K(HTAG,"",8*4)
String_K(IOP,"Associated I/O Module",32*4)
Enumeration(IOPTYPE,"IOM Type",AI_HART=51,AI_LLMUX,AO_HART,Def=AI_HART)
Real(INITVAL,"Initialization Value",0)//- The value to which a primary should initialize. There is one value for each primary.
Boolean(OPCHAR,"Output Characterization",false)//Specifies whether the output characterization function is to be used for this data point or not
Enumeration(PNTFORM,"",Full,Component,Def=Component)
Enumeration(PNTTYPE,"",AI=1,AO,DI,DO,Def=AO)
Enumeration(PTEXECST,"Point Execution State",Inactive,Active,Def=Inactive)

/*Real(CALBIAS,"Calibration Bias",0)// - Specifies a bias value which is applied to the raw analog data.
Integer(DEBUG,"Spare for debug information",0)
Integer(FETCHMODE,"Fetch Mode",0)// - When this mode is set, the IO Channel works without the IOM function block.
Real(FREEZETIME,"Maximum Freeze Time",4.)
Real(INITVAL,"Initialization Value",0)//- The value to which a primary should initialize. There is one value for each primary.
Integer(IOCTYPE,"Channel Type",-1)
Enumeration(OPTDIR,"Output Direction",DIRECT,REVERSE,Def=DIRECT)//Direct or Reverse mode settings define the output action to be applied to OP that determines the OPFINAL value used to control the actual field device.
                                                                //When reverse mode OPFINAL = 100 - OP.
*/