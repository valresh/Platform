#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "./Sheme/showsheme.h"
#include <QTimer>
#include <QLibrary>
#include "CommProc.h"
#include "Start.h"
#include "Err.h"
#include "Prop.h"
#include "CommProc.h"
#include <QMessageBox>
#include "listemf.h"
#include <unistd.h> // Для getpid()
#include "qtrends.h"
#include "minitrend.h"
#include "SysDataTypes.h"
#include "DB.h"

//extern Q_DECL_IMPORT PosErrInFile PosErr;

void Test();
const char * MainWindow::StateRead()
{
  static char Txt[256];
  strcpy( Txt, ui->StateRead->text().toStdString().c_str() );
  return Txt;
}
const char * MainWindow::StateWrite()
{
  static char Txt[256];
  strcpy( Txt, ui->StateWrite->text().toStdString().c_str() );
  return Txt;
}
const char * MainWindow::ParamsRead()
{
  static char Txt[256];
  strcpy( Txt, ui->ParamsRead->text().toStdString().c_str() );
  return Txt;
}
const char * MainWindow::ParamsWrite()
{
  static char Txt[256];
  strcpy( Txt, ui->ParamsWrite->text().toStdString().c_str() );
  return Txt;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , Find(this)
    , Msgs ( this )
    , ui(new Ui::MainWindow)
{
    try
    {
      pMainWnd = this;
      // Установка значения
//      settings->setValue("section/size", value);
      // Считывание значения
 //     int value = settings->value("section/size", default);
      InitErr( this );
      CLEAR(main_cpu)
      CLEAR(Set_cpu)
      Max_N_cpu = 0;
      Step_cpu = 0;
      //
      setWindowIcon( QIcon(":/monitor.png"));
      setWindowIcon( QIcon("monitor.png"));
      ProcInfo();
      ui->setupUi(this);
      Msgs.Add("#Старт");
      strcpy ( PROJECT, "319_VSB_KF" );
      ui->Project->setText( PROJECT );
      ui->StateRead->setText( DB::GetChar( DB::_("Система", "Чтение состояния"), "T" ));
      ui->StateWrite->setText( DB::GetChar( DB::_("Система", "Запись состояния"), "T" ));
      ui->ParamsRead->setText( DB::GetChar( DB::_("Система", "Чтение параметров"), "N" ));
      ui->ParamsWrite->setText( DB::GetChar( DB::_("Система", "Запись параметров"), "T" ));
      ui->StepStop->setText( DB::GetChar( DB::_("Система", "Шаг останова"), "0" ));
      ObjectLoaded = false;
      SetPaths();
      listshem.SetList( PROJECT_ROOT );
      listtrends.SetList( PROJECT_ROOT );
      bool Res = PROPS.LoadProp();
        if ( !Res )
          QMessageBox::information ( this, "Ошибка", "Настройки не загрузились" );
        pModelDLL = new CCSVData();
      //  char Root[1024];
      //  sprintf ( Root, "%sINI", PROJECT_ROOT );
      int ResLoad = pModelDLL->Load ( 6, PROJECT_ROOT, "ModelDLL", "" );
      pModelDLL->Sort( );
      pStart = NULL;
      timer = new QTimer ( this );
      connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);
      connect(&listshem, &ListShem::ShowSheme, this, &MainWindow::Show_Sheme, Qt::DirectConnection);
      connect(&Find, &FindObj::ShowSheme, this, &MainWindow::Show_Sheme, Qt::DirectConnection);
      connect(&listtrends, &ListTrends::ShowTrend, this, &MainWindow::ShowTrend, Qt::DirectConnection);
      InitMes( );
      timer->start( 1000 );
      pHydro = NULL;
      KKK();
      //  minitrend * pM = new minitrend();
      //  pM->show();
      set_thread_affinity( 0 );
      time = QTime::currentTime();
      //        on_Start_clicked();
    }
    catch ( const char * error )
    {
        KKK();
        SysMsgErr("Ошибка при старте в %s(%d)", PosErr.File, PosErr.Line );
        Msgs.Add ( "Конец" );
        Msgs.show();
    }
    catch ( ... )
    {
        SysMsgErr("Ошибка при старте" );
        Msgs.Add ( "Конец" );
        Msgs.show();
        KKK();
    }
    //Qt::BlockingQueuedConnection
}

int ThreadsStopped = 0;

