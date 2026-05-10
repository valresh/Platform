#pragma once
#include "./Y_export.h"
#include <rsuErr.h>

struct Y_DATA_API CIO_W
{
	double BackVar;
	double Min, Max;
	int Status;
  bool IsLimits;
	CIO_W() { Status = 0; BackVar = 0.;	Min = 0.; Max	= 100.; IsLimits = false; };
};

struct Y_DATA_API CSignal : public CIO_W
{
	char * Name;
	enum eType { NoType, Double, Int, Bool, Char, String, Z_OBJ, Byte };
  enum
		{
    maxSignalToDestin = 100,
  };
	eType Type;
	union
		{
		double * pDbl;
		int * pInt;
		bool * pBool;
		char * pChar;
		BYTE * pByte;
		};
  CSignal * pRef;//его использование не нашел, поэтому задействую для связки Signal -> var
  char * pFrom;
	char * pTo[maxSignalToDestin];
  USHORT stringSize;
  DWORD Y_Type;//как в CY_Base
  int numConnection;
	enum {//Состояния сигналов
		NR   = 0x00000000,
		CND  = 0x00000001, //CoNDitional
		BAD  = 0x00000002, //BAD value
		QST  = 0x00000004, //QueSTionable value
		NCOM = 0x00000008, //No COMmunication
		NFP  = 0x00000010, //Not From Process
		PTPF = 0x00000020, //Path To Process Failed
		CLPp = 0x00000040, //CLamP high
		CLPm = 0x00000080, //CLamP low
    CLP  = 0x000000C0,
		CAL  = 0x00000100, //CALibration
		NEFV = 0x00000200, //Not EFfecTive
		O_S  = 0x00000400, //Out of Service
		MNT  = 0x00000800, //MaiNTenance
		IOPp = 0x00001000, //Input Open high
		IOPm = 0x00002000, //Input OPen low
		IOP  = 0x00003000, //Input OPen low
		OOP  = 0x00004000, //Output OPen
		NRDY = 0x00008000, //PI/O Not ReaDY
		PFAL = 0x00010000, //PI/O Power FAiLure
		LPFL = 0x00020000, //PI/O Long Power FaiLure
		MINT = 0x00040000, //Master INiTialize
		SINT = 0x00080000, //Slave INiTialize
		SVPB = 0x00100000, //SV PushBack
    SIGNALS_MASK = 0x001FFFFF,//вместо обнуления надо стирать по маске
		USED = 0x00200000,  //Задается в модели
    Y_CONNECT = 0x00400000,  //есть связь через y.csv
    _ReverseExchange = 0x00800000,//изменить направление обмена относительно конфигурации, например для INDST3.JSV <-- DSET.SV
    NONE_LIMITED = 0x01000000,//DINT в Prosafe ставит что бы не обрезался по SH SL
                              //+ переменная->терминальное_подключение
    _SkipTrasfer = 0x02000000,//запрет на передачу с использованием этого сигнала
    Z_Connection = 0x04000000,//подключение к %Z точке - можно вырабатывать +-IOP
		};
	inline int IsBAD() { return Status & ( BAD | IOP ); };
	inline int IsIOPp() { return Status & IOPp; };
	inline int IsIOPm() { return Status & IOPm; };
	inline int IsCLPp() { return Status & CLPp; };
	inline int IsCLPm() { return Status & CLPm; };
	inline int Used() { return Status & USED; };
	inline void On( DWORD Flag ) { Status |= Flag; };
	inline void Off( DWORD Flag ) { Status &= ~Flag; };
	inline void Set( DWORD Flag,  DWORD Mask ) 
    {
    Mask &= Flag;
    Status &= ~Flag;
    Status |= Mask;
    }
	CSignal() { Name = ""; Type = NoType, 
		          pDbl = NULL; Status = 0; BackVar = 0.; pFrom = NULL; ZeroMemory( pTo, sizeof(pTo) );
              pRef = NULL; stringSize = 0; Y_Type = 0; numConnection = -1;
              };
  int SaveState();
  int RestoreState( char * StrName );
  int StateSave( IYokoStateSer* pStateSer );
  int StateRestore( IYokoStateSer* pStateSer );
  void Set( double & Var );
  void Set( int & Var );
  void Set( bool & Var );
  template<size_t _S>
  void Set( char (&Var)[_S] )
  {
    pChar = Var;
    Type = String;
    stringSize = _S;
  }
  void operator << ( double & Var );
  void operator << ( int & Var );
  void operator << ( bool & Var );
  template<size_t _S>
  void operator << ( char (&Var)[_S] )
  {
    pChar = Var;
    Type = String;
    stringSize = _S;
  }
  //void operator << ( char * Var );//обязательно надо присваивать stringSize
  void operator << ( char & Var );
  void operator << ( BYTE & Var );
  operator int( )
    {
		if ( Type == Double )
      {
      if ( *pDbl )
        return 1;
      else
        return 0;
      }
		if ( Type == Int )
      {
      if ( *pInt )
        return 1;
      else
        return 0;
      }
		if ( Type == Bool )
      {
      if ( *pBool )
        return 1;
      else
        return 0;
      }
    if ( Type == Byte )
    {
      if ( *pByte )
        return 1;
      else
    return 0;
    }
    Status &= CSignal::BAD;
    return 0;
    }
  void Null( );
	inline void operator = ( int Var ) 
		{ 
		if ( Type == Double )
			{
			*pDbl = Var;
			return;
			}
		if ( Type == Int )
			{
			*pInt = Var; 
			return;
			}
		if ( Type == Byte )
			{
			*pByte = (BYTE)Var;
			return;
			}
		KKK();//ASS(FALSE); 
		};
  inline void operator = ( double Var ) 
  { 
    if ( Type == Double )
    {
      *pDbl = Var;
      return;
    }
    if ( Type == Int )
    {
      *pInt = (int)Var; 
      return;
    }
    if ( Type == Byte )
    {
      *pByte = (BYTE)Var;
      return;
    }
    KKK();//ASS(FALSE); 
  };
  template<class T>
  void operator >> ( T & Var ) 
  { 
    if ( Type == Double )
    {
      Var = (T)(*pDbl);
      return;
    }
    if ( Type == Int )
    {
      Var = (T)*pInt; 
      return;
    }
    if ( Type == Byte )
    {
      Var = (T)*pByte;
      return;
    }
    if( Type == Bool )
    {
      Var = *pBool ? 1 : 0;
      return;
    }
    if( Type == Char )
    {
      Var = *pChar; 
      return;
    }
    //ASSD(FALSE); 
  };
	NEWY
};

