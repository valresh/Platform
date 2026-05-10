#include "stdafx.h"
#include "UniModel.h"
#include "Err.h"
//#include "CommProc.h"
//#include "Data.h"
#include "SetData.h"

CCommonModel * CreateReflux( char * _ObjName, IBaseModel * _pMain  );
CCommonModel * CreateRecycle( char * _ObjName, IBaseModel * _pMain  );
CCommonModel * CreateDrossel( char * _ObjName, IBaseModel * _pMain  );
CCommonModel * CreateClear_H2O( char * _ObjName, IBaseModel * _pMain  );
CCommonModel * CreateElectroHeat( char * _ObjName, IBaseModel * _pMain  );
CCommonModel * CreateComposition( char * _ObjName, IBaseModel * _pMain  );


CUniModel::CUniModel( char * _ObjName, LPCTSTR Type ) :	IBaseModel ( _ObjName ) 
#include "ACS_Constr.h"
#include "ACS.h"
//, In("Вход",this)
//, Out("Выход",this)
	{
  UNIMODEL
	lstrcpy ( ObjName, _ObjName );
	#include "IO_Clear.h"
	#include "Points.h"
  pCurrentModel = NULL;
//
  //pCompOut = NULL;
  //ID_CompOut = 0;
  //Mass = false;
  //MustSetComp = true;
  //Восстановление = NULL;
  //Засорение = NULL;
  //Засор = NULL;
  //Defect = 0;
  //Omega_def = 1.;
  //for ( int n = 0; n < K_GAS; n++ )
  //  k_Comp[n] = 1.;
  //FlowQ = 0.;
  if ( Type[0] == 0 || lstrcmp ( Type, "Дроссель" ) == 0 )
    {
    char szObjName[1024];
    sprintf(szObjName, "Дроссель(%s)", ObjName);
    pCurrentModel = CreateDrossel ( szObjName, this );
    return;
    }
  if ( lstrcmp ( Type, "Рецикл" ) == 0 )
    {
    pCurrentModel = CreateRecycle( "Рецикл", this );
    return;
    }
  if ( lstrcmp ( Type, "Состав" ) == 0 )
    {
    pCurrentModel = CreateComposition( ObjName, this );
    return;
    }
  if ( lstrcmp ( Type, "Орошение" ) == 0 )
    {
    pCurrentModel = CreateReflux( "Орошение", this );
    return;
    }
  if ( lstrcmp ( Type, "Осушка" ) == 0 )
    {
    pCurrentModel = CreateClear_H2O( "Осушка", this );
    return;
    }
  if ( lstrcmp ( Type, "Электроподогрев" ) == 0 )
    {
    pCurrentModel = CreateElectroHeat( "Электроподогрев", this );
    return;
    }
	}

CUniModel::~CUniModel()
	{
	}

int CUniModel::Init( int Regim )
	{
	int Res = 0;
//	#include "IO_Init.h"
//	#include "Points.h"
	//#include "ACS_Init.h"  
	//#include "ACS.h"
  //pCompOut = NULL;
  //NewComp ( &ID_CompOut, &pCompOut, "Выход из %s", ObjName );
  //С_O2 = 0.209476;
  //C_N2 = 1. - С_O2;
	return 0;
	}

DWORD ClassRootNode_Filtr;
DWORD ClassRootNode_Dross;
DWORD LastDefect; 
CDef * ObjRoot;

#undef MODEL_F
#define MODEL_F(ClassName,Default) \
  {\
  if ( ClassRootNode_Filtr == 0 )\
  {\
  ClassRootNode_Filtr = GiveClassRoot( ClassName );\
  }\
  RootNode = NewDefect( );\
  ObjRoot = GetDefect( RootNode );\
  ObjRoot->Parent = ClassRootNode_Filtr;\
  lstrcpy ( ObjRoot->Name, ObjName );\
  ObjRoot->Flags = CDef::Defects|CDef::Node|CDef::Active;\
  LastDefect = 0;\
  }

#undef MODEL_D
#define MODEL_D(ClassName,Default) \
  {\
  if ( ClassRootNode_Dross == 0 )\
  {\
  ClassRootNode_Dross = GiveClassRoot( ClassName );\
  }\
  RootNode = NewDefect( );\
  ObjRoot = GetDefect( RootNode );\
  ObjRoot->Parent = ClassRootNode_Dross;\
  lstrcpy ( ObjRoot->Name, ObjName );\
  ObjRoot->Flags = CDef::Defects|CDef::Node|CDef::Active;\
  LastDefect = 0;\
  }

