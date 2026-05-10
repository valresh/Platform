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
#undef SharedAnalog32Savebale
#define SharedAnalog32Savebale(Var,otherName )\
  theList.AddHW( #Var, offsetof(shareObj,Var), offsetof(netObj,Var), enumValueFlt, TypeID );\
  if( otherName ) theList.AddHW( otherName, offsetof(shareObj,Var), offsetof(netObj,Var), enumValueFlt, TypeID );

#undef SharedDigital32Savebale
#define SharedDigital32Savebale(Var,otherName )\
  theList.AddHW( #Var, offsetof(shareObj,Var), offsetof(netObj,Var), enumValueInt, TypeID );\
  if( otherName ) theList.AddHW( otherName, offsetof(shareObj,Var), offsetof(netObj,Var), enumValueInt, TypeID );

#undef SharedBoolSavebale
#define SharedBoolSavebale(Var,otherName )\
  theList.AddHW( #Var, offsetof(shareObj,Var), offsetof(netObj,Var), enumValueBol, TypeID );\
  if( otherName ) theList.AddHW( otherName, offsetof(shareObj,Var), offsetof(netObj,Var), enumValueBol, TypeID );
