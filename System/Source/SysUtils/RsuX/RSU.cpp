#include "DataTypes.h"
#include "CommProc.h"
#include "mio.hpp"
#include "Err.h"
#include "Param.h"
#include <Split.h>
#include "RSU/rsu_basemodel.h"

#define fs std::filesystem
#if 0
class  KRsuMemoryManager
{
public:
    struct SWorkingKvant
    {
        SWorkingKvant()
        {
            memset(this, 0, sizeof(*this));
        }
        ~SWorkingKvant()
        {
        }
        BYTE* pMem;
        mio::mmap_sink* hVarMapping;
        DWORD Size;
        DWORD Pos;
        SWorkingKvant* pNext;
        BYTE* GetMemory(DWORD count);
        DWORD LeftFree();
    };
    SWorkingKvant* m_pFirstBlock, * m_pActiveBlock;
    DWORD m_addMBytes;
public:
    KRsuMemoryManager();
    ~KRsuMemoryManager();
protected:
    KRsuMemoryManager(KRsuMemoryManager& src);
    KRsuMemoryManager& operator = (KRsuMemoryManager& src);
public:
    size_t Create(LPCSTR pszName, DWORD nInitialMBytes, DWORD addMBytes = 20);
    BYTE* AllocMemory(DWORD size);
    void Clear();
};

BYTE* OnDiskAllocator(mio::mmap_sink*& mapper, bool& isNew, DWORD requestedSeze, LPCSTR fileName, LPCSTR fileExtension, bool forceCreate);

BYTE* KRsuMemoryManager::SWorkingKvant::GetMemory(DWORD count)
{
    if ((Pos + count) > Size)
        return NULL;
    BYTE* p = &pMem[Pos];
    memset(p, 0, count);
    Pos += count;
    return p;
}

DWORD KRsuMemoryManager::SWorkingKvant::LeftFree()
{
    return Size - Pos;
}

//////////////////////////////////////////////////////////////////////////
static LPCSTR s_szMemExt = "mem";
KRsuMemoryManager::KRsuMemoryManager()
    : m_pFirstBlock(NULL)
    , m_pActiveBlock(NULL)
    , m_addMBytes(10)
{
}

KRsuMemoryManager::~KRsuMemoryManager()
{
}

KRsuMemoryManager::KRsuMemoryManager(KRsuMemoryManager& src)
{
}

KRsuMemoryManager& KRsuMemoryManager::operator = (KRsuMemoryManager& src)
{
    return *this;
}

static DWORD toBytes(DWORD mb)
{
    return mb * 1024 * 1024;
}

size_t KRsuMemoryManager::Create(LPCSTR pszName, DWORD nInitialMBytes, DWORD addMBytes /*= 20*/)
{

    if (m_pActiveBlock)
        return m_pActiveBlock->LeftFree();
    m_addMBytes = addMBytes;
    SWorkingKvant kvant;
    bool bNewMem;

    DWORD lenName = (DWORD)strlen(pszName) + 1;

    DWORD realSize = toBytes(nInitialMBytes) + sizeof(kvant) + lenName;

    kvant.pMem = OnDiskAllocator(kvant.hVarMapping, bNewMem, realSize, pszName, s_szMemExt, true);

    if (!kvant.pMem)
        return 0;

    kvant.Size = toBytes(nInitialMBytes);

#ifndef LINUX
    memset(kvant.pMem, 0, realSize);
#endif // NOT LINUX

    memcpy(kvant.GetMemory(sizeof(kvant)), &kvant, sizeof(kvant));

    m_pFirstBlock = m_pActiveBlock = (SWorkingKvant*)kvant.pMem;

    LPSTR psz4Name = (LPSTR)m_pActiveBlock->GetMemory(lenName);
    strncpy(psz4Name, pszName, lenName);
    return m_pActiveBlock->Size;
}

