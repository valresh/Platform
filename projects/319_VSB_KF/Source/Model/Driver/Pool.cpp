#include "stdafx.h"
#include "Driver.h"
#include "Err.h"
#include "MM.h"
#include "HydroStruct.h"

int CDriver::InitPool ( struct CVarPool * pPool, struct CGroupObject * pObj )
{
    pObj->pObject = this;
    pObj->MainPool = true;
    pObj->ObjName = ObjName;
    pObj->Model = Model;
    //
    int kPoints = 0;
    IMM MM;
    int nSrc = 0;
    CObjectPoint * pPnt = pPool->pFirstPoint;
    CObjectPoint * pSrc = NULL;
    while ( pPnt )
        {
        kPoints++;
        if ( pPnt->IO == '-' )
        {
            pSrc = pPnt;
            nSrc++;
        }
        if ( pPnt->pObj && strstr ( pPnt->pObj->ObjName, "Р-3002S1" ))
            KKK();
        pPnt->kScalarVar = 0;
        pPnt->kScalarEq = 0;
        pPnt->GroupNumbVar = NULL;
        pPnt->AddrVar = NULL;
        pPnt = pPnt->pNextPoolPoint;
        }
    int nDst = 0;
    pPnt = pPool->pFirstPoint;
    while ( pPnt )
    {
        if ( pPnt->IO == '+' )
        {
            if(pSrc)
            {
                pSrc->pObj->pObject->SrcMM(pPool->Model, &MM);
                pPnt->pObj->pObject->DstMM(pPool->Model, &MM);
                nDst++;
            }
        }
        pPnt = pPnt->pNextPoolPoint;
    }
    //
    pPool->kScalarVar = 0;
    pPool->kPoints = kPoints;
    //
    pObj->kPoints = kPoints;
    pObj->pPoints = NULL;// Упорядоченные ссылки на CObjectPoint по порядковым номерам в пределах внутренней группы
    pObj->kPointsVar = NULL; // Число скалярных переменных у каждой точки
    pObj->kLinearVar = 0; // Чисто скалярных переменных у объекта
    pObj->NumbVarInGroup = NULL;/*[kV]*/; // Номера скалярных переменных в общем списке группы
    pObj->AddrVar = NULL;    // Адреса скалярных переменных
    pObj->kLinearEq = 0;                // Общее число уравнений для скалярных переменных
    /////////////////////////////////////////////////////
    return 0;
}

