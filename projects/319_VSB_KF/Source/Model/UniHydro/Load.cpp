#include "stdafx.h"
#include "UniHydro.h"
//#include "Properties.h"
#include "Err.h"
#include "Data.h"
#include "CommProc.h"
#include "BaseModel.h"
#ifdef LINUX
#include "WinFiles.h"
#endif

void * Trace = NULL;
//extern char PathDllCat[256];

int CUniHydro::Load_Data( char * File )
  {
  //Trace = Fopen ( "Trace.csv", "wt" );
////////////////////////////////////////
  //Obj2Points.Free();
  //Obj1Points.Free();
  //ObjProps.Free();
  //ObjRefs.Free();
  //Groups.Free();
  //DynCons.Free();
// 
////////////////////////////////////////
//  PROGRESS("Загрузка моделей")
//  PathDllCat[0] = 0;
//  PROPS.Get( "Путь DLL катализатора", PathDllCat );
  int kErr = 0;
  char Path[1024];
  sprintf( Path, "%sDATA\\START\\%s.csv", PROJECT_ROOT, File );
  FILE * F = fopen ( Path, "rt" );
  if ( F == NULL )
    return __LINE__;
  char * ListNames[] = {
  "Описание",
  "Чтение состояния",
  "Чтение параметров",
  "Запись состояния",
  "Описание состояния",
  "Запись параметров",
  "Описание параметров",
  "Системные объекты",
  NULL };
//
  char Shema[256];
  fgets ( Shema, 255, F );
  while ( fgets ( Shema, 255, F ))
    {
    int L = lstrlen ( Shema ) - 1;
    if ( L > 0 )
      Shema[L] = 0;
    char * B = strchr ( Shema, ';' );
    if ( B == NULL )
      break;
    B++;
    char * E = strchr ( B, ';' );
    if ( E )
      *E = 0;
    int NumbName = -1;
    int n = 0;
    while ( ListNames[n] )
      {
      if ( lstrcmp ( B, ListNames[n] ) == 0 )
        {
        NumbName = n;
        break;
        }
      n++;
      }
    if ( NumbName < 0 )
      break;
    if ( lstrcmp ( B, "Системные объекты" ) == 0 )
      {
      B--;
      *B = 0;
      sprintf( MODELS_PATH, "%sINI\\%s.csv", PROJECT_ROOT, Shema );
      }
    }
  int k = 0;
  bool Read = false;
  while ( 1 )
    {
    if ( Read )
      {
      if ( fgets ( Shema, 255, F ) == NULL )
        break;
      }
    Read = true;
    if ( Shema[0] == '#' )
      continue;
    if ( Shema[0] == '#' && Shema[1] == '#' && Shema[2] == '#' )
      break;
    if ( Shema[0] == ';' )
      continue;
    char * P = strchr ( Shema, ';' );
    if ( P )
      *P = 0;
    else
      {
      P = strchr ( Shema, 0x0d );
      if ( P )
        *P = 0;
      else
        P = strchr ( Shema, 0x0a );
      if ( P )
        *P = 0;
      }
    sprintf ( Path, "%sDATA\\MODEL\\%s.dat", PROJECT_ROOT, Shema );
    if ( Load_File( Path, Shema ))
      kErr++;
    }
  fclose ( F );
  return kErr;
  }


static int kObj;