BYTE* KRsuMemoryManager::AllocMemory(DWORD size)
{

    if (!size)
        return NULL;
    if (!m_pActiveBlock)
        return NULL;
    DWORD needSize = size;
    size += sizeof(DWORD);

    BYTE* p = NULL;
    if (m_pActiveBlock->LeftFree() >= size)
        p = m_pActiveBlock->GetMemory(size);
    else
    {
        for (SWorkingKvant* pb = m_pFirstBlock; pb; pb = pb->pNext)
        {
            if (pb->LeftFree() >= size)
            {
                p = pb->GetMemory(size);
                break;
            }
        }
    }
    if (!p)
    {
        int n = 0;
        for (SWorkingKvant* pb = m_pFirstBlock; pb; pb = pb->pNext, ++n)
        {
        }
        SWorkingKvant kvant;

        DWORD bytes = std::max(size, toBytes(m_addMBytes)) + sizeof(kvant);
        LPCSTR pName = (LPCSTR)m_pFirstBlock->pMem + sizeof(kvant);
        char szName[512];
        snprintf(szName, sizeof(szName), "%s_%d", pName, n);

        bool bNewMem;
        kvant.pMem = OnDiskAllocator(kvant.hVarMapping, bNewMem, bytes, szName, s_szMemExt, true);

        if (!kvant.pMem)
            return NULL;
        kvant.Size = bytes;// - sizeof(kvant);
#ifndef LINUX
        memset(kvant.pMem, 0, bytes);
#endif // NOT LINUX
        memcpy(kvant.GetMemory(sizeof(kvant)), &kvant, sizeof(kvant));
        m_pActiveBlock->pNext = (SWorkingKvant*)kvant.pMem;
        m_pActiveBlock = m_pActiveBlock->pNext;
        p = m_pActiveBlock->GetMemory(size);
    }

    *(DWORD*)p = needSize;
    p += sizeof(DWORD);

    return p;
}

void KRsuMemoryManager::Clear()
{
    for (SWorkingKvant* pb = m_pFirstBlock; pb; pb = pb->pNext)
    {
        pb->Pos = sizeof(SWorkingKvant);
    }
    m_pActiveBlock = m_pFirstBlock;
}
//264566856 "H5xx_Objs""noname"

BYTE* OnDiskAllocator(mio::mmap_sink*& mapper, bool& isNew, DWORD requestedSeze, LPCSTR fileName, LPCSTR fileExtension, bool forceCreate)
{
    isNew = false;
    auto filePath = fs::path(fileName);
    if (!fs::exists(filePath))
    {
        auto fullFileName = std::string(fileName) + "." + std::string(fileExtension);
        auto folderPath = "/home/resh/Platform/projects/319_VSB_KF/Memory/";//rsuGetMemoryPath();
        filePath = (fs::path(folderPath) / fullFileName).generic_string();
    }

    // if (!MapFile(mapper, filePath, requestedSeze, forceCreate, isNew))
    // {
    //     return nullptr;
    // }

    isNew = forceCreate;
    return (BYTE*)mapper->data();
}
#endif
//////////////////////////////////////////////////////////////////////////////////////////////

class  KNoName
{
public:
    typedef int (*tNamesStringComparator)(LPCSTR lpString1, LPCSTR lpString2);
    tNamesStringComparator pfStringComparator;

    struct SNoNameHeader
    {
        char szName[32];//Идентификатор файла
        DWORD nCount;//Количество имён
        DWORD nCurr;//Текущая позиция в szBuff
        DWORD nMaxPos;//Текущая позиция в szVars

        DWORD shiftOrds;
        DWORD shiftBuff4Ords;
        DWORD shiftObjects;

        DWORD countOrds;//MAX_NAMES
        DWORD nBuff4Ords;//MAX_BUFFER
        DWORD nBuffObjects;//MEM_SIZE

        DWORD clientCounter;
    };

    struct SBuffOrd
    {
        DWORD nOrd;
        DWORD nClass;
        DWORD nEntry;
        int  nNumber;
        size_t  nData;
    };

#pragma pack(push)
#pragma pack( 1 )
    struct SBuffRecSeparator
    {
        DWORD strLength;
        DWORD crc32;
        char str[1];
    };
#pragma pack(pop)

    SNoNameHeader* pHeader;
    SBuffOrd* pOrd;
    BYTE* pszString4Ords;//szBuff
    BYTE* pszObjects;//szVars
    mio::mmap_sink* m_hVarMapping;
    bool m_bCrc2Upper;
    bool m_bCompareByCrc;

