Enumeration(ACCLOCK,"Access Lock",Operator,Engineer,OtherFB,Def=Operator)//" - Specifies who can change the function block's value (PV) or state, as applicable"
Boolean(AUTOTRIGGER,"Auto Trigger Mode",true)// - Enable or disable mode
String_K(BLCKCOMMENT1,"Block Comment 1",40*4)// - Lets you enter pertenient comment to be associated with this block .
String_K(BLCKCOMMENT2,"Block Comment 2",40*4)// - Lets you enter pertenient comment to be associated with this block .
String_K(BLCKCOMMENT3,"Block Comment 3",40*4)// - Lets you enter pertenient comment to be associated with this block .
String_K(BLCKCOMMENT4,"Block Comment 4",40*4)// - Lets you enter pertenient comment to be associated with this block .
String_K(CHANNAME,"Channel Name",40*4)
Boolean(CONVTOLONG,"Convert to Long",false)// - Determines if numeric values will be represented as floating point numbers, when not checked, or longs.
Integer(DEVADDR,"Serial Link Device Address",1)// - Identifies a device on the serial link connected to the FTA.
String_K(EUDESC,"Block Engineering Units Descriptor",24*4)// - Used to describe an important aspect of the block, for example, Engineering Units.
Integer(IOCNUMBER,"Channel Number",-1)
String_K(NAME,"Module Name",32*4)
Integer(NNUMERIC,"Number of Numeric Values",1)// - Specifies the number of numeric values in the array of data from the serial device.
//Integer(ORDERINCM,"Execution order ",10)// of a component block relative to other component blocks within a Control Module (CM) function block.
Real_A(PVEUHI,"Process Variable (PV) High Range",120,100.)//(in Engineering Units) - This value corresponds to 100% of full scale.
Real_A(PVEULO,"Process Variable (PV) Low Range",120,0)//(in Engineering Units) - This value corresponds to 0% of full scale.
Real_A(PVRAWHI,"Process Variable (PV) Raw High Range",120,100.)//Specifies the high end of the normal operating range for the raw PV (PVRAW) input value. 
Real_A(PVRAWLO,"Process Variable (PV) Raw Low Range",120,0)//Specifies the low end of the normal operating range for the raw PV (PVRAW) input value. 
Enumeration(SIMMODE,"Simulation Mode",None,DirectSub,SimValSub,Initdisable,Def=None)
Integer(STARTINDEX,"Starting Element Index",1)//- Identifies the start of the contiguous array in the device. 
Boolean(SUBVAL,"Value to Substitute when error occurs",false)
Enumeration(SUBVALTYPE,"Type of Value to Substitute when error occurs",LastKnownGood,SUBVALparam,NaN,Def=LastKnownGood)
Enumeration(WRITEOPT,"Read/Write Option",WriteOnDiff,WriteAlways,WriteOnChg,ReadOnly,WriteAlwaysWriteOnly,WriteOnChgWriteOnly,Def=WriteOnDiff)//
