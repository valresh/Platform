#pragma once

#include "Chem.h"
#include "Err.h"
#include "Extensions.h"

#undef IN_DLL
#ifdef ALL_DATA
#define IN_DLL __declspec(dllexport)
#else
#define IN_DLL __declspec(dllimport)
#endif

extern IN_DLL int ID_Air;
extern IN_DLL struct CComp * pAir;
extern IN_DLL struct CFlow * pAirFlow;
extern IN_DLL int ID_N2;
extern IN_DLL struct CComp * pN2;
extern IN_DLL struct CFlow * pN2Flow;
extern IN_DLL int ID_Water;
extern IN_DLL struct CComp * pWater;
extern IN_DLL struct CFlow * pWaterFlow;

/////////////////////////////////////////////////////////////
struct IN_DLL CBaseStruct
  {
  int Type;
  int ID;
  CStr Name;  // достаточно ObjName
  int Numb;   // уникальный номер для ObjName
  int kLinearVar;      // число переменных в линейных уравнениях
  int kLinearEq;       // число линейных уравнений
  CStr SrcName;
  virtual void Init( ) = 0;
  virtual void Save() = 0;
  virtual void Restore() = 0;
  void SaveBase();
  void RestoreBase();
  virtual int SetVarAddr( double ** Addr ) = 0;
  };

struct IN_DLL CSpecialData  // расширенные данные, связанные со составами 
  {
  CStr Name;
  CStr Model;
  enum eSpecType { NoType, Component, GammaMix };
  eSpecType Type;
//
//  CSpecialData();
  virtual void GetData( void ** ppData ) { ASS(0) };
  virtual void Init() = 0;
  virtual int Save() = 0; // Записанный Size
  virtual int Restore( int Size ) = 0; // Прочитанный Size
  virtual void GetParams( IBaseModel * pMain ) = 0;
  virtual void UpdateParams( struct CParams & Param ) = 0;
  };

struct CGammaData : public CSpecialData
  {
  virtual void Init( ) { Type = GammaMix; };
  virtual int Save() = 0;
  virtual int Restore( int Size ) = 0;
  virtual void GetParams( IBaseModel * pMain ) = 0;
  virtual void Gamma( void * pData, double P, double T, bool IsComp[K_GAS], double Cmol[K_GAS], double P_sat_id[K_GAS], double Gamma[K_GAS], double & dH_liq ) = 0;
  };

struct IN_DLL CSolidData // Компоненты, не дающие давления 
  {
  int Type;
  enum eSpecType { NoType };
  double Cmass;
  //
  virtual void Init( ) = 0;
  virtual void Save( ) = 0;
  virtual void Restore() = 0;
//
  virtual double Ro( double TC ) = 0; // плотность
  virtual double h( double TC ) = 0;  // удельная энтальпия
  virtual double TC( double h ) = 0;  // температура по энтальпии
  };


typedef CBaseStruct * pBaseStruct;

struct IN_DLL CComp_W 
  {
  bool IsComp[K_GAS];  //
  double Cmol[K_GAS];  // мольные доли реальных и псевдо компонент
  };

enum
  {
  K_EXTENSIONS = 8,
  K_SOLID_DATA = 4,
  };

struct IN_DLL CComp : public CBaseStruct, public CComp_W
{
	enum { TypeID = 1 };
	//IExtension * Extensions[K_EXTENSIONS]; //  
	//CSolidData * SolidData[K_SOLID_DATA];
	CSpecialData * pSpecial;
	DWORD SpecialType;
	DWORD SpecialSize;
///
	void Save();
	void Restore();
	
	CComp()
	{
		Init( );
	}

	CComp( char * _Name, int _Numb )
	{
		Init( );
		Name = _Name;
		Numb = _Numb;
	}

	void Init( )
	{
		Type = TypeID;
		Numb = -1;
		memset( IsComp, 0, sizeof ( IsComp ));
		memset( Cmol, 0, sizeof ( Cmol ));
		kLinearVar = 0;
		kLinearEq = 0;
		pSpecial = NULL;
		SpecialType = 0;
		SpecialSize = 0;
	}
///
	void Set ( bool _IsComp[K_GAS], double _Cmol[K_GAS] )
	{
		if(this == pN2) return;
		memcpy( Cmol, _Cmol, sizeof ( Cmol ));
		memcpy( IsComp, _IsComp, sizeof ( IsComp ));
		Norm (0.0);
	}
///
	void Get ( bool _IsComp[K_GAS], double _Cmol[K_GAS] )
	{
		Norm(0.0);
		memcpy(_Cmol, Cmol, sizeof(Cmol));
		memcpy(_IsComp, IsComp, sizeof(IsComp));
	}
///
	void Set ( CComp * pComp )
	{
		if(this == pN2) return;
		if(pComp == NULL) pComp = pN2;
		memcpy(Cmol, pComp -> Cmol, sizeof ( Cmol ));
		memcpy(IsComp, pComp -> IsComp, sizeof ( IsComp ));
		Norm(0.0);
	}