int CUniHydro::Load_File(char * Path, char * CurrShema)
{
#ifndef LINUX
    _HANDLE hFile = CreateFile(Path, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		SysMSG( "Ошибка открытия файла данных '%s'", Path );
		return __LINE__;
	}
	DWORD Key = 0;
	DWORD Lr;
	ReadFile(hFile, &Key, sizeof(Key), &Lr, NULL);
	if(Key != 0x2F437A3A)
	{
		SysMSG("Ошибка структуры файла данных '%s'", Path);
		CloseHandle(hFile);
		return __LINE__;
	}
	ReadFile(hFile, &kObj, sizeof(kObj), &Lr, NULL);
	int kPipe = 0;
	ReadFile(hFile, &kPipe, sizeof(kPipe), &Lr, NULL);
	ReadFile(hFile, &Key, sizeof(Key), &Lr, NULL);
	if(Trace) Fprintf(Trace, "########;%s;%d;\n", CurrShema, kObj);
	////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////
	if(Trace) Fprintf(Trace, "--------;Объекты;%d\n", kObj);
	int ID = 1;
	char Shema[64] = {0};
	sprintf_s(Shema, 64, "%s;", CurrShema);
	for(int n = 0; n < kObj; n++)
	{
		//    Fsend("Trace.txt", "%d\n", n );
		CObjectRef & S = ObjRefs.push_back();
		S.Init();
		S.Numb = ObjRefs.L - 1;
		S.Shema = Shema;
		char ObjName[MAX_OBJ_NAME];
		char Model[MAX_MODEL_NAME];
		char Master[MAX_OBJ_NAME];
		char Data3[MAX_OBJ_NAME];
		ReadFile(hFile, ObjName, sizeof(ObjName), &Lr, NULL );
		ReadFile(hFile, Model, sizeof(Model), &Lr, NULL );
		ReadFile(hFile, Master, sizeof(Master), &Lr, NULL );
		ReadFile(hFile, Data3, sizeof(Data3), &Lr, NULL );
		bool SENS = lstrcmp(Model, "Датчик") == 0;
		if(strstr(Model, "Вызов_пульта"))
			lstrcpy(Model, "Temp");
		//
		S.ObjName = ObjName;
		S.Model = Model;
		S.Master = Master;
		S.Data3 = Data3;
		//    S.Group = false;
		//    S.Point = false;
		ReadFile(hFile, &S.L, 8, &Lr, NULL);
		ReadFile(hFile, &S.R, 8, &Lr, NULL);
		ReadFile(hFile, &S.T, 8, &Lr, NULL);
		ReadFile(hFile, &S.B, 8, &Lr, NULL);
		/////////////////      
		int kPoints;
		DWORD Pos = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
		ReadFile(hFile, &kPoints, sizeof(kPoints), &Lr, NULL);
		if(Trace) Fprintf( Trace, "%s;%s;", ObjName, Model);
		/////////////////      
		for(int p = 0; p < kPoints; p++)
		{
			char Point[MAX_POINT_NAME];
			ReadFile(hFile, Point, sizeof(Point), &Lr, NULL);
			if(Trace) Fprintf(Trace, "%s;", Point);
			if(strncmp(Point, "Row_", 4) == 0) continue;
			CObjectPoint & P = ObjPoints.push_back(); // 
			P.ObjName = ObjName;
			//
			P.PntName = Point;
			P.pObj = NULL;//&S;
			S.Add(&P);
		}
		if(Trace) Fprintf(Trace, "\n" );
		if(S.kPoints != kPoints)
		{
			SysMSG( "Ошибка структуры данных объектов'%s'", Path );
			CloseHandle(hFile);
			return __LINE__;
		}
	}
	Key = 0;
	ReadFile(hFile, &Key, sizeof(Key), &Lr, NULL);
	if(Key != 0x1724FF31)
	{
		SysMSG("Ошибка структуры файла данных '%s'", Path );
		CloseHandle(hFile);
		return __LINE__;
	}
	///////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////
	//  DynCons.Free();
	int kDC;
	ReadFile(hFile, &kDC, sizeof(kDC), &Lr, NULL);
	///////////////////////////////////////////////////////////////////////
	if(Trace) Fprintf(Trace, "--------;Связи;%d\n", kDC);
	CDC_Save Save;
	for(int n = 0; n < kDC; n++)
	{
		ReadFile(hFile, &Save, sizeof(Save), &Lr, NULL);
		CVarPool * pConn = FindCons(Save.Name, Save.Type);
		CPipeRef * pPipe = NULL;
		if(pConn == NULL)
		{
			CVarPool & Pool = VarPool.push_back();
			Pool.Init();
			Pool.Name = Save.Name;
			Pool.Model = Save.Model;
			Pool.Type = Save.Type;
			Pool.SetThisObject();
			pConn = &Pool;
			CPipeRef & Pipe = Pipes.push_back();
			Pipe.PipeName = Save.Name;
			Pipe.Type = Save.Type;
			Pipe.pObject = Pool.pThisObject;
			Pipe.Shema = CurrShema;
			Pipe.pPool = pConn;
			pConn->pPipe = pPipe = &Pipe;
		}
		else
			pPipe = pConn->pPipe;
		//
		if((strncmp(Save.Obj1, "Pixel", 5) != 0 && strncmp(Save.Obj1, "Обрыв", 5) != 0) && Save.Obj1[0])
		{
			CPoolPoint * pPnt = new CPoolPoint();
			pPnt->ObjName = Save.Obj1;
			pPnt->PntName = Save.Ceil1;
			pConn->Add(pPnt);
		}
		if((strncmp(Save.Obj2, "Pixel", 5) != 0 && strncmp(Save.Obj2, "Обрыв", 5) != 0) && Save.Obj2[0])
		{
			CPoolPoint * pPnt = new CPoolPoint();
			pPnt->ObjName = Save.Obj2;
			pPnt->PntName = Save.Ceil2;
			pConn->Add(pPnt);
		}
		if(Save.kXY > 0)
		{
			CSegment * pSeg = pPipe->AddSegment();
			pSeg->kPnt = Save.kXY;
			pSeg->XY = new double[2*pSeg->kPnt];
			ReadFile(hFile, pSeg->XY, 2*sizeof(double) * pSeg->kPnt, &Lr, NULL);
		}
		if(Trace) Fprintf(Trace, "%s;%s(%s);%s(%s);\n", Save.Name, Save.Obj1, Save.Ceil1, Save.Obj2, Save.Ceil2);
		///////////////////////////////////////////
		//////////////////////////////////////////////
	}
	//
	Key = 0;
	ReadFile(hFile, &Key, sizeof(Key), &Lr, NULL);
	if(Key != 0x72AE8126)
	{
		SysMSG("Ошибка структуры файла данных '%s'", Path);
		CloseHandle(hFile);
		return __LINE__;
	}
	CloseHandle(hFile);
	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////
	// Формирование объектов из массива DC
	///  FormObjFromDC( );
	// С этого места труб нет, есть только объекты
	// Переменная идут парами <объект 1>.<точка> (в списке объекта 2) и <объект 2>.<точка> (в списке объекта 1)
	if(Trace) Fclose(Trace);
	//////////////////////////////////////////////////////////////////
#else
  FILE * Trace = NULL;
  ToLinux( Path );
  static int K = 0;
  K++;
  if ( K >= 8 )
    KKK();
  QFile * hFile = new QFile( Path );
  if (!hFile->open(QIODevice::ReadOnly| QIODevice::Unbuffered, QFileDevice::ReadOther ))
    {
    SysMSG( "Ошибка открытия файла данных '%s'", Path );
    CloseHandle( hFile);
    return __LINE__;
    }
  DWORD Key = 0;
  DWORD Lr;
  ReadFile( hFile, &Key, sizeof(Key), &Lr, NULL);
  if(Key != 0x2F437A3A)
    {
    SysMSG("Ошибка структуры файла данных '%s'", Path);
    CloseHandle(hFile);
    return __LINE__;
    }
  ReadFile(hFile, &kObj, sizeof(kObj), &Lr, NULL);
  int kPipe = 0;
  ReadFile(hFile, &kPipe, sizeof(kPipe), &Lr, NULL);
  ReadFile(hFile, &Key, sizeof(Key), &Lr, NULL);
  if(Trace) fprintf(Trace, "########;%s;%d;\n", CurrShema, kObj);
  ////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////
  if(Trace) fprintf(Trace, "--------;Объекты;%d\n", kObj);
  int ID = 1;
  char Shema[64] = {0};
  sprintf_s(Shema, 64, "%s;", CurrShema);
  for(int n = 0; n < kObj; n++)
    {
    //    Fsend("Trace.txt", "%d\n", n );
    CObjectRef & S = ObjRefs.push_back();
    S.Init();
    S.Numb = ObjRefs.L - 1;
    S.Shema = Shema;
    char ObjName[MAX_OBJ_NAME*2];
    char Model[MAX_MODEL_NAME*2];
    char Master[MAX_OBJ_NAME*2];
    char Data3[MAX_OBJ_NAME*2];
    ReadFile(hFile, ObjName, MAX_OBJ_NAME, &Lr, NULL );
    ReadFile(hFile, Model, MAX_MODEL_NAME, &Lr, NULL );
    ReadFile(hFile, Master, MAX_OBJ_NAME, &Lr, NULL );
    ReadFile(hFile, Data3, MAX_OBJ_NAME, &Lr, NULL );
    ToUTF8( ObjName, MAX_OBJ_NAME * 2 );
    ToUTF8( Model, MAX_OBJ_NAME * 2 );
    ToUTF8( Master, MAX_OBJ_NAME * 2 );
    ToUTF8( Data3, MAX_OBJ_NAME * 2 );
    bool SENS = lstrcmp(Model, "Датчик") == 0;
    if(strstr(Model, "Вызов_пульта"))
      lstrcpy(Model, "Temp");
    //
    S.ObjName = ObjName;
    S.Model = Model;
    S.Master = Master;
    S.Data3 = Data3;

    //    S.Group = false;
    //    S.Point = false;
    ReadFile(hFile, &S.L, 8, &Lr, NULL);
    ReadFile(hFile, &S.R, 8, &Lr, NULL);
    ReadFile(hFile, &S.T, 8, &Lr, NULL);
    ReadFile(hFile, &S.B, 8, &Lr, NULL);
    /////////////////
    if ( strstr ( ObjName, "Сырье печи к змеевикам" ) )
        KKK();
    int kPoints;
//    DWORD Pos = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
    ReadFile(hFile, &kPoints, sizeof(kPoints), &Lr, NULL);
    if(Trace) Fprintf( Trace, "%s;%s;", ObjName, Model);
    /////////////////
    for(int p = 0; p < kPoints; p++)
    {
      char Point[MAX_POINT_NAME*2];
      ReadFile(hFile, Point, MAX_POINT_NAME, &Lr, NULL);
      ToUTF8( Point, MAX_POINT_NAME * 2 );
      if(Trace) Fprintf(Trace, "%s;", Point);
      if(strncmp(Point, "Row_", 4) == 0) continue;
      CObjectPoint & P = ObjPoints.push_back(); //
      P.ObjName = ObjName;
      //
      P.PntName = Point;
      P.pObj = NULL;//&S;
      S.Add(&P);
    }
    if(Trace) Fprintf(Trace, "\n" );
    if(S.kPoints != kPoints)
    {
      SysMSG( "Ошибка структуры данных объектов'%s'", Path );
      CloseHandle(hFile);
      return __LINE__;
    }
  }
  Key = 0;
  ReadFile(hFile, &Key, sizeof(Key), &Lr, NULL);
  if(Key != 0x1724FF31)
  {
    SysMSG("Ошибка структуры файла данных '%s'", Path );
    CloseHandle(hFile);
    return __LINE__;
  }
  ///////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////
  //  DynCons.Free();
  int kDC;
  ReadFile(hFile, &kDC, sizeof(kDC), &Lr, NULL);
  ///////////////////////////////////////////////////////////////////////
  if(Trace) Fprintf(Trace, "--------;Связи;%d\n", kDC);
  CDC_Save Save;
  for(int n = 0; n < kDC; n++)
    {
    ReadFile(hFile, &Save, sizeof(Save), &Lr, NULL);
    CVarPool * pConn = FindCons( UTF( Save.Name ), Save.Type );
    CPipeRef * pPipe = NULL;
    if(pConn == NULL)
      {
      CVarPool & Pool = VarPool.push_back();
      Pool.Init();
      Pool.Name = UTF( Save.Name );
      Pool.Model = UTF( Save.Model );
      Pool.Type = Save.Type;
      Pool.SetThisObject();
      pConn = &Pool;
      CPipeRef & Pipe = Pipes.push_back();
      Pipe.PipeName = UTF( Save.Name );
      Pipe.Type = Save.Type;
      Pipe.pObject = Pool.pThisObject;
      Pipe.Shema = CurrShema;
      Pipe.pPool = pConn;
      pConn->pPipe = pPipe = &Pipe;
    }
    else
      pPipe = pConn->pPipe;
    //
    char Obj1[256];
    CharToUTF8( Obj1, 256, Save.Obj1 );
    char Ceil1[256];
    CharToUTF8( Ceil1, 256, Save.Ceil1 );
    char Obj2[256];
    CharToUTF8( Obj2, 256, Save.Obj2 );
    char Ceil2[256];
    CharToUTF8( Ceil2, 256, Save.Ceil2 );
 //
    if((strstr( Obj1, "Pixel") == 0 && strstr( Obj1, "Обрыв") == 0) && Obj1[0])
    {
      CPoolPoint * pPnt = new CPoolPoint();
      pPnt->ObjName = Obj1;
      pPnt->PntName = Ceil1;
      pConn->Add(pPnt);
    }
    if((strstr( Obj2, "Pixel") == 0 && strstr( Obj2, "Обрыв") == 0) && Obj2[0])
    {
      CPoolPoint * pPnt = new CPoolPoint();
      pPnt->ObjName = Obj2;
      pPnt->PntName = Ceil2;
      pConn->Add(pPnt);
    }
    if(Save.kXY > 0)
      {
      CSegment * pSeg = pPipe->AddSegment();
      pSeg->kPnt = Save.kXY;
      pSeg->XY = new double[2*pSeg->kPnt];
      ReadFile(hFile, pSeg->XY, 2*sizeof(double) * pSeg->kPnt, &Lr, NULL);
    }
    if(Trace) Fprintf(Trace, "%s;%s(%s);%s(%s);\n", Save.Name, Save.Obj1, Save.Ceil1, Save.Obj2, Save.Ceil2);
    ///////////////////////////////////////////
    //////////////////////////////////////////////
  }
  //
  Key = 0;
  ReadFile(hFile, &Key, sizeof(Key), &Lr, NULL);
  if(Key != 0x72AE8126)
  {
    SysMSG("Ошибка структуры файла данных '%s'", Path);
    CloseHandle(hFile);
    return __LINE__;
  }
  CloseHandle(hFile);
  //////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////
  // Формирование объектов из массива DC
  ///  FormObjFromDC( );
  // С этого места труб нет, есть только объекты
  // Переменная идут парами <объект 1>.<точка> (в списке объекта 2) и <объект 2>.<точка> (в списке объекта 1)
  if(Trace) Fclose(Trace);
  //////////////////////////////////////////////////////////////////

#endif	///
	return 0;
}

