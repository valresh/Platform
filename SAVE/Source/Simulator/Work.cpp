#include "Start.h"
#include "BaseModel.h"
#include "CommProc.h"
#include <QLibrary>
#include "Err.h"
#include "Prop.h"
#include "DataTypes.h"
#include "SysDataTypes.h"
#include "SetDataTypes.h"
#include <QSemaphore>
#include <unistd.h>
#include "Interface/HydroGroupsInterface.h"
#include "mainwindow.h"
#include <QElapsedTimer>

QSemaphore Sem_Model;
QSemaphore Sem_Hydro;
QSemaphore Sem_DCU;
HydroGroupsInterface * HydroGroups = NULL;
int kGroups = 0;
double gMaxNew_dt = 2e-5;
int kStep = 0;
bool SaveParams = false;
bool SaveState = false;

void Start::InitWork()
{
    pMainHydro->SetData( 1003, &HydroGroups );
    kGroups = HydroGroups->GetCount();
    pSys->dt = 1e-6;
    pSys->AccelZ = 1000.;
}

void Step100( double Curr_Time_real, double& dTimeModel, double& dTimeReal, int & dStep )
{
    Curr_Time_real *= 1e-3;
    double Curr_Time_mod = pSys ->dModelT;
    double dT_mod = ( Curr_Time_mod - dTimeModel ) * 3600.;
    double dT_real = Curr_Time_real - dTimeReal;
    double Accel = 0.;
    if ( dT_mod < 1e10 * dT_real )
        Accel = dT_mod / dT_real;
    pSys -> Accel = Accel;
    int dS = pSys -> m_nStep - dStep;
    pSys -> Speed = dS * 60./ dT_real;
    //
    if ( Accel > 0.1 )
    {
        double AccelZ = pSys->AccelZ;
        ///    AccelZ = 1000.;
        if ( AccelZ > 0.1 )
        {
            double Delta = Accel / AccelZ;
            if ( Delta > 2.0 ) Delta = 2.0;
            if ( Delta < 0.5 ) Delta = 0.5;
            double dTopt = pSys -> dt / Delta;
            double New_dt = 0.5 * pSys -> dt + 0.5 * dTopt;
            if ( New_dt > gMaxNew_dt ) //1e-4
                New_dt = gMaxNew_dt;//1e-4;
            pSys -> dt = New_dt;
        }
    }
    else
    {
        double New_dt = pSys->dt * 2;
        if ( New_dt > gMaxNew_dt ) //1e-4
            New_dt = gMaxNew_dt;//1e-4;
        pSys -> dt = New_dt;
    }
    //
    dTimeModel = Curr_Time_mod;
    dTimeReal  = Curr_Time_real;
    dStep = pSys -> m_nStep;
    pSys->Time_real = dTimeReal;
}

void Models::stop ()
{
    Stop = true;
}

static char Msg[1024];
void Models::OutMsg( const char * Fmt, ... )
{
    va_list arg;
    va_start(arg,Fmt);
    vsnprintf ( Msg, sizeof(Msg), Fmt, arg );
    va_end(arg);
    emit OutTxt( Msg );
}
#include <sys/ptrace.h>

