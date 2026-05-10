#include "stdafx.h"
#include "CtrlConn.h"
#include "CommProc.h"
#include "SetData.h"
#include <RsuX.h>
#include <MM.h>
#include "Prop.h"

static void * F_err = NULL; 

void CCtrlConn::ReadRsuCsv( int NumbFile, char * File )
{
    Char<256>Path;
    Path.Prt( "%sDATA\\Acy\\%s.csv", PROJECT_ROOT, File );
    FILE * F = NULL;
    F = fopen ( Path.Str, "rt" );
    if ( F == NULL )
    {
        //???		MsgErr (  NULL, "Файл %s не найден или захвачен", File );
        Fprintf ( F_err, ";;;;;;Ошибка открытия '%s'\n", File );
        //    SysMSG ( "#Ошибка чтения '%s'", File );
        return;
    }
    bool GetMes = false;
    PROPS.Get ( "Брать размерности", GetMes );
    int Line = 1;
    Err.File = File;
    Err.F = F_err;
    char StrXLS[16][256] = { 0 };
    char Str[256] = { 0 };
    bool PAZ = strstr ( File, "PAZ" ) != NULL;
    // РСУ
    // 0, A - Точка РСУ
    // 1, B - Alt
    // 2, C - ТипDV
    // 3, D - Описание
    // 4, E - Объект
    // 5, F - Тип
    // 6, G - Точка

    // 7, H - Формула
    // 8, I - Размерность
    // 9, J - min
    //10, K - max;
    //11 -L - датчик
    //12 - Позиция  модуля;
    //13 - Контроллер;
    //14 - Станция управления;
    //15 - Номер канала
    /////////////////////////////////

    fgets ( Str, 255, F );
    while( 1 )
    {
        if( !fgets( Str, _countof(Str)-1, F ) )
            break;
        Line++;
        if ( Line == 2154 )
            KKK();
        Err.Line = Line;
        int L = strlen( Str ) - 1;
        Str[L] = 0;
        if ( L < 3 )
            continue;
        if ( Str[0] == '#' || Str[0] == ';' )
            continue;
        Split( Str, StrXLS );
        bool IsEq = StrXLS[7][0] == '#' && StrXLS[7][1] == '=' && StrXLS[2][1] == 'I';
        if ( GetMes && StrXLS[8][0] )
        {
            // Размерность
            int Mes = GetEngUnits( StrXLS[8] );
            if ( Mes < 0 )
            {
                Err.Head();Fprintf( Err.F, "%s;;;Неизвестная размерность\n", StrXLS[8] );
            }
        }
        if ( StrXLS[0][0] == 0 || ( StrXLS[4][0] == 0 && !IsEq ))
            continue; // не заданы РСУ или объект
        //    if ( Line >= 969 )
        //      KKK();
        //    if ( strstr ( StrXLS[0], "Comm_App.PSA_Digital_1.Value1339"))
        //      KKK();
        bool ToRSU = StrXLS[2][1] == 'I';
        char * TypeObj = StrXLS[5];
        IMM * pPnt = NULL;
        IBaseModel * pModel = NULL;
        bool * pConnection = NULL;
        if ( TypeObj[0] == 'M' )
        {
            // Модель
            pModel = IBaseModel::Find( StrXLS[4] );

            if ( pModel == NULL )
            {
                Err.Head();Fprintf( Err.F, "%s;;;;Объект не найден\n", StrXLS[4] );
                continue;
            }
            pPnt = GetObjPnt( &Err, StrXLS[4], StrXLS[6], StrXLS[2], &pConnection );
            if ( pPnt == NULL )
            {
                //        Err.Head();Fprintf( Err.F,"%s;%s;;Точка не найдена\n", StrXLS[4], StrXLS[6] );
                continue;
            }
            //
            //CConnectInfo Info;
            //char * Type;
            //char * ConnName;
            //void * pSrcAddr;
            //char * Param;
            //IBaseModel * pRefModel;
            //virtual bool Connect( CConnectInfo * pInfo ) = 0;
            CACSPnt * pACS = (CACSPnt*)pPnt;
            /////////////////////////////////
            int Mes = -1;
            double M = 1.;
            if ( GetMes && StrXLS[8][0] )
            {
                // Размерность
                //        if ( strstr ( StrXLS[8], "A" ))
                //          KKK();
                Mes = GetEngUnits( StrXLS[8] );
                if ( Mes < 0 )
                {
                    // уже было          Err.Head();Fprintf( Err.F, "%s;;;Неизвестная размерность\n", StrXLS[8] );
                }
                else
                {
                    M = Scale ( Mes );
                    if ( pACS->pV )
                        pACS->pV->SetMes( Mes, M );
                    if ( pModel )
                        pModel->SetData ( sd_SetSensMes, &Mes );
                }
            }
            /////////////////////////////////
            if ( StrXLS[2][1] == 'I' )
                ((CACSPnt*)pPnt)->pV->SD = 'S';
            else
                ((CACSPnt*)pPnt)->pV->SD = 'D';
 /*
            if ( StrXLS[11][0] )
                pACS->pV->RSU = StrXLS[11];
            else
            {
                if ( StrXLS[1][0] )
                    pACS->pV->RSU = StrXLS[1];
                else
                {
                    if ( StrXLS[0][0] == '%' )
                        pACS->pV->RSU = StrXLS[0] + 2;
                    else
                        pACS->pV->RSU = StrXLS[0];
                }
            }
*/
        }
        /////////////////////////////////
        if ( TypeObj[0] == 'S' )
        {
            pPnt = CreateSensor( &Err, StrXLS[4], StrXLS[6] );
        }
        if ( TypeObj[0] == 'K' )
        {
            pPnt = CreateKlapan( &Err, StrXLS[4], StrXLS[6] );
        }
        int DirHS = 0;
        // if ( strcmp ( TypeObj, "H" ) == 0 )
        // {
        //     // HS
        //     pPnt = GetHSPnt( &Err, StrXLS[4] );
        //     if ( pPnt == NULL )
        //         continue;
        // }
        // if ( strcmp ( TypeObj, "HS->RSU" ) == 0 )
        // {
        //     // HS->RSU
        //     DirHS = 1;
        //     pPnt = GetHSPnt( &Err, StrXLS[4] );
        //     if ( pPnt == NULL )
        //         continue;
        // }
        // if ( strcmp ( TypeObj, "RSU->HS" ) == 0 )
        // {
        //     // RSU->HS
        //     DirHS = -1;
        //     pPnt = GetHSPnt( &Err, StrXLS[4] );
        //     if ( pPnt == NULL )
        //         continue;
        // }
/*
        if ( strcmp ( TypeObj, "PHS->RSU" ) == 0 )
        {
            // PHS->RSU
            //      if ( strstr ( StrXLS[4], "HS-KHM314A"))
            //        KKK();
            int Value_1 = atoi( StrXLS[6] );
            bool Inverse = false;
            //      if ( StrXLS[7][0] == '#' && StrXLS[7][1] == '0' )
            //        Inverse = true;
            pPnt = GetPolyHSPnt( &Err, StrXLS[4], Value_1, Inverse );
            if ( pPnt == NULL )
                continue;
        }

        if ( strcmp ( TypeObj, "RSU->PHS" ) == 0 )
        {
            // RSU->PHS
            int Value_1 = atoi( StrXLS[6] );
            bool Inverse = false;
            //      if ( StrXLS[7][0] == '#' && StrXLS[7][1] == '0' )
            //        Inverse = true;
            pPnt = GetPolyHSPnt( &Err, StrXLS[4], Value_1, Inverse );
            if ( pPnt == NULL )
                continue;
        }
*/
        /*
    if ( TypeObj[0] == 'P' )
      {
// PolyHS
      int Value_1 = atoi( StrXLS[6] );
      bool Inverse = false;
      if ( StrXLS[7][0] == '#' && StrXLS[7][1] == '0' )
        Inverse = true;
      pPnt = GetPolyHSPnt( &Err, StrXLS[4], Value_1, Inverse );
      if ( pPnt == NULL )
        continue;
      }
*/
        bool V = false;
        if ( !V && strcmp ( TypeObj, "RSU->V" ) == 0 )
        {
            // Value
            V = true;
            pPnt = GetValuePnt( &Err, StrXLS[4], StrXLS[2] );
            if ( pPnt == NULL )
                continue;
        }
        if ( !V && strcmp ( TypeObj, "V->RSU" ) == 0 )
        {
            // Value
            V = true;
            pPnt = GetValuePnt( &Err, StrXLS[4], StrXLS[2] );
            if ( pPnt == NULL )
                continue;
        }
        if ( !V && TypeObj[0] == 'V' )
        {
            // Value
            V = true;
            pPnt = GetValuePnt( &Err, StrXLS[4], StrXLS[2] );
            if ( pPnt == NULL )
                continue;
        }
        if ( pPnt == NULL && !IsEq )
        {
            Err.Head();Fprintf( Err.F, "%s;;;;Неизвестный тип\n", TypeObj );
            continue;
        }
        char SrcType = ' ';
        if ( pPnt )
            SrcType = pPnt->GetType();
        /////////////
        //
        if ( strstr ( StrXLS[0], "301_11_AI02.30TC037.30TT037"))
            KKK();
        IMM * pRSU = GetRSUPnt( &Err, StrXLS[0], StrXLS[2], Path );
        if ( pRSU == NULL )
        {
            if ( pConnection )
                *pConnection = false;
            continue;
        }
        CConnect & Conn = Connections.push_back();
        Conn.Init();
        Conn.SetDefect = false;
        Conn.pStable = NULL;
        // .....................................................
        if ( DirHS != 0 )
        {
            if ( DirHS > 0 )
            {
                // HS->RSU
                if ( StrXLS[2][1] != 'I' )
                {
                    Err.Head();Fprintf( Err.F, "%s;;;;Тип точки не согласуется с направлением HS\n", StrXLS[2] );
                }
                Conn.pSrc = pPnt;
                Conn.pDst = pRSU;
            }
            else
            {
                // RSU->HS
                if ( StrXLS[2][1] != 'O' )
                {
                    Err.Head();Fprintf( Err.F, "%s;;;;Тип точки не согласуется с направлением HS\n", StrXLS[2] );
                }
                Conn.pSrc = pRSU;
                Conn.pDst = pPnt;
            }
            if ( CStableConnect::UseStable )
            {
                Conn.pStable = new CStableConnect( &Conn );
                Conn.pStable->Type = 'D';
                Conn.pStable->pData = pPnt;
            }
        }
        else
        {
            if ( StrXLS[2][1] == 'I' )
            {
                Conn.pSrc = pPnt;
                Conn.pDst = pRSU;
            }
            else
            {
                if ( StrXLS[2][1] != 'O' )
                {
                    Err.Head();Fprintf( Err.F, "%s;;;;Неизвестный тип точки\n", StrXLS[2] );
                    continue;
                }
                Conn.pSrc = pRSU;
                Conn.pDst = pPnt;
            }
            if ( CStableConnect::UseStable )
            {
                Conn.pStable = new CStableConnect ( &Conn );
                Conn.pStable->Type = StrXLS[2][0];
                Conn.pStable->pData = pPnt;
            }
        }
        Conn.Type[0] = StrXLS[2][0];
        Conn.Type[1] = StrXLS[2][1];
        //    strcpy_s ( Conn.Obj, sizeof ( Conn.Obj ), StrXLS[4] );
        //    strcpy_s ( Conn.Pnt, sizeof ( Conn.Pnt ), StrXLS[6] );
        //    strcpy_s ( Conn.RSU, sizeof ( Conn.RSU ), StrXLS[0] );
        Char<256>Ref;
        Ref.Prt ( "%s.%s", StrXLS[4], StrXLS[6] );
        if ( ToRSU )
            Conn.RefFrom = Ref;
        else
            Conn.RefTo = Ref;
        Conn.Desc = StrXLS[3];
        if ( StrXLS[1][0] != 0 && StrXLS[1][0] != 'Z' && StrXLS[2][0] == 'A' && StrXLS[2][1] == 'I' && (StrXLS[4][0] != 0 || StrXLS[7][0] != 0) )
        {
            Conn.Sens = StrXLS[1];
            //      if ( lstrcmp ( StrXLS[6], "Показания" ) == 0 )
            //        Fsend( "Sens.txt", "%s;%s\n", StrXLS[1], StrXLS[4] );
        }
        if ( StrXLS[2][0] == 'D' && StrXLS[2][1] == 'I' )
        {
            Conn.Sens = StrXLS[1];
            //      if ( lstrcmp ( StrXLS[6], "Показания" ) == 0 )
            //        Fsend( "Sens.txt", "%s;%s\n", StrXLS[1], StrXLS[4] );
        }
        Conn.Line = Line;
        Conn.PreDstValue = 0.;
        //if ( StrXLS[11][0] != 0 )
        //  {
        //  GiveCommData( StrXLS[11], CSensor::TypeID, (void**)&Conn.pSens );
        //  }
        Conn.UseMinMax = false;
        if ( StrXLS[9][0] != 0 && StrXLS[10][0] != 0 )
        {
            Conn.Min = atof( StrXLS[9] );
            Conn.Max = atof( StrXLS[10] );
            if ( Conn.Max <= Conn.Min )
            {
                Err.Head();Fprintf( Err.F, ";;;;Ошибка задания min, max\n" );
                continue;
            }
            Conn.UseMinMax = true;
        }
        Conn.OP = NULL;
        Conn.kOP = 0;
        if ( StrXLS[7][0] )
        {
            if ( SimpleFormula( Conn, StrXLS[7] ))
            {
                char Txt[256];
                sprintf ( Txt, "%s%s", StrXLS[0], StrXLS[7] );
                if ( ToRSU )
                    Conn.RefTo = Txt;
                else
                    Conn.RefFrom = Txt;
                continue;
            }
            else
            {
                char Txt[256];
                sprintf ( Txt, "%s#ошибка", StrXLS[0] );
                if ( ToRSU )
                    Conn.RefTo = Txt;
                else
                    Conn.RefFrom = Txt;
            }
            Err.Head();Fprintf( Err.F, "%s;;;;Неизвестная формула\n", StrXLS[7] );
        }
        else
        {
            if ( ToRSU )
                Conn.RefTo = StrXLS[0];
            else
                Conn.RefFrom = StrXLS[0];
        }
        Conn.pDef = NULL;
        KKK();
#ifdef LINUX
       ConnInfo & Info = Connections_Info.push_back();
        Info.Descr = Conn.Desc.Str;
       if ( ToRSU )
            {
            Info.Model =  Conn.RefFrom.Str;
            Info.RSU =  Conn.RefTo.Str;
            Info.Type = 'I';
            }
       else
            {
           Info.Model =  Conn.RefTo.Str;
           Info.RSU = Conn.RefFrom.Str;
           Info.Type = 'O';
           }
#endif
    }
    fclose( F );
}

