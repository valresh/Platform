#ifdef FLEXRSULIST_EXPORTS
#define FLEXRSULIST_API _EXP
#else
#define FLEXRSULIST_API _IMP
#endif

extern FLEXRSULIST_API SListValue _theList;

#undef SharedAnalog32
#define SharedAnalog32(Var) _theList.AddRaw( #Var, evtAnalog32, _S::TypeID, offsetof( _S, Var), sizeof(tAnalog32) );

#undef SharedAnalog32Savebale
#define SharedAnalog32Savebale(Var) SharedAnalog32(Var)

/*
#undef SharedArrayAnalog32
#define SharedArrayAnalog32(Var,Dim)\
{\
  char szName[64];\
  for( int i=0; i<Dim; ++i )\
  { sprintf_s( szName, "%s[%d]", #Var, i );\
  _theList.AddRaw( szName, evtAnalog32, _S::TypeID, offsetof( _S, Var[i]), sizeof(tAnalog32) );}\
}
*/

#undef SharedDigital32
#define SharedDigital32(Var) _theList.AddRaw( #Var, evtDigital32, _S::TypeID, offsetof( _S, Var), sizeof(tDigital32) );

#undef SharedDigital32Savebale
#define SharedDigital32Savebale(Var) SharedDigital32(Var)

#undef SharedDigital64
#define SharedDigital64(Var) _theList.AddRaw( #Var, evtDigital64, _S::TypeID, offsetof( _S, Var), sizeof(tDigital64) );

#undef SharedChars
#define SharedChars(Var,Dim) _theList.AddRaw( #Var, evtChars, _S::TypeID, offsetof( _S, Var), sizeof(tChars)*Dim );

#undef SharedCharsSavebale
#define SharedCharsSavebale(Var,Dim) SharedChars(Var,Dim)

#undef SharedAnalog64
#define SharedAnalog64(Var) _theList.AddRaw( #Var, evtAnalog64, _S::TypeID, offsetof( _S, Var), sizeof(tAnalog64) );

#undef SharedAnalog64Savebale
#define SharedAnalog64Savebale(Var) SharedAnalog64(Var)