    enum
    {
        eClassNamesCount = 0x10000,
        eRsuSupportCount = 15,
    };
    static LPCSTR s_ClassNames[eClassNamesCount];
 //   static tRsuHandleInitData s_InitDataCalls[eRsuSupportCount];
public:
    // KNoName();
    // KNoName(KNoName& src);
    // bool IsOpened();
    // ~KNoName();
    // KNoName& operator = (const KNoName& src);
public:
//    static KNoName* Create(LPCSTR pszName, bool bMaster, DWORD nNames = 800000, DWORD nBtsStrings = 0xd00000, DWORD nBtsObjs = 0x0dd00000);
public:
//     static int RegisterClassInNames(DWORD idClass, LPCSTR pszClassName);
// //    static void RegisterInitCBaseCall(tRsuHandleInitData pInitData);
public:
    // struct CBase* CreateStruct(LPCTSTR ObjName, DWORD TypeID, int number);
    // struct CBase* CreateStruct(LPCTSTR ObjName, DWORD TypeID, DWORD sizeObj, int number);
    // struct CBase* FindStruct(LPCTSTR ObjName, DWORD TypeID, int number);
    // int CreateRefToData(LPCSTR ObjName, struct CBase* pBase, int newNumber = -1 ); //если newNumber >= 0, то сделать в другом домене
    // bool WhileBase(DWORD& ID, const DWORD ClassID, struct CBase** base, LPCSTR* pszName, int* pFcsNumber);
    // bool GetNameEx(DWORD ID, LPCSTR* pszClass, LPCSTR* pszEntry, int* pFcsNumber = NULL);
    // bool GetNameEx(DWORD ID, DWORD ClassID, LPCSTR* pszClass, LPCSTR* pszEntry, int* pFcsNumber = NULL);
    // void GetName(DWORD ID, char* szClass, size_t _SC, char* szEntry, size_t _SE);
    // void Reset();//использовать только bMaster-ом
    // void PrintStatistic();
    // size_t GetNAddr(DWORD ID);
    // CBase* GetBase(size_t AddrBase);
    // void SetCompareMethod(bool bUseCrc);//устанавливать везде
    // tNamesStringComparator SetStringComparator(tNamesStringComparator pfnOther);
//protected:
    // int GetID(LPCSTR szClass, LPCSTR szEntry, bool bAdd, int number);
    // int GetID(DWORD TypeID, LPCSTR szEntry, bool bAdd, int number);
    CBase* GetNameData(int ID);
    // CBase* AppendData(int ID, DWORD TYPE, DWORD size = 0);//int GiveCommData( int ID, DWORD TYPE, void** ppData )
//private:
    LPCSTR Class(SBuffOrd& _ord, DWORD& crc);
    LPCSTR Entry(SBuffOrd& _ord, DWORD& crc);
    // int Number(SBuffOrd& _ord);
    // LPCSTR Class(DWORD ID, DWORD& crc);
    // LPCSTR Entry(DWORD ID, DWORD& crc);
    // int Number(DWORD ID);
    // void SetBuffer(LPCSTR text);
    // int InitData(struct CBase* pObj);
    // LPCSTR GetEmpty();
};

CBase* KNoName::GetNameData(int ID)
{
    if (!pHeader)
    {
        return nullptr;
    }

    ID--;
    ASS(ID < pHeader->nCount);
    ASSD(ID < pHeader->countOrds);
    if (pOrd[ID].nData != -1)
    {
        CBase* pBase = (CBase*)(pszObjects + pOrd[ID].nData);
#ifdef _WIN32
        if (!pBase->size)
        {
            MessageBox(nullptr, s_pText, s_pCaption, MB_OK | MB_ICONASTERISK);
        }
#endif
        ASS(pBase->size > 0);
        return pBase;
    }

    return nullptr;
}
LPCSTR KNoName::Class(SBuffOrd& _ord, DWORD& crc)
{
    if (!pszString4Ords)
    {
        return nullptr;
    }

    SBuffRecSeparator* buf = (SBuffRecSeparator*)(pszString4Ords + _ord.nClass);
    crc = buf->crc32;
    return buf->str;
}