CVarPool * CUniHydro::FindCons(char * Name, int Type)
{
	for(int n = 0; n < VarPool.L; n++)
	{
		CVarPool & Pool = VarPool[n];
		if ( Pool.Name == Name && Pool.Type == Type)
			return &Pool;
    }
	return NULL;
}

extern CMem<CObjectRef,256,256> * pObjRefs;
int CompObjByName(const void * p1, const void * p2);
extern CMem<CVarPool,256,256> * pVarConn;
int CompConnByName(const void * p1, const void * p2);
extern CMem<CObjectPoint,512,256> * pObjPoints;
int CompObjPoints(const void * p1, const void * p2);
int FindObjPoints(const void * p1, const void * p2);

//#define ONLY_LOAD
#ifdef LINUX
static int FindObjByName ( const void * p1, const void * p2 )
{
  char * n1 = (char*)p1;
  int n2 = *(int*)p2;
  CObjectRef & Obj2 = (*pObjRefs)[n2];
  return lstrcmp ( n1, Obj2.ObjName );
}
#endif

int CUniHydro::FormStruct()
{
// Сортируем ObjRefs, VarPool
	pObjRefs = &ObjRefs;
	pVarConn = &VarPool;
	pObjPoints = &ObjPoints;
// Сортировка объектов
	int kErr = 0;
	SortObjByName = new int[ObjRefs.L];
	for(int n = 0; n < ObjRefs.L; n++)
	{
		SortObjByName[n] = n;
	}
	qsort(SortObjByName, ObjRefs.L, sizeof(int), CompObjByName);
// Сортировка связей
	SortPoolByName = new int[VarPool.L];
	for(int n = 0; n < VarPool.L; n++)
	SortPoolByName[n] = n;
	qsort ( SortPoolByName, VarPool.L, sizeof(int), CompConnByName);
////////////////////////////////////////////////////
	SortObjPoints = new int[ObjPoints.L];
	for(int n = 0; n < ObjPoints.L; n++)
		SortObjPoints[n] = n;
	qsort(SortObjPoints, ObjPoints.L, sizeof(int), CompObjPoints);
//	Сжатие списков
	CompressObjList();
////////////////////////////////////////////////////
	for(int n = 0; n < ObjPoints.L; n++)
	{
		CObjectPoint & P = ObjPoints[n];
        int * pN = (int*)bsearch(P.ObjName, SortObjByName, ObjRefs.L, sizeof(int), FindObjByName );
		ASS(pN)
		if(pN == NULL) continue;
		int N = *pN;
		CObjectRef & O = ObjRefs[N];
		P.pObj = &O;
	}
////////////////////////////////////////////////////
	for(int n = 0; n < ObjRefs.L; n++)
	{
		int N = SortObjByName[n];
		CObjectRef & Obj = ObjRefs[N];
		Obj.pObject = Load_Obj(Obj.ObjName, Obj.Model);
#ifndef ONLY_LOAD
		if(Obj.pObject == NULL)
		{
			kErr++;
			SysMSG("Ошибка загрузки '%s'", (char*)Obj.Model);
			continue;
		}
		Obj.pObject->Data3 = Obj.Data3;
		Obj.pObject->pFirstProp = Obj.pFirstProp;
		if(Obj.pObject->pUniModel)
		{
			if(ObrUniModelPoints(Obj))
				kErr++;
			continue;
		}
//??    SysMSG("Ошибочный тип модели '%s' (не UNIMODEL)", (char*)Obj.Model );
#endif
	}
#ifdef ONLY_LOAD
	exit(0);
#endif
//	Переменные и группы
//	связь точек объектов и труб
	for(int n = 0; n < VarPool.L; n++)
	{
		CVarPool & Pool = VarPool[n];
		Pool.nGroup = -1;
		CPoolPoint * pPnt = Pool.pFirstPoolPnt;
		Pool.kPoints = 0;
		while(pPnt)
		{
			int * pN = (int*)bsearch(pPnt, SortObjPoints, ObjPoints.L, sizeof(int), FindObjPoints);
			ASS(pN)
			if(pN == NULL)
			{
				pPnt = pPnt->pNext;
				continue;
			}
			int N = *pN;
			CObjectPoint & ObjPnt = ObjPoints[N];
 ///!!!!! ObjPnt <-> pPnt
			Pool.Add(&ObjPnt);
			ObjPnt.pPool = &Pool;
			pPnt = pPnt->pNext;
		}
	}
	CompressConnectionList();
/////////////////////////////////////////////////////////////////////////////
// создание групп
// 0 группа - все пулы, в которых не строятся матрицы
// у этой групы на каждом шагу по времени вызываются модули управления пулом,
// которые в определённом (для каждого типа своём) порядке
// вызывают SetVar объектов
	CGroupRef & Grp2 = Groups.push_back();
	Grp2.Init();
	Grp2.Numb = 0;
	for(int n = 0; n < VarPool.L; n++)
	{
		CVarPool & Pool = VarPool[n];
		if(!Pool.HaveMatr())
		{
			Grp2.Add(&Pool);
			Pool.nGroup = 0;
			CObjectPoint * pPnt = Pool.pFirstPoint;
			while(pPnt)
			{
// объекты относятся к данному пулу и группе
				CObjectRef * pObj = pPnt->pObj;
				CGroupObject & GrpObj = Grp2.Add(pObj, pPnt);
				GrpObj.Add(pPnt);
				pPnt = pPnt->pNextPoolPoint;
			}
		}
	}
// остальные группы включают все пулы, в которых строятся матрицы
// линейных уравнений относительно скалярных переменных
// скалярные переменные объединённы в структуры
	int kIter = 0;
	int kPool = 0;
	int NumbPool[1024] = {0};
#if (!(OLD_CALC_MODE >> 0 & 1))
	CGroupRef & Grp = Groups.push_back();
	Grp.Init();
#endif
	while(kIter++ < 1000)
	{
		for(int n = 0; n < VarPool.L; n++)
		{
			CVarPool & Pool = VarPool[n];
			if(!Pool.HaveMatr()) continue;
			if(Pool.nGroup < 0)
			{
				NumbPool[kPool++] = n;
				break;
			}
		}
	if(kPool <= 0) break;
// новая группа.
#if (OLD_CALC_MODE >> 0 & 1)
	CGroupRef & Grp = Groups.push_back();
	Grp.Init();
#endif
	Grp.Numb = Groups.L - 1;
	while(kPool > 0)
	{
		int nPool = NumbPool[kPool-1];
		kPool--;
		CVarPool & Pool = VarPool[nPool];
		Grp.Add(&Pool);
		Pool.nGroup = Grp.Numb;
		if(Pool.Type != eГидравлика) KKK();
// перебираем переменные VarPool
// они относятся к текущей группе
		CObjectPoint * pPnt = Pool.pFirstPoint;
		while(pPnt)
		{
// объекты относятся к данному пулу и группе
			CObjectRef * pObj = pPnt->pObj;
			CGroupObject & GrpObj = Grp.Add( pObj, pPnt );
			GrpObj.Add(pPnt);
// подключение других пулов к данной группе
// перебираем переменные объекта, относящиеся к той же внутренний группе PntGroup
			int PntGroup = pPnt->PntGroup;
			CObjectPoint * pOtherPnt = pObj->pFirstPoint;
			while(pOtherPnt)
			{
				if(pOtherPnt == pPnt || pOtherPnt->PntGroup != PntGroup)
				{
					pOtherPnt = pOtherPnt->pNext;
					continue;
				}
				CVarPool * pPool = pOtherPnt->pPool;
				if(pPool && pPool ->nGroup < 0)
				{
					ASS(kPool < 1024)
					NumbPool[kPool++] = pPool->Numb;
					pPool->nGroup = Grp.Numb;
				}
				pOtherPnt = pOtherPnt->pNext;
			}
			pPnt = pPnt->pNextPoolPoint;
		}
	}
}

 /////////////////////////////////////////////////////////
	return kErr;
}

