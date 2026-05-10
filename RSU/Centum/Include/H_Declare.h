
#undef  INTEGER
#define INTEGER(Var,Desc) int Var;
#undef  INTEGER_A
#define INTEGER_A(Var,Dim,Desc) int Var[Dim]; char Var##Name[Dim][8];

#undef  DOUBLE
#define DOUBLE(Var,Desc) double Var;
#undef  DOUBLE2
#define DOUBLE2(Var_1,Var_2,Desc) double Var_1, Var_2;
#undef  DOUBLE_A
#define DOUBLE_A(Var,Dim,Desc) double Var[Dim]; char Var##Name[Dim][8];
#undef  MASSIW_D
#define MASSIW_D(Var,Dim,Desc) double Var[Dim]; char Var##Name[Dim][8];
#undef  DOUBLE_AA
#define DOUBLE_AA(Var,Dim,Max,Desc) double Var[Dim][Max];

#undef  STRING
#define STRING(Var,Max,Desc) char Var[Max+1];
#undef  STRING_A
#define STRING_A(Var,Dim,Max,Desc) char Var[Dim][Max+1]; char Var##Name[Dim][8];
#undef  STRING_skipSave
#define STRING_skipSave(Var,Max,Desc) char Var[Max+1];

#undef  ENUM
#define ENUM(Var,Desc,...) \
struct _##Var\
		{\
		BYTE V;\
		enum { __VA_ARGS__ };\
		void operator = ( int Z ) { V = Z; };\
		void operator = ( BYTE Z ) { V = Z; };\
		bool operator == ( int Z ) { return V == Z; };\
		void operator = ( _##Var Z ) { V = Z.V; };\
		operator int () { return V; };\
		operator BYTE () { return V; };\
		};\
	_##Var Var;

#undef  ENUM_A
#define ENUM_A(Var,Dim,Desc,...) \
struct _##Var\
		{\
		BYTE V;\
		enum { __VA_ARGS__ };\
		void operator = ( int Z ) { V = Z; };\
		void operator = ( BYTE Z ) { V = Z; };\
		bool operator == ( int Z ) { return V == Z; };\
		void operator = ( _##Var Z ) { V = Z.V; };\
		operator int () { return V; };\
		operator BYTE () { return V; };\
		};\
	_##Var Var[Dim];

#undef  ENUM_S
#define ENUM_S(Struct,Var,Desc,...) \
struct _##Var\
		{\
		BYTE V;\
		enum { __VA_ARGS__ };\
		void operator = ( int Z ) { V = Z; };\
		void operator = ( BYTE Z ) { V = Z; };\
		bool operator == ( int Z ) { return V == Z; };\
		void operator = ( _##Var Z ) { V = Z.V; };\
		operator int () { return V; };\
		operator BYTE () { return V; };\
		};\
	_##Var Var;

#undef  ENUM_SA
#define ENUM_SA(Struct,Var,Dim,Desc,...) \
struct _##Var\
		{\
		BYTE V;\
		enum { __VA_ARGS__ };\
		void operator = ( int Z ) { V = Z; };\
		void operator = ( BYTE Z ) { V = Z; };\
		bool operator == ( int Z ) { return V == Z; };\
		void operator = ( _##Var Z ) { V = Z.V; };\
		operator int () { return V; };\
		operator BYTE () { return V; };\
		};\
	_##Var Var[Dim];

#undef  FLAG
#define FLAG(Var,Desc) BYTE Var;
#undef  FLAG_A
#define FLAG_A(Var,Dim,Desc) BYTE Var[Dim];
#undef  FLAG_S
#define FLAG_S(Struct,Var,Desc) BYTE Var;
#undef  FLAG_SA
#define FLAG_SA(Struct,Var,Dim,Desc) BYTE Var[Dim];

#undef  DATA
#define DATA(Var,Dim,Desc) BYTE Var[Dim];

#undef  STRUCT
#define STRUCT(Name) struct C##Name { 

#undef  STRUCT_END
#define STRUCT_END(Name) }; C##Name Name; 

#undef  ACK_ALARMS
#define ACK_ALARMS

#undef  ADD_OPMK
#define ADD_OPMK
//#undef IN
//#undef OUT
//#undef HANDLER

#define H_DECLARE