LPCSTR KNoName::Entry(SBuffOrd& _ord, DWORD& crc)
{
    if (!pszString4Ords)
    {
        return nullptr;
    }

    SBuffRecSeparator* buf = (SBuffRecSeparator*)(pszString4Ords + _ord.nEntry);
    crc = buf->crc32;
    return buf->str;
}


#define _EXP
#define _IMP

typedef uint64_t UINT64;

#include "../../../RSUs/HoneyWell5xx/Include/HoneywellStruct.h"


struct SConnectionsCont
{
//    SConnectionMB *pInConns;
    void *pInConns;
    DWORD inConsC;
};

typedef DWORD typeHASH32_;
typedef DWORD tHcdVarSizeType_;

enum eHcdVType
{
    ehtDisable = -1,
    ehtHZ = 0,
    ehtDigital32 = 1,
    ehtAnalog32 = 2,
    ehtBool = 3,
    ehtDynStr = 4,
    ehtEnum = 6,
};

struct SVarInfo
{
    typedef BYTE tFlagType_;
    enum eFlags : tFlagType_
    {
        efEmpty               = 0,
        efEnum                = 0x01,
        efSaveInState         = 0x02,//сохранять в состоянии, всегда
    };
    LPCSTR pszName;
    LPCSTR pszLgxParamName;
    eHcdVType evtType;
    tHcdVarSizeType_ shift, size;
    tFlagType_ flags;//eFlags
    typeHASH32_ CRC;
    LPCSTR pszEnumVals;
    // SVarInfo() : pszName( NULL ), pszLgxParamName(NULL), evtType( ehtHZ ), shift(0), size(0), flags(0), CRC(0), pszEnumVals(NULL)
    // {
    // }
    // NEWHCD;
};

class KBmBase
{
public:
    enum
    {
        sizeofBlockName = 48*4,
        sizeofTypeName = 32*4,
        sizeofAssignedTo = 32*4,
        sizeofContainer = 32*4,
        countNames = 4,
    };
    enum blockVarType
    {
        blockVar_Params,
        blockVar_States,
        blockVar_All
    };
public:
    BYTE* pObjAddr;
    BYTE* pThisAddr;
    SVarInfo * ClassVarInfo;
    int kClassVarInfo;
    int nBlockCrcs;
public:
    char BlockName[sizeofBlockName];
    char EntityName[sizeofBlockName];
    char TypeName[sizeofTypeName];
    char HandlerName[sizeofAssignedTo];
    char AssignedTo[sizeofAssignedTo];//физическое расположение
    char Container[sizeofContainer];//фактическое использование, как бы ссылка на AssignedTo
    typeHASH32_ crcBlockSplitted[countNames];
    DWORD H_Type;
    int L_Class_FullW;
    BYTE * pClass_FullW;
public:
    // KBmBase();
    // void CreateCrcs();
    // virtual void SetParmList(){}
    // virtual bool GetVar( LPCSTR pField, BYTE **ppVar, eVarType *pType, USHORT *pVarSize = NULL, LPCSTR *ppszEnum = NULL, BYTE *pFlags = NULL );
    // virtual bool SetValue( LPCSTR pField, LPCSTR pszVal );
    // virtual void OnNoSetValue( LPCSTR pField, LPCSTR pszVal );
    // virtual KBmBase* FindObj( LPCSTR pObj );
    // virtual KBmBase* FindObj( typeHASH32_ *pCrcs, int nCrcs, int nAlgFind = 0 );
    // virtual KBmBase* QuickFindObj( LPCSTR pszObj, int nAlgFind = 0 );
    // virtual void StepT( SStepCalcParams &dt );
    // virtual void StepBeforeRestoreState( LPCSTR pSystemName, KBmBase *pRoot, KBmBase *pModule );
    // virtual void StepAfterRestoreState(){}
    // virtual void OnReadProject( KBmBase *pRoot, KBmBase *pModule ){}
    // virtual void SaveBlock( std::fstream& hFile ){}
    // virtual bool RestoreBlock( std::fstream& hFile )
    // {
    //     return true;
    // }
    // virtual void GetParams( KHBridge2SysParam &params ){}
    // //
    // int GetFirstVar( BYTE maskFlags );//SVarInfo::efParam SVarInfo::efVar;
    // LPCSTR GetVar( int &pos, LPBYTE &pVar, eVarType &pType, USHORT *pVarSize );
    // //пока не придумал как использовать 2 xyCHANNEL как 1, то буду их связывать руками
    // virtual LPCSTR SetDestIO( KBmBase *dest, LPCSTR pszField = NULL ){ return NULL;}
    // virtual BOOL IsBlock() = 0;
    // virtual KBmBase* WhoHasConnection( LPCSTR pszObj, LPCSTR pszFld, bool bOutput, LPCSTR *ppFld );
    // virtual bool IsArmAssigned() { return false ;}
    // virtual void OnAssignField( LPCSTR pszFieldName ){}//вызывается из KCalcRes::operator =
    // virtual void AddAsOutputConnection( class SConnectionMB *pCon ){}
protected:
    // template<class T>
    // void initW( T *wLocal, T *wStatic )
    // {
    //     pObjAddr = (BYTE*)wLocal;
    //     memmove( ((BYTE*)static_cast<T*>(wLocal)) + sizeof(CBase),
    //             ((BYTE*)static_cast<T*>(wStatic)) + sizeof(CBase),
    //             sizeof(T) - sizeof(CBase));
    // }
    // template<class T>
    // void Add2Params( T &Var, LPCSTR pFieldName, KHBridge2SysParam &params )
    // {
    //     params.Add( Var, BlockName, pFieldName );
    // }
    // template< size_t x>
    // static int CreateCrcsImpl( LPCSTR pszIn, typeHASH32_ (&crcSplits)[x] )
    // {
    //     char szSplits[x][KBmBase::sizeofBlockName] = {};
    //     int cParts = SplitString( pszIn, szSplits, '.', false );
    //     ASSD( cParts < x );