void CUniHydro::Prepare()
{
	CGroupRef & Grp = Groups[0]; 
	kObjects = Grp.Objects.L;
	Grp.kObjects = kObjects;
	Grp.PrePare();
	for(int nGr = 1; nGr < Groups.L; nGr++)
	{
		CGroupRef & Grp = Groups[nGr]; 
		Grp.PrePare();
		kObjects += Grp.Objects.L;
	}
}

void CGroupRef::PrePare()
{
// вызываем встроенную функцию пулов
// создаются структуры CBaseStruct
// сохраняются номера переменных
	CVarPool * pPool = pFirstPool;
        if ( strstr ( pPool->Name, "10294" ))
            KKK();
        if ( strstr ( pPool->Name, "М_P_3002S" ))
            KKK();

    kVar = 0;
	while(pPool)
	{
		CGroupObject & Obj = Objects.push_back(); // объект для формирования матриц Pipe, ...
		Obj.Init();
		Obj.pGroup = this;
		Obj.MainPool = true;
		CObjectPoint * pPnt = pPool->pFirstPoint;
		while(pPnt)
		{
			Obj.AddPntToPool(pPnt);
			pPnt = pPnt->pNextPoolPoint;
		}
		ASS(pPool->pThisObject && pPool->pThisObject->pPoolCtrl)
		pPool->FirstGroupNumbVar = kVar;
		pPool->pThisObject->pPoolCtrl->InitPool(pPool, &Obj);
		kVar += pPool->kScalarVar;
		pPool = pPool->pNextPool;
	}
	kObjects = Objects.L;
	kScalarVariables = 0;
	for(int n = 0; n < Objects.L; n++)
	{
		CGroupObject & Obj = Objects[n];
		Obj.Prepare();
		kScalarVariables += Obj.kLinearVar;
    }
	if(kScalarVariables <= 0) return;
	OptOrd = NewArr(int,kObjects);
	nStepStart = NewArr(int,kScalarVariables); // Номер шага включения переменной
	nStepEnd = NewArr(int,kScalarVariables);    // Номер шага исключения переменной
	// 
	ActiveSet = new int[kScalarVariables]; // Текущий набор переменных
	memset(ActiveSet, 0, kScalarVariables << 2);
	int kActiveSetr = 0; 
	ObjUseVars = new int[kScalarVariables];     // Сколько раз данная переменная еще будет использована
	memset(ObjUseVars, 0, 4 * kScalarVariables);
//////////////////////////////////////////////
	int * NoOpt = new int[kObjects];
	for ( int n = 0; n < kObjects; n++ )
	{
		CGroupObject & Obj = Objects[n];
		for ( int p = 0; p < Obj.kLinearVar; p++ )
		{
			int nVar = Obj.NumbVarInGroup[p];
			ObjUseVars[nVar]++;
		}
		NoOpt[n] = n;
	}
	int kNoOpt = kObjects;
	///////////////////////////// Начинаем с 1-го
	int nObj = 0;
	int kOptOrd = 0;
	CGroupObject & Obj = Objects[nObj];
	Obj.Order = kOptOrd;
	OptOrd[kOptOrd++] = nObj;
	kCurrVars = 0;
	kEQ = 0;
	MaxEQ = 0;
	MaxVAR = 0;
	MemSave = 0;
	Step = 1;
	NoOpt[0] = NoOpt[kNoOpt-1];
	AddObject(Obj.kLinearVar, Obj.NumbVarInGroup, Obj.kLinearEq, kNoOpt == 1);
	kNoOpt--;
	while ( kNoOpt > 0 )
	{
		Step++;
		int nMin = -1;
		int DeltaMin = 10000;
		for ( int n = 0; n < kNoOpt; n++ )
		{
			int nObj = NoOpt[n];
			CGroupObject & Obj = Objects[nObj];
			// Новый объект должен быть связян с активным набором
			if ( HaveCommVar ( Obj.kLinearVar, Obj.NumbVarInGroup ))
			{
				int kDelete = 0; 
				int kAdd = 0;
				UseVars ( Obj.kLinearVar, Obj.NumbVarInGroup, kDelete, kAdd  );
				int Delta = kAdd - kDelete;
				if ( Delta < DeltaMin )
				{
					nMin = n;
					DeltaMin = Delta;
				}
			}
		}
		if(nMin < 0)
		{
			// Не получилось - повторим
			for ( int n = 0; n < kNoOpt; n++ )
			{
				int nObj = NoOpt[n];
				CGroupObject & Obj = Objects[nObj];
				int kDelete = 0; 
				int kAdd = 0;
				UseVars(Obj.kLinearVar, Obj.NumbVarInGroup, kDelete, kAdd);
				int Delta = kAdd - kDelete;
				if(Delta < DeltaMin)
				{
					nMin = n;
					DeltaMin = Delta;
				}
			}
		}
		ASS( nMin >= 0 )
		int nObjMin = NoOpt[nMin];
		/// Выбираем nObjMin
		CGroupObject & Obj = Objects[nObjMin];
		AddObject ( Obj.kLinearVar, Obj.NumbVarInGroup, Obj.kLinearEq, kNoOpt == 1 );
		OptOrd[kOptOrd++] = nObjMin;
		NoOpt[nMin] = NoOpt[kNoOpt-1];
		kNoOpt--;
		///
	}
	StepVar_Beg = NewArr(CStepActionVar,kScalarVariables);
	StepVar_End = NewArr(CStepActionVar,kScalarVariables);
	for(int n = 0; n < kScalarVariables; n++)
	{
		StepVar_Beg[n].Step = nStepStart[n];
		StepVar_Beg[n].Var = n;
		StepVar_End[n].Step = nStepEnd[n];
		StepVar_End[n].Var = n;
	}
	int SortByStep(const void * p1, const void * p2);
	qsort(StepVar_Beg, kScalarVariables, sizeof(CStepActionVar), SortByStep);
	qsort(StepVar_End, kScalarVariables, sizeof(CStepActionVar), SortByStep);
	A = NewArr(double, kScalarVariables * ( MaxEQ + 4 ));
	B = NewArr(double, MaxEQ + 4);
	X = NewArr(double,kScalarVariables);
	Pos_in_CurrVars = NewArr(int,kScalarVariables);
	CurrVars = NewArr(int,kScalarVariables);
	State_Var = NewArr(int,kScalarVariables);
	Numb_Var = NewArr(int,kScalarVariables+1);
	BegSt = NewArr(int,kScalarVariables+1);
	pNumb = NewArr(int,MemSave); 
	pVar = NewArr(double,MemSave);
	PosInSt = 0;
	ShowErrors = true;
#if (!(OLD_CALC_MODE >> 0 & 1))
	b_test = new double[kVar];
	x_test = new double[kVar];
	amount_eq = kVar;
	cont = true;
	nnzMaybe = amount_eq * 20;
	Arow = new int[nnzMaybe];
	Acol = new int[nnzMaybe];
	Aval = new double[nnzMaybe];
	ss_test = new LUsolver(amount_eq);
	std::memset(Arow, 0, sizeof(int) * nnzMaybe);
	std::memset(Acol, 0, sizeof(int) * nnzMaybe);
	std::memset(Aval, 0, sizeof(double) * nnzMaybe);
	ss_test = new LUsolver(amount_eq);

	t_eq = new int[kObjects + 1];
	std::memset(t_eq, 0, sizeof(int) * (kObjects + 1)) ;

	for ( int nObj = 0; nObj < kObjects; nObj++ )
	{
		int nOpt = OptOrd ? (OptOrd[nObj])  : nObj;   
		CGroupObject & Obj = Objects[nOpt];
		int kScalarVariables = Obj.kLinearVar;
		int kEq =  Obj.kLinearEq;
		if (this->kVar != 0)
		{
			int N_obj = Obj.kLinearEq;
			t_eq[nObj + 1] = t_eq[nObj] + N_obj;
		}
	}
#endif
}