MainWindow::~MainWindow()
{
    //  delete ui;
//    StartWork.quit();
//    StartWork.wait();
    ThreadsStopped = 0;
    Stop = true;
    while ( ThreadsStopped < 3 )
      {
      sleep ( 1 );
      }
//    ModelsWork.quit();
//    ModelsWork.wait();
//    HydroWork.quit();
//    HydroWork.wait();
//    DCUWork.quit();
//    DCUWork.wait();

}

void MainWindow::updateTime()
{
    // for ( int n = 0; n < kSH; n++ )
    // {
    //   pSHm[n]->updateTime();
    // }
    //  PD.SetSys();
    Step_cpu++;
    if ( Step_cpu >= 10 )
        ProcInfo();
    WinList::UpdateAllTime();
    PD.SetCPU( main_cpu[0].N, main_cpu[1].N, main_cpu[2].N);
    char Txt[128];
    Sprintf ( Txt, "%4.2lf%%", PD.SysProc[0] );
    ui->cpu0->setText( Txt );
    Sprintf ( Txt, "%4.2lf%%", PD.SysProc[1] );
    ui->cpu1->setText( Txt );
    Sprintf ( Txt, "%4.2lf%%", PD.SysProc[2] );
    ui->cpu2->setText( Txt );
    Sprintf ( Txt, "%d", main_cpu[0].N );
    ui->cpu_0->setText( Txt );
    Sprintf ( Txt, "%d", main_cpu[1].N );
    ui->cpu_1->setText( Txt );
    Sprintf ( Txt, "%d", main_cpu[2].N );
    ui->cpu_2->setText( Txt );
    Sprintf ( Txt, "%4.2lf МБ", PD.Mem );
    ui->mem_sys->setText( Txt );
    Sprintf ( Txt, "%4.2lf%%", Use_Mem());
    ui->mem_int->setText( Txt );
    int sec = time.secsTo( QTime::currentTime() );
    sprintf ( Txt, "%3.1lfмин", (double)sec / 60. );
    ui->Time->setText( Txt );

/*
read -r _ user nice system idle iowait irq softirq steal guest guest_nice < /proc/stat
                                                                                 prev_idle=$((idle + iowait))
  prev_total=$((user + nice + system + idle + iowait + irq + softirq + steal))

      sleep 1

# Считываем значения второй раз
      read -r _ user nice system idle iowait irq softirq steal guest guest_nice < /proc/stat
          idle=$((idle + iowait))
  total=$((user + nice + system + idle + iowait + irq + softirq + steal))

# Рассчитываем разницу
  diff_idle=$((idle - prev_idle))
  diff_total=$((total - prev_total))
  usage=$((100 * (diff_total - diff_idle) / diff_total))
*/}


void MainWindow::ProcInfo()
    {
    int kProc = sysconf (_SC_NPROCESSORS_ONLN);
    KKK();
    CPU  cpu[32];// = new   CPU[kProc];
    char Txt[1024];
    FILE * F = fopen ( "/proc/cpuinfo", "rt" );
    int n = -1;
    while ( fgets ( Txt, 1024, F ) )
    {
        if ( strncmp ( Txt, "processor", 9 ) == 0 )
        {
            n++;
            char * P = strchr ( Txt, ':');
            int N = atoi ( P + 1 );
            ASS( N == n )
            cpu[n].N = N;
            continue;
        }
        if ( strncmp ( Txt, "core id", 7 ) == 0 )
        {
            char * P = strchr ( Txt, ':');
            cpu[n].CoreId = atoi ( P + 1 );
            continue;
        }
        if ( strncmp ( Txt, "cpu MHz", 7 ) == 0 )
        {
            char * P = strchr ( Txt, ':');
            cpu[n].MHz = atof ( P + 1 );
            continue;
        }
    }
    fclose ( F );
    // int n0 = 0;
    // for ( int n = 0; n < kProc; n++ )
    //     {
    //     if ( cpu[n].MHz > cpu[n0].MHz )
    //         n0 = n;
    //     }
    // int n1 = n0 + 2;
    // if ( n1 >= kProc )
    //     n1 -= kProc;
    // for ( int n = 0; n < kProc; n++ )
    //     {
    //     if ( n != n0 && cpu[n].MHz > cpu[n1].MHz )
    //         n1 = n;
    //     }
    // int n2 = n1 + 2;
    // if ( n2 >= kProc )
    //     n2 -= kProc;
    // for ( int n = 0; n < kProc; n++ )
    //     {
    //     if ( n != n0 && n != n1 && cpu[n].MHz > cpu[n2].MHz )
    //         n2 = n;
    //     }
    int n0 = 3;
    int n1 = 1;
    int n2 = 0;

    main_cpu[0] = cpu[n2];
    main_cpu[1] = cpu[n0];
    main_cpu[2] = cpu[n1];
    Set_cpu[0] = false;
    Set_cpu[1] = true;
    Set_cpu[2] = true;
    set_thread_affinity( main_cpu[0].N);
    Step_cpu = 0;
    Max_N_cpu = n0;
    if ( n1 > Max_N_cpu)
        Max_N_cpu = n1;
    if ( n2 > Max_N_cpu)
        Max_N_cpu = n2;
    KKK();
}


