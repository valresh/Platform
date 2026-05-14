#if 0
#include <chrono>
#include <fstream>
#include <filesystem>
#include "Start.h"
#include "Interface/HydroBasicTypes.h"
#include "Interface/HydroGroupsInterface.h"
#include "Connect.h"
#include "Work.h"
#ifdef _WIN32
#include "IO.h"
#endif
#include "stdlib.h"
#include "Info.h"
#include "SetData.h"
#include "SysDataTypes.h"
#include "Names.h"
#include "Prop.h"
#include "time.h"
#include "Control.h"
#include "Resume.h"
#include "Common.h"

#include <rsus/rsu_basemodel.h>
#include "LogFile.h"

#include <crosspath.h>
#include "crossstring.h"
#include "crossfilesystem.h"
#include "FaultService.h"

//bool Release = false;
#ifdef _WIN32
__declspec(dllexport) IBaseModel* pMainHydro = NULL;
__declspec(dllexport) HydroGroupsInterface* HydroGroups = NULL;

HWND hInstrWnd = NULL;

#else
__attribute__((visibility("default"))) IBaseModel* pMainHydro = NULL;
__attribute__((visibility("default"))) HydroGroupsInterface* HydroGroups = NULL;
#endif

IBaseModel* pData = NULL;
IBaseModel* pChem = NULL;
IBaseModel* pFA = NULL;
IBaseModel* pTrend = NULL;
IBaseModel* pCtrlConn = NULL;
IBaseModel* pCtrlCopy = NULL;
IProgress* pProgress;
bool g_bAutoSave = false;
#ifdef _WIN32
HWND gblhwndMain = NULL;
HWND g_hwndOwner = NULL;
#endif
std::chrono::high_resolution_clock::time_point ticks;//Debug
int CompObjRef(const void* p1, const void* p2);
int CompPipeRef(const void* p1, const void* p2);
//IBaseModel * LoadModel( CObjPoint & OP );
void WorkMsg(const char* pszMsg, int Line);

void ReadConnections();
//
void ReadParamsFile(char* File);
void ReadParamsList(char* List);
void ReadStateFile(char* File);
void ReadStateList(char* List);
void WriteParamsFile(char* File);
void WriteParamsList(char* List);
void WriteStateFile(char* File);
void WriteStateList(char* List);
int GetAllConnections(char* ObjName, char* PntName, char& SD, char* RSU[16], char* Desc[16]);

IBaseModel* g_RuntimeModels[eRM_COUNT] = { NULL };
IBaseModel* pControl = NULL;

bool bSendOnCopyData = false;

void LoadRsuModelsCP(int& iRuntimeModel);
void SetModelProps(IBaseModel* pModel);

static rsu_cp::RsuClientsHolder g_RsuHolder;
fs::path ProjectModelPath()
{
    return fs::path(PROJECT_ROOT) / "DATA" / "Acy" / "Models.csv";
}

void TestStruct()
{
    return;
    char Class[256];
    char ObjName[256];
    GetName(15360, Class, 255, ObjName, 255);
    ////////////////////////////
    return;
    const char* pszName;
    CBase* pBase;
    UINT ID = 0;
    int fcsNumber = 0;
    int k = 0;
    while (WhileBase(ID, id_Unknown, &pBase, &pszName, &fcsNumber))
    {
        k++;
        ASS(pBase->ID_CLASS < 30000)
    }
    KKK();
}

int InitDll();
bool Load_Data(char* Root);
extern bool Stop;

void START_API Terminate()
{
    using namespace std::chrono_literals;

    Stop = true;
    std::this_thread::sleep_for(1000ms);
    void EndWork();
    EndWork();
}

#if 0
extern int _CrtDbgMode[3];
extern _HFILE _CrtDbgFile[3];

_HFILE __cdecl CrtSetReportFile(
    int nRptType,
    _HFILE hFile
)
{
    _HFILE oldFile;

    oldFile = _CrtDbgFile[nRptType];
    if (_CRTDBG_FILE_STDOUT == hFile)
        _CrtDbgFile[nRptType] = GetStdHandle(STD_OUTPUT_HANDLE);
    else
        if (_CRTDBG_FILE_STDERR == hFile)
            _CrtDbgFile[nRptType] = GetStdHandle(STD_ERROR_HANDLE);
        else
            _CrtDbgFile[nRptType] = hFile;
    return oldFile;
}

int __cdecl CrtSetReportMode(
    int nRptType,
    int fMode
)
{
    int oldMode;
    oldMode = _CrtDbgMode[nRptType];
    _CrtDbgMode[nRptType] = fMode;

    return oldMode;
}
#endif

#define TRY try {
#define CATCH(Numb) } catch(...) { SysMSG ( "Прерывание в блоке %d", Numb ); Critical( "Прерывание в блоке %d", Numb ); return 1; }

