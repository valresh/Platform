#include "Start.h"
#include "BaseModel.h"
#include "CommProc.h"
#include <QLibrary>
#include "Err.h"
#include "Prop.h"
#include "DataTypes.h"
#include "SysDataTypes.h"
#include <unistd.h>
#include <QSemaphore>
#include "mainwindow.h"
#include "SetDataTypes.h"

IBaseModel * pCtrlConn;
extern  QSemaphore Sem_Model;
IBaseModel *g_RuntimeModels[eRM_COUNT] = { NULL };
int kRuntimeModel = 0;

void LoadRsuModelsCP(int& iRuntimeModel);

Start::Start(QObject* parent )
{

}

Start::~Start()
{
}


Models::Models(QObject* parent )
{

}

Models::~Models()
{
}

void Models::start()
{
    Go();
}


Hydro::Hydro(QObject* parent )
{

}

Hydro::~Hydro()
{
}


void Hydro::start()
{
    Go();
}


DCU::DCU(QObject* parent )
{

}

DCU::~DCU()
{
}



void DCU::start()
{
    Go();
}


bool WasStarted = false;
void Start::start()
{
    if ( WasStarted )
    {
        sleep(1);
        return;
    }
    WasStarted = true;
    emit started();
    if ( !Prepare())
    {
        //  emit stopped();
        return;
    }
    //
    emit loaded();
    // while ( !Stop )
    //   {
    //   sleep(1);
    //   KKK();
    //   }
    InitWork();
    Pause = false;
    pMainWnd->ModelsWork.start();
    pMainWnd->HydroWork.start();
    pMainWnd->DCUWork.start();
    Sem_Model.release(2);
    pMainWnd->StartWork.quit();
    KKK();
}



void Start::stop ()
{
    emit stopped();
}

int Q_DECL_IMPORT RestoreParamsFromFile(LPCTSTR szFilePath);
int Q_DECL_IMPORT RestoreStateFromFile ( LPCTSTR File_Path );

bool Start::Load()
{
    // QString Path = EXE_PATH;
    // Path += "/libUniHydro.so";
    // QLibrary Lib(Path);
    // bool Res = Lib.load();
    // if ( !Res )
    // {
    //  QString Err = Lib.errorString();
    //  return false;
    // }
    // tCreateObject pCreateObj = (tCreateObject)Lib.resolve("CreateObject");
    // if ( pCreateObj == NULL )
    //   return false;
    // IBaseModel * pMainHydro = (*pCreateObj)( "Гидравлика", "" );
    // int ResStart = pMainHydro->Init( 0 );
    return true;
}

IBaseModel * LoadStdLib ( const char * Dll, const char * Name )
{
    IBaseModel * pLib = Load_Object( Dll, Name );
    if (!pLib)
    {
        SysMSG( "Ошибка загрузки библиотеки '%s'", Name );
        //      Critical( "Ошибка загрузки библиотеки '%s'", Name );
        return NULL;
    }
    //  pLib->Init( 0 );
    return pLib;
}