void MainWindow::ClosedShema( ShowSheme * I )
{
}

void MainWindow::Show_Sheme  ( const char * Sheme,  const char * Selected  )
{
    if ( !ObjectLoaded )
        return;
    ShowSheme * pS = new ShowSheme ( NULL, Sheme,   Selected  );
    pS->pMainWnd = this;
    //  connect( pS, &ShowSheme::Closed, this, &MainWindow::ClosedShema, Qt::AutoConnection );
    //  pSH->SetCrd( "/mnt/Projects/319_VSB_KF/DATA/SCHEME/A-3001_Е-3015.crd" );
    pS->show();
    // char Path[1024];
    // sprintf ( Path, "%sDATA/SCHEME/%s.emf", PROJECT_ROOT, Sheme );
    // pSH->loadEmfFile(QString(Path));
}

void MainWindow::ShowTrend ( const char * Trend )
{
    if ( !ObjectLoaded )
        return;
    QTrends * pT = new QTrends ( NULL, Trend );
    pT->show();
}

void MainWindow::start()
{

}

void MainWindow::stop()
{
}

void MainWindow::started()
{

}

void MainWindow::stopped()
{
    //    Msgs.Add ( "Конец" );
    //   Msgs.show();

}

void MainWindow::loaded()
{
    ObjectLoaded = true;
    WinList::Restore( this );
    int sec = time.secsTo( QTime::currentTime() );
    char Txt[64];
    sprintf ( Txt, "#Проект загружен %dс", sec );
    ui->Msg->setText( Txt );
    // Show ( "A-3001_Е-3015" );
}

void MainWindow::on_Start_clicked()
{
  DB::Set( DB::_("Система", "Чтение состояния"), StateRead() );
  DB::Set( DB::_("Система", "Запись состояния"), StateWrite() );
  DB::Set( DB::_("Система", "Чтение параметров"), ParamsRead() );
  DB::Set( DB::_("Система", "Запись параметров"), ParamsWrite() );
  DB::Set( DB::_("Система", "Шаг останова"), STR(ui->StepStop->text()));
  //
  int StepStop = atoi (STR(ui->StepStop->text()));
  if ( StepStop >= 0 )
    {
      Pause = true;
    SysSteps = StepStop;
    }
  time = QTime::currentTime();
  StartWork.setStackSize(10000000);
  pStart = new Start();
  pStart->pMainWnd = this;
  pStart->moveToThread(&StartWork);
  connect(&StartWork,&QThread::started,  pStart, &Start::start, Qt::QueuedConnection);
  //
  connect(&ModelsWork,&QThread::started,  pStart, &Start::start, Qt::QueuedConnection);
  connect(pStart, &Start::started,   this, &MainWindow::started, Qt::QueuedConnection);
  connect(pStart, &Start::stopped,   this, &MainWindow::stopped, Qt::QueuedConnection);
  connect(pStart, &Start::loaded,   this, &MainWindow::loaded, Qt::QueuedConnection);
  connect(pStart, &Start::stopped,   &ModelsWork,   &QThread::quit,   Qt::QueuedConnection);
  //  connect(pStart, &Start::stopped,   p_worker, &Worker::deleteLater,      Qt::DirectConnection);
  //  connect(Models, &QThread::finished, thread,   &QThread::deleteLater,     Qt::DirectConnection);
  ModelsWork.setStackSize(10000000);
  pModels = new Models();
  pModels->moveToThread(&ModelsWork);
  connect(&ModelsWork,&QThread::started,  pModels, &Models::start, Qt::QueuedConnection);
  //
  HydroWork.setStackSize(10000000);
  pHydro = new Hydro();
  pHydro->moveToThread(&HydroWork);
  connect(&HydroWork,&QThread::started,  pHydro, &Hydro::start, Qt::QueuedConnection);
  //
  DCUWork.setStackSize(10000000);
  pDCU = new DCU();
  pDCU->moveToThread(&DCUWork);
  connect(&DCUWork,&QThread::started,  pDCU, &DCU::start, Qt::QueuedConnection);
  //
  StartWork.start();
  ui->Msg->setText( "Загрузка ..." );
  //  pStart->start();
}

