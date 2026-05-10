#include "stdafx.h"
#include "CtrlConn.h"
#include "Err.h"
#include "CommProc.h"
#include "SetData.h"
#include "Trans.h"
#include "SysDataTypes.h"


const double Press_Max = 6.;
int K = 0;
void CCtrlConn::Calc( double dt )
{
    //  if ( K++ == 2 )
    //    PrtDefects( );
    Time = pSys->dModelT;
    Accel = pSys->Accel;
    _dt = pSys->dt;
    Steps = pSys->m_nStep;
    SET_BP BreakPoint;
    if ( InitState )
    {
        for ( int n = 0; n < IBaseModel::kObjects; n++ )
        {
            IBaseModel * pModel = IBaseModel::AllObjects[n];
            pModel->SetData ( sd_SetInitState, NULL );
        }
        InitState = false;
    }
    double Ta = T_air;
    Т_воздуха = T_air;
    // Press_KIP[0] = *KIP[0];
    // Press_KIP[1] = *KIP[1];
    // Press_KIP[2] = *KIP[2];
    //if ( kStep < 50 )
    //  Press_KIP[0] = Press_Max;
    //else
    //  KKK();
    double P = Press_KIP[0];
    if ( ReRead )
    {
        Connections.Free();
        Connections.L = 0;
        Step1();
        ReRead = false;
    }
    if ( NoElectroShort )
    {
        TimeNoElectroShort -= dt;
        if ( TimeNoElectroShort < 0. )
        {
            for(int i = 0; i < IBaseModel::kObjects; i++)
            {
                IElectroModel* pElectro = dynamic_cast<IElectroModel*>(IBaseModel::AllObjects[i]);
                if(pElectro)
                    pElectro->ElectroOn();
            }
            pExt->IsElectro_380 = true;
            pExt->IsElectro_6000 = true;
            NoElectroShort = false;
            TimeNoElectroShort = 0.;
        }
    }
    UVal Val;
    void * pVal = &Val;
    kErr = 0;
    for ( int n = 0; n < Connections.L; n++ )
    {
        CConnect & Conn = Connections[n];
        if((!bIn && Conn.Type[1] == 'I') || (bIn && Conn.Type[1] == 'O'))
            continue;
        if ( Conn.Type[0] == 'X' )
        {
            Duplex_A( Conn );
            continue;
        }
        if ( Conn.Type[0] == 'x' )
        {
            Duplex_D( Conn );
            continue;
        }
        if ( Conn.ThisLS )
        {
            if ( Conn.pDefectValueLS && Conn.pDef )
                *Conn.pDefectValueLS =  Conn.pDef->SetDefect( *Conn.pTrueValueLS );
            continue;
        }
        //if ( Conn.Formula.Func[0] == 'F' )
        //  {
        //  VBS.Formula( Conn.Formula.Func, Conn.pSrc, Conn.pDst );
        //  continue;
        //  }
        if ( Conn.Formula.Func[0] == '#' )
        {
            if ( Conn.Type[0] == 'D' && Conn.Type[1] == 'I' && Conn.pDef &&
                ( Conn.pDef->Defect == eПостоянно_0 || Conn.pDef->Defect == eПостоянно_1 ))
            {
                int V = 0;
                if ( Conn.pDef->Defect == eПостоянно_1 )
                    V = 1;
                Conn.pDst->SetValue( 'D', &V );
            }
            else
                SpecFunc( Conn, Conn.Formula, Conn.pSrc, Conn.pDst, Conn.PreDstValue );
            //if ( Conn.Type[0] == 'A' && Conn.pSens )
            //  {
            //  UVal Dst;
            //  if ( !Conn.pDst->GetValue( 'A', &Dst ))
            //    return;
            //  Conn.pSens->SensValue = Conn.pSens->ShowValue = Conn.pSens->PhysValue = Dst.D;
            //  }
            continue;
        }
        if ( Conn.Formula.Func[0] == 'R' && Conn.Formula.Func[1] == 'N' )
        {
            // RND
            if ( Conn.pSrc && Conn.pDst )
            {
                if ( Conn.Type[0] == ' ' )
                    Conn.Type[0] = Conn.pDst->GetType();
                if ( Conn.Type[0] == 'A' )
                {
                    Conn.pSrc->GetValue( Conn.Type[0], pVal );
                    UVal Res;
                    if ( Conn.Formula.Func[3] == 'T' )
                    {
                        Res.D = ( Val.D - T_air ) * ( 1. - 2. * Conn.Formula.A ) + T_air;
                    }
                    else
                    {
                        Res.D = Val.D * ( 1. - 2. * Conn.Formula.A );
                    }
                    if ( Conn.Type[0] == 'A' && Conn.pDef  && Conn.pDef->Defect >= 0 )
                    {
                        Res.D = Conn.pDef->SetDefect( Res.D );
                    }
                    Conn.PreDstValue = Res.D;
                    Conn.pDst->SetValue( Conn.Type[0], &Res );
                }
            }
            continue;
        }
        if(!lstrcmp(Conn.Formula.Func, m_szScale))
        {
            UVal Res;
            if ( Conn.Type[0] == ' ' )
                Conn.Type[0] = Conn.pDst->GetType();
            Conn.pSrc->GetValue( Conn.Type[0], &Res );
            char typeSrc = Conn.pSrc->GetType(), typeDest = Conn.pDst->GetType();
            if(typeSrc == 'D' && typeDest == 'A')
                Res.D = Conn.pDef->SetDefect( Res.I * Conn.Formula.A + Conn.Formula.C );
            else if(typeSrc == 'D' && typeDest == 'D')
                Res.I = (int)(Res.I * Conn.Formula.A + Conn.Formula.C);
            else if(typeSrc == 'A' && typeDest == 'D')
                Res.I = (int)(Res.D * Conn.Formula.A + Conn.Formula.C);
            else
                Res.D = Conn.pDef->SetDefect( Res.D * Conn.Formula.A + Conn.Formula.C );
            Conn.pDst->SetValue( typeDest, &Res );
            continue;
        }
        else if(!lstrcmp(Conn.Formula.Func, "ScaleN") && ( !Conn.pDef  || Conn.pDef->Defect == -1 ))
        {
            UVal Res;
            Conn.pSrc->GetValue( Conn.Type[0], &Res );
            if ( Res.D < Conn.Formula.Min )
                Res.D = Conn.Formula.Min;
            if ( Res.D > Conn.Formula.Max )
                Res.D = Conn.Formula.Max;
            Res.D = Res.D * Conn.Formula.A + Conn.Formula.C;
            Conn.pDst->SetValue( Conn.Type[0], &Res );
            continue;
        }
        ////////////////////////////////////////////////
        if ( Conn.pSrc && Conn.pDst )
        {
            if ( Conn.Type[0] == ' ' )
                Conn.Type[0] = Conn.pDst->GetType();
            BOOL bIO = FALSE;
            if ( Conn.Type[0] == 'A' )
            {
                CACSPnt* pPnt = NULL;
                if( Conn.pSrc->ID == 1)
                    pPnt = (CACSPnt*)Conn.pSrc;
                if( pPnt && pPnt->pV->Type[2] == 'O')
                {
                    bIO = TRUE;
                    Conn.pDst->GetValue(Conn.Type[0], pVal);
                    if( Conn.DstOld.A != Val.D )
                    {
                        IMM * pTemp;
                        pTemp = Conn.pDst;
                        Conn.pDst = Conn.pSrc;
                        Conn.pSrc = pTemp;
                    }
                }
                Conn.pSrc->GetValue( 'A', pVal );
                if ( Conn.UseMinMax )
                {
                    if ( Val.D < Conn.Min )
                        Val.D = 0.;
                    else
                    {
                        if ( Val.D > Conn.Max )
                            Val.D = 100.;
                        else
                            Val.D = ( Val.D - Conn.Min ) / ( Conn.Max - Conn.Min ) * 100.;
                    }
                }
                if ( Conn.kOP > 0 )
                {
                    // Формула
                    CRes X, Y;
                    X.Type = 'D';
                    X.dVal = Val.D;
                    if ( WorkOP ( Conn.kOP, Conn.OP, X, Y ))
                    {
                        if ( Y.Type == 'D' )
                            Val.D = Y.dVal;
                        else
                            Val.D = Y.iVal;
                    }
                }
                if ( Conn.pDef  && Conn.pDef->Defect >= 0 )
                {
                    Val.D = Conn.pDef->SetDefect( Val.D );
                }
                Conn.PreDstValue = Val.D;
                Conn.pDst->SetValue( Conn.Type[0], pVal );
                if(bIO)
                    Conn.DstOld.A = Val.D;
                continue;
            }
            /////////////////////////////////////////////////////
            if ( Conn.Type[0] == 'D' )
            {
                CACSPnt *pPnt = NULL;
                char SrcType = Conn.pSrc->GetType( );
                char DstType = Conn.pDst->GetType( );
                if(Conn.pSrc->ID == 1)
                    pPnt = (CACSPnt*)Conn.pSrc;
                if(Conn.pDst->ID == 1)
                    pPnt = (CACSPnt*)Conn.pDst;
                if(pPnt && pPnt->pV->Type[2] == 'O')
                {
                    bIO = TRUE;
                    Conn.pDst->GetValue(DstType, &Val.I);
                    if(Conn.DstOld.D != Val.I)
                    {
                        IMM * pTemp;
                        pTemp = Conn.pDst;
                        Conn.pDst = Conn.pSrc;
                        Conn.pSrc = pTemp;
                    }
                }
                Conn.pSrc->GetValue( SrcType, pVal );
                if ( strstr ( Conn.RefTo, "301_11_AI02.30TC037.30TT037"))
                    KKK();
                if ( Conn.kOP > 0 )
                {
                    // Формула
                    CRes X, Y;
                    if ( SrcType == 'A' )
                    {
                        X.Type = 'D';
                        X.dVal = Val.D;
                    }
                    else
                    {
                        X.Type = 'I';
                        X.iVal = Val.I;
                    }
                    if ( WorkOP ( Conn.kOP, Conn.OP, X, Y ))
                    {
                        if ( DstType == 'A' )
                        {
                            if ( Y.Type == 'D' )
                                Val.D = Y.dVal;
                            else
                                Val.D = Y.iVal;
                        }
                        else
                        {
                            if ( Y.Type == 'D' )
                                Val.I = (int)Y.dVal;
                            else
                                Val.I = Y.iVal;
                        }
                    }
                }
                Conn.pDst->SetValue( DstType, pVal );
                if(bIO)
                    Conn.DstOld.D = Val.I;
                continue;
            }
            char SrcType = Conn.pSrc->GetType( );
            char DstType = Conn.pDst->GetType( );
            Conn.pSrc->GetValue( SrcType, pVal );
            Conn.pDst->SetValue( DstType, pVal );
        }
        else
            KKK();
    }
}

