#pragma once
#include <memory>

#include "MemAlloc.h"
#include <macros/saverHelps.h>
#include <yMagicKeys.h>
#include <Y_Signal.h>


#undef IN
#undef OUT

#undef INIT
#define INIT(Parm) Parm = 0;

#undef GETOUT
#define GETOUT \
	{\
	if ( strcmp ( Name, "OUT" ) == 0 )\
		{\
		return &OUT;\
		}\
	}
#undef GETSUB
#define GETSUB \
	{\
	if ( strcmp ( Name, "SUB" ) == 0 )\
		{\
		return &SUB;\
		}\
	}

#undef GET_OUT
#define GET_OUT(Parm) \
	{\
	if ( strcmp ( Name, #Parm ) == 0 )\
		{\
		ASS( kOut < _countof(Out) );\
		Out[kOut].Name = #Parm;\
		Out[kOut] << W->Parm;\
		kOut++;\
		return &Out[kOut-1];\
		}\
	}

#undef SET_OUT
#define SET_OUT(VarName,Var) \
	{\
	ASS( kOut < _countof(Out) );\
	Out[kOut].Name = VarName;\
	Out[kOut].pFrom = Name;\
	Out[kOut] << Var;\
	kOut++;\
	}

#undef SET_NULL_OUT
#define SET_NULL_OUT(VarName) \
	{\
	ASS( kOut < _countof(Out) );\
	Out[kOut].Name = VarName;\
	Out[kOut].Null();\
	kOut++;\
	}

#undef GET_OUT_NAME
#define GET_OUT_NAME(Parm,OutName) \
	{\
	if ( strcmp ( Name, OutName ) == 0 )\
		{\
		ASS( kOut < _countof(Out) );\
		Out[kOut].Name = OutName;\
		Out[kOut] << W->Parm;\
		kOut++;\
		return &Out[kOut-1];\
		}\
	}
#undef GET_OUT_PARM
#define GET_OUT_PARM(Parm,OutName) \
	{\
	if ( strcmp ( Name, OutName ) == 0 )\
		{\
		ASS( kOut < _countof(Out) );\
		Out[kOut].Name = OutName;\
		Out[kOut] << Parm;\
		kOut++;\
		return &Out[kOut-1];\
		}\
	}

#undef GET_LOC_OUT_NAME
#define GET_LOC_OUT_NAME(Parm,OutName) \
	{\
	if ( strcmp ( Name, OutName ) == 0 )\
		{\
		ASS( kOut < _countof(Out) );\
		Out[kOut].Name = OutName;\
		Out[kOut] << Parm;\
		kOut++;\
		return &Out[kOut-1];\
		}\
	}

#undef GET_STD_OUT
#define GET_STD_OUT \
	{\
	for ( int n = 0; n < kOut; n++ )\
	  {\
		if ( strcmp ( Out[n].Name, Name ) == 0 ) return &Out[n];\
		}\
	}

#undef GET_IN
#define GET_IN(Parm) \
	{\
	if ( strcmp ( Name, #Parm ) == 0 ) return &Parm;\
	}

#undef GET_IN_NAME
#define GET_IN_NAME(IN,IN_Name) \
	{\
	if ( strcmp ( Name, IN_Name ) == 0 ) return &IN;\
	}

#undef GET_IN_NAME
#define GET_IN_NAME(Parm,ParmName) \
	{\
	if ( strcmp ( Name, ParmName ) == 0 ) return &Parm;\
	}

#undef PARM_IN
#define PARM_IN(Parm) \
	{\
	if ( Parm && Parm->pFrom )\
		{\
		char Name[128];\
		char Used = ' ';\
		if ( Parm->Used() ) Used = '>';\
		sprintf_s( Name, 128, "#+%c%s", Used, #Parm );\
		PARM(Parm->pFrom,Name)\
		}\
	}

#undef PARM_ALL_OUT
#define PARM_ALL_OUT \
	{\
	for ( int n = 0; n < kOut; n++ )\
		{\
		if ( Out[n].pTo[0] )\
			{\
			char Name[256];\
			char Used = ' ';\
			if ( Out[n].Used() ) Used = '<';\
			sprintf_s ( Name, sizeof(Name), "#-%c%s", Used, Out[n].Name );\
			PARM(Out[n].pTo[0], Name )\
			}\
		}\
	}

#undef PARM_OUT
#define PARM_OUT \
	{\
	if ( OUT.pTo[0] )\
		{\
		char Name[256];\
		char Used = ' ';\
		if ( OUT.Used() ) Used = '<';\
		sprintf_s ( Name, sizeof(Name), "#-%c%s", Used, OUT.Name );\
		PARM(OUT.pTo[0], Name )\
		}\
	}

#undef PARM_SUB
#define PARM_SUB \
	{\
	if ( SUB.pTo[0] )\
		{\
		char Name[256];\
		char Used = ' ';\
		if ( SUB.Used() ) Used = '<';\
		sprintf_s ( Name, sizeof(Name), "#-%c%s", Used, SUB.Name );\
		PARM(SUB.pTo[0], Name )\
		}\
	}

#undef PARM
#define PARM(Val,Name) params.Add( Val, Name );

#undef INFO
#define INFO(Val,Name) \
	{\
	ASS(kInfo<_countof(Info));\
	SYParam * pParm = MemAlloc<SYParam>(1);\
	Info[kInfo++] = pParm;\
  KYBridge2SysParam tmp( pParm, 1);\
	tmp.Add( Val, Name );\
	}

#undef  SHIFT_VALUE
#define SHIFT_VALUE( Params ) &(W->Params)
#undef  SHIFT_VAL
#define SHIFT_VAL( Params ) 0



#undef SAVE_OUT
#define SAVE_OUT \
	CY_Base::SaveState( );\
	for ( int n = 0; n < kOut; n++ )\
		{\
		if ( Out[n].pTo[0] )\
			Out[n].SaveState();\
		}

#undef SAVE_BASE_OUT
#define SAVE_BASE_OUT \
	CY_Base::SaveState( );\
	if ( OUT.pTo[0] )\
	  OUT.SaveState();\

//    pSRFile->InvalidLen = CSRFile::CopyFirst;

//template<size_t _S>
//int RestOutImpl( CSignal (&outs)[_S], int kOut, char *StrName )
//{
//  for ( int n = 0; n < kOut; n++ )
//  {
//    if ( outs[n].pTo[0] )
//    {
//      if ( outs[n].RestoreState( StrName ) == 0 )
//        return 0;
//    }
//  }
//  return 1;
//}
//#undef REST_OUT
//#define REST_OUT \
//	if ( CY_Base::RestoreState ( StrName ) == 0 )\
//		return 0;\
//  if( 0==RestOutImpl( Out, kOut, StrName) ) \
//    return 0;
//
//
//#undef REST_BASE_OUT
//#define REST_BASE_OUT \
//	if ( CY_Base::RestoreState ( StrName ) == 0 )\
//		return 0;\
//	if ( OUT.pTo[0] )\
//		{\
//		if ( OUT.RestoreState( StrName ) == 0 )\
//			return 0;\
//		}
//
//#undef DEF_DBL 
//#define DEF_DBL(Var,Def) \
//	{\
//	if ( SetYDefault )\
//	  W->Var = Def;\
//	}
//#undef DEF_INT
//#define DEF_INT(Var,Def) \
//	{\
//	if ( SetYDefault )\
//	  W->Var = Def;\
//	}
//
//#ifndef CLEAR
//#define CLEAR(Var) memset(Var,0,sizeof(Var));
//#endif
