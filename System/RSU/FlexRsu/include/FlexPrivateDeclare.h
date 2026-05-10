
#undef PrivateAnalog32
#define PrivateAnalog32(Var) tAnalog32 Var;

#undef PrivateAnalog32Savebale
#define PrivateAnalog32Savebale(Var) PrivateAnalog32(Var)

#undef PrivateDigital32
#define PrivateDigital32(Var) tDigital32 Var;

#undef PrivateDigital32Savebale
#define PrivateDigital32Savebale(Var) PrivateDigital32(Var)

#undef PrivateChars
#define PrivateChars(Var,Dim) tChars Var[Dim];

#undef PrivateConstString
#define PrivateConstString(Var) tPrivConstDynString Var;