void CCtrlConn::Duplex_A( CConnect & Conn )
{
    if ( Conn.pSrc == NULL || Conn.pDst == NULL )
        return;
    char Ts = Conn.pSrc->GetType();
    if ( Ts != 'A')
        return;
    char Td = Conn.pDst->GetType();
    if ( Td != 'A')
        return;
    UVal Src;
    if ( !Conn.pSrc->GetValue( Ts, &Src ))
        return;
    UVal Dst;
    if ( !Conn.pDst->GetValue( Ts, &Dst ))
        return;
    if ( Src.D == Dst.D )
    {
        Conn.SrcOld.A = Src.D;
        Conn.DstOld.A = Dst.D;
        return;
    }
    //
    bool ChangeS = Conn.SrcOld.A != Src.D;
    bool ChangeD = Conn.DstOld.A != Dst.D;
    if ( ChangeS )
    {
        // Src - приоритет
        Conn.pDst->SetValue( Ts, &Src );
        Conn.SrcOld.A = Src.D;
        Conn.DstOld.A = Src.D;
        return;
    }
    Conn.pSrc->SetValue( Td, &Dst );
    Conn.SrcOld.A = Dst.D;
    Conn.DstOld.A = Dst.D;
}

void CCtrlConn::Duplex_D( CConnect & Conn )
{
    if ( Conn.pSrc == NULL || Conn.pDst == NULL )
        return;
    char Ts = Conn.pSrc->GetType();
    if ( Ts != 'D')
        return;
    char Td = Conn.pDst->GetType();
    if ( Td != 'D')
        return;
    UVal Src;
    if ( !Conn.pSrc->GetValue( Ts, &Src ))
        return;
    UVal Dst;
    if ( !Conn.pDst->GetValue( Ts, &Dst ))
        return;
    if ( Src.I == Dst.I )
    {
        Conn.SrcOld.D = Src.I;
        Conn.DstOld.D = Dst.I;
        return;
    }
    //
    bool ChangeS = Conn.SrcOld.D != Src.I;
    bool ChangeD = Conn.DstOld.D != Dst.I;
    if ( ChangeS )
    {
        // Src - приоритет
        Conn.pDst->SetValue( Ts, &Src );
        Conn.SrcOld.D = Src.I;
        Conn.DstOld.D = Src.I;
        return;
    }
    Conn.pSrc->SetValue( Td, &Dst );
    Conn.SrcOld.D = Dst.I;
    Conn.DstOld.D = Dst.I;
}


