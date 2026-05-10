/*
#undef  ACK_ALARM
#define ACK_ALARM \
  theList.AddHW( "UnackAlarmExists", offsetof(shareObj,notACK), offsetof(netObj,notACK),enumValueChr, TypeID );\
  theList.AddHW( "Квитирование"    , offsetof(shareObj,setACK), offsetof(netObj,setACK),enumValueChr, TypeID );\
  theList.AddHW( "EHLN"            , offsetof(shareObj,btEHLN), offsetof(netObj,btEHLN),enumValueChr, TypeID );\
  theList.AddHW( "MESS"            , offsetof(shareObj,btType), offsetof(netObj,btType),enumValueChr, TypeID );\
  theList.AddHW( "NoACK"           , offsetof(shareObj,notACK), offsetof(netObj,notACK),enumValueChr, TypeID );\
  theList.AddHW( "Shelved"         , offsetof(shareObj,Shelved),offsetof(netObj,Shelved),enumValueInt, TypeID );
*/
#undef DBLE
#define DBLE(Var,Def,Desc) theList.AddHW( #Var, offsetof(shareObj,Var), offsetof(netObj,Var), enumValueDbl, TypeID );

/*#undef REAL
#define REAL(Var,Def,Desc) theList.AddHW( #Var, offsetof(shareObj,Var), offsetof(netObj,Var), enumValueFlt, TypeID );*/

#undef SDEnum
#define SDEnum(Var,Def,Desc)  theList.AddHW( #Var, offsetof(shareObj,Var), offsetof(netObj,Var), enumValueChr, TypeID );

#undef Int32
#define Int32(Var,Def,Desc) theList.AddHW( #Var, offsetof(shareObj,Var), offsetof(netObj,Var), enumValueInt, TypeID );

#undef Boolean
#define Boolean(Var,Def,Desc) theList.AddHW( #Var, offsetof(shareObj,Var), offsetof(netObj,Var), enumValueBol, TypeID );

#undef  STRING
#define STRING(Var,Max,SecName) theList.AddHW( #Var, offsetof(shareObj,Var), offsetof(netObj,Var), enumValueStr, TypeID, Max );\
if(SecName[0])\
  theList.AddHW( SecName, offsetof(shareObj,Var), offsetof(netObj,Var), enumValueStr, TypeID, Max );
/*
#undef  ENUMERATION
#define ENUMERATION(Var,Desc,...) theList.AddHW( #Var, offsetof(shareObj,Var), offsetof(netObj,Var), enumValueChr, TypeID, #__VA_ARGS__ );
*/