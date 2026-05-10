String_K(DESC,"Block Descriptor",132*4)
Enumeration(ALMOPT,"Alarming Option",NONE,OFFNORMAL,CHNSOFTS,Def=NONE)//the alarm option to be used for this point
Integer(CHANNUM,"CHANNUM",1)
String_K(CONTAINEDIN,"Contained in",16*4)
Integer(DEBOUNCE,"Contact Debounce Time",10)
String_K(DEVICELOCATION,"Associated Device Location",64*4)
Enumeration(DITYPE,"Digital Input Type",STATUS,LATCHED,Def=STATUS)
Integer(DLYTIME,"Delay Time",5)//Specifies the time in seconds that a point in alarm is to be held in the alarm state even if the condition clears
Enumeration(EVTOPT,"Event Reporting Option",NONE,SOE,Def=NONE)// Defines the type of event reporting to be supported for this data point
String_K(IOP,"Associated I/O Module",32*4)
Enumeration(IOPTYPE,"IOM Type",DI_HV=54,DI_24,DI_SOE,Def=DI_24)
Enumeration(PNTFORM,"",Full,Component,Def=Component)
Enumeration(PNTTYPE,"",AI=1,AO,DI,DO,Def=DI)
Enumeration(PVSOURCE,"Identifies the source of the PV input",SUB,MAN,AUTO,Def=AUTO)
Enumeration(PVSRCOPT,"PV Source Option",OnlyAuto,All,Def=OnlyAuto)//Identifies the PV source selection options available to this function block.

/*
Integer(DEBUG,"Spare for debug information",0)
Integer(FETCHMODE,"Fetch Mode",0)// - When this mode is set, the IO Channel works without the IOM function block.
Real(FREEZETIME,"Maximum Freeze Time",4.)
Enumeration(INBADOPT,"Input Bad Option",Off,On,Hold,Def=Off)// - Configurable Value from the following range is substituted when input is determined to be bad
Integer(IOCTYPE,"Channel Type",-1)
Integer(ORDERINCM,"Execution order",10)// of a component block relative to other component blocks within a Control Module (CM) function
*/