#include "stdafx.h"
#include "Volumetric.h"
#include "Err.h"
#include "CommProc.h"
#include "SetMatr.h"
#include "Work.h"
#include "math.h"


#undef IF
#define IF(Node) if ( lstrcmp ( Node, Name ) == 0 )
extern "C"

    int CVolumetric::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
{
    return CPump_b::TestNodes(kNodes, ppNodes);
}

#include "SetMatr.h"

double kk = 1.0;

double CVolumetric::Calc_Omega()
{
    const double Descent = 1000.0;
    const double Omega_Max = 1000.0;
    return (atan(Descent*(Head - P_max)) + _PI / 2.0) / _PI * Omega_Max;
    double _F_mol = 0.0;
    //if(Р_нагнетания >= H_nom /0.9)
    //_F_mol = 0.1;
    //else
    //_F_mol = ((-atan(Descent * (Р_нагнетания.Value - P_max)) + _PI / 2.0) * F_nom / H2O_To_m3) / _PI * Speed;
    if(fabs(Head) > P_max)
        return -F_real;
    else
        return F_real;
    // 	if(_F_mol < 1.0)
    // 		_F_mol = 0.0;
    return _F_mol;
}

int CVolumetric::SetMatr( void * pExternals, int PntGroup,  double dt, 
                         double A[/* kExkV */], double B[ /*kE*/ ] )
{
    SET_BP BreakPoint;
    double * Eq = A;

    if ( F_nom > 0. )
        F_real = F_nom * K_F;
    else
        F_real = F_mass * K_F;

    switch ( PntGroup )
    {
    case 1:
    {
        if ( Head < P_max )
        {
            double Omega_Подтекания = 1.;  // по аналогии с новым способом расчета
            Eq[P_0] = -Omega_Подтекания;
            Eq[P_1] = Omega_Подтекания;
            Eq[F_0] = 1.0;
            if ( F_nom > 0. )
            {
                if(To_m3 > 0.0)
                    B[0] = F_real / To_m3 * (*pMotor->pRelOborot) + Omega_Подтекания * Head;
                else
                    B[0] = Omega_Подтекания * Head;
            }
            else
                B[0] = F_real / To_kg * (*(pMotor->pRelOborot)) + Omega_Подтекания * Head;
            // 2 ур-e
            Eq += 4;
            Eq[F_0] = 1.0;
            Eq[F_1] = -1.0;
            FINITE(B[0])
        }
        else
        {
            double Omega_Сброса = 10000.;  // Лучше параметр
            Eq[F_0] = 1.0;
            Eq[P_0] = -Omega_Сброса;
            Eq[P_1] = Omega_Сброса;
            if ( F_nom > 0. )
                B[0] = F_real / To_m3 * (*pMotor->pRelOborot) + Omega_Сброса * P_max;
            else
                B[0] = F_real / To_kg * (*pMotor->pRelOborot) + Omega_Сброса * P_max;
            // 2 ур-e
            Eq += 4;
            Eq[F_0] = 1.0;
            Eq[F_1] = -1.0;
        }
    }
    break;
    case 2:
    {
        MATR_OMEGA(pCoolObj->Omega)
    }
    break;
    default:
        ASS(0)
        break;
    }
    return 0;
}

#include "SetVar.h"

int CVolumetric::SetVar( void * pExternals, int PntGroup, double dt, 
                        double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
{
    SET_BP BreakPoint;
    return CPump_b::SetVar(pExternals, PntGroup, dt, Var, pVar);
}