    //     for( int i=0; i<cParts; ++i )
    //     {
    //         _Ktoupper(szSplits[i]);
    //         crcSplits[i] = CreateHASH32( szSplits[i], false );
    //     }
    //     return cParts;
    // }
};


#include <sys/mman.h>
#include <fcntl.h>
LPCSTR KNoName::s_ClassNames[eClassNamesCount];

KNoName nn;

int sW = sizeof ( CBase);
int sP = 500;
void TestMem2();

void TestMem()
{
//    TestMem2();
//    return;

//    char * path = "/home/resh/Platform/projects/319_VSB_KF/Memory/H5xx_Mem.mem";
 //   char * path = "/home/resh/Platform/projects/319_VSB_KF/Memory/H5xx_Mem.mem";
    char * path = "/home/resh/Platform/projects/319_VSB_KF/Memory/H5xx_IOs.noname";
    int size = fs::file_size( path);
    int fd = open( path,O_RDWR );
    void* addr = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
//    KRsuMemoryManager::SWorkingKvant kvant;
 //   kvant.pMem = (BYTE*)addr;
 //   kvant.Size = size;// - sizeof(kvant);
//    KRsuMemoryManager::SWorkingKvant* m_pFirstBlock, * m_pActiveBlock;
 //   m_pFirstBlock = m_pActiveBlock = (KRsuMemoryManager::SWorkingKvant*)kvant.pMem;
  //  DWORD lenName = 8;
    nn.pHeader = (KNoName::SNoNameHeader*)addr;
    //LPSTR psz4Name = (LPSTR)m_pActiveBlock->GetMemory(lenName);
//    strncpy(psz4Name, pszName, lenName);
    BYTE* pMem = (BYTE*)nn.pHeader;
    KNoName::SNoNameHeader & header = *nn.pHeader;
    nn.pOrd = (KNoName::SBuffOrd*)(pMem + header.shiftOrds);
    nn.pszString4Ords = pMem + header.shiftBuff4Ords;
    nn.pszObjects = pMem + header.shiftObjects;
    DWORD crc = 0;
    //  FILE * F = fopen ( "/home/resh/Platform/projects/319_VSB_KF/TEMP/H_Obj.txt", "wt" );
    //  for ( int n = 0; n < nn.pHeader->nCount; n++)
    //  {
    //      LPCSTR pClass = nn.Class(nn.pOrd[n],crc);
    //      LPCSTR pEntry = nn.Entry(nn.pOrd[n],crc);
    //      fprintf ( F, "%d;%s;%s;\n", n, pEntry, pClass );
    //  }
    //  fclose ( F);
    int n = 1980;
    LPCSTR pClass = nn.Class(nn.pOrd[n],crc);
    LPCSTR pEntry = nn.Entry(nn.pOrd[n],crc);
    CBase* pBase = nn.GetNameData(n);
    W_AICHANNEL * pAI = (W_AICHANNEL*)pBase;
    BYTE * pW = (BYTE*)static_cast<W_AICHANNEL_W*>(pAI);
    pW -= 20;
    for ( int n = 0; n < 40; n++ )
    {
        W_AICHANNEL_W * pWAI = (W_AICHANNEL_W*)pW;
        KKK();
        pW++;
    }
    KKK();;
    //W_PID * pPID = (W_PID*)pBase;
    //BYTE * pB = (BYTE *)pBase;
    //W_PID_W * pPID_W = (W_PID_W*)(pB+sW);
    // for ( int p = 496; p < 520; p++ )
    // {
    //     W_PID_P * pPID_P = (W_PID_P*)(pB+p);
    //     KKK();
    // }
    // BYTE * pD = (BYTE*)&pPID->CVEUHI;
    KKK();
}