bool MainWindow::Use_RSU()
{
    if ( ui->Use_RSU->checkState() == Qt::Checked )
        return true;
    return false;
}

bool MainWindow::TS()
{
    if ( ui->Tune_Shem->checkState() == Qt::Checked )
        return true;
    return false;
}

void MainWindow::on_Pause_clicked()
{
    Pause = true;
}

void MainWindow::on_Contunue_clicked()
{
    Pause = false;

}


void MainWindow::on_Shema_clicked()
{
    if ( !ObjectLoaded )
        return;
    listshem.show();

}


void MainWindow::on_Trends_clicked()
{
    if ( !ObjectLoaded )
        return;
    listtrends.show();
    KKK();
    //  Trends * pTrends = new Trends ( NULL, NULL );
    //  pTrends->show();
}


void MainWindow::on_Messages_clicked()
{
    Msgs.show();
}


void MainWindow::on_Settings_clicked()
{
//    if ( !ObjectLoaded )
//        return;
}

void MainWindow::ShowSteps ( int kStep )
{
    char Txt[128];
    sprintf_s ( Txt, 128, "%d", kStep );
    ui->kStep->setText( Txt );
}
void MainWindow::ShowAccel ( double Accel )
{
    char Txt[128];
    sprintf_s ( Txt, 128, "%3.2lf", Accel );
    ui->Accel->setText( Txt );
    sprintf_s ( Txt, 128, "%3.0lf", pSys->Speed );
    ui->Speed->setText( Txt );
}

void MainWindow::on_exit_triggered()
{
    QCoreApplication::exit(0);
}

void MainWindow::OutTxt( const char * Txt )
{
    if ( Txt[0] == '#' )
        ui->Msg->setText( Txt + 1 );
    else
        ui->Msg->setText( Txt );

    Msgs.Add ( Txt );
}

void MainWindow::OutMsg( const char * Title, const char * Txt,
                        DWORD flags,
                        DWORD def,
                        int * Res )
{
    *Res = QMessageBox::critical( NULL, Title, Txt, (QMessageBox::StandardButtons)flags, (QMessageBox::StandardButton)def );
}


void MainWindow::on_AccelZ_returnPressed()
{
    // QString S = ui->AccelZ->text();
    // double Acc = S.toDouble();
    // emit SetAccel ( Acc );
}

void MainWindow::ShowData_1 ( double ProcUser, int Mem )
{
    // char Txt[128];
    // Sprintf ( Txt, "%4.2lf%%", PD.ProcUer );
    // ui->CPU_2->setText( Txt );
    // Sprintf ( Txt, "%d", PD.Mem );
    // ui->MEM_2->setText( Txt );

}
void MainWindow::ShowData_2 ( double ProcUser, int Mem )
{
    // char Txt[128];
    // Sprintf ( Txt, "%4.2lf%%", PD.ProcUer );
    // ui->CPU_3->setText( Txt );
    // Sprintf ( Txt, "%d", PD.Mem );
    // ui->MEM_3->setText( Txt );

}

extern bool SaveParams;
extern bool SaveState;

void MainWindow::on_save_params_clicked()
{
    SaveParams = true;
}


void MainWindow::on_save_state_clicked()
{
    SaveState = true;
}


void MainWindow::on_save_all_clicked()
{
    SaveParams = true;
    SaveState = true;
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    QCoreApplication::exit(0);

}



void MainWindow::on_FindObj_clicked()
{
    Find.exec();
    int Res = Find.result();
    KKK();
}


void MainWindow::on_RSU_clicked()
{
    Init_RSU();
    showRSU.Init();
    showRSU.Go();

}


void MainWindow::on_Stop_clicked()
{
  QCoreApplication::exit(0);

}

