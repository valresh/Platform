#include "SysDataTypes.h"


char * CSystem::ClassName = "Модель";

int CSystem::Init()
  {
  INIT(CSystem)
  //
	dModelT = 0.;
	CLEAR(kCall) 
	CLEAR(hThread)
	CLEAR(ThreadId)
    CLEAR(Used)
	T2 = 0;
	dt = 1e-4;
	m_nStep = 0;
	Accel = 0.;
	Time_real = 0.;
	StepCalcAccel = 50;
	bSaveParams = false;
	bRestoreParams = false;
	bSaveState = false;
	bRestoreState = false;
    bWorkReg = true;
    bInit_0 = false;
	Debug = false;
	nSave = 0;
	CLEAR(Mod_Name)
	CLEAR(FileWriteState)
	CLEAR(FileWriteParams)
	CLEAR(FileReadState)
	CLEAR(FileReadParams)
	StartStatus = STATUS_UNKNOWN;
	AccelZ = 10.;
//	Time_Sample = 1.;	// по умолчанию 1 час
//	CLEAR(EventLogFilePath);
	IsPumpSbros = false;
	IsBlk = false;
//	bSirena = false;
  startTime = 0;//Для отображения в АРМ-ах модельного времени
//	CLEAR(FileBlk);
//	CLEAR(FileView)
//	nView = 0;
  bNoPCYWork = false;
  ShowParamsLevel = Основной;
//	IsBlkWork = true;
//  AutoReg = false;
 // CtrlStab = false;

  nEvents = 0;// Количество записей в Events.dat
  nTrends = 0;// Количество записей в Trends.dat
 	nMsgToOperator = 0;// Количество записей в MsgToOperator.dat
//	Offset_FirstDefect = 0;
	UseProcessors = 1;
	bModelPause = false;
  nAppRun = 0;
//
	DbgProcessorFlag = 0;
	DbgResetFlag = 0;
	IdDbg = 0;
  return 0;
	}

#undef   SHIFT1
#define  SHIFT1(Var,Type) (DWORD_PTR)(&hs.Var)-(DWORD_PTR)(&hs), \
  (DWORD_PTR)(&sh.Var)-(DWORD_PTR)(&sh), Type, TypeID
#undef   SHIFT2
#define  SHIFT2(Var,Type) (DWORD_PTR)( hs.Var)-(DWORD_PTR)(&hs), \
                                                    (DWORD_PTR)( sh.Var)-(DWORD_PTR)(&sh), Type, TypeID
void CSystem::BuildList()
  {
  CSystem hs; CShSystem sh;
  SValueDef list[] =
    {
    { "ACCEL", SHIFT_OBJ(AccelZ), enumValueDbl },
    };
//##  FILL_VALUE
  }

int CSystem::SaveState( )
  {
  T2 = dModelT;
  ASS(pSRFile->WStruct("dt", sizeof(dt), &dt));
  return 0;
  }

int CSystem::RestoreState( char * StrName )
{
	if (!lstrcmp (StrName, "dt"))
	{
		ASS(pSRFile->RStruct( sizeof (dt), &dt));
		return 0;
	}
	ASS(FALSE)
		return 1;
}

#define PARM_STRUCT
#include "IO_Parms.h"

int CSystem::GetParams(  )
  {
	PARM(dModelT,"#Время")//0
	PARM(dt,"#Шаг по времени")//1
	PARM(m_nStep,"#Число шагов")//2
	PARM(Accel,"#Ускорение")//3
	PARM(Time_real,"#Время моделирования")//4
//
    // SYSTEM_INFO SysInfo;
    // GetSystemInfo( &SysInfo );
    // int kProcessors = SysInfo.dwNumberOfProcessors;
    // char Name[2546];
    // for ( int n = 0; n < kProcessors; n++ )
    // 	{
    // 	Sprintf ( Name, "#%d-Число объектов", n + 1 );
    // 	PARM( kCall[n], Name )//5
    // 	}
    // for ( int n = 0; n < kProcessors; n++ )
    // 	{
    // 	Sprintf ( Name, "#%d-Использование процессора", n + 1 );
    // 	PARM( Used[n], Name )//5
    // 	}
//
	PARM(AccelZ,"#Заданное ускорение")//14
	return 0;
  }
