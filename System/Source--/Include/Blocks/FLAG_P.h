Enumeration(ACCLOCK,"Access Lock",Operator,Engineer,OtherFB,Def=Operator)// - Specifies who can change the Flag function block's state (using PV and PVFL), and who can change the Numeric function block's value (PV).
String_K(DESC,"Block Descriptor",132*4)
String_K(EUDESC,"Block Engineering Units Descriptor",24*4)
String_K(NAME,"",32*4)
Enumeration(NORMAL,"Defines the normal state of the Flag function block.",None,State0,State1,Def=None)

String_K(BLCKCOMMENT1,"Block Comment 1",40*4)// - Lets you enter pertenient comment to be associated with this block .
String_K(BLCKCOMMENT2,"Block Comment 2",40*4)// - Lets you enter pertenient comment to be associated with this block .
String_K(BLCKCOMMENT3,"Block Comment 3",40*4)// - Lets you enter pertenient comment to be associated with this block .
String_K(BLCKCOMMENT4,"Block Comment 4",40*4)// - Lets you enter pertenient comment to be associated with this block .

Struct_EIE(HIALM,ALPRIOR,PR,SV,HIALMFLAG,TYPE)
Boolean(INALM,"In Alarm Flag",false)

Struct_EII(CHGOFSTALM,ALPRIOR,PR,SV,TM)
Boolean(CHGOFSTALMENB,"COS Alarm Enable option ",false)