void CGroupRef::UseVars (  int kPoints, int NumbVar[], int & kDelete, int & Add  )
  {
  kDelete = 0;
  Add = 0;
  if ( NumbVar == NULL )
    return;
  for ( int n = 0; n < kPoints; n++ )
    {
    int N = NumbVar[n];
    if ( ActiveSet[N] )
      {
      if ( ObjUseVars[N] == 1 )
        kDelete++;
      }
    else
      Add++;
    }
  }

bool CGroupRef::HaveCommVar ( int kPoints, int NumbVar[] )
  {
  if ( NumbVar == NULL )
    return true;
  for ( int n = 0; n < kPoints; n++ )
    {
    int N = NumbVar[n];
    if ( ActiveSet[N] )
      return true;
    }
  return false;
  }

void CGroupRef::AddObject ( int kPoints, int NumbVar[], int kObjEq, bool LastStep )
  {
  int kDelete = 0;
  int kAdd = 0;
  if ( NumbVar == NULL )
    return;
  // Сначала все добавляем
  for ( int n = 0; n < kPoints; n++ )
    {
    int N = NumbVar[n];
    if ( ActiveSet[N] != 2 )
      {
      if ( ActiveSet[N] == 0 )
        {
        ASS( ObjUseVars[N] > 0 )//Если она добавляется, то по крайней мере 1 раз должна использоваться
        }
      ObjUseVars[N]--; 
      nStepStart[N] = Step;
      kAdd++;
      ActiveSet[N] = 1; // Переменная только что добавлена
      }
    }
  kCurrVars += kAdd;
  if ( kCurrVars > MaxVAR )
    MaxVAR = kCurrVars;
  kEQ += kObjEq;
  if ( kEQ > MaxEQ )
    MaxEQ = kEQ;
  // Потом - убираем
  for ( int n = 0; n < kPoints; n++ )
    {
    int N = NumbVar[n];
    if ( ActiveSet[N] == 2 || LastStep || ObjUseVars[N] == 0 )
      {
      if ( ObjUseVars[N] > 0 )
        ObjUseVars[N]--; 
      if ( ObjUseVars[N] == 0 )
        {
        MemSave += kCurrVars + 1;
        kCurrVars--;
        kEQ--;
        ActiveSet[N] = 0;
        nStepEnd[N] = Step;
        kDelete++;
        }
      }
    }
  for ( int n = 0; n < kPoints; n++ )
    {
    int N = NumbVar[n];
    if ( ActiveSet[N] == 1 )
      ActiveSet[N] = 2;
    }
  ///////////////////////
  }