//void SetDefects( CACS_AD * pDef )
/**********
bool FindLimits( char * ACSName )
  {
  //if ( pDef ->Defect < 0 )
  //  return;
  //if ( strchr ( pDef->ACSName, ':' ) != NULL )
  //  {
  //  goto SetDef;
  //  }
  CY_Base * pDst = (*pFindYObj) ( ACSName );
  ASS(pDst)
  if ( pDst->Model[0] == 'P' && pDst->Model[1] == 'I' )
    {
    CY_IO * pIO = (CY_IO*)pDst;
    ASS(pIO->OUT.pTo )
    char NameDst[32];
    lstrcpy ( NameDst, pIO->OUT.pTo );
    char * P = strchr ( NameDst, '.' );
    if ( P )
      *P = 0;
    pDst = (*pFindYObj) ( NameDst );
    ASS(pDst)
 //
//    return;
    }
  if ( pDst->Y_Type == id__Z )
    {
    CY_DI * pDI = (CY_DI*)pDst;
    if ( pDI->OUTD.pTo == NULL )
      return;
    char Ref[132];
    lstrcpy ( Ref, pDI->OUTD.pTo );
    char * P = strchr ( Ref, '.' );
    if ( P )
      *P = 0;
    pDst = (*pFindYObj) ( Ref );
    ASS(pDst)
      //    return;
      //    pDst = (*pFindYObj) ( ((CY_IO*)pDst)->OUTD.To );
    }
  if ( pDef->ACSName[0] == '%' && pDef->ACSName[1] == '%' )
    {
    char NewName[128];
    lstrcpy ( NewName, pDef->ACSName + 2 );
    lstrcpy ( pDef->ACSName, NewName );
    }
  switch ( pDst->Y_Type )
    {
    case CPVI::TypeID:
      {
      CY_PVI * pY = (CY_PVI*)pDst;
      pDef->SH = pY->W->SH;
      pDef->SL = pY->W->SL;
      pDef->HH = pY->W->HH;
      pDef->LL = pY->W->LL;
      pDef->PH = pY->W->PH;
      pDef->PL = pY->W->PL;
      }
      break;
    case CPID::TypeID:
      {
      CY_PID * pY = (CY_PID*)pDst;
      pDef->SH = pY->W->SH;
      pDef->SL = pY->W->SL;
      pDef->HH = pY->W->HH;
      pDef->LL = pY->W->LL;
      pDef->PH = pY->W->PH;
      pDef->PL = pY->W->PL;
      }
      break;
    case CS_ANLG_S::TypeID:
      {
      CYS_ANLG_S * pY = (CYS_ANLG_S*)pDst;
      pDef->SH = pY->W->SH;
      pDef->SL = pY->W->SL;
      pDef->HH = pY->W->HH;
      pDef->LL = pY->W->LL;
      pDef->PH = pY->W->PH;
      pDef->PL = pY->W->PL;
      }
      break;
    case CMLD::TypeID:
      {
      CY_MLD * pY = (CY_MLD*)pDst;
      ASS(0)
      }
      break;
    case id_Sensor:
      {
      CY_IO * pY = (CY_IO*)pDst;
      ASS(0)
      }
      break;
    case CSS_DUAL::TypeID:
      {
      CY_SS_DUAL * pY = (CY_SS_DUAL*)pDst;
      pDef->SH = pY->W->SH;
      pDef->SL = pY->W->SL;
      pDef->HH = pY->W->HH;
      pDef->LL = pY->W->LL;
      pDef->PH = pY->W->PH;
      pDef->PL = pY->W->PL;
      }
      break;
    case CDLAY::TypeID:
      {
      CY_DLAY * pY = (CY_DLAY*)pDst;
      pDef->SH = pY->W->SH;
      pDef->SL = pY->W->SL;
      }
      break;
    case CFUNC_VAR::TypeID:
      {
      CY_FUNC_VAR * pY = (CY_FUNC_VAR*)pDst;
      pDef->SH = pY->W->SH;
      pDef->SL = pY->W->SL;
      }
      break;
    default:
      ASS(0)
        return;
    }
*********/
