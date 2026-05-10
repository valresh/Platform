#include "stdafx.h"
#include "AVO.h"
#include "SetData.h"
#include "MM.h"
//#include "Err.h"
//#include "CommProc.h"
//#include "Data.h"

LPCSTR g_szHelpFileName = "Базовый АВО.pdf";

CAVO::CAVO( char * _ObjName ) :	IBaseModel ( _ObjName ) 
#include "ACS_Constr.h"
#include "ACS.h"
  , Heat ( "АВО", this )
  , Heat1flow ( "АВО новое", this )
  , HeatAir("Подогреватель",this)
  , Cond("Конденсация",this)
{
  UNIMODEL
    ObjName = _ObjName;
  Model = "AVO";
#include "IO_Clear.h"
#include "Points.h"
#include "AVO_Defects.h"
  pIn_Water = NULL;
  pOut_Water = NULL;
  pIn_Water_2 = NULL;
  pIn_Steam = NULL;
  pOut_Steam = NULL;
  Heat_Defect = false;
  Trub_Defect = false;
  Angle_Defect = false;
  k_Heat = 1.;
  Def_Omega_Trub = 0.;
  Def_Angle = 1.;
  FlowQ = 0.;
  CLEAR(pSrc)
  Static = false;
  Ctrl_Jal_1 = 0;
  Ctrl_Jal_2 = 0;
  Ctrl_Jal_3 = 0;
  Ctrl_Jal_4 = 0;
  pJal[0] = &Jalousie_1;
  pJal[1] = &Jalousie_2;
  pJal[2] = &Jalousie_3;
  pJal[3] = &Jalousie_4;
  kMotor = 0;
  memset(pMotors, 0, sizeof(pMotors[0]) * MAX_MOTOR);
}

CAVO::~CAVO()
{
}

int CAVO::Init( int )
{
  int Res = 0;
  Reg_T_out.PID = true;
  //  SetParamsList( sizeof ( *this ), &pParams, &kParams );
  return 0;
}

int CAVO::Step0( )
{
  //	Omega = 1e4;
  //  Fsend ( "Опасные объекты.csv", "%s;Да;\n", ObjName );
  return 0;
}

int CAVO::Step1( )
{
  Jalousie_1 = Jalousie_2 = Jalousie_Eff;
  if ( pIn_Steam )
  {
    In_Air.P = 1.;
    Out_Air.P = 1.;
    Heat_Air.P = 1.;
    HeatAir.pIn_Trub = pIn_Steam;
    HeatAir.pOut_Trub = pOut_Steam;
    HeatAir.pIn_Vol = &In_Air;
    HeatAir.pOut_Vol = &Heat_Air;
    Heat.pIn_Vol = &Heat_Air;
    Heat.pOut_Vol = &Out_Air;
    Heat1flow.pIn_Const = &Heat_Air;
    Heat1flow.pOut_Const = &Out_Air;
  }
  else
  {
    In_Air.P = 1.;
    Out_Air.P = 1.;
    Heat.pIn_Vol = &In_Air;
    Heat.pOut_Vol = &Out_Air;
    Heat1flow.pIn_Const = &In_Air;
    Heat1flow.pOut_Const = &Out_Air;
  }
#ifdef EQUIPMENT
  static bool First = true;
  if ( First )
  {
    First = false;
  }
#endif
  return 0;
}

int CAVO::StepT ( double dt )
{
  Static = LocalStatic;
  Calc ( dt );
  return 0;
}

#undef IF
#define IF(Parm) if ( strcmp ( pP->Name, Parm ) == 0 ) {
#undef EIF
#define EIF return 0; }
int CAVO::SetData( int TypeData, void * pData )
{
  switch ( TypeData )
  {
    case sd_ShowObjInfo:
      {
        Char<256>* pInfo = (Char<256>*)pData;
        pInfo->Prt( "%s: F=%4.2lfкг, O=%4.1lf, Ti=%4.2lf, To=%4.2lf", (char*)ObjName, _F_kg, _Oborot, _T_in, _T_out );
        return 0;
      }
    case sd_GetOverGasSource:
      {
        *(double**)pData = &FlowQ;
        return sd_GetOverGasSource;
      }
    case sd_SetEquipParam:
      {
        CEquipParam * pP = (CEquipParam*)pData;
        IF("Площадь")
        if ( pP->Value[0] == 0 )
          return 0;
        S = atof( pP->Value );
        if ( S < 1. || S > 100000.  )
        {
          pP->Msg = "Неправильное значение площади, м2";
          return -1;
        }
        EIF
          IF("Диалог")
          if ( pP->Value[0] == 0 )
          return 0;
        int Диалог = atoi( pP->Value );
        if ( Диалог < 0 || Диалог >= 256 )
        {
          pP->Msg = "Неправильный Диалог(0,...,255)";
          return -1;
        }
        EIF
          return 1;
      }
  }
  return 1;
}
int CAVO::SaveState( )
{
  S_CLASS("W",_W)
  return 0;
}

int CAVO::RestoreState( char * StrName )
{
  COPY_FIRST
    R_CLASS("W",_W)
    return 1;
}

int CAVO::DstMM( char * DataName, struct IMM * pMM )
{
  if (!strcmp( DataName, "Motor"))
  {
    ASS(pMM->ID == CMotorData::TypeID)
    CMotorData* pMotor = (CMotorData*)pMM->pData;
    int iMotor = 0;
    while(iMotor < MAX_MOTOR)
    {
      if(pMotors[iMotor] == pMotor)
        return 0;
      if(pMotors[iMotor] == NULL)
      {
        lstrcpy(pSrc[kMotor], pMM->ObjName);
        pMotors[kMotor] = pMotor;
        kMotor++;
        break;
      }
      iMotor++;
    }
    return 0;
  }
  return 1;
}