	double Norm ( double Eps = 1e-5 );

	void operator = ( CComp & Data )
	{
		ASS( this != pN2 );
		memcpy ( Cmol, Data.Cmol, sizeof ( Cmol ));
		memcpy ( IsComp, Data.IsComp, sizeof ( IsComp ));
	};

	bool Finite( )
	{
		for(int n = 0; n < K_GAS; n++)
		{
			if(!_finite(Cmol[n])) return false;
		}
		return true;
	}
	void Mult( double M )
	{
		for(int n = 0; n < LAST_GAS; n++)
		{
			if(IsComp[n]) Cmol[n] *= M;
		}
	}

	void Summ ( double W_1, CComp * pComp_1, double W_2, CComp * pComp_2 );
	bool Read ( char * Name );
	bool ReadMix ( char * Name );
	bool Write ( char * Name );
	int SetVarAddr( double ** Addr ) { ASS(0); return 0; };
	NEW
};


IN_DLL CComp * Get_Comp ( int ID );
IN_DLL CComp * Get_Comp ( char * Name );
IN_DLL CFlow * Get_Flow ( int ID );

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

struct IN_DLL CFlow_Data
  {
  double Flow_mol; // Поток в кмоль/ч
  double P;        // Абсолютное давление кг/см2
  double T;        // температура в градусах Цельсия
  double To_m3;    // перевод в кмоль/ч -> м3/ч
  double To_kg;    // перевод в кмоль/ч -> кг/ч
  double h_Flow;   // кДж/кмоль
  };

//==================================================
struct IN_DLL CFlow : public CBaseStruct, public CFlow_Data 
  {  
  enum { TypeID = 2 };
  int ID_COMP;
  CComp * pComp; 
//
  void Save();
  void Restore();
	CFlow( )
		{
    Init( );
		}
  CFlow( char * _Name, int _Numb )
    {
    Init( );
    Name = _Name;
    Numb = _Numb;
    }
  void Init( )
    {
    memset ( static_cast<CFlow_Data*>(this), 0, sizeof ( CFlow_Data ));
    ID_COMP = 0;
    Numb = -1;
    Type = TypeID;
    pComp = NULL; 
    kLinearVar = 2;      // число переменных в линейных уравнениях
    kLinearEq = 1;       // число линейных уравнений
    }

  inline CComp * Get_Comp ( ) 
    {
    if ( ID_COMP == 0 )
      return NULL;
    pComp = ::Get_Comp ( ID_COMP );
    return pComp;
    }
  void operator = ( CFlow & Data )
		{
// Кроме F и P
    T = Data.T; 
    To_m3 = Data.To_m3; 
    To_kg = Data.To_kg; 
    h_Flow = Data.h_Flow;
    ID_COMP = Data.ID_COMP;
    pComp = Data.pComp;
    };
	void operator = ( CFlow * pData )
		{
// Кроме F и P
    T = pData->T; 
    To_m3 = pData->To_m3; 
    To_kg = pData->To_kg; 
    h_Flow = pData->h_Flow;
    ID_COMP = pData->ID_COMP;
    pComp = pData->pComp;
		};

  void operator << ( CFlow & Data )
		{
    T = Data.T; 
    To_m3 = Data.To_m3; 
    To_kg = Data.To_kg; 
    h_Flow = Data.h_Flow;
    ID_COMP = Data.ID_COMP;
    pComp = Data.pComp;
		};
	void operator << ( CFlow * pData )
		{
    T = pData->T; 
    To_m3 = pData->To_m3; 
    To_kg = pData->To_kg; 
    h_Flow = pData->h_Flow;
    ID_COMP = pData->ID_COMP;
    pComp = pData->pComp;
		};
  void Copy ( CFlow & From )
    {
    Flow_mol = From.Flow_mol;
    P = From.P;
    T = From.T; 
    To_m3 = From.To_m3; 
    To_kg = From.To_kg; 
    h_Flow = From.h_Flow;
    ID_COMP = From.ID_COMP;
    pComp = From.pComp;
    };
  void Copy ( CFlow * pFrom )
    {
    if ( pFrom == NULL )
      pFrom = pN2Flow;
    Flow_mol = pFrom->Flow_mol;
    P = pFrom->P;
    T = pFrom->T; 
    To_m3 = pFrom->To_m3; 
    To_kg = pFrom->To_kg; 
    h_Flow = pFrom->h_Flow;
    ID_COMP = pFrom->ID_COMP;
    pComp = pFrom->pComp;
    };
  double Flow_kg( )
		{
		return Flow_mol * To_kg;
		};
  int SetVarAddr( double ** Addr )  { Addr[0] = &P; Addr[1] = &Flow_mol; return 2; };
  NEW
  };