#include <QDir>
//#include <string.h>

void Prt( const char * Path, const char * Name)
{
    int size = fs::file_size( Path);
    int fd = open( Path,O_RDWR );
    void* addr = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    nn.pHeader = (KNoName::SNoNameHeader*)addr;
    BYTE* pMem = (BYTE*)nn.pHeader;
    KNoName::SNoNameHeader & header = *nn.pHeader;
    nn.pOrd = (KNoName::SBuffOrd*)(pMem + header.shiftOrds);
    nn.pszString4Ords = pMem + header.shiftBuff4Ords;
    nn.pszObjects = pMem + header.shiftObjects;
    DWORD crc = 0;
    char File[1024];
    sprintf_s ( File, 1024, "/home/resh/Platform/projects/319_VSB_KF/TEMP/%s.txt", Name );
    FILE * F = fopen ( File, "wt" );
    for ( int n = 0; n < nn.pHeader->nCount; n++)
        {
        LPCSTR pClass = nn.Class(nn.pOrd[n],crc);
        LPCSTR pEntry = nn.Entry(nn.pOrd[n],crc);
        fprintf ( F, "%d;%s;%s;\n", n, pEntry, pClass );
        }
    fclose ( F );
}

void TestMem2()
{
    QString Dir = "/home/resh/Platform/projects/319_VSB_KF/Memory/";
    QDir DirList( Dir );
    QFileInfoList L = DirList.entryInfoList(  QDir::NoFilter, QDir::Name);
    foreach (QFileInfo f, L)
        {
        QString Path = f.absoluteFilePath();
        QString Name = f.baseName();
        if ( strstr (Path.toStdString().c_str(), ".noname"))
        {
            Prt( Path.toStdString().c_str(), Name.toStdString().c_str());
        }
        KKK();
        }
    KKK();
}

//struct RSU_Obj
//{
//    CStr ObjName;
//    const char * Model;
//    const char * File;
//};

CMem<RSU_Obj, 1024, 1024> RSU_Pnt;
#define MAX_FILES 16
#define MAX_MODELS 256
CStr Files[MAX_FILES];
int kFiles = 0;
CStr Models[MAX_MODELS];
int kModels = 0;
bool WasInit = false;

bool CompName ( int Level, int lN, const char * Name, int lS, char * S )
{
    if ( Level > 50 )
        return true;
    if ( lN == 0 && lS == 0 )
        return true;
    if ( lS == 1 && S[0] == '*' )
        return true;
    if ( lN > 0 && lS == 0 )
        return false;
    if ( S[0] == '*' )
    {
        for ( int n = 0; n < lN; n++ )
        {
            if ( CompName ( Level + 1, lN - n, Name + n, lS - 1, S + 1 ))
                return true;
        }
        return false;
    }
    if ( S[0] == '?' )
    {
        return CompName ( Level + 1, lN - 1, Name + 1, lS - 1, S + 1 );
    }
    //    if ( ToBigEng( S[0] ) != ToBigEng( Name[0] ))
    //        return false;
    if (  S[0]  != Name[0])
        return false;
    return CompName ( Level + 1, lN - 1, Name + 1, lS - 1, S + 1 );
}