#undef DEFECT
#define DEFECT(DefectVar,Comment) \
  {\
  LastDefect = NewDefect( );\
  DefectVar = GetDefect( LastDefect );\
  DefectVar->Parent = RootNode;\
  lstrcpy ( DefectVar->Name, Comment );\
  lstrcpy ( DefectVar->ObjName, ObjName );\
  DefectVar->Flags = CDef::Defects|CDef::Defect|CDef::Active;\
  DefectVar->ScriptID = 0;\
  DefectVar->pModel = this;\
  }

#undef DEF_D
#define DEF_D(Var, vName, vDef, vMin, vMax ) \
  {\
  Var = NewDefectPram( LastDefect );\
  Var->Type = CDefParam::tDbl;\
  lstrcpy(Var->Name, vName );\
  Var->Min = (float)vMin;\
  Var->Max = (float)vMax;\
  Var->Dbl = Var->_Dbl = (double)(vDef);\
  }


int CUniModel::Step0( )
	{
//  Omega = 1000.;
  if ( IsDefects )
    {
    //if ( Тип == 0 )
    //  {
    //  MODEL_D("Дроссели",true)
    //  DEFECT(Восстановление,"Восстановление")
    //  DEFECT(Засорение,"Засорение дросселя")
    //  DEF_D (Засор, "Степень засорения, %", 50, 0, 100 )
    //  }
    //if ( Тип == 4 )
    //  {
    //  MODEL_F("Фильтры",true)
    //  DEFECT(Восстановление,"Восстановление")
    //  DEFECT(Засорение,"Засорение фильтра")
    //  DEF_D (Засор, "Степень засорения, %", 50, 0, 100 )
    //  }
    /*
    if ( DefModel[0] && DefDefect[0] )
      {
      MODEL_F(DefModel,true)
      DEFECT(Восстановление,"Восстановление")
      DEFECT(Засорение,DefDefect)
	    if ( DefValue[0] )
        {
        DEF_D (Засор, DefValue, 50, 0, 100 )
        }
	    if( Тип == 14 )
	      {
		    DEFECT(Опустошение,"Опустошение")
		    DEFECT(Переполнение,"Переполнение")
	      }
      }
	  DEFECT(Загазованность,"Загазованность")
      DEF_D (Уровень_загазованности, "Загазованность, % ПДК", 70, 0, 200 )
      */
    }
	return 0;
	}

int CUniModel::Step1( )
	{
  //if ( Тип == 4 )
  //  KKK();
  //switch ( Тип )
  //  {
  //  case 0: //Дроссель
  //  case 3: //Уплотнения
  //  case 4: //Фильтр
  //    Resist( 1e-4 );
  //  break;
  //  case 8: //МЭА
  //    MEA.Read( "МЭА" );
  //  break;
  //  case 9: //Состав
  //    for ( int c = 0; c < LAST_GAS; c++ )
  //      if ( MEA.Cmol[c] >= 0 )
  //        MEA.IsComp[c] = true;
  //      else
  //        MEA.IsComp[c] = false;
  //  break;
  //  }
	return 0;
	}

int CUniModel::StepT ( double dt )
	{
  Calc ( dt );
	return 0;
	}
extern char * Types[];

int CUniModel::SetData( int TypeData, void * pData )
	{
  //if ( TypeData == sd_ShowObjInfo )
  //  {
  //  Char<256>* pInfo = (Char<256>*)pData;
  //  pInfo->Prt( "%s: %s dP=%4.2lf, F=%4.2lfкг", ObjName, Types[Тип], dP, F_kg );
  //  return 0;
  //  }
  //if ( TypeData == sd_GetFlowZ )
  //  {
  //  Flow = *(double*)pData;
  //  Тип = 11;
  //  return sd_GetFlowZ;
  //  }
  //if ( TypeData == sd_GetPipeFlow )
  //  {
  //  *(CFlow**)pData = pFlow_in;
  //  return sd_GetPipeFlow;
  //  }
  //if ( TypeData == sd_GetOverGasSource )
  //  {
  //  *(double**)pData = &FlowQ; 
  //  return sd_GetOverGasSource;
  //  }
	return 1;
	}

int CUniModel::SaveState( )
  {
	S_CLASS("W",_W)
  return 0;
  }

int CUniModel::RestoreState( char * StrName )
  {
  COPY_FIRST
	R_CLASS("W",_W)
  return 1;
  }

int CUniModel::SrcMM( char * DataName, struct IMM * pMM )
  {
  int Res = IBaseModel::SrcMM( DataName, pMM );
  if ( Res == 0 )
    return 0;
  if ( Lstrcmp( DataName, "Нагрев" ) == 0 )
    {
    pMM->ID = 0x2A3BE4FF;
    pMM->pData = &Qh;
    pMM->ObjName = ObjName;
    return 0;
    }
  return 2;
  }