void Models::Go()
{
    ptrace(PTRACE_TRACEME, getpid(), 0, 0);
    bool First = true;
    connect ( this, &Models::ShowSteps, pMainWnd, &MainWindow::ShowSteps,Qt::QueuedConnection );
    connect ( this, &Models::ShowAccel, pMainWnd, &MainWindow::ShowAccel,Qt::QueuedConnection );
    connect ( this, &Models::ShowData, pMainWnd, &MainWindow::ShowData_1,Qt::QueuedConnection );
    connect ( this, &Models::OutTxt, pMainWnd, &MainWindow::OutTxt,Qt::QueuedConnection );
    double TimeModel = 0., TimeReal = 0.;
    int Steps = 0;
    QElapsedTimer timer;
    timer.start();
    while ( !Stop )
    {
        if ( Pause )
        {
            sleep(1);
            if ( SaveParams )
            {
                char Path[1024];
                sprintf ( Path, "%sDATA/STATES/%s.parm", PROJECT_ROOT, pMainWnd->ParamsWrite());
                int L = SaveParamsToFile( Path );
                if ( L > 0 )
                    OutMsg( "Параметры сохранены '%s'(%d)", pMainWnd->ParamsWrite(), L );
                else
                    OutMsg( "Ошибка сохранения параметров '%s'", pMainWnd->ParamsWrite() );
                SaveParams = false;
            }
            if ( SaveState )
            {
                char Path[1024];
                sprintf ( Path, "%sDATA/STATES/%s.dat", PROJECT_ROOT, pMainWnd->StateWrite());
                int L = SaveStateToFile( Path );
                if ( L > 0 )
                    OutMsg( "Состояние сохранено '%s'(%d) ", pMainWnd->StateWrite(), L );
                else
                    OutMsg( "Ошибка сохранения состояния '%s'", pMainWnd->StateWrite() );
                SaveState = false;
            }
            continue;
        }
        if ( pMainWnd->Set_cpu[1] )
        {
            pMainWnd->Set_cpu[1] = false;
            set_thread_affinity( pMainWnd->main_cpu[1].N );
        }
        if ( SaveParams )
        {
            char Path[1024];
            sprintf ( Path, "%sDATA/STATES/%s.parm", PROJECT_ROOT, pMainWnd->ParamsWrite());
            int L = SaveParamsToFile( Path );
            if ( L > 0 )
                OutMsg( "Параметры сохранены '%s'(%d)", pMainWnd->ParamsWrite(), L );
            else
                OutMsg( "Ошибка сохранения параметров '%s'", pMainWnd->ParamsWrite() );
            SaveParams = false;
        }
        if ( SaveState )
        {
            char Path[1024];
            sprintf ( Path, "%sDATA/STATES/%s.dat", PROJECT_ROOT, pMainWnd->StateWrite());
            int L = SaveStateToFile( Path );
            if ( L > 0 )
                OutMsg( "Состояние сохранено '%s'(%d) ", pMainWnd->StateWrite(), L );
            else
                OutMsg( "Ошибка сохранения состояния '%s'", pMainWnd->StateWrite() );
            SaveState = false;
        }
        kStep++;
        pSys->m_nStep = kStep;
        if ( kStep % 10 == 0 )
        {
            emit ShowSteps ( kStep );
        }
        if ( kStep % 50 == 0 )
        {
            //      PD.Set();
            Step100( timer.elapsed(), TimeModel, TimeReal, Steps );
            emit ShowAccel ( pSys->Accel );
            //      emit ShowData( PD.ProcUer, PD.Mem );
        }
        //////////////////////////////////////////////////
        Sem_Model.acquire(2);
        try {
        double dt = pSys->dt;
        pSys->dModelT += dt;
        for ( int n = 0; n < kGroups; n++)
            {
            HydroGroups->GetObjectMatrix( n, dt );
            }
        Sem_Hydro.release();
        Sem_DCU.release();
        if ( First )
            {
            First = false;
            continue;
            }
        bool bIn = false;
        pCtrlConn->SetData(sd_SetConnTypy, &bIn);
        pCtrlConn->StepT(dt);
        for(int nModel = 0; nModel < IBaseModel::kObjects; nModel++)
        {
            IBaseModel * pModel = IBaseModel::AllObjects[nModel];
            if(pModel->TypeObj == IBaseModel::RSU_Obj || pModel->TypeObj == IBaseModel::Y_Obj)
                continue;
            if( pModel == pCtrlConn )
                continue;
            int Res = pModel->StepT( dt );
        }
        bIn = true;
        pCtrlConn->SetData(sd_SetConnTypy, &bIn);
        pCtrlConn->StepT(dt);
        } catch (...)
        {
            SysMSG ( "Работа моделей закончилась аварийно" );
        }
        //////////////////////////////////////////////////
    }
    SysMSG ( "Работа моделей закончилась" );
}


void Hydro::Go()
{
    ptrace(PTRACE_TRACEME, getpid(), 0, 0);
    connect ( this, &Hydro::ShowData, pMainWnd, &MainWindow::ShowData_2,Qt::QueuedConnection );
    try {
    while ( !Stop )
    {
        if ( Pause )
        {
            sleep(1);
            continue;
        }
        if ( pMainWnd->Set_cpu[2] )
        {
            pMainWnd->Set_cpu[2] = false;
            set_thread_affinity( pMainWnd->main_cpu[2].N );
        }
        //////////////////////////////////////////////////
        Sem_Hydro.acquire();
        double dt = 1e-4;
        bool SetVarInHydro = true;
        for ( int n = 0; n < kGroups; n++)
        {
            HydroGroups->HydroCalc(n, dt, SetVarInHydro );
        }
        Sem_Model.release();
        if ( kStep % 50 == 0 )
        {
            //      PD.Set();
            //      emit ShowData( PD.ProcUer, PD.Mem );
        }
        //////////////////////////////////////////////////
    }
    } catch (...)
    {
        SysMSG ( "Работа гидравлики закончилась аварийно" );
    }
SysMSG ( "Работа гидравлики закончилась" );
}


void DCU::Go()
{
    ptrace(PTRACE_TRACEME, getpid(), 0, 0);
    connect ( this, &DCU::ShowData, pMainWnd, &MainWindow::ShowData_2,Qt::QueuedConnection );
    int OLD_CALC_MODE = (HydroGroups->GetCalculationMethod()) >> 0 & 1;
//
    for (int i = 0; i < kRuntimeModel; i++)
        {
        if (!g_RuntimeModels[i])
            continue;
        g_RuntimeModels[i]->PreStepT();
        }
 //
    try {
    while ( !Stop )
    {
       if ( Pause )
        {
            sleep(1);
            continue;
        }
       Sem_DCU.acquire();
       double dt = pSys->dt;
       for (int i = 0; i < kRuntimeModel; i++)
           {
               if (!g_RuntimeModels[i])
                   continue;
               g_RuntimeModels[i]->StepT(dt);
           }
        Sem_Model.release();
    }
    } catch (...)
    {
    SysMSG ( "Работа РСУ закончилась аварийно" );
    }
SysMSG ( "Работа РСУ закончилась" );
}
