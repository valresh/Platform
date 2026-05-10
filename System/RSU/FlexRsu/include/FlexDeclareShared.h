
#undef SharedAnalog32
#define SharedAnalog32(Var) tAnalog32 Var;

#undef SharedArrayAnalog32
#define SharedArrayAnalog32(Var,Dim) tAnalog32 Var[Dim];

#undef SharedAnalog32Savebale
#define SharedAnalog32Savebale(Var) SharedAnalog32(Var)

#undef SharedDigital32
#define SharedDigital32(Var) tDigital32 Var;

#undef SharedDigital64
#define SharedDigital64(Var) tDigital64 Var;

#undef SharedDigital32Savebale
#define SharedDigital32Savebale(Var) SharedDigital32(Var)

#undef SharedChars
#define SharedChars(Var,Dim) tChars Var[Dim];

#undef SharedCharsSavebale
#define SharedCharsSavebale(Var,Dim) tChars Var[Dim];

#undef SharedAnalog64
#define SharedAnalog64(Var) tAnalog64 Var;

#undef SharedAnalog64Savebale
#define SharedAnalog64Savebale(Var) SharedAnalog64(Var)
