#undef  ACK_ALARM
#define ACK_ALARM \
  theList.AddHW( "UnackAlarmExists", offsetof(shareObj,notACK), offsetof(netObj,notACK),enumValueChr, TypeID ); \
  theList.AddHW( "Квитирование"    , offsetof(shareObj,setACK), offsetof(netObj,setACK),enumValueChr, TypeID ); \
  theList.AddHW( "EHLN"            , offsetof(shareObj,btEHLN), offsetof(netObj,btEHLN),enumValueChr, TypeID ); \
  theList.AddHW( "MESS"            , offsetof(shareObj,btType), offsetof(netObj,btType),enumValueChr, TypeID ); \
  theList.AddHW( "NoACK"           , offsetof(shareObj,notACK), offsetof(netObj,notACK),enumValueChr, TypeID ); \
  theList.AddHW( "Shelved"         , offsetof(shareObj,Shelved),offsetof(netObj,Shelved),enumValueInt, TypeID );

#undef  EnumerationDecl
#define EnumerationDecl(Type,...) static char * __enum_vals_##Type = #__VA_ARGS__;

#undef Boolean
#define Boolean(Var,Desc,Def) theList.AddHW( #Var, offsetof(shareObj,Var), offsetof(netObj,Var), enumValueChr, TypeID );

#undef BooleanEx2
#define BooleanEx2(Var,Desc,Def,Name2) Boolean(Var,Desc,Def) theList.AddHW( #Name2, offsetof(shareObj,Var), offsetof(netObj,Var), enumValueChr, TypeID );

#undef Byte_A
#define Byte_A(Var,Desc,Len,Def) theList.AddHW( #Var, offsetof(shareObj,Var), offsetof(netObj,Var), enumValueChr, TypeID, _countof(((shareObj*)0)->Var), sizeof(((shareObj*)0)->Var[0]) );

#undef Boolean_A
#define Boolean_A(Var,Desc,Len,Def) theList.AddHW( #Var, offsetof(shareObj,Var), offsetof(netObj,Var), enumValueChr, TypeID, _countof(((shareObj*)0)->Var), sizeof(((shareObj*)0)->Var[0]) );

//#undef Boolean_AA
//#define Boolean_AA(Var,Desc,Len,Def) theList.AddHW( #Var, offsetof(shareObj,Var), offsetof(netObj,Var), enumValueChr, TypeID, _countof(((shareObj*)0)->Var), ((shareObj*)0)->Var );

#undef  String_K
#define String_K(Var,Desc,Max) theList.AddHWs( #Var, offsetof(shareObj,Var), offsetof(netObj,Var), enumValueStr, TypeID, Max );

#undef  String_A
#define String_A(Var,Desc,Max,Count) theList.AddHWS( #Var, offsetof(shareObj,Var), offsetof(netObj,Var), enumValueStr, TypeID, _countof(((shareObj*)0)->Var[0]), _countof(((shareObj*)0)->Var) );

#undef Integer
#define Integer(Var,Desc,Def) theList.AddHW( #Var, offsetof(shareObj,Var), offsetof(netObj,Var), enumValueInt, TypeID );

#undef Integer_A
#define Integer_A(Var,Desc,Len,Def) theList.AddHW( #Var, offsetof(shareObj,Var), offsetof(netObj,Var), enumValueInt, TypeID, _countof(((shareObj*)0)->Var), sizeof(((shareObj*)0)->Var[0]) );

#undef UInt64
#define UInt64(Var,Desc,Def) theList.AddHW( #Var, offsetof(shareObj,Var), offsetof(netObj,Var), enumValueI64, TypeID );

#undef Time
#define Time(Var,Desc,Def) theList.AddHW( #Var, offsetof(shareObj,Var), offsetof(netObj,Var), enumValueI64, TypeID );