bool FiltrName( char * szFilter, const char * Name )
{
    int lN = strlen ( Name );
    bool Res = false;
    char * P =  szFilter;
    while ( 1 )
    {
        char * Pn = strchr ( P, ';' );
        if ( Pn )
            *Pn = 0;
        if ( CompName ( 1, lN, Name, lstrlen( P ), P ) )
        {
            if ( Pn )
                *Pn = ';';
            Res = true;
            break;
        }
        if ( Pn )
        {
            *Pn = ';';
            P = Pn + 1;
        }
        else
            break;
    }
    return Res;
}

void GetRSUPnt ( const char * Filtr, const char * File, const char * TypeObj,
                            CMem<RSU_Obj, 1024, 1024> * pRSU_Pnt)
{
    pRSU_Pnt->Reset();
    char F[1024];
    strcpy ( F, Filtr );
    for ( int n = 0; n < RSU_Pnt.L; n++ )
    {
        RSU_Obj * Obj =  RSU_Pnt.Get( n );
        if ( TypeObj[0] != '*' && strcmp ( Obj->Model, TypeObj ))
            continue;
        if ( File[0] != '*' && strcmp ( Obj->File, File ))
            continue;
        if ( !FiltrName( F, Obj->ObjName.Str ))
            continue;
        RSU_Obj & SEL = pRSU_Pnt->push_back();
        SEL.ObjName.Str = Obj->ObjName.Str;
        SEL.File = Obj->File;
        SEL.Model = Obj->Model;
        SEL.pBase = Obj->pBase;
    }
}

void GetRSUFilesList  ( int *kF, const char * pList[16] )
{
    *kF = kFiles;
    for ( int n = 0; n < kFiles; n++ )
        pList[n] = Files[n].Str;
}

void GetRSUModelsList  ( int *kF, const char * pList[256] )
{
    *kF = kModels;
    for ( int n = 0; n < kModels; n++ )
        pList[n] = Models[n].Str;
}

void AddClass  ( LPCSTR pClass )
{
    for ( int n = 0; n < kModels; n++ )
    {
        if ( strcmp ( pClass, Models[n]) == 0 )
            return;
    }
     Models[kModels++] = pClass;
}

#define MAX_DATA 64
struct MapData
{
  char FileName[32];
  int Size;
  BYTE * Addr;
};

extern MapData mapdata[MAX_DATA];
extern int k_mapdata;
#define countof(array) (sizeof(array)/sizeof(array[0]))
static rsu_cp::RsuClientsHolder g_RsuHolder;
static IBaseModel * RuntimeModels[eRM_COUNT];
int iRuntimeModel = 0;

void Obr_RSU( MapData & data )
    {
    nn.pHeader = (KNoName::SNoNameHeader*)data.Addr;
    BYTE* pMem = (BYTE*)nn.pHeader;
    if ( pMem == NULL )
        return;
    KNoName::SNoNameHeader & header = *nn.pHeader;
    nn.pOrd = (KNoName::SBuffOrd*)(pMem + header.shiftOrds);
    nn.pszString4Ords = pMem + header.shiftBuff4Ords;
    nn.pszObjects = pMem + header.shiftObjects;
    if ( nn.pHeader->nCount <= 0 )
        return;
    Files[kFiles++] = data.FileName;
    DWORD crc = 0;
    const char * pOldClass = "";
    for ( int n = 0; n < nn.pHeader->nCount; n++)
      {
        LPCSTR pClass = nn.Class(nn.pOrd[n],crc);
        if ( strcmp ( pClass, pOldClass ))
            AddClass( pClass );
        pOldClass = pClass;
        LPCSTR pEntry = nn.Entry(nn.pOrd[n],crc);
        CBase* pBase = nn.GetNameData(n+1);
//        W_AICHANNEL * pAI = (W_AICHANNEL*)pBase;
        RSU_Obj & Obj = RSU_Pnt.push_back();
        Obj.ObjName = pEntry;
        Obj.Model = pClass;
        Obj.pBase = pBase;
        Obj.File = Files[kFiles-1].Str;
        Obj.Ref = NULL;
    }
}