/**/

IBaseModel** ppScript;
int nScript;

bool ReadScripts()
{
    int nNewScript = 0;
    auto path = std::filesystem::path(PROJECT_ROOT) / "DATA" / "SCRPT";
    if (!std::filesystem::exists(path))
        return true;
    for (const auto& entry : std::filesystem::directory_iterator(path, std::filesystem::directory_options::skip_permission_denied))
    {
        if (entry.is_regular_file())
        {
            int i;
            for (i = 0; i < nScript; i++)
            {
                if (!lstrcmp(ppScript[i]->ObjName, entry.path().filename().string().c_str()))
                    break;
            }
            if (i == nScript)
                nNewScript++;
        }
    }

    if (nNewScript != 0)
    {
        IBaseModel** ppTemp = new IBaseModel * [nNewScript];
        if (nScript != 0)
        {
            memcpy(ppTemp, ppScript, nScript * sizeof(IBaseModel*));
            IBaseModel** ppDelete = ppScript;
            delete[] ppDelete;
        }
        ppScript = ppTemp;
    }

    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_regular_file())
        {
            int i;
            for (i = 0; i < nScript; i++)
            {
                if (!lstrcmp(ppScript[i]->ObjName, entry.path().filename().string().c_str()))
                    break;
            }
            if (i == nScript)
            {
                ppScript[nScript++] = Load_Object("Script", entry.path().filename().string().c_str());
            }
        }
    }

    for (int i = 0; i < nScript; i++)
    {
        ppScript[i]->SetData(sd_CompileScripts, NULL);
    }

    return true;

}

void PrintDebugLine(char* Str)
{
    CLogFile::LogEx(LOGLEVEL_LOW, Str);
#ifdef _WIN32
    OutputDebugString(Str);
#endif
}

void PrintDebugLine(char* Str, std::chrono::high_resolution_clock::time_point tStart, std::chrono::high_resolution_clock::time_point& tStep)
{
    char buffer[100];
    auto now = std::chrono::high_resolution_clock::now();
    sprintf(buffer, "Time %s: %.2fs Step: %.2fs\n", Str, std::chrono::duration<double>(now - tStart).count(), std::chrono::duration<double>(now - tStep).count());
    PrintDebugLine(buffer);
    tStep = std::chrono::high_resolution_clock::now();
}