#undef Time_A
#define Time_A(Var,Desc,Len,Def) theList.AddHW( #Var, offsetof(shareObj,Var), offsetof(netObj,Var), enumValueI64, TypeID, _countof(((shareObj*)0)->Var), sizeof(((shareObj*)0)->Var[0]) );

#undef Real
#define Real(Var,Desc,Def) theList.AddHW( #Var, offsetof(shareObj,Var), offsetof(netObj,Var), enumValueDbl, TypeID );

#undef Real_A
#define Real_A(Var,Desc,Len,Def) theList.AddHW( #Var, offsetof(shareObj,Var), offsetof(netObj,Var), enumValueDbl, TypeID, _countof(((shareObj*)0)->Var), sizeof(((shareObj*)0)->Var[0]) );

#undef  Enumeration
#define Enumeration(Var,Desc,...) theList.AddHW( #Var, offsetof(shareObj,Var), offsetof(netObj,Var), enumValueChr, TypeID, #__VA_ARGS__ );

#undef  Enumeration_A
#define Enumeration_A(Var,Desc,Len,...) theList.AddHW( #Var, offsetof(shareObj,Var), offsetof(netObj,Var), enumValueChr, TypeID, _countof(((shareObj*)0)->Var), sizeof(((shareObj*)0)->Var[0]), #__VA_ARGS__ );

#undef StructRE
#define StructRE(Var,Desc,realVal, enumVal) theList.AddHW( #Var"."#realVal, offsetof(shareObj,Var.realVal), offsetof(netObj,Var.realVal), enumValueDbl, TypeID );