RSU_Obj * SortObj[50000];

int CompRSUObj ( const void * p1, const void * p2 )
{
	RSU_Obj * o1 = *(RSU_Obj**)p1;
	RSU_Obj * o2 = *(RSU_Obj**)p2;
	return strcmp ( o1->ObjName.Str, o2->ObjName.Str );
}

int TestRSUObj ( const void * p1, const void * p2 )
{
	const char * Name = (const char *)p1;
	RSU_Obj * o2 =*(RSU_Obj**)p2;
	return strcmp ( Name, o2->ObjName.Str );
}

RSU_Obj * Find_RSU( const char * Name )
{
	Init_RSU();
	RSU_Obj ** pO = (RSU_Obj**)bsearch ( Name, SortObj, RSU_Pnt.L, sizeof(RSU_Obj*), TestRSUObj );
	if ( pO )
		return *pO;
	return NULL;
}
bool  Q_DECL_EXPORT GetRSUVar( const char * Name, const char * Field, BYTE **Addr, eVarType & Type)
{
	RSU_Obj * pObj = Find_RSU( Name );
	if ( pObj == NULL )
		return false;
	KBmBase * blk = (KBmBase*)pObj->pBase;
	USHORT VarSize /*= NULL*/;
	LPCSTR Enum;
	int k = blk->kClassVarInfo;
	return true;
}

void Init_RSU()
  {
  if ( WasInit )
    return;
  WasInit = true;
	 for ( int n = 0; n < k_mapdata; n++ )
		 {
		 Obr_RSU( mapdata[n]);
		 }
	for ( int n = 0; n < RSU_Pnt.L; n++ )
		{
			RSU_Obj * pO = RSU_Pnt.Get( n );
			SortObj[n] = pO;
		}
	qsort ( SortObj, RSU_Pnt.L, sizeof(RSU_Obj*), CompRSUObj );
	RSU_Obj * pO = Find_RSU( "43FI401.43FT401" );
	KKK();
  }

void Q_DECL_EXPORT Load_RSU()
	{
	}

IBaseModel* Load_ObjectRSU( const char * DllName, const char * ObjName);

void LoadRsuModelsCP( )
	{
		char Path[1024];
		Sprintf ( Path, "%sDATA/Acy/Models.csv", PROJECT_ROOT );
		CSplit<3,64> Split;
		if ( !Split.Open(Path ))
		{
			SysMSG( "Файл '%s' не найден ", Path );
			return;
		}
		while( Split.Next() )
		{
			if ( Split.Cols[0][0] == '#' )
				continue;
			ASS( iRuntimeModel < eRM_COUNT );
			IBaseModel* pO = FindBaseModel(Split.Cols[1]);
			if (!pO)
			{
				char * Model = Split.Cols[0];
				char * Obj = Split.Cols[1];
				pO = Load_ObjectRSU( Model, Obj );
			}
			//        ASS(pO);
			if ( pO == NULL )
				continue;
			pO->Model = Split.Cols[0];
			pO->PropsWasRead = strcmp( Split.Cols[2], "Save") == 0 ;
			RuntimeModels[iRuntimeModel++] = pO;
			pO->ModelFlags = Flag_NoAll;
		}
		Split.Close();
	}

	IBaseModel * Load_ObjectEx ( const char * DllName, const char * ObjName );

	IBaseModel* Load_ObjectRSU( const char * DllName, const char * ObjName)
	{
		bool v_ValidLib = false;
		IBaseModel* ret = g_RsuHolder.create_client (DllName, "", ObjName, v_ValidLib );
		if ( !ret && !v_ValidLib)
		//    IBaseModel * ret = Load_ObjectEx ( DllName, ObjName );
		//    if (!ret)
		{
			std::string v_msg = "Load rsu library error: " + g_RsuHolder.get_last_error();
			SysMSG(v_msg.c_str());
		}
		return ret;
	}