bool PrepareRuntimeModels()
{
    CLEAR(g_RuntimeModels)
        long nAllocNum = -1;
    bool bDebugMode = IsDebugMode(nAllocNum);
#ifdef _WIN32
    if (!bDebugMode) _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    else
    {
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
        _CrtSetBreakAlloc(nAllocNum);
    }

    Char<512>PathWarn;
    PathWarn.Prt("%sTemp/Warnings.txt", PROJECT_ROOT);
    HANDLE hFile = CreateFile(PathWarn, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
    if (!bDebugMode)
        _CrtSetReportFile(_CRT_WARN, hFile);
#endif
    //
    pContinue = Continue;
    pPause = Pause;
    if (!PROPS.ReadProp())
    {
        SysMSG("Ошибка чтения настроек");
    }
    char Project_Name[64];
    char Project_Root[64];
    PROPS.Get(PROPKEY_PROJECT, Project_Name);
    PROPS.Get(PROPKEY_ROOT_DIR, Project_Root);
    ShortParamsList = true;
    PROPS.Get(PROPKEY_SHORT_PRMLIST, ShortParamsList);
    PROPS.Get("Температура окружающей среды", T_air);
    double* pT_min = &T_min;
    PROPS.Get("Минимальная температура", T_min);
    PROPS.Get("Максимальная температура", T_max);
    extern double gMaxNew_dt;
    PROPS.Get("Максимальный шаг по времени", gMaxNew_dt);
    //  PROPS.Get( "Максимальное давление", CReal::Max_Pz );
      //  bool MaxRelease = true;
    //  PROPS.Get( "Максимальная версия", MaxRelease );
    //  if ( MaxRelease )
    //    SetMaxRelease();
    //  INI_C ( "Проект", "Проект", "SYSTEM", Project_Name );
    //  INI_C ( "Проект", "Корневая папка", "SYSTEM", Project_Root );
    //
    //  pMainHydro->SetData ( sd_InitHydro, Project_Root );
    if (pMainHydro == NULL)
    {
        int Res = InitDll();
        if (Res)
        {
            SysMSG("Ошибка инициализации моделей");
            Critical("Ошибка инициализации моделей");
            return false;
        }
    }

    return true;
}

int LoadLibraries()
{
    GiveCommData("Модель", CExternalCondition::TypeID, (void**)&pExt);
    GiveCommData("Модель", CSystem::TypeID, (void**)&pSys);
    pSys->WaterIAPWS = false;
    PROPS.Get(PROPKEY_WATER_IAPWS, pSys->WaterIAPWS);
    bool r = pSys->WaterIAPWS;
    //WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
    pChem = Load_Object("Chem", "Химия", "PROJECT");
    if (!pChem)
    {
        pChem = Load_Object("Chem", "Химия");
        if (!pChem)
        {
            SysMSG("Ошибка загрузки термодинамической библиотеки");
            Critical("Ошибка загрузки термодинамической библиотеки");
            return __LINE__;
        }
    }
    int Res = pChem->Init(0);
    if (Res)
    {
        SysMSG("Ошибка %d загрузки Chem library", Res);
        Critical("Ошибка %d загрузки Chem library", Res);
        return __LINE__;
    }
    UseAlt_CompNames = false;
    PROPS.Get("Альтернативные", UseAlt_CompNames);
    //WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
    pData = Load_Object("Data", "Данные", "PROJECT");
    if (!pData)
    {
        pData = Load_Object("Data", "Данные");
        if (!pData)
        {
            SysMSG("Ошибка загрузки библиотеки хранения данных");
            Critical("Ошибка загрузки библиотеки хранения данных");
            return __LINE__;
        }
    }
    pData->Init(0);
    pTrend = Load_Object("Trends", "Тренды", "PROJECT");
    if (!pTrend)
    {
        pTrend = Load_Object("Trends", "Тренды");
        if (!pTrend)
        {
            SysMSG("Ошибка загрузки библиотеки трендов");
            Critical("Ошибка загрузки библиотеки трендов");
            return __LINE__;
        }
    }
    //WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
    pCtrlConn = Load_Object("CtrlConn", "Управление", "PROJECT");
    if (!pCtrlConn)
    {
        pCtrlConn = Load_Object("CtrlConn", "Управление");
        if (!pCtrlConn)
        {
            SysMSG("Ошибка загрузки библиотеки связей");
            Critical("Ошибка загрузки библиотеки связей");
            return __LINE__;
        }
    }
    pCtrlConn->Init(0);
    char Project_Root[64] = {};
    PROPS.Get(PROPKEY_ROOT_DIR, Project_Root);
    char* pChar = strrchr(Project_Root, std::filesystem::path::preferred_separator);
    lstrcpy(Project_Root, ++pChar);
    pChar = strchr(Project_Root, '_');
    if (pChar) lstrcpy(Project_Root, ++pChar);
    //WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
    IBaseModel* pACS = NULL;
    pACS = Load_Object("ACS", "ACS", "PROJECT");
    if (!pACS)
    {
        pACS = Load_Object("ACS", "ACS");
        if (!pACS)
        {
            SysMSG("Ошибка загрузки библиотеки ACS");
            Critical("Ошибка загрузки библиотеки ACS");
            return __LINE__;
        }
    }
    IBaseModel* pCtrlReg = NULL;
    pCtrlReg = Load_Object("CtrlReg", "Регуляторы", "PROJECT");
    if (!pCtrlReg)
    {
        pCtrlReg = Load_Object("CtrlReg", "Регуляторы");
        if (!pCtrlReg)
        {
            SysMSG("Ошибка загрузки библиотеки CtrlReg");
            Critical("Ошибка загрузки библиотеки CtrlReg");
            return __LINE__;
        }
    }
    //WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
    ReadScripts();
    SysMSG("#Сборка гидравлики");
    Res = pMainHydro->Init(0);
    if (Res)
    {
        SysMSG("Ошибка загрузки моделей");
        Critical("Ошибка загрузки моделей");
        return 2;
    }
    SysMSG("#Инициализация объектов");
    bool bOG = false;
    PROPS.Get(PROPKEY_GAS_CONTAMIN, bOG);
    if (bOG)
    {
        Load_Object("OGCtrl", "OG", "OG");
    }
    return 0;
}

void InitHydraulics()
{
    PROGRESS("Инициализация гидравлики")
        /*
          IUniModel::kUniObjects = 0;
          pMainHydro->SetData ( sd_CalcGydroCount, &IUniModel::kUniObjects );

          if ( IUniModel::kUniObjects > 0 )
            {
            IUniModel::UniObjects = new IBaseModel*[IUniModel::kUniObjects];
            memset ( IUniModel::UniObjects, 0, 4 * IUniModel::kUniObjects );
            pMainHydro->SetData ( sd_SetAllGydroObj, &IUniModel::UniObjects );
            }
          else
            IUniModel::UniObjects = NULL;
        */
}

void LoadBaseModels(int OLD_CALC_MODE)
{
    PROGRESS("Загрузка базовых моделей")
        //
        //
        //  if ( IUniModel::kUniObjects > 0 )
        //    qsort( IUniModel::UniObjects, IUniModel::kUniObjects, sizeof(DWORD_PTR), CompIBaseModel );
        //
        ////////////////////////////////////////
        ////////////////////////////////////////

        pExt->T_Air = 15.0;	// Температура окружающего воздуха//-параметр(2)
    pExt->PressAirKip = 7.0;//Давление воздуха КИП
    pExt->WorkUPS = true;
    pExt->WorkDCS = true;
    pExt->IsElectro_380 = true;
    pExt->IsElectro_6000 = true;
    pExt->IsPumpSbros = true;
    pExt->IsBlk = false;
    pExt->CtrlMinMax = true;
    pExt->IsEmptyVol = true;
    //
    if (!OLD_CALC_MODE)
        pSys->dt = 2e-4;
    //
    pSys->m_nStep = 0;
    pSys->StartStatus = CSystem::STATUS_INIT;
    bool Blk = false;
    PROPS.Get(PROPKEY_BLOCKAGES, Blk);
    pExt->IsBlk = Blk;
    pSys->IsBlk = Blk;
    ////////////////////////////////////////
    pTrend->Init(0);
    //WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
  /*
    pTrend = Load_Object( "Trends", "Тренды" );
    if ( pTrend == NULL )
    {
        SysMSG( "Ошибка загрузки библиотеки Trends" );
        Critical( "Ошибка загрузки библиотеки Trends" );
        return __LINE__;
    }
    pTrend->Init( 0 );
    pTrend->PreStepT();
  */
  //////////////////////////////////////////////////////////////////////////
    int iRuntimeModel = 0;
    /*
      g_RuntimeModels[iRuntimeModel] = Load_Object( "Trends", "Тренды"  );
      IBaseModel * pТренды = g_RuntimeModels[iRuntimeModel];
      if ( g_RuntimeModels[iRuntimeModel] == NULL )
        {
        SysMSG( "Ошибка загрузки библиотеки трендов" );
        Critical( "Ошибка загрузки библиотеки трендов" );
        return __LINE__;
        }
      iRuntimeModel++;
    */
    /*
      if ( IUniModel::kUniObjects > 0 )
        {
        //char DirName[256];
        //PROPS.Get ( "Корневая папка", DirName );
        //ASS( iRuntimeModel < _countof(g_RuntimeModels) );
        g_RuntimeModels[iRuntimeModel] = Load_Object( "Trends", "Тренды"  );
        IBaseModel * pТренды = g_RuntimeModels[iRuntimeModel];
        if ( g_RuntimeModels[iRuntimeModel] == NULL )
          {
          SysMSG( "Ошибка загрузки библиотеки трендов" );
          Critical( "Ошибка загрузки библиотеки трендов" );
          return __LINE__;
          }
        iRuntimeModel++;
        }
      else
        {
        pChem = NULL;
        pData = NULL;
        }
    */
    ASS(iRuntimeModel < countof(g_RuntimeModels));
    // TEMP: VM
    //return 0;
    LoadRsuModelsCP(iRuntimeModel);
}

void PrepareObjetsProperties()
{
    PROGRESS("Чтение свойств объектов")
        GetStateParmFiles();
#if 1
    char* Rd = Read_Params;
    if (Read_Params[0] != 0)
    {
        auto path = std::filesystem::path(PROJECT_ROOT) / "DATA" / "STATES" / (std::string(Read_Params) + ".propx");
        int Res = RestorePropsFromFile(path.string().c_str());
    }
#endif
    // Установка Prop's
    bool Err = false;
    SysMSG("#Задание свойств объектов");
    IBaseModel* pModel = IBaseModel::pFirst;
    IBaseModel* pLastModel = NULL;
    int n = 0;
    while (pModel)
    {
        n++;
        if (pModel->pGetProps)
            SetModelProps(pModel);
        pLastModel = pModel;
        pModel = pModel->pNext;
    }
    bool FullKlapDescr = false;
    PROPS.Get(PROPKEY_VALVE_DESC, FullKlapDescr);
    UseKlapDefault = !FullKlapDescr;
    // TEMP: VM
    //return 0;
}

void InitBaseModels()
{
    PROGRESS("Инициализация базовых моделей")
        for (int i = 0; i < countof(g_RuntimeModels); i++)
        {
            if (!g_RuntimeModels[i]) continue;
            g_RuntimeModels[i]->Init(0);
        }

    // TEMP: VM
    //return 0;
}

bool InitWorkingModels()
{
    PROGRESS("Инициализация рабочих моделей")
    IBaseModel* pModel = IBaseModel::pFirst;
    int nModel = 0;
    bool Err = false;
    for (; pModel; pModel = pModel->pNext)
    {
        BOOL skip = FALSE;
        for (int i = 0; i < countof(g_RuntimeModels); i++)
        {
            if (pModel == g_RuntimeModels[i])
            {
                skip = TRUE;
                break;
            }
            if (g_RuntimeModels[i] == NULL)
                break;
        }

        if (skip)
        {
            continue;
        }

        if (pModel == pMainHydro || pModel == pData || pModel == pChem || pModel == pTrend || pModel == pCtrlConn/*|| pModel == pMainLogic || pModel == pMainHeat  || pModel == pMainControl*/)
        {
            continue;
        }

        int RegInit = 3;
        if (pModel->TypeObj == IBaseModel::GroupModel)
            RegInit = 1;
        nModel++;
        if (pModel->Init(RegInit))
        {
            SysMSG("Ошибка инициализации объекта '%s'('%s')", pModel->ObjName.Str, pModel->Model.Str);
            Critical("Ошибка инициализации объекта '%s'('%s')", pModel->ObjName.Str, pModel->Model.Str);
            Err = true;
        }
    }

    PROGRESS(!Err ? "Models initialized successfully" : "Error: Can't initialize models");
    return !Err;
}

void SetModelObjects()
{
    IBaseModel::SetObjectsList();
    IBaseModel** ppMod = IBaseModel::AllObjects;
    int CompIBaseModel(const void* p1, const void* p2);
    qsort(IBaseModel::AllObjects, IBaseModel::kObjects, sizeof(DWORD_PTR), CompIBaseModel);
}

void ReadParameters()
{
    Monitor("Чтение параметров");
    PROGRESS("Чтение параметров")
        SysMSG("#Чтение параметров");
    //  char ParamsName[256];
    //??  GetStateParmFiles( );
    //////////////
    ReadParamsFile(Read_Params);
    ////////
    void ReadKlapans();
    ReadKlapans();
    bool ReadEquipment = false;
    PROGRESS("Чтение данных оборудования")
        PROPS.Get(PROPKEY_READ_EQUIPM, ReadEquipment);
    if (ReadEquipment)
    {
        SysMSG("#Чтение данных оборудования");
        void ReadEquipment();
        ReadEquipment();
    }
    ////////////////////////////////////////
    PROGRESS("Начальный шаг базовых моделей")
        SysMSG("#Начальный шаг моделей объектов");
    /*for( int i=0; i<_countof(g_RuntimeModels); i++ )
      {
      if( !g_RuntimeModels[i] )
        continue;
        g_RuntimeModels[i]->Step0();
      }*/
    PROGRESS("Начальный шаг рабочих моделей")
}

struct cmp_str
{
    bool operator()(CStr a, CStr b) const
    {
        return std::strcmp(a, b) < 0;
    }
};

bool ResetModel()
{
    /******
  for ( int n = 0; n < IUniModel::kUniObjects; n++ )
    {
    IBaseModel * pModel = IUniModel::UniObjects[n];
    if ( IUniModel::UniObjects[n]->Step0( ))
      {
      SysMSG( "Ошибка на начальном шаге объекта '%s'('%s')",
        IUniModel::UniObjects[n]->ObjName, IUniModel::UniObjects[n]->Model );
      Err = true;
      }
    }
/****/

    bool Err = false;
    for (int n = 0; n < IBaseModel::kObjects; n++)
    {
        IBaseModel* pModel = IBaseModel::AllObjects[n];
        if (pModel && pModel->Step0())
        {
            std::string msg = "Ошибка на начальном шаге объекта '" +
                std::string((char*)pModel->ObjName) +
                "'('" + std::string((char*)pModel->Model) + "')";
            SysMSG(msg.c_str());
            Critical("Ошибка на начальном шаге объекта '%s'('%s')",
                (char*)pModel->ObjName, (char*)pModel->Model);
            Err = true;
        }
    }
    /****/

    return !Err;
}

void ModelStateRead(bool External)
{
    int nModel = 0;
    IBaseModel* pModel = IBaseModel::pFirst;
    while (pModel)
    {
        if (pModel->pMain)
        {
            pModel->Step0();
            nModel++;
        }
        pModel = pModel->pNext;
    }
    pMainHydro->Step0();
    //PrintDebugLine("__Step0", &tStart, &tStep);
////////////////////////////////////////////////////
    Monitor("Чтение состояния");
    PROGRESS("Чтение состояния")
        ReadStateFile(ReadState);
    //PrintDebugLine("__ReadStateFile", &tStart, &tStep);
///////////////////////////////////////////////////////////////////////////
    SysMSG("#Связь объектов");
    extern int StepGO;
    PROPS.Get(PROPKEY_BREAK_STEP, StepGO);
    if (UseStepStop) StepGO = StepStop;
    if (!External)
    {
        double AccelZ = 1.0;
        PROPS.Get(PROPKEY_ACCELERATION, AccelZ);
        pSys->AccelZ = AccelZ;
    }
    //PrintDebugLine("__GetProps1", &tStart, &tStep);
/// Анализатор /////////////////////////////////////////////////////////////////////////
    bool UseControl = false;
    PROPS.Get(PROPKEY_ANALYSIS, UseControl);
    if (!UseControl)
    {
        pControl = NULL;
        IBaseControl* pCtrl = pFirstControl;
        while (pCtrl)
        {
            pCtrl->Use = false;
            pCtrl = pCtrl->pNextControl;
        }
    }
    //PrintDebugLine("__GetProps2", &tStart, &tStep);
    extern int kHydroThreads;
    kHydroThreads = 1;
    PROPS.Get(PROPKEY_HYDRO_THREADS, kHydroThreads);
    if (kHydroThreads < 1)
        kHydroThreads = 1;
    if (kHydroThreads > 4)
        kHydroThreads = 4;
    extern int kModelThreads;
    kModelThreads = 1;
    //PrintDebugLine("__GetProps3", &tStart, &tStep);
    PROPS.Get(PROPKEY_MODEL_THREADS, kModelThreads);
    if (kModelThreads < 1)
        kModelThreads = 1;
    if (kModelThreads > 3)
        kModelThreads = 3;
    //PrintDebugLine("__GetProps4", &tStart, &tStep);
}

bool BaseModelFirstStep()
{
    //  PROGRESS("Первый шаг базовых моделей" )
    Monitor("Модель запущена");
    PROGRESS("Модель запущена")
        SysMSG("#Первый шаг моделей объектов");
    TestStruct();
    bool Err = false;
    for (int n = 0; n < IBaseModel::kObjects; n++)
    {
        IBaseModel* pModel = IBaseModel::AllObjects[n];
        if (pModel && pModel->Step1())
        {
            SysMSG("Ошибка на первом шагe объекта '%s'('%s')",
                (char*)pModel->ObjName.Str, (char*)pModel->Model.Str);
            Critical("Ошибка на первом шагe объекта '%s'('%s')",
                (char*)pModel->ObjName.Str, (char*)pModel->Model.Str);
            Err = true;
        }
    }
    return !Err;
}

void StartModel(int OLD_CALC_MODE)
{
    typedef bool (*tSetConnectionsList)();
    extern IN_DLL tSetConnectionsList pSetConnectionsList;
    if (pSetConnectionsList)
        (*pSetConnectionsList)();
    TestStruct();
    ///
     ////////////////////////////////////////
        // запись в лог
        // Для отображения в АРМ-ах модельного времени
    pSys->startTime = time(NULL);
    pSys->m_nStep = 0;
    pSys->StartStatus = 0;
    pSys->dModelT = 0.;
    if (OLD_CALC_MODE)
        pSys->dt = 2e-4;
    PROPS.Get(PROPKEY_INIT_STEP, pSys->dt);

    pSys->Accel = 0.;
    pSys->Time_real = 0.;
    pSys->nEvents = 0;
    pSys->nTrends = 0;
    pSys->nMsgToOperator = 0;
    //  PROGRESS("Модель запущена" )
    if (!WriteResume)
    {
        PROPS.Get(PROPKEY_CONTROL_VARS, WriteResume);
    }
    if (WriteResume) CStateCtrl::ReadAll();
    ///////////////////////////////////////
    SysMSG("#Модель работает");
    if (pProgress) pProgress->Stop();
    pTrend->PreStepT();
    ////////////////////////////////////////////////////////////////
    void InitCommonWork();
    void InitWork();
    //??  if ( UseCommonFiles )
    //??    InitCommonWork();
    //??  else
    InitWork();
    TestStruct();
}

int SaveParamStep = -1;

int START_API Start(bool External, bool bRestart/* = false */, int SaveParamStep_in /* = -1*/)
{
    auto ticks = std::chrono::high_resolution_clock::now();
    auto stepTicks = std::chrono::high_resolution_clock::now();//Debug
    int OLD_CALC_MODE = HydroGroups->GetCalculationMethod();
    SaveParamStep = SaveParamStep_in;
    //SaveParamStep = 1000;

    //  Место вывода сообщений об утечке памяти
    TRY
        if (!PrepareRuntimeModels())
            return 2;
    CATCH(1)
        PrintDebugLine("PrepareRuntimeModels", ticks, stepTicks);

    TRY
        if (int res = LoadLibraries())
            return res;
    CATCH(2)
        PrintDebugLine("LoadLibraries", ticks, stepTicks);

    TRY
        InitHydraulics();
    CATCH(3)
        PrintDebugLine("InitHydraulics", ticks, stepTicks);

    TRY
        LoadBaseModels(OLD_CALC_MODE);
    CATCH(5)
        PrintDebugLine("LoadBaseModels", ticks, stepTicks);

    TRY
        PrepareObjetsProperties();
    CATCH(6)
        PrintDebugLine("PrepareObjetsProperties", ticks, stepTicks);

    TRY
        InitBaseModels();
    CATCH(8)
        PrintDebugLine("InitBaseModels", ticks, stepTicks);

    TRY
        if (!InitWorkingModels()) return 1;
    CATCH(9)
        PrintDebugLine("InitWorkingModels", ticks, stepTicks);

    TRY
        SetModelObjects();
    CATCH(10)
        PrintDebugLine("SetModelObjects", ticks, stepTicks);

    TRY
        ReadParameters();
    CATCH(11)
        PrintDebugLine("ReadParameters", ticks, stepTicks);

    TRY
        if (!ResetModel()) return 2;
    CATCH(12)
        PrintDebugLine("ResetModel", ticks, stepTicks);

    TRY
        ModelStateRead(External);
    CATCH(13)
        PrintDebugLine("ModelStateRead", ticks, stepTicks);

    TRY
        if (!BaseModelFirstStep()) return 2;
    CATCH(14)
        PrintDebugLine("BaseModelFirstStep", ticks, stepTicks);

    TRY
        StartModel(OLD_CALC_MODE);
    CATCH(15)
        PrintDebugLine("--- EndStart ---", ticks, stepTicks);

    CFaultService service;
    service.ExportFaultsToJson();

    return 0;
}

int CompIBaseModel(const void* p1, const void* p2)
{
    IBaseModel** m1 = (IBaseModel**)p1;
    IBaseModel** m2 = (IBaseModel**)p2;
    int Res = lstrcmp((*m1)->ObjName, (*m2)->ObjName);
    if (Res)
        return Res;
    Res = lstrcmp((*m1)->Model, (*m2)->Model);
    return Res;
}

char START_STATE[_MAX_PATH];

int Start(LPCTSTR szFileStart, double AccelZ, LPCTSTR szFileConfig, LPCTSTR szFileState, LPCTSTR szFileParam, BOOL IsPause, int SaveParamStep_in/* = -1*/)
{
    if (szFileConfig[0])
        lstrcpy(START_CONFIG, szFileConfig);
    if (szFileState[0])
        lstrcpy(START_STATE, szFileState);
    ASS(InitMes() == 0);
    GetCommData("Модель", CSystem::TypeID, (void**)&pSys);
    assert(pSys);
    pSys->AccelZ = AccelZ;
    if (pSys->AccelZ < 0.5)
        pSys->AccelZ = 0.5;
    if (IsPause)
        pSys->StartStatus = CSystemB::STATUS_INIT;
    //if (IsPause)
    //	pSys->StepGO = 0;
    Start(true, false, SaveParamStep_in);
    return 0;
}

IBaseModel* Load_ObjectEx(const char* DllName, const char* ObjName)
{
    char Dll[256];
    lstrcpy(Dll, DllName);
    char* Path = strchr(Dll, '(');
    if (Path)
    {
        *Path = 0;
        Path++;
        char* E = strchr(Path, ')');
        if (E)
            *E = 0;
        auto hLib = Load_Dll(Dll, Path);
        if (hLib == NULL) return NULL;
        tCreateObject pCreateObject = (tCreateObject)hLib->get_symbol("CreateObject");
        if (pCreateObject == NULL) return NULL;
        return (*pCreateObject)(ObjName, NULL);
    }
    return Load_Object(Dll, ObjName);
}

IBaseModel* Load_ObjectRSU(const std::string& DllName, const std::string& ObjName)
{
    bool v_ValidLib = false;
    IBaseModel* ret = g_RsuHolder.create_client(DllName, "", ObjName, v_ValidLib);
    if (!ret && !v_ValidLib)
        ret = Load_ObjectEx(DllName.c_str(), ObjName.c_str());
	if (!ret)
    {
        std::string v_msg = "Load rsu library error: " + g_RsuHolder.get_last_error();
        SysMSG(v_msg.c_str());
    }
    return ret;
}

void LoadRsuModelsCP(int& iRuntimeModel)
{
    fs::path mp = ProjectModelPath();

    std::vector<std::string> rsu_vec;
    std::string fn = mp.generic_string();
    std::ifstream file_in(fn);
    if (file_in)
    {
        std::string line;
        std::getline(file_in, line); //Пропускаем строку с названиями полей
        while (std::getline(file_in, line))
        {
            line = cross::string::rtrim(line);
            rsu_vec.push_back(line);
        }
    }
    else
    {
        std::string v_msg = std::string("Файл не найден ") + fn + "\n";
        SysMSG(v_msg.c_str());
    }
    if (!rsu_vec.size())
        return;

    for (const auto& s : rsu_vec)
    {
        if (cross::string::starts_with(s.c_str(), U'#'))
            continue;

        std::vector<std::string> v_names;
        cross::string::split_strings(s, ";", std::back_inserter(v_names));

        std::string v_msg = std::string("#Загрузка библиотеки ") + v_names[0].c_str() + "\n";
        SysMSG(v_msg.c_str());

        if (v_names.size() < 2)
        {
            v_msg = std::string("Ошибка загрузки библиотеки ") + v_names[0].c_str() + " - отсутствует имя объекта\n";
            SysMSG(v_msg.c_str());
            continue;
        }

        assert(iRuntimeModel < countof(g_RuntimeModels));

        IBaseModel* pO = FindBaseModel((char*)v_names[1].c_str());
        if (!pO)
        {
            pO = Load_ObjectRSU(v_names[0], v_names[1]);
        }
        
        assert(pO);
        pO->PropsWasRead = v_names.size() > 2 && !cross::string::stricmp(v_names[2], "Save");
        if (pO)
        {
            if (cross::string::stricmp(v_names[0], "Копия") == 0)
                pCtrlCopy = pO;
            pO->PropsWasRead = v_names.size() > 2 && !cross::string::stricmp(v_names[2], "Save");
            g_RuntimeModels[iRuntimeModel++] = pO;
            v_msg = "#Библиотека '" + v_names[0] + "' загружена успешно!\n";
            SysMSG(v_msg.c_str());
        }
        else
        {
            v_msg = std::string("Ошибка загрузки библиотеки ") + v_names[0].c_str() + "\n";
            SysMSG(v_msg.c_str());
        }
    }
}

#include "IO_Props.h"
void Set(CProps* pProp, CObjProps* pValue)
{
    switch (pProp->Type)
    {
    case 'I':
    {
        *(int*)(pProp->Addr) = atoi((char*)(pValue->Value));
    }
    return;
    case 'D':
    {
        *(double*)(pProp->Addr) = cross::locale::safe_atof((char*)(pValue->Value));
    }
    return;
    case 'S':
    {
        lstrcpy((char*)(pProp->Addr), (char*)(pValue->Value));
    }
    return;
    case 'L':
    {
        int N = 0;
        char* Val = (char*)(pValue->Value);
        int lVal = lstrlen(Val);
        char* P = pProp->Default;
        while (1)
        {
            if (strncmp(P, Val, lVal) == 0)
            {
                *(int*)(pProp->Addr) = N;
                return;
            }
            N++;
            P = strchr(P, ';');
            if (P == NULL)
                break;
            P++;
        }
    }
    return;
    default:
        ASS(0)
            return;
    }
}

void SetModelProps(IBaseModel* pModel)
{
    if (pModel == NULL || pModel->pGetProps == NULL)
        return;
    CProps Props[64];
    int K = (*(pModel->pGetProps)) (Props, pModel);
    CObjProps* pProp = pModel->pFirstProp;
    while (pProp)
    {
        for (int n = 0; n < K; n++)
        {
            if (pProp->PropName == Props[n].Name)
            {
                Set(&Props[n], pProp);
                break;
            }
        }
        pProp = pProp->pNext;
    }
}

CCSVData ModelDLL;

int InitDll()
{
    ModelDLL.Load(5, PROJECT_ROOT, "ModelDLL", "MD");
    ASS(ModelDLL.Sort())
        //
        auto hLib = Load_Dll("HydroWW");
    if (hLib == NULL)
    {
        //SysMsg("Ошибка загрузки 'Hydro.dll'" );
        return -1;
    }
    //
    tCreateObject GetBaseModel = (tCreateObject)hLib->get_symbol("CreateObject");
    AddressHydroGroupsInterface GetHydroGroupsInterface = (AddressHydroGroupsInterface)hLib->get_symbol("GetHydroGroupsInterface");
    if (GetBaseModel == NULL || GetHydroGroupsInterface == NULL)
    {
        //SysMsg("Ошибка загрузки 'Hydro'" );
        return 1;
    }
    pMainHydro = (*GetBaseModel)("Гидравлика", NULL);
    pMainHydro->SetData(sd_SetModelDLL, &ModelDLL);

    HydroGroups = GetHydroGroupsInterface();
    return 0;
}

extern char START_STATE[_MAX_PATH];

void PrtEquipment(char* File, char* Title, char* Model, char* AddModel)
{
    auto path = std::filesystem::path(PROJECT_ROOT) / "DATA" / "Equipment" / "New" / (std::string(File) + ".csv");
    void* F = Fopen(path.string().c_str(), "wt");
    if (F == NULL)
        return;
    Fprintf(F, "%s\n", Title);
    int K = IBaseModel::kObjects;
    for (int n = 0; n < K; n++)
    {
        IBaseModel* pModel = IBaseModel::AllObjects[n];
        if ((char*)pModel->Model == NULL)
            continue;
        if (pModel->Model != Model && pModel->Model != AddModel)
            continue;
        Fprintf(F, "%s;\n", pModel->ObjName);
    }
    Fclose(F);
}
#endif
