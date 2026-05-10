#include "stdafx.h"
#include "UniHydro.h"
#include "Err.h"
#include "CommProc.h"
#include "SetData.h"
#include "Prop.h"

extern struct CCSVData * pModelDLL;//Проект;Модель;DLL;Описание

CUniHydro::CUniHydro(const char * _ObjName): IBaseModel(_ObjName)
#include "ACS_Constr.h"
#include "ACS.h"
{
    ObjName = _ObjName;
    Model = "UniHydro";
#include "IO_Clear.h"
#include "Points.h"
}

CUniHydro::~CUniHydro()
{
}

int CUniHydro::Init( int Regim )
{
    if ( Regim )
        return 0;
    //	bool Old = TranslateTxt;
    //	TranslateTxt = true;
    char Модель[256];
    lstrcpy ( Модель, "N" );//START_CONFIG );
    if ( Модель[0] == 0 )
    {
        if ( !PROPS.Get("Модель", Модель ))
            return __LINE__;
        lstrcpy ( START_CONFIG, Модель );
    }
    int Res = Load_Data( Модель );//(1)
    if(Res) return Res;
    //  if(FormStruct())
    //    return 1;
    Res = FormStruct();
    Prepare();
    pDataPtr = new CDataPtr;
    pDataPtr->kPipeRef = 0;
    pDataPtr->pPipeRef = NULL;
    pDataPtr->kObjPoint = 0;
    pDataPtr->pObjPoint = NULL;
    //
    pDataPtr->kObjRef = ObjRefs.L;
    pDataPtr->pObjRef = NewArr(CObjectRef*,ObjRefs.L);
    for ( int n = 0; n < ObjRefs.L; n++ )
    {
        CObjectRef & Obj = ObjRefs[n];
        Obj.Selected = false;
        pDataPtr->pObjRef[n] = &Obj;
#ifdef LINUX
        IBaseModel * pO = Obj.pObject;
        if ( pO )
            pO->Shema.Str = Obj.Shema.Str;
#endif
    }
    pDataPtr->kPipeRef = Pipes.L;
    pDataPtr->pPipeRef = NewArr(CPipeRef*,Pipes.L);
    for ( int n = 0; n < Pipes.L; n++ )
    {
        CPipeRef & Obj = Pipes[n];
        pDataPtr->pPipeRef[n] = &Obj;
    }
    ///////////
    //pDataPtr->kHGroup = Groups.L;
    //pDataPtr->pHGroup = new CGroupRef*[Groups.L];
    //for ( int n = 0; n < Groups.L; n++ )
    //  {
    //  CGroupRef & Grp = Groups[n];
    //  pDataPtr->pHGroup[n] = &Grp;
    //  }
    //TranslateTxt = Old;
    return 0;
}

int CUniHydro::Step0( )
{
    return 0;
}

int CUniHydro::Step1( )
{
    return 0;
}

int CUniHydro::StepT ( double dt )
{
    return 0;
}

int CUniHydro::SetData( int TypeData, void * pData )
{
    switch ( TypeData )
    {
    case sd_CalcObjCount:
    {
        *(int*)pData = kObjects;
        return 0;
    }
    break;
    case sd_ResetAll:
    {
        ASS(0)
    }
    break;
    case sd_SetAllObj:
    {
        HydroGroupsInterface * HydroGroups = static_cast<HydroGroupsInterface*>(this);
        *(HydroGroupsInterface **)pData = HydroGroups;
        return 0;
    }
    break;
    case sd_SetGroups:
    {
        *(CMem<CGroupRef,64,64>**)pData = &Groups;
        return 0;
    }
    break;
    case sd_GetDataPtr:
    {
        *(CDataPtr**)pData = pDataPtr;
        return 0;
    }
    break;
    case sd_SetModelDLL:
    {
        pModelDLL = (CCSVData*)pData;
        return 0;
    }
    break;
    case sd_SetProgress:
    {
        pProgress = (IProgress*)pData;
        return 0;
    }
    break;
    case sd_FindPath:
    {
        return 0;
    }
    break;
    case 10001:
    {
        ASS(0)
        return 0;
    }
    break;
    }
    return 1;
}

int CUniHydro::SaveState()
{
    S_CLASS("W",_W_UniHydro)
    return 0;
}

int CUniHydro::RestoreState(char * StrName)
{
    R_CLASS("W",_W_UniHydro)
    return 1;
}
//-----------------------------------------------------
void CUniHydro::SetCounter(DWORD index, unsigned long long Counter)
{
    //
}
unsigned long long  CUniHydro::GetCounter(DWORD index)
{
    return 0;
}

DWORD CUniHydro::GetCount()
{
    return Groups.L;
}

DWORD CUniHydro::GetScalarVariables(DWORD index)
{
    return 0;
}

DWORD CUniHydro::GetObjects(DWORD index)
{
    return 0;
}

DWORD CUniHydro::GetMaxEQ(DWORD index)
{
    CGroupRef & G = Groups[index];
    DWORD MaxEQ = G.MaxEQ;
    return MaxEQ;
}

DWORD CUniHydro::GetMaxVAR(DWORD index)
{
    CGroupRef & G = Groups[index];
    DWORD MaxVAR = G.MaxVAR;
    return MaxVAR;
}

void CUniHydro::SetA(DWORD index, double * A)
{
    CGroupRef & G = Groups[index];
    G.A = A;
}

void CUniHydro::SetB(DWORD index, double * B)
{
    CGroupRef & G = Groups[index];
    G.B = B;
}

void CUniHydro::ClearThread(DWORD index)
{
    CGroupRef & G = Groups[index];
    //??	G.nThread = 0;
}

void CUniHydro::SetThread(DWORD index, DWORD N)
{
    CGroupRef & G = Groups[index];
    //??	G.nThread = N;
}

#include <omp.h>
int CUniHydro::HydroCalc(DWORD index, double dt, bool VarInHydro)
{
    CGroupRef & G = Groups[index];
#if (OLD_CALC_MODE >> 0 & 1)

    int Res = G.HydroCalc(dt, VarInHydro);
#else
    int Res = 0;
    for(int i = 0; i < Groups.L; ++i)
    {
        CGroupRef & G = Groups[i];
        Res += G.HydroCalc(dt, VarInHydro);
    }
#endif
    return Res;
}
int CUniHydro::SetAllVars(DWORD index, double dt)
{
    CGroupRef & G = Groups[index];
    int Res = G.SetAllVars(dt);
    return Res;
}

void CUniHydro::GetObjectMatrix(DWORD index, double dt)
{
    CGroupRef & G = Groups[index];
    G.GetObjMatr(dt);
}

void CUniHydro::CreateCriticalSection(DWORD index)
{
    CGroupRef & G = Groups[index];
    G.UseCS = true;
    //??InitializeCriticalSection(&G.CS);
}
