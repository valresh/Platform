Enumeration(ACCLOCK,"Access Lock",Operator,Engineer,OtherFB,Def=Operator)//" - Specifies who can change the function block's value (PV) or state, as applicable"
Boolean(AUTOTRIGGER,"Auto Trigger Mode",true)// - Enable or disable mode
String_K(BLCKCOMMENT1,"Block Comment 1",40*4)// - Lets you enter pertenient comment to be associated with this block .
String_K(BLCKCOMMENT2,"Block Comment 2",40*4)// - Lets you enter pertenient comment to be associated with this block .
String_K(BLCKCOMMENT3,"Block Comment 3",40*4)// - Lets you enter pertenient comment to be associated with this block .
String_K(BLCKCOMMENT4,"Block Comment 4",40*4)// - Lets you enter pertenient comment to be associated with this block .
String_K(CHANNAME,"Channel Name",40*4)
Integer(DEVADDR,"Serial Link Device Address",1)// - Identifies a device on the serial link connected to the FTA.
Integer(IOCNUMBER,"Channel Number",-1)
String_K(NAME,"Module Name",32*4)
Integer(NFLAG,"The number of Boolean ",1)//values that must be read or written by this block.
//Integer(ORDERINCM,"Execution order ",10)// of a component block relative to other component blocks within a Control Module (CM) function block.
Enumeration(SIMMODE,"Simulation Mode",None,DirectSub,SimValSub,Initdisable,Def=None)
Integer(STARTINDEX,"Starting Element Index",1)//- Identifies the start of the contiguous array in the device. 
Boolean(SUBVAL,"Value to Substitute when error occurs",false)
Enumeration(SUBVALTYPE,"Type of Value to Substitute when error occurs",LastKnownGood,SUBVALparam,NaN,Def=LastKnownGood)
Enumeration(WRITEOPT,"Read/Write Option",WriteOnDiff,WriteAlways,WriteOnChg,ReadOnly,WriteAlwaysWriteOnly,WriteOnChgWriteOnly,Def=WriteOnDiff)//
