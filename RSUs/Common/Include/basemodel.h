#ifndef BASEMODEL_H
#define BASEMODEL_H
#include <QtCore/qglobal.h>

#ifdef UTILS_EXPORTS
#define IN_DLL Q_DECL_EXPORT
#else
#define IN_DLL Q_DECL_IMPORT
#endif


#define PURE = 0
#define NOT_USED { ASS(FALSE); return 1; }

#if 1

#define NO_STEP_T 0x0100
#define MAX_SUMM_NAME 128
#define MAX_DLL_NAME 32
#define MAX_OBJ_NAME 64
#define MAX_PARM_NAME 128
#define MAX_MODEL_NAME 32
#define MAX_POINT_NAME 32

// интерфейс модели
#define MAX_OBJECTS 70000
#define MODEL_IN_USE	0x02
#define MODEL_DEBUG		0x04
#define MODEL_SENS		0x08
//#define MODEL_DATA 0x10 //??? вроде не используется, а мешает РСУ
//
#define NO_STEP_0 0x0200
#define NO_STEP_1 0x0400
#define NO_INIT   0x0800
#define DELETE_ON_EXIT  0x1000

#endif

struct IBaseModelInfo
{
    virtual int GetParams(int & N, struct QParams Params[]);
    virtual int GetConnections(int & Cnt, struct CConnection Connections[/*100*/]);
};

struct IN_DLL ISet
{
    virtual bool IsOn() = 0;
    virtual void Off() = 0;
    virtual void On() = 0;
    virtual void GetParam() = 0;
    virtual void OutValue ( char Txt[64] ) = 0;
    virtual void Set ( double Parm ) = 0;
    static struct ISet * pFirstGlobalISet;
    static struct ISet * pLastGlobalISet;
    ISet * pGlobalNextISet;
    ISet * pModelNextISet;
    struct IBaseModel * pModel;
    char * Descr;
    char * Objname;
    ISet( IBaseModel * pModel );
    ISet( );
};

class IRenderer
{
public:
    template<class... Args>
    inline void Render(Args... args)
    {
        constexpr auto bufferSize = (0 + ... + sizeof(args));
        char buffer[bufferSize];
        int offset = 0;
        ([&]
         {
             memcpy(buffer + offset, &args, sizeof(args));
             offset += sizeof(args);
         } (), ...);
        OnRender(&buffer, bufferSize);
    }

protected:
    virtual void OnRender(void* data, size_t length) = 0;
};

typedef void (*tSysMsg)(const char * Fmt, ...);
typedef int (*tGetProps)(struct CProps * pProps, struct IBaseModel * pObj);
enum eModelFlags { Flag_NoInit = 0x01,  Flag_NoStep0 = 0x02, Flag_NoStep1 = 0x04,
                                  Flag_NoStepT = 0x08, Flag_NoAll = 0x0F };