void CCtrlConn::SpecFunc ( CConnect & Conn, CFormula & Formula, struct IMM * pX, struct IMM * pRes, double & SensValue )
{
    DA X;
    char TypeX;
    DA Res;
    char TypeRes;
    if ( Formula.Func[1] == '=' )
    {
        TypeRes = pRes->GetType();
        if ( TypeRes == 'A')
        {
            if ( Conn.pDef == NULL || Conn.pDef->Defect < 0 )
                SensValue = Res.A = Formula.C;
        }
        if ( TypeRes == 'D')
            SensValue = Res.D = (int)Formula.C;
        if ( Conn.Type[0] == 'A' && Conn.pDef  && Conn.pDef->Defect >= 0 )
            Res.A = Conn.pDef->SetDefect( SensValue );
        if ( !pRes->SetValue( TypeRes, &Res ))
            kErr++;
        return;
    }
    else
    {
        if ( Formula.Func[1] == '~' )
            KKK();
        if ( pX )
        {
            TypeX = pX->GetType();
            if ( !pX->GetValue( TypeX, &X ))
            {
                kErr++;
                return;
            }
        }
    }
    switch ( Formula.Func[1])   // Первый символ S
    {
    case '!':
        if ( TypeX != 'D' && TypeX != 'B' )
        {
            kErr++;
            return;
        }
        if ( X.D )
            Res.D = 0;
        else
            Res.D = 1;
        SensValue = Res.D;
        TypeRes = 'D';
        break;
    case '>':
        if ( TypeX != 'A')
        {
            kErr++;
            return;
        }
        if ( Conn.pDef )
            X.A = Conn.pDef->SetDefect( X.A );
        if ( X.A > Formula.C )
            Res.D = 1;
        else
            Res.D = 0;
        SensValue = Res.D;
        TypeRes = 'D';
        break;
    case '<':
        if ( TypeX != 'A')
        {
            kErr++;
            return;
        }
        if ( Formula.C <= 0.81 )
            KKK();
        if ( Conn.pDef )
            X.A = Conn.pDef->SetDefect( X.A );
        if ( X.A < Formula.C )
            Res.D = 1;
        else
            Res.D = 0;
        SensValue = Res.D;
        TypeRes = 'D';
        break;
    case '~':
        if ( TypeX == 'D')
        {
            if ( X.D == Formula.C )
                Res.D = 1;
            else
                Res.D = 0;
            SensValue = Res.D;
            TypeRes = 'D';
            break;
        }
        if ( TypeX == 'A' )
        {
            if ( Conn.pDef )
                X.A = Conn.pDef->SetDefect( X.A );
            if ( X.A == Formula.C )
                Res.D = 1;
            else
                Res.D = 0;
            SensValue = Res.D;
            TypeRes = 'D';
            break;
        }
        kErr++;
        return;
        break;
    case '+':
    {
        TypeRes = pRes->GetType();
        DA Old;
        if ( !pRes->GetValue( TypeRes, &Old ))
            return;
        if ( TypeX == 'A' && TypeRes == 'A' )
        {
            if ( Conn.pDef )
                X.A = Conn.pDef->SetDefect( X.A );
            Res.A = Old.A + X.A;
            SensValue = Res.A;
            break;
        }
        if ( TypeX == 'D' && TypeRes == 'D' )
        {
            Res.D = Old.D + X.D;
            SensValue = Res.D;
            break;
        }
        if ( TypeX == 'A' && TypeRes == 'D' )
        {
            if ( Conn.pDef )
                X.A = Conn.pDef->SetDefect( X.A );
            Res.D = Old.D + (int)X.A;
            SensValue = Res.D;
            break;
        }
        if ( TypeX == 'D' && TypeRes == 'A' )
        {
            Res.A = Old.A + X.D;
            SensValue = Res.A;
            break;
        }
    }
    break;
    case '*':
        if ( TypeX != 'A')
        {
            kErr++;
            return;
        }
        if ( Conn.pDef )
            X.A = Conn.pDef->SetDefect( X.A );
        Res.A = X.A * Formula.C;
        if ( Formula.Func[2] == '+' )
            Res.A += Formula.A;
        SensValue = Res.A;
        TypeRes = 'A';
        break;
    case '/':
        if ( TypeX != 'A')
        {
            kErr++;
            return;
        }
        if ( Conn.pDef )
            X.A = Conn.pDef->SetDefect( X.A );
        Res.A = X.A / Formula.C;
        SensValue = Res.A;
        TypeRes = 'A';
        break;
    case 'L':
    {
        if ( TypeX != 'A')
        {
            kErr++;
            return;
        }
        if ( Conn.pDef )
            X.A = Conn.pDef->SetDefect( X.A );
        double L = X.A;
        double Lm = 0.;
        if ( L < Formula.A )
            Lm = 0.;
        else
        {
            if ( L > Formula.C )
                Lm = 100.;
            else
                Lm = ( L - Formula.A ) / ( Formula.C - Formula.A ) * 100.;
        }
        Res.A = Lm;
        SensValue = Res.A;
        TypeRes = 'A';
    }
    break;
    default:
        KKK();
        return;
    }


    if ( !pRes->SetValue( TypeRes, &Res ))
    {
        kErr++;
        return;
    }
}