#undef StructRB
#define StructRB(Var,Desc,realVal,bVal) \
  theList.AddHW( #Var"."#realVal, offsetof(shareObj,Var.realVal), offsetof(netObj,Var.realVal), enumValueDbl, TypeID );\
  theList.AddHW( #Var"."#bVal, offsetof(shareObj,Var.bVal), offsetof(netObj,Var.bVal), enumValueChr, TypeID );

#undef StructBE
#define StructBE(Var,Desc,bVal, enumVal) theList.AddHW( #Var"."#bVal, offsetof(shareObj,Var.bVal), offsetof(netObj,Var.bVal), enumValueChr, TypeID );

#undef Struct3B
#define Struct3B(Var,Desc,Val1,Val2,Val3) \
  theList.AddHW( #Var"."#Val1, offsetof(shareObj,Var.Val1), offsetof(netObj,Var.Val1), enumValueChr, TypeID );\
  theList.AddHW( #Var"."#Val2, offsetof(shareObj,Var.Val2), offsetof(netObj,Var.Val2), enumValueChr, TypeID );\
  theList.AddHW( #Var"."#Val3, offsetof(shareObj,Var.Val3), offsetof(netObj,Var.Val3), enumValueChr, TypeID );

#undef Struct4B
#define Struct4B(Var,Desc,Val1,Val2,Val3,Val4) \
  theList.AddHW( #Var"."#Val1, offsetof(shareObj,Var.Val1), offsetof(netObj,Var.Val1), enumValueChr, TypeID );\
  theList.AddHW( #Var"."#Val2, offsetof(shareObj,Var.Val2), offsetof(netObj,Var.Val2), enumValueChr, TypeID );\
  theList.AddHW( #Var"."#Val3, offsetof(shareObj,Var.Val3), offsetof(netObj,Var.Val3), enumValueChr, TypeID );\
  theList.AddHW( #Var"."#Val4, offsetof(shareObj,Var.Val4), offsetof(netObj,Var.Val4), enumValueChr, TypeID );

#undef Struct5B
#define Struct5B(Var,Desc,Val1,Val2,Val3,Val4,Val5) \
    theList.AddHW( #Var"."#Val1, offsetof(shareObj,Var.Val1), offsetof(netObj,Var.Val1), enumValueChr, TypeID );\
    theList.AddHW( #Var"."#Val2, offsetof(shareObj,Var.Val2), offsetof(netObj,Var.Val2), enumValueChr, TypeID );\
    theList.AddHW( #Var"."#Val3, offsetof(shareObj,Var.Val3), offsetof(netObj,Var.Val3), enumValueChr, TypeID );\
    theList.AddHW( #Var"."#Val4, offsetof(shareObj,Var.Val4), offsetof(netObj,Var.Val4), enumValueChr, TypeID );\
    theList.AddHW( #Var"."#Val5, offsetof(shareObj,Var.Val5), offsetof(netObj,Var.Val5), enumValueChr, TypeID );

#undef Struct2B
#define Struct2B(Var,Desc,Val1,Val2) \
  theList.AddHW( #Var"."#Val1, offsetof(shareObj,Var.Val1), offsetof(netObj,Var.Val1), enumValueChr, TypeID );\
  theList.AddHW( #Var"."#Val2, offsetof(shareObj,Var.Val2), offsetof(netObj,Var.Val2), enumValueChr, TypeID );

#undef Struct_BEI
#define Struct_BEI(Var,ValB,tValE,ValE,ValI) \
  theList.AddHW( #Var"."#ValB, offsetof(shareObj,Var.ValB), offsetof(netObj,Var.ValB), enumValueChr, TypeID );\
  theList.AddHW( #Var"."#ValE, offsetof(shareObj,Var.ValE), offsetof(netObj,Var.ValE), enumValueChr, TypeID, __enum_vals_##tValE );\
  theList.AddHW( #Var"."#ValI, offsetof(shareObj,Var.ValI), offsetof(netObj,Var.ValI), enumValueInt, TypeID );

#undef Struct_BEIII
#define Struct_BEIII(Var,ValB,tValE,ValE,ValI,ValI2,ValI3) \
  theList.AddHW( #Var"."#ValB, offsetof(shareObj,Var.ValB), offsetof(netObj,Var.ValB), enumValueChr, TypeID );\
  theList.AddHW( #Var"."#ValE, offsetof(shareObj,Var.ValE), offsetof(netObj,Var.ValE), enumValueChr, TypeID, __enum_vals_##tValE );\
  theList.AddHW( #Var"."#ValI, offsetof(shareObj,Var.ValI), offsetof(netObj,Var.ValI), enumValueInt, TypeID );\
  theList.AddHW( #Var"."#ValI2, offsetof(shareObj,Var.ValI2), offsetof(netObj,Var.ValI2), enumValueInt, TypeID );\
  theList.AddHW( #Var"."#ValI3, offsetof(shareObj,Var.ValI3), offsetof(netObj,Var.ValI3), enumValueInt, TypeID );

#undef Struct_EIE
#define Struct_EIE(Var,tValB,ValB,ValI,tValE,ValE) \
  theList.AddHW( #Var"."#ValB, offsetof(shareObj,Var.ValB), offsetof(netObj,Var.ValB), enumValueChr, TypeID, __enum_vals_##tValB );\
  theList.AddHW( #Var"."#ValI, offsetof(shareObj,Var.ValI), offsetof(netObj,Var.ValI), enumValueInt, TypeID );\
  theList.AddHW( #Var"."#ValE, offsetof(shareObj,Var.ValE), offsetof(netObj,Var.ValE), enumValueChr, TypeID, __enum_vals_##tValE );

#undef Struct_EEEE
#define Struct_EEEE(Var,tValE1,ValE1,tValE2,ValE2,tValE3,ValE3,tValE4,ValE4) \
  theList.AddHW( #Var"."#ValE1, offsetof(shareObj,Var.ValE1), offsetof(netObj,Var.ValE1), enumValueChr, TypeID, __enum_vals_##tValE1 );\
  theList.AddHW( #Var"."#ValE2, offsetof(shareObj,Var.ValE2), offsetof(netObj,Var.ValE2), enumValueChr, TypeID, __enum_vals_##tValE2 );\
  theList.AddHW( #Var"."#ValE3, offsetof(shareObj,Var.ValE3), offsetof(netObj,Var.ValE3), enumValueChr, TypeID, __enum_vals_##tValE3 );\
  theList.AddHW( #Var"."#ValE4, offsetof(shareObj,Var.ValE4), offsetof(netObj,Var.ValE4), enumValueChr, TypeID, __enum_vals_##tValE4 );

#undef Struct_EI
#define Struct_EI(Var,tValB,ValB,ValI) \
  theList.AddHW( #Var"."#ValI, offsetof(shareObj,Var.ValI), offsetof(netObj,Var.ValI), enumValueInt, TypeID );

#undef Struct_EB
#define Struct_EB(Var,tValE,ValE,ValB) \
  theList.AddHW( #Var"."#ValB, offsetof(shareObj,Var.ValB), offsetof(netObj,Var.ValB), enumValueChr, TypeID );

#undef Struct_REBEIIR
#define Struct_REBEIIR(Var,R1,tE2,E2,B3,tE4,E4,I5,I6,R7) \
  theList.AddHW( #Var"."#R1, offsetof(shareObj,Var.R1), offsetof(netObj,Var.R1), enumValueDbl, TypeID );\
  theList.AddHW( #Var"."#E2, offsetof(shareObj,Var.E2), offsetof(netObj,Var.E2), enumValueChr, TypeID, __enum_vals_##tE2 );\
  theList.AddHW( #Var"."#B3, offsetof(shareObj,Var.B3), offsetof(netObj,Var.B3), enumValueChr, TypeID );\
  theList.AddHW( #Var"."#E4, offsetof(shareObj,Var.E4), offsetof(netObj,Var.E4), enumValueChr, TypeID, __enum_vals_##tE4 );\
  theList.AddHW( #Var"."#I5, offsetof(shareObj,Var.I5), offsetof(netObj,Var.I5), enumValueInt, TypeID );\
  theList.AddHW( #Var"."#I6, offsetof(shareObj,Var.I6), offsetof(netObj,Var.I6), enumValueInt, TypeID );\
  theList.AddHW( #Var"."#R7, offsetof(shareObj,Var.R7), offsetof(netObj,Var.R7), enumValueDbl, TypeID );

#undef Struct_REBEIIIR
#define Struct_REBEIIIR(Var,R1,tE2,E2,B3,tE4,E4,I5,I6,I7,R8) \
  theList.AddHW( #Var"."#R1, offsetof(shareObj,Var.R1), offsetof(netObj,Var.R1), enumValueDbl, TypeID );\
  theList.AddHW( #Var"."#E2, offsetof(shareObj,Var.E2), offsetof(netObj,Var.E2), enumValueChr, TypeID, __enum_vals_##tE2 );\
  theList.AddHW( #Var"."#B3, offsetof(shareObj,Var.B3), offsetof(netObj,Var.B3), enumValueChr, TypeID );\
  theList.AddHW( #Var"."#E4, offsetof(shareObj,Var.E4), offsetof(netObj,Var.E4), enumValueChr, TypeID, __enum_vals_##tE4 );\
  theList.AddHW( #Var"."#I5, offsetof(shareObj,Var.I5), offsetof(netObj,Var.I5), enumValueInt, TypeID );\
  theList.AddHW( #Var"."#I6, offsetof(shareObj,Var.I6), offsetof(netObj,Var.I6), enumValueInt, TypeID );\
  theList.AddHW( #Var"."#I7, offsetof(shareObj,Var.I7), offsetof(netObj,Var.I7), enumValueInt, TypeID );\
  theList.AddHW( #Var"."#R8, offsetof(shareObj,Var.R8), offsetof(netObj,Var.R8), enumValueDbl, TypeID );

#undef Struct_EBEIII
#define Struct_EBEIII(Var,tE1,E1,B2,tE3,E3,I4,I5,I6) \
  theList.AddHW( #Var"."#E1, offsetof(shareObj,Var.E1), offsetof(netObj,Var.E1), enumValueChr, TypeID, __enum_vals_##tE1 );\
  theList.AddHW( #Var"."#B2, offsetof(shareObj,Var.B2), offsetof(netObj,Var.B2), enumValueChr, TypeID );\
  theList.AddHW( #Var"."#E3, offsetof(shareObj,Var.E3), offsetof(netObj,Var.E3), enumValueChr, TypeID, __enum_vals_##tE3 );\
  theList.AddHW( #Var"."#I4, offsetof(shareObj,Var.I4), offsetof(netObj,Var.I4), enumValueInt, TypeID );\
  theList.AddHW( #Var"."#I5, offsetof(shareObj,Var.I5), offsetof(netObj,Var.I5), enumValueInt, TypeID );\
  theList.AddHW( #Var"."#I6, offsetof(shareObj,Var.I6), offsetof(netObj,Var.I6), enumValueInt, TypeID );

#undef Struct_IR
#define Struct_IR(Var,I1,R2,Def2) theList.AddHW( #Var"."#R2, offsetof(shareObj,Var.R2), offsetof(netObj,Var.R2), enumValueDbl, TypeID );\
  theList.AddHW( #Var"."#I1, offsetof(shareObj,Var.I1), offsetof(netObj,Var.I1), enumValueInt, TypeID );

#undef Struct_BEIR
#define Struct_BEIR(Var,B1,tE2,E2,I3,R4)\
  theList.AddHW( #Var"."#B1, offsetof(shareObj,Var.B1), offsetof(netObj,Var.B1), enumValueChr, TypeID );\
  theList.AddHW( #Var"."#E2, offsetof(shareObj,Var.E2), offsetof(netObj,Var.E2), enumValueChr, TypeID, __enum_vals_##tE2 );\
  theList.AddHW( #Var"."#I3, offsetof(shareObj,Var.I3), offsetof(netObj,Var.I3), enumValueInt, TypeID );\
  theList.AddHW( #Var"."#R4, offsetof(shareObj,Var.R4), offsetof(netObj,Var.R4), enumValueDbl, TypeID );

#undef Struct_BEIIa
#define Struct_BEIIa(Var,B1,tE2,E2,I3,I4arr,I4count) \
  theList.AddHW( #Var"."#B1, offsetof(shareObj,Var.B1), offsetof(netObj,Var.B1), enumValueChr, TypeID );\
  theList.AddHW( #Var"."#E2, offsetof(shareObj,Var.E2), offsetof(netObj,Var.E2), enumValueChr, TypeID, __enum_vals_##tE2 );\
  theList.AddHW( #Var"."#I3, offsetof(shareObj,Var.I3), offsetof(netObj,Var.I3), enumValueInt, TypeID );\
  theList.AddHW( #Var"."#I4arr, offsetof(shareObj,Var.I4arr), offsetof(netObj,Var.I4arr), enumValueInt, TypeID, _countof(((shareObj*)0)->Var.I4arr), sizeof(((shareObj*)0)->Var.I4arr) );

#undef StructA_BBEI
#define StructA_BBEI(Var,Count,B1,B2,tE3,E3,I4)\
  theList.AddHW( #Var"."#B1, offsetof(shareObj,Var.B1), offsetof(netObj,Var.B1), enumValueChr, TypeID, _countof(((shareObj*)0)->Var.B1), sizeof(((shareObj*)0)->Var.B1) );\
  theList.AddHW( #Var"."#B2, offsetof(shareObj,Var.B2), offsetof(netObj,Var.B2), enumValueChr, TypeID, _countof(((shareObj*)0)->Var.B2), sizeof(((shareObj*)0)->Var.B2) );\
  theList.AddHW( #Var"."#I4, offsetof(shareObj,Var.I4), offsetof(netObj,Var.I4), enumValueInt, TypeID, _countof(((shareObj*)0)->Var.I4), sizeof(((shareObj*)0)->Var.I4) );

#undef Struct_BBEI
#define Struct_BBEI(Var,B1,B2,tE3,E3,I4)\
  theList.AddHW( #Var"."#B1, offsetof(shareObj,Var.B1), offsetof(netObj,Var.B1), enumValueChr, TypeID );\
  theList.AddHW( #Var"."#B2, offsetof(shareObj,Var.B2), offsetof(netObj,Var.B2), enumValueChr, TypeID );\
  theList.AddHW( #Var"."#E3, offsetof(shareObj,Var.E3), offsetof(netObj,Var.E3), enumValueChr, TypeID, __enum_vals_##tE3 );\
  theList.AddHW( #Var"."#I4, offsetof(shareObj,Var.I4), offsetof(netObj,Var.I4), enumValueInt, TypeID );

#undef Struct_BxIx
#define Struct_BxIx(Var,B1,B1c,I2,I2c)\
  theList.AddHW( #Var"."#B1, offsetof(shareObj,Var.B1), offsetof(netObj,Var.B1), enumValueChr, TypeID, _countof(((shareObj*)0)->Var.B1), sizeof(((shareObj*)0)->Var.B1) );\
  theList.AddHW( #Var"."#I2, offsetof(shareObj,Var.I2), offsetof(netObj,Var.I2), enumValueInt, TypeID, _countof(((shareObj*)0)->Var.I2), sizeof(((shareObj*)0)->Var.I2) );

#undef Struct_BxRx
#define Struct_BxRx(Var,B1,B1c,D2,D2c)\
  theList.AddHW( #Var"."#B1, offsetof(shareObj,Var.B1), offsetof(netObj,Var.B1), enumValueChr, TypeID, _countof(((shareObj*)0)->Var.B1), sizeof(((shareObj*)0)->Var.B1) );\
  theList.AddHW( #Var"."#D2, offsetof(shareObj,Var.D2), offsetof(netObj,Var.D2), enumValueDbl, TypeID, _countof(((shareObj*)0)->Var.D2), sizeof(((shareObj*)0)->Var.D2) );

#undef Struct_R
#define Struct_R(Var,R2,Def2) theList.AddHW( #Var"."#R2, offsetof(shareObj,Var.R2), offsetof(netObj,Var.R2), enumValueDbl, TypeID );

#undef Struct_3R
#define Struct_3R(Var,Desc,Val1,Val2,Val3) \
  theList.AddHW( #Var"."#Val1, offsetof(shareObj,Var.Val1), offsetof(netObj,Var.Val1), enumValueDbl, TypeID );\
  theList.AddHW( #Var"."#Val2, offsetof(shareObj,Var.Val2), offsetof(netObj,Var.Val2), enumValueDbl, TypeID );\
  theList.AddHW( #Var"."#Val3, offsetof(shareObj,Var.Val3), offsetof(netObj,Var.Val3), enumValueDbl, TypeID );

//#undef Struct_3R_A
//#define Struct_3R_A(Var,Desc,Len,Val1,Val2,Val3)

#undef Struct_BERB
#define Struct_BERB(Var,B1,tE2,E2,R3,B4)\
  theList.AddHW( #Var"."#B1, offsetof(shareObj,Var.B1), offsetof(netObj,Var.B1), enumValueChr, TypeID );\
  theList.AddHW( #Var"."#E2, offsetof(shareObj,Var.E2), offsetof(netObj,Var.E2), enumValueChr, TypeID, __enum_vals_##tE2 );\
  theList.AddHW( #Var"."#R3, offsetof(shareObj,Var.R3), offsetof(netObj,Var.R3), enumValueDbl, TypeID );\
  theList.AddHW( #Var"."#B4, offsetof(shareObj,Var.B4), offsetof(netObj,Var.B4), enumValueChr, TypeID );

#undef Struct_ER
#define Struct_ER(Var,tE2,E2,R3)\
  theList.AddHW( #Var"."#E2, offsetof(shareObj,Var.E2), offsetof(netObj,Var.E2), enumValueChr, TypeID, __enum_vals_##tE2 );\
  theList.AddHW( #Var"."#R3, offsetof(shareObj,Var.R3), offsetof(netObj,Var.R3), enumValueDbl, TypeID );\

#undef Struct_RRE
#define Struct_RRE(Var,R1,R2,tE3,E3)\
  theList.AddHW( #Var"."#R1, offsetof(shareObj,Var.R1), offsetof(netObj,Var.R1), enumValueDbl, TypeID );\
  theList.AddHW( #Var"."#R2, offsetof(shareObj,Var.R2), offsetof(netObj,Var.R2), enumValueDbl, TypeID );\
  theList.AddHW( #Var"."#E3, offsetof(shareObj,Var.E3), offsetof(netObj,Var.E3), enumValueChr, TypeID, __enum_vals_##tE3 );

#undef Struct_BIRBSIR
#define Struct_BIRBSIR(Var,Desc,Val1,Val2,Val3,Val4,szVal5,lenV5,Val6,Val7)\
  theList.AddHW( #Var"."#Val1, offsetof(shareObj,Var.Val1), offsetof(netObj,Var.Val1), enumValueChr, TypeID );\
  theList.AddHW( #Var"."#Val2, offsetof(shareObj,Var.Val2), offsetof(netObj,Var.Val2), enumValueInt, TypeID );\
  theList.AddHW( #Var"."#Val3, offsetof(shareObj,Var.Val3), offsetof(netObj,Var.Val3), enumValueDbl, TypeID );\
  theList.AddHW( #Var"."#Val4, offsetof(shareObj,Var.Val4), offsetof(netObj,Var.Val4), enumValueChr, TypeID );\
  theList.AddHW( #Var"."#szVal5, offsetof(shareObj,Var.szVal5), offsetof(netObj,Var.szVal5), enumValueChr, TypeID, _countof(((shareObj*)0)->Var.szVal5), sizeof(((shareObj*)0)->Var.szVal5) );\
  theList.AddHW( #Var"."#Val6, offsetof(shareObj,Var.Val6), offsetof(netObj,Var.Val6), enumValueInt, TypeID );\
  theList.AddHW( #Var"."#Val7, offsetof(shareObj,Var.Val7), offsetof(netObj,Var.Val7), enumValueDbl, TypeID );

#undef Struct_EII
#define Struct_EII(Var,tE1,E1,I2,I3) \
  theList.AddHW( #Var"."#E1, offsetof(shareObj,Var.E1), offsetof(netObj,Var.E1), enumValueChr, TypeID, __enum_vals_##tE1 );\
  theList.AddHW( #Var"."#I2, offsetof(shareObj,Var.I2), offsetof(netObj,Var.I2), enumValueInt, TypeID );\
  theList.AddHW( #Var"."#I3, offsetof(shareObj,Var.I3), offsetof(netObj,Var.I3), enumValueInt, TypeID );

#undef Struct2BR
#define Struct2BR(Var,Desc,Val1,Val2,R3)\
  theList.AddHW( #Var"."#Val1, offsetof(shareObj,Var.Val1), offsetof(netObj,Var.Val1), enumValueChr, TypeID );\
  theList.AddHW( #Var"."#Val2, offsetof(shareObj,Var.Val2), offsetof(netObj,Var.Val2), enumValueChr, TypeID );\
  theList.AddHW( #Var"."#R3, offsetof(shareObj,Var.R3), offsetof(netObj,Var.R3), enumValueDbl, TypeID );

#undef StructBR
#define StructBR(Var,Desc,Val1,R2)\
  theList.AddHW( #Var"."#Val1, offsetof(shareObj,Var.Val1), offsetof(netObj,Var.Val1), enumValueChr, TypeID );\
  theList.AddHW( #Var"."#R2, offsetof(shareObj,Var.R2), offsetof(netObj,Var.R2), enumValueDbl, TypeID );