struct IN_DLL IBaseModel
{
    static IBaseModel * pFirst;
    static IBaseModel * pLast;
    static tSysMsg pSysMsg;
#define ModMsg(...) if (IBaseModel::pSysMsg )(*IBaseModel::pSysMsg)(__VA_ARGS__)
    static bool NotConnect;
    IBaseModel * pNext;
    IBaseModel * pPrev;
    IBaseModel * pMain;
    IBaseModel * pFirstChild;
    IBaseModel * pNextChild;
    struct CObjProps * pFirstProp;
    tGetProps pGetProps;
    struct QParams * pFirstParams;
    struct QParams * pLastParams;
    struct CDef * pRecoveryDef;
    struct CDef * pFirstDefect;
    struct CDef * pLastDefect;
    struct ISet * pFirstISet;
    struct ISet * pLastISet;
    struct CExtern_Pnt * pModel_Pnt;
    bool PropsWasRead;
    //
    struct IUniModel * pUniModel;
    struct IPoolCtrlModel * pPoolCtrl;
    static IBaseModel ** AllObjects;
    static int kObjects;
    quint64 Counter;  // microseconds
    DWORD nThread;
    DWORD kGroups;
    DWORD nGroup;
    DWORD RootNode;
    //
    CStr ObjName;
    CStr Model;
    CStr Data3;
    CStr Shema;
    enum eType { None, MainModel, SubModel, GroupModel, Y_Obj, RSU_Obj };
    eType TypeObj;
    //
//    CSortTree RootPoint;
//    CSortTreeInfo Points;
    int ModelGroup;
    DWORD ModelFlags;
    DWORD ClassID;
    DWORD ID_NAME;
    int BreakCount;
    int NewData;
    int PathLabel;
    //
    bool HaveProperties;
    bool HaveExtensions;
    bool bp;
    bool PathTempLabel;
//  BYTE MaxPathTempLabel;
//
    virtual int Init( int );
    virtual int Step0();
    virtual int Step1();
    virtual int PreStepT();
    virtual int StepT(double dt);
    // сохранение и восстановление параметров, состояния, прочее.
    virtual int GetParams( char * StrName );
    virtual int UpdateParam( struct QParams & Param );
    virtual int SaveState( );
    virtual int RestoreState ( char * StrName );
    virtual void Finalize() {}
    //
    virtual int SetData( int TypeData, void * pData );
    virtual int SetProp ( struct CObjProps * pProp );
    virtual int GetProp( );
    virtual void DrawObj ( struct CDrawObjData * pDraw ){};
    virtual void DrawObject ( struct CDrawObjData * pDraw, class CGDIResourceMgr *pResMgr) {}
#ifdef _WIN32
    virtual BOOL NeedDrawBefore() { return FALSE; }
    virtual void Render(IRenderer &);
    virtual std::string ClassName();
#endif
    virtual int GetConnections ( int & Cnt, struct CConnection Connections[/*100*/]);
    virtual int OnDefect( struct CDef * pDefect );
    virtual void SetName( char * Name, IBaseModel * pMain );
    virtual bool AddPoint( struct CExtern_Pnt * pNewItem );
    virtual CExtern_Pnt * FindPoint( const char * PntName );
    virtual CExtern_Pnt * GetPoint( const char * PntName );
    virtual int SrcMM( char * DataName, struct IMM * pMM );// Обращение к источнику прямых ссылок
    virtual int DstMM( char * DataName, struct IMM * pMM );// Вызов получателя прямых ссылок
    virtual double * GetValueAddr();
    virtual int ResetVariable(){return 0;};
    virtual char * GetModelName();
    virtual bool GetParamsList( struct QParams ** pParams, int * kParams );
    void SetParamsList( DWORD SizeOfThis, struct QParams ** pParams, int * kParams );
    //
    IBaseModel();
    IBaseModel( bool );
    IBaseModel( char * Name );
    IBaseModel( char * Name, IBaseModel * pMain );
    virtual ~IBaseModel();
    void InitName( );
    void ClearAndConnect( );
    bool AddParam( struct QParams * pNewParam );
    bool AddDefect( struct CDef * pNewDefect );
    bool AddSet( struct ISet * pNewISet );
    void ShowAddParams( char * TabName = (char*) "Добавлены" );
    void ShowAddPoints( char * TabName = (char*) "Добавлены" );
    static IBaseModel * Find (const char * Name );
    static bool Find ( char * Object, char * Param, char & Type, void ** pAddr, void ** pParam = NULL );
    static void SetObjectsList( );
    void Owner( struct CBase * pBase );
    struct CBase * GetBase( int & Pos );
    struct QParams * GetParam( const char * ParamName );
    void PrtNoUsed( void * pData );
};

IN_DLL IBaseModel * FindBaseModel ( char * Name );
IN_DLL bool SaveBaseModel ( IBaseModel * pModel );
IN_DLL void SetModelList();



struct IN_DLL CCounter
{
//     IBaseModel * pModel;
// #pragma warning(suppress:4251)
//     std::chrono::high_resolution_clock::time_point Start;
//     //
//     CCounter ( IBaseModel * pModel );
//     ~CCounter(  );
};

#define COUNTER
//CCounter	Time_Counter( this );
#include <QRect>
#include <QPainter>


struct IN_DLL IPoolCtrlModel
{
    virtual int InitPool ( struct CVarPool * pPool, struct CGroupObject * pPoolObject ) = 0;
};

#define POOLCTRL pPoolCtrl = static_cast<IPoolCtrlModel*>(this);

//
struct IN_DLL IUniModel
{
    static IBaseModel ** UniObjects;
    static int kUniObjects;
    virtual int SetMatr( void * pExternals, int PntGroup,  double dt,
                        double A[/* kExkV */], double B[ /*kE*/ ] ) = 0;   // A * X = B
    virtual int SetVar( void * pExternals, int PntGroup, double dt,
                       // Переменные на входах
                       double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] ) = 0;
    virtual int TestNodes( int kNodes, struct CObjectPoint ** ppNodes ) = 0;
};

#define UNIMODEL pUniModel = static_cast<IUniModel*>(this);

struct IN_DLL IElectroModel
{
    virtual void ElectroOn() = 0;
    virtual void ElectroOff() = 0;
};

//#pragma warning ( disable : 9035 )

#endif // BASEMODEL_H
