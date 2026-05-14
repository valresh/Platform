#ifndef HYDROBASICTYPES_H
#define HYDROBASICTYPES_H
#include "CommProc.h"

#define MAX_OBJ_NAME 64
#define MAX_MODEL_NAME 32
#define MAX_POINT_NAME 32

enum ePipeType
{
  eНет,
  eГидравлика,
  eЛогика,
  eУправление,
  eTransfer,
  eConnection,
  eMotor,
  eSignal,
  ePipeFlow,
  eObject,
  eContact,
  eCatalyst
};

// точки, привязанные к объекту
struct CObjectPoint
{
    CStr ObjName;
    CStr PntName;
    ePipeType Type;
    int PntGroup;     // Внутренняя группа
    int NumbInGroup;  // Положение в группе
    char IO;
    //
    struct CBaseStruct * pVar;
    int kScalarVar;
    int * GroupNumbVar;
    double ** AddrVar;
    int kScalarEq;

    struct CObjectRef * pObj;
    struct CVarPool * pPool;
    CObjectPoint * pNext;
    CObjectPoint * pNextPoolPoint;
    CObjectPoint * pNextGroupPoint;
};

struct CObjProps
{
    CStr PropName;
    CStr Value;
    struct IBaseModel * pPipe;
    CObjProps * pNext;
};

struct CObjectRef
{
    CStr ObjName;
    CStr Model;
    CStr Shema;
    CStr Master;
    CStr Data3;
    int Numb;
    int kPoints;
    int kProps;
    int Flags;
    int nGroup;
    int PntGroup;
    double X, Y;
    double L, R, T, B;
    bool Selected;
    bool InPath;
    struct CObjectPoint * pFirstPoint;
    struct CObjectPoint * pLastPoint;
    struct CObjProps * pFirstProp;
    struct CObjProps * pLastProp;
    struct CObjectRef * pMain;
    struct IBaseModel * pObject;
    struct CGroupObject * pGrp;
    void Init()
    {
      kPoints = 0;
      kProps = 0;
      pFirstPoint = NULL;
      pLastPoint = NULL;
      pFirstProp = NULL;
      pLastProp = NULL;
      pObject = NULL;
      pGrp = NULL;
      Selected = false;
      pMain = this;
      ObjName.Str = NULL;
      Model.Str = NULL;
      Shema.Str = NULL;
      Data3.Str = NULL;
    }
    void Add(struct CObjectPoint * pP)
    {
      if(pFirstPoint == NULL)
        pFirstPoint = pP;
      if(pLastPoint)
        pLastPoint->pNext = pP;
      pLastPoint = pP;
      pP->pNext = NULL;
      kPoints++;
    }
    void Add(struct CObjProps * pP)
    {
      if(pFirstProp == NULL)
        pFirstProp = pP;
      if(pLastProp)
        pLastProp->pNext = pP;
      pLastProp = pP;
      pP->pNext = NULL;
      kProps++;
    }
    void operator = (CObjectRef & From);
    char * NameCoord();
};

struct CSegment
{
    int kPnt;
    double * XY;
    CSegment * pNext;
    NEW
    CSegment()
    {
      kPnt = 0;
      XY = NULL;
      pNext = NULL;
    }
    ~CSegment()
    {
      if(XY) delete XY;
      XY = NULL;
    }
};

struct CPipePoint
{
    int N_Obj;
    //std::string Obj;
    //std::string  Point;
    CStr Obj;
    CStr Point;
    char TypeF;
    //  double XY[4];
    IBaseModel * pObject;
    CPipePoint * pNext;
    CPipePoint()
    {
      N_Obj = -1;
      Obj.Str = NULL;
      Point.Str = NULL;
      TypeF = ' ';
      //  double XY[4];
      pObject = NULL;
      pNext = NULL;
    }
};

struct CPipeRef
{
    CStr PipeName;
    CStr Shema;
    ePipeType Type;
    int kPoints;
    CPipePoint * pFirstPoint;
    CPipePoint * pLastPoint;
    int kSeg;
    CSegment * pFirstSeg;
    CSegment * pLastSeg;
    CVarPool * pPool;
    IBaseModel * pObject;
    bool Selected;
    bool Break;
    CSegment * AddSegment();
    void Init()
    {
      Break = false;
      Selected = false;
      kPoints = 0;
      pFirstPoint = NULL;
      pLastPoint = NULL;
      kSeg = 0;
      pFirstSeg = NULL;
      pLastSeg = NULL;
      pObject = NULL;
      pPool = NULL;
    }
    void Add(CPipePoint * pP)
    {
      if(pFirstPoint == NULL)
        pFirstPoint = pP;
      if(pLastPoint)
        pLastPoint->pNext = pP;
      pLastPoint = pP;
      pP->pNext = NULL;
      kPoints++;
    }
    void Add(CSegment * pS)
    {
      if(pFirstSeg == NULL)
        pFirstSeg = pS;
      if(pLastSeg)
        pLastSeg -> pNext = pS;
      pLastSeg = pS;
      pS->pNext = NULL;
      kSeg++;
    }
};

struct CDataPtr
{
    DWORD kPipeRef;
    struct CPipeRef ** pPipeRef;

    DWORD kObjPoint;
    struct CObjectPoint ** pObjPoint;

    DWORD kObjRef;
    struct CObjectRef ** pObjRef;
};

struct CDC_Save
{
    char Name[64];
    char Model[64];
    ePipeType Type;
    char Obj1[128];
    char Master1[64];
    char Ceil1[32];
    char Obj2[128];
    char Master2[64];
    char Ceil2[32];
    int kXY;
};
#endif // HYDROBASICTYPES_H
