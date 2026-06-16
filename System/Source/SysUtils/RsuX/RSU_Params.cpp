#include "DataTypes.h"
#include "CommProc.h"
#include "Err.h"
#include "Param.h"


#define _EXP
#define _IMP

typedef uint64_t UINT64;

#include "/home/resh/QtRSU/RSUs/HoneyWell5xx/Include/HoneywellStruct.h"
#include "WP_Declare.h"

extern RSU_Obj * RSU_Pnt;
extern int kRSU;
extern BYTE * pszObjects;

#undef String_K
#define String_K(Var,Comm,Len) \
{\
QParams &P = pList->push_back();\
P.Set ( NULL, Comm, pW->Var,Len);\
}

#undef Real
#define Real(Var,Desc,Def) \
{\
    QParams &P = pList->push_back();\
    P.Set ( NULL, Desc, pW->Var,8);\
}

#undef Boolean
#define Boolean(Var,Desc,Def) \
{\
   QParams &P = pList->push_back();\
   P.Set ( NULL, Desc, pW->Var,1);\
}
#undef  Enumeration
#define Enumeration(Var,Desc,...) \
{\
   QParams &P = pList->push_back();\
   P.Set ( NULL, Desc, pW->Var.V,1);\
}


void AICHANNEL( void * pBase, CMem<QParams, 16, 16> * pParamsP, CMem<QParams, 16, 16> * pParamsW )
{
    BYTE * pB = (BYTE*)pBase;
    W_AICHANNEL * pW = (W_AICHANNEL*)(pB-8);
    int s = sizeof ( W_AICHANNEL );
    int d = (BYTE*)&pW->PV - (BYTE*)pW;
//////////////////////////////
    CMem<CParams, 16, 16> * pList = pParamsP;
    String_K(DESC,"Block Descriptor",132*4)
    String_K(EUDESC,"Block Engineering Units Descriptor",16*4)
    Real(LRL,"Lower Range Limit",NaN)
    Real(LRV,"PV Low Range",NaN)
    Real(PVRAWHI,"PV Raw High Range",100.)
    Real(PVRAWLO,"PV Raw Low Range",0.)
    Real(URL,"Upper Range Limit",NaN)
    Real(URV,"Upper Range Value",NaN)
/////////////////////////////////
    pList = pParamsW;
    Boolean(BADPVFL,"BAD PV",false)
    Real(PV,"Process Variable",NaN)
    Real(PVRAW,"Process Variable Raw Value",NaN)
}

void AOCHANNEL( void * pBase, CMem<QParams, 16, 16> * pParamsP, CMem<QParams, 16, 16> * pParamsW )
{
    BYTE * pB = (BYTE*)pBase;
    W_AOCHANNEL * pW = (W_AOCHANNEL*)(pB-8);
    //////////////////////////////
    CMem<CParams, 16, 16> * pList = pParamsP;
    String_K(DESC,"Block Descriptor",132*4)
    String_K(EUDESC,"Block Engineering Units Descriptor",16*4)
    Enumeration(PNTTYPE,"AI=1,AO,DI,DO",AI=1,AO,DI,DO,Def=AO)
    /////////////////////////////////
    pList = pParamsW;
    Real(OP,"Output",-6.9)//Defines the output value from this point in percent
}

void PID( void * pBase, CMem<QParams, 16, 16> * pParamsP, CMem<QParams, 16, 16> * pParamsW )
{
    BYTE * pB = (BYTE*)pBase;
		W_PID * pW = (W_PID*)(pB-8);
    CMem<CParams, 16, 16> * pList = pParamsP;
    String_K(DESC,"Block Descriptor",132*4)
    String_K(EUDESC,"Block Engineering Units Descriptor",24*4)
    Real(K,"Total Gain for Calculated Variable",1.0)
    Real(KLIN,"Linear Gain factor",1.)
    Real(T1,"Integral Time Constant (in Minutes)",0)
    Real(T2,"Derivative Time Constant (in Minutes)",0)
    Boolean_A(MODEAPPL,"",4,false)//MODEAPPL[1] is On, if Manual (Man) mode is valid
    Real(CVEUHI,"",100.)
    Real(CVEULO,"",0.)
    Real(PVEUHI,"PVEUHI",100.)
    Real(PVEULO,"PVEULO",0.)
    Real(SPEUHI,"SP High Range (in Engineering Units)",100)//This value corresponds to 100% of full-scale value
    Real(SPEULO,"SP Low Range (in Engineering Units)",0)//This value corresponds to 0% of full-scale value
    Real(OPEXHILM,"OP Extended High Limit (in percentage)",106.9)
    Real(OPEXLOLM,"OP Extended Low Limit (in percentage)",-6.9)
    Real(OPHILM,"OPHILM",105.)
    ///////////////////////////////////////////////////////////////
     pList = pParamsW;
		Real(PV,"PV",NaN)
		Real(PVP,"PV %",NaN)
		Real(SP,"SP",0)
		Real(SPP,"SP %",NaN)
		Real(SPTV,"SP Target Value (in Engineering Units)",0)//Contains the target SP value when SP ramping is being used
		Real(DEV,"PV-SP",NaN)//- The difference between the Process Variable (PV) and Set Point (SP).
		Real(OP,"OP %",0)
		Real(OPEU,"OP in Engineering Units",0)
		Enumeration(MODE,"(MODE=Man(0),Auto(1),Cas(2),...",Man,Auto,Cas,BCas,Normal,None,Def=Man)//
		Real(CV,"CV",NaN)//The result (calculated value) of the function blocks. The value may be in Percent or Engineering Units, depending on the function block
		Real(DELCV,"Delta CV",NaN)//in Percent.
		Boolean(SPHIFL,"SP > High",false)//Indicates if SP has exceeded its upper limit, as specified by the Set Point High Limit (SPHILM).
		Boolean(SPLOFL,"SP < Low",false)//Indicates if SP has exceeded its lower limit, as specified by the Set Point Low Limit (SPLOLM).
}

RSU_Obj * Find_RSU( const char * Name );

void Q_DECL_EXPORT GetObjParams( LPCSTR ObjName, LPCSTR Model, CMem<QParams, 16, 16> * pParamsP, CMem<QParams, 16, 16> * pParamsW )
{
    pParamsP->L = 0;
    pParamsW->L = 0;
		RSU_Obj * pObj = Find_RSU( ObjName );
		int n = pObj->nMap;
		BYTE * pM = Map[n].Addr;
		void * pBase = pM + pObj->pBase;
    if ( strcmp ( Model, "AICHANNEL") == 0 )
    {
        AICHANNEL( pBase, pParamsP, pParamsW ) ;
        return;
    }
    if ( strcmp ( Model, "AOCHANNEL") == 0 )
    {
        AOCHANNEL( pBase, pParamsP, pParamsW ) ;
        return;
    }
    if ( strcmp ( Model, "PID") == 0 )
    {
        PID( pBase, pParamsP, pParamsW ) ;
        return;
    }
}

void * Q_DECL_EXPORT GetObj( const char *  ObjName )
{
	RSU_Obj * pObj = Find_RSU( ObjName );
	if ( pObj == NULL )
		return NULL;
	int n = pObj->nMap;
	BYTE * pM = Map[n].Addr;
	void * pBase = (pM + pObj->pBase );
	return pBase;
}




