
#undef PrivateAnalog32
#define PrivateAnalog32(Var,Lgx) tAnalog32 Var;

#undef PrivateAnalog32Savebale
#define PrivateAnalog32Savebale(Var,Lgx) PrivateAnalog32(Var,Lgx)

#undef PrivateDigital32
#define PrivateDigital32(Var,Lgx) tDigital32 Var;

#undef PrivateDigital32Savebale
#define PrivateDigital32Savebale(Var,Lgx) PrivateDigital32(Var,Lgx)

#undef PrivateBool
#define PrivateBool(Var,Lgx) tBool Var;

#undef PrivateBoolSavebale
#define PrivateBoolSavebale(Var,Lgx) PrivateBool(Var,Lgx)

#undef PrivatePoint2String
#define PrivatePoint2String(Var,Lgx) KDynString Var;

#undef PrivateEnum
#define PrivateEnum(Var,Lgx,...)\
struct _##Var\
    {\
    tEnum V;\
    enum { __VA_ARGS__ };\
    void operator = ( int Z ) { V = Z; };\
    void operator = ( tEnum Z ) { V = Z; };\
    bool operator == ( int Z ) { return V == Z; };\
    void operator = ( _##Var Z ) { V = Z.V; };\
    operator int () { return V; };\
    operator tEnum () { return V; };\
    static LPCSTR GetVals() { return #__VA_ARGS__;}\
    };\
    _##Var Var;