int CompPoints ( const void * p1, const void * p2 )
  {
  CObjectPoint ** ppPtn1 = (CObjectPoint**)p1;
  CObjectPoint ** ppPtn2 = (CObjectPoint**)p2;
  if ( (*ppPtn1)->NumbInGroup > (*ppPtn2)->NumbInGroup )
    return 1;
  if ( (*ppPtn1)->NumbInGroup < (*ppPtn2)->NumbInGroup )
    return -1;
  ASS(0)
  return 0;
  }

void CGroupObject::Prepare()
  {
  if ( pObject && pObject->pPoolCtrl )
    {
    // контроллер пулов
    CObjectPoint * pPnt = pFirstPoolPoint;
    kPoints = 0;
    while ( pPnt )
      {
      kPoints++;
      pPnt = pPnt->pNextPoolPoint;
      }
    //
    pPoints = NewArr(CObjectPoint*,kPoints);
    int nPnt = 0;
    pPnt = pFirstPoolPoint;
    while ( pPnt )
      {
      pPoints[nPnt++] = pPnt;
      pPnt = pPnt->pNextPoolPoint;
      }
    ASS(nPnt == kPoints )
    return;
    }
  CObjectPoint * pPnt = pFirstGroupPoint;
  kPoints = 0;
  while ( pPnt )
    {
    kPoints++;
    pPnt = pPnt->pNextGroupPoint;
    }
//
  pPoints = NewArr(CObjectPoint*,kPoints);
  kPointsVar = NewArr(int,kPoints);
  int nPnt = 0;
  pPnt = pFirstGroupPoint;
  while ( pPnt )
    {
    pPoints[nPnt++] = pPnt;
    pPnt = pPnt->pNextGroupPoint;
    }
  ASS(nPnt == kPoints )
  if ( kPoints >= 2 )
    {
    qsort( pPoints, kPoints, sizeof ( CObjectPoint *), CompPoints );
    }
  kLinearVar = 0;
  kLinearEq = 0;
  for ( int n = 0; n < kPoints; n++ )
    {
    CObjectPoint * pPnt = pPoints[n];
    kPointsVar[n] = pPnt->kScalarVar;
    kLinearVar += kPointsVar[n];
    kLinearEq += pPnt->kScalarEq;
    }
  NumbVarInGroup = NULL;
  AddrVar = NULL;
  if ( kLinearVar > 0 )
    {
    NumbVarInGroup = NewArr(int,kLinearVar);     // номера переменных в массиве переменных группы
    AddrVar = NewArr(double*,kLinearVar); // абсолютные адреса переменных
    }
  int nLinearVar = 0;
  for ( int n = 0; n < kPoints; n++ )
    {
    CObjectPoint * pPnt = pPoints[n];
    int kPointVar = pPnt->kScalarVar;
    memmove ( NumbVarInGroup + nLinearVar, pPnt ->GroupNumbVar, kPointVar * sizeof ( int ) );
    memmove ( AddrVar + nLinearVar, pPnt->AddrVar, kPointVar * sizeof ( double * ) );
    nLinearVar += kPointVar;
    }
  /////////////////////////////////////////////////////
  l_A = kLinearEq * kLinearVar;
  A = NewArr ( double, l_A ); // Ур-я имеют вид A * X = B  
  Ind_A = NewArr ( int, l_A );
  B = NewArr ( double, kLinearEq );                                
  IsMatrInd = false;
///
  }