struct IN_DLL CMotorData
{
	enum 
	{
		TypeID = 0x347F215D,
		MAX_DRIVE = 16, 
	};
	bool* pOn;
	double* pRelOborot; // 0 - 1
	double** ppPower;     // кДж/ч
	double AbsOborot; // об/мин
	double AngleOfAttack;
	NEW
};


struct IN_DLL CSignalData_W
  {
  double Signal;
  CSignalData_W()
    {
    memset ( this, 0, sizeof ( *this ));
    }
  };

struct IN_DLL CSignalData : public CBaseStruct, public CSignalData_W
  {
  enum { TypeID = 4 };
  double * pSignal;
  CSignalData( char * _Name, int _Numb ): CBaseStruct(), CSignalData_W()
    {
    Init( );
    Type = TypeID;
    Name = _Name;
    Numb = _Numb;
    }
  void Init( )
    {
    Signal = 0.;
    pSignal = NULL;
    kLinearVar = 0;  // число переменных в линейных уравнениях
    kLinearEq = 0;   // число линейных уравнений
    }
  void Save();
  void Restore();
  int SetVarAddr( double ** ppAddr ) { ppAddr[0] = &Signal; return 1; };
  NEW
  };

struct IN_DLL CContact_W
  {
  double O;
  CContact_W()
    {
    memset ( this, 0, sizeof ( *this ));
    }
  };

struct IN_DLL CContact : public CBaseStruct, public CContact_W
  {
  enum { TypeID = 5 };
//
  class CVolPour * pBottom; // для этого объекта связь снизу 
  class CVolPour * pTop; // для этого объекта связь сверху
  CContact( char * _Name, int _Numb ): CBaseStruct(), CContact_W()
    {
    Init( );
    Type = TypeID;
    Name = _Name;
    Numb = _Numb;
    pBottom = NULL;
    pTop = NULL;
    }
  void Init( )
    {
    kLinearVar = 0;  // число переменных в линейных уравнениях
    kLinearEq = 0;   // число линейных уравнений
    }
  void Save();
  void Restore();
  int SetVarAddr( double ** ppAddr ) {  return 0; };
  NEW
  };

struct IN_DLL CPortion_W
  {
  double Massa;
  CPortion_W()
    {
    memset ( this, 0, sizeof ( *this ));
    }
  };

struct IN_DLL CPortion : public CBaseStruct, public CPortion_W
  {
  enum { TypeID = 6 };
  CPortion( char * _Name, int _Numb ): CBaseStruct(), CPortion_W()
    {
    Init( );
    Type = TypeID;
    Name = _Name;
    Numb = _Numb;
    }
  void Init( )
    {
    kLinearVar = 0;  // число переменных в линейных уравнениях
    kLinearEq = 0;   // число линейных уравнений
    }
  void Save();
  void Restore();
  int SetVarAddr( double ** ppAddr ) { ppAddr[0] = &Massa; return 1; };
  NEW
  };

IN_DLL CBaseStruct * Create ( char * Name, int Type, int Numb );
int  IN_DLL Get_ID_Data ( char * Name, int Type );
void IN_DLL GetCompData( CComp * pComp, double & C, double & Ckg );
void IN_DLL DumpFlow( CFlow * pFlow );
void IN_DLL DumpComp( CComp * pComp );
void IN_DLL NewComp( int * ID, CComp ** pComp, char * Name, ... );
IN_DLL CComp * NewComp( char * Name, int Numb );

IN_DLL bool ReadCompData ( char * Name, bool IsComp[], double Cmol[], double & F, double & P, double & T );
IN_DLL bool WriteCompData ( char * Name, bool IsComp[], double Cmol[], double & F, double & P, double & T );
//typedef
//  void (*tSetFlow )( CFlow * pFlow, double P, double T, char * Comp );
//extern IN_DLL tSetFlow pSetFlow;

void IN_DLL PrtVars( void * F );
void IN_DLL GetAllStruct( int * kStruct, CBaseStruct *** ppAllStruct );