bool Start::Prepare()
{
    if ( !PROPS.ReadProp())
    {
//    CLogFile::Log ("Start 9: !PROPS.ReadProp");
        SysMSG ( "Ошибка чтения настроек" );
    }
    char Project_Name[64];
    char Project_Root[64];
    PROPS.Get( "Проект", Project_Name );
//    ShortParamsList = true;
//    PROPS.Get(PROPKEY_SHORT_PRMLIST, ShortParamsList);
//    pSys = new CSystem;//(CSystem*)CreateStruct( 1, "Система" );;
    PROPS.Get( "Корневая папка", Project_Root );
    PROPS.Get( "Температура окружающей среды", T_air );
    PROPS.Get( "Минимальная температура", T_min );
    PROPS.Get( "Максимальная температура", T_max );
    extern double gMaxNew_dt;
    PROPS.Get( "Начальный шаг по времени", gMaxNew_dt );
    ////
    GiveCommData( "Условия", CExternalCondition::TypeID, (void**)&pExt );
    GiveCommData( "Модель", CSystem::TypeID, (void**)&pSys );
    pSys->Init();
    pSys->WaterIAPWS = false;
    PROPS.Get( "Вода по IAPWS", pSys->WaterIAPWS );
    IBaseModel * pChem = LoadStdLib ( "Chem", "Химия" );
    pChem->Init(0);
    UseAlt_CompNames = false;
    PROPS.Get ( "Альтернативные", UseAlt_CompNames );
    IBaseModel * pData = LoadStdLib( "Data", "Данные" );
    pData->Init(0);
    IBaseModel * pTrend = LoadStdLib( "Trends", "Тренды" );
    pCtrlConn = LoadStdLib("CtrlConn", "Управление");
    IBaseModel * pACS = LoadStdLib( "ACS", "ACS");
    IBaseModel * pCtrlReg = LoadStdLib( "CtrlReg", "Регуляторы");
    //////////////////////////////////////////////
    ///
    SysMSG ( "#Загрузка проекта" );
    pMainHydro = Load_Object( "UniHydro", "Гидравлика" );
    if ( pMainHydro == nullptr )
        {
        SysMSG( "Ошибка загрузки UniHydro" );
        return __LINE__;
        }
    int Res = pMainHydro->Init( 0 );
    if ( Res )
        {
        SysMSG( "Ошибка загрузки моделей" );
        //    Critical( "Ошибка загрузки моделей" );
        THROW
        return __LINE__;
        }
    SysMSG ( "#Инициализация объектов" );
    // bool bOG = false;
    // PROPS.Get(PROPKEY_GAS_CONTAMIN, bOG );
    // if( bOG )
    //  {
    //   Load_Object("OGCtrl", "OG", "OG");
    //   }
    pExt->Init();
    // pExt->T_Air = 15.0;	// Температура окружающего воздуха//-параметр(2)
    // pExt->PressAirKip = 7.0;//Давление воздуха КИП
    // pExt->WorkUPS = true;
    // pExt->WorkDCS = true;
    // pExt->IsElectro_380 = true;
    // pExt->IsElectro_6000 = true;
    // pExt->IsPumpSbros = true;
    // pExt->IsBlk = false;
    // pExt->CtrlMinMax = true;
    // pExt->IsEmptyVol = true;
    // //
    pSys->m_nStep = 0;
    // pSys->StartStatus = CSystem::STATUS_INIT;
    // bool Blk = false;
    // PROPS.Get( PROPKEY_BLOCKAGES, Blk );
    // pExt->IsBlk = Blk;
    // pSys->IsBlk = Blk;
    ////////////////////////////////////////
    kRuntimeModel = 0;
    if ( pMainWnd->Use_RSU())
    {
        SysMSG ( "#Загрузка РСУ" );
        LoadRsuModelsCP( kRuntimeModel );
        //pTrend->Init(0);
        //
        SysMSG ("#Инициализация РСУ");
        for(int i=0; i<kRuntimeModel; i++)
        {
            try
            {
                //            CLogFile::Log ("Start_30 i %d, ObjName %s", i, g_RuntimeModels[i]->ObjName);
                g_RuntimeModels[i]->Init(0);
            }
            catch(...)
            {
                KKK();
                //            CLogFile::Log ("Start_31 Error: i %d", i);
            }
        }
    }
    SysMSG ("#Инициализация рабочих моделей");
    IBaseModel * pObj = IBaseModel::pFirst;
    int kIter = 0;
    while ( pObj )
    {
        if ( pObj->ModelFlags & Flag_NoInit )
        {
            pObj = pObj->pNext;
            continue;
        }
        kIter++;
        pObj->Init( 3 );
        pObj = pObj->pNext;
    }
    IBaseModel::SetObjectsList();
    SysMSG ("#Чтение параметров");
    char Path[1024];
    sprintf ( Path, "%sDATA/STATES/%s.parm", PROJECT_ROOT, pMainWnd->ParamsRead());     int ResParm = RestoreParamsFromFile( Path );
    //
    bool Err = false;
    SysMSG( "#Начальный шаг моделей объектов" );
    for ( int n = 0; n < IBaseModel::kObjects; n++ )
        {
        IBaseModel * pModel = IBaseModel::AllObjects[n];
        if ( pModel == NULL )
            continue;
        if ( pModel->ModelFlags & Flag_NoStep0 )
            continue;
        if ( pModel->Step0( ))
            {
            SysMSG( "Ошибка на начальном шаге объекта '%s'('%s')",
                   (char*)pModel->ObjName, (char*)pModel->Model );
            //      Critical( "Ошибка на начальном шаге объекта '%s'('%s')",
            //               (char*)pModel->ObjName, (char*)pModel->Model );
            Err = true;
            }
        }
    if(Err)
        return __LINE__;
    SysMSG ("#Чтение состояния");
    char PathState[1024];
    sprintf ( PathState, "%sDATA/STATES/%s.dat", PROJECT_ROOT, pMainWnd->StateRead() );
    int ResState = RestoreStateFromFile( PathState );
    SysMSG( "#Первый шаг моделей объектов" );
    for ( int n = 0; n < IBaseModel::kObjects; n++ )
        {
        IBaseModel * pModel = IBaseModel::AllObjects[n];
        if ( pModel == NULL )
            continue;
        if ( pModel->ModelFlags & Flag_NoStep1 )
            continue;
        if ( pModel->Step1( ))
            {
            SysMSG( "Ошибка на первом шагe объекта '%s'('%s')",
                   (char*)pModel->ObjName, (char*)pModel->Model );
            //      Critical( "Ошибка на первом шагe объекта '%s'('%s')",
            //               (char*)pModel->ObjName, (char*)pModel->Model );
            Err = true;
            }
        }
    if(Err) return false;
    if( SysErrors > 0 )
        {
        SysMSG(  "Загрузка остановлена из-за ошибок" );
        return false;
        }
    SysMSG( "#Модель работает" );
//
    pCtrlConn->SetData( sd_SetConnections, &pMainWnd->showRSU.pConn_Info );
    KKK();
    return true;
    }

IBaseModel * Load_ObjectEx ( const char * DllName, const char * ObjName )
{
    char Dll[256];
    strcpy ( Dll, DllName );
    char * Path = strchr(Dll, '(');
    if (Path )
        {
        *Path = 0;
        // Path++;
        // char * E = strchr(Path, ')');
        // if(E)
        //     *E = 0;
        // HANDLE hLib = Load_Dll(Dll,Path);
        // if(hLib == NULL) return NULL;
        // tCreateObject pCreateObject = (tCreateObject)::GetProcAddress((HMODULE)hLib, "CreateObject");
        // if(pCreateObject == NULL) return NULL;
        // return (*pCreateObject)(ObjName, NULL );
        }
    return Load_Object( Dll, ObjName, NULL);
}
