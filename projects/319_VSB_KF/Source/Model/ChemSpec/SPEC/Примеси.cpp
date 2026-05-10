#include "stdafx.h"
#include "Chem.h"
#include "Data.h"
#include "math.h"
#include "Err.h"


#undef SAVE_DATA
#define SAVE_DATA(V,L) { pSRFile->WF( V, L ); }
#undef REST_DATA
#define REST_DATA(Addr,Size) { pSRFile->RF( Addr, Size ); }


struct CSulphurData_W 
  {
  enum eS { Меркаптаны, Сульфиды, Тиофены, Дисульфиды, СуммаS };
  double Cmol[СуммаS];
  };


//CSpecialData::CSpecialData()
//  {
//  ;
//  }

struct CSulphurData : public CSpecialData, public CSulphurData_W
  {
  static double kH2[СуммаS];
  static double kH2S[СуммаS];
  static double kOut[СуммаS];
  CSulphurData( );
  void Init( );
  virtual int Save();
  virtual int Restore( int Size );
  void GetParams( IBaseModel * pMain );
  void GetData( void ** ppData );
  void Norm( );
  void UpdateParams( struct CParams & Param ){;};
  NEW
  };

double CSulphurData::kH2[СуммаS];
double CSulphurData::kH2S[СуммаS];
double CSulphurData::kOut[СуммаS];

void CSulphurData::GetData( void ** ppData )
  {
  CSulphurData_W * pW = static_cast<CSulphurData_W*> ( this );
  *ppData = pW;
  }

CSpecialData * GetSulphurData()
  {
  CSulphurData * pSD = new CSulphurData;
  pSD->Model = "Соединения серы";
  pSD->Init();
  return pSD;
  }

CSulphurData::CSulphurData( )
  {
  Type = CSpecialData::Component;
  CLEAR( Cmol )
  }

void CSulphurData::Init( )
  {
  Type = CSpecialData::Component;
  Cmol[Меркаптаны] = 0.5;
  Cmol[Сульфиды] = 0.3;
  Cmol[Тиофены] = 0.1;
  Cmol[Дисульфиды] = 0.1;
  // M + H2 = B + H2S
  kH2 [Меркаптаны] = 1.;
  kH2S[Меркаптаны] = 1.;
  kOut[Меркаптаны] = 1.;
  // S + 2 * H2 = 2 * B + H2S
  kH2[Сульфиды] = 1.;
  kH2S[Сульфиды] = 1.;
  kOut[Сульфиды] = 2.;
  // T + 4 * H2 = B + H2S
  kH2[Тиофены] = 4.;
  kH2S[Тиофены] = 1.;
  kOut[Тиофены] = 1.;
  // O + H2 -> B
  kH2[Дисульфиды] = 1.;
  kH2S[Дисульфиды] = 0.;
  kOut[Дисульфиды] = 1.;
  }

int CSulphurData::Save()
  {
  SAVE_DATA( static_cast<CSulphurData_W*>(this), sizeof ( CSulphurData_W ));
  return sizeof ( CSulphurData_W );
  }

int CSulphurData::Restore( int Size )
  {
  ASS( Size == sizeof ( CSulphurData_W ))
  REST_DATA( static_cast<CSulphurData_W*>(this), sizeof ( CSulphurData_W ));
  return Size;
  }

#include "IO_Parms.h"
#define this pMain
void CSulphurData::GetParams( IBaseModel * pMain )
  {
  TAB("Соединения серы", 1 )
  PARM( (char*)Name.Str, "#Состав" );
  PARM( Cmol[Меркаптаны], "#Меркаптаны" );
  PARM( Cmol[Сульфиды], "#Сульфиды" );
  PARM( Cmol[Тиофены], "#Тиофены" );
  PARM( Cmol[Дисульфиды], "#Дисульфиды" );
  ETAB
  }

void CSulphurData::Norm( )
  {
  double S = Cmol[Меркаптаны] + Cmol[Сульфиды] + Cmol[Тиофены] + Cmol[Дисульфиды];
  if ( S <= 0. )
    {
    ASS(0)
    return;
    }
  double N = 1. / S;
  Cmol[Меркаптаны] *= N;
  Cmol[Сульфиды] *= N;
  Cmol[Тиофены] *= N;
  Cmol[Дисульфиды] *= N;
  }

/////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
struct CSummaS : public CReal
  {
  virtual CSpecialData * GetSpecialData ( char * Params );
  CSummaS();
  };

CSummaS  SummaS;

CGas * Create_S()
  {
  return &SummaS;
  }

// СуммаS + 4 * H2 = B + H2S
// Mw = 190.05
// dS = 32./190.05 = 0.16837674296237832149434359379111

CSummaS::CSummaS()
  {
  Tb = 200.;
  Mw = 190.5;
  lstrcpy ( CGas::Name, "СуммаS" );
  lstrcpy ( CGas::Alt_Name, "СуммаS" );
  }

bool Get ( char * Params, char * Name, double & Value )
  {
  char * B = strstr ( Params, Name );
  if ( B == NULL )
    return false;
  char * E = strchr ( B, '=' );
  if ( E == NULL )
    return false;
  Value = atof ( E + 1 );
  return true;
  }

CSpecialData * CSummaS::GetSpecialData ( char * Params )
  {
  CSulphurData * pSp = new CSulphurData();
  pSp->Init();
//Меркаптаны=0.5,Сульфиды=0.3,Тиофены=0.1,Дисульфиды=0.1
  Get ( Params, "Меркаптаны", pSp->Cmol[pSp->Меркаптаны] );
  Get ( Params, "Сульфиды", pSp->Cmol[pSp->Сульфиды] );
  Get ( Params, "Тиофены", pSp->Cmol[pSp->Тиофены] );
  Get ( Params, "Дисульфиды", pSp->Cmol[pSp->Дисульфиды] );
  pSp->Norm();
  return pSp;
  }

#if 0 
///////////////////////////////////////////////////////////////////////////
const double Mw_H2S = 34.082;
const double Mw_H2 = 2.016;
CМеркаптаны  Меркаптаны;
CМеркаптаны::CМеркаптаны()
  {
  Tb = -273.;
  Mw = 0.;
  Alt_Name = Name = "Меркаптаны";
  }

// M + H2 = B + H2S
void CМеркаптаны::Init()
  {
  pGas[G_Меркаптаны] = this;
  Numb = G_Меркаптаны;
  kH2 = 1.;
  kH2S = 1.;
  kOut = 1.;
  Tb = 80.;
  GetBaseComp ( Tb );
  Mw = kH2S * Mw_H2S - kH2 * Mw_H2;
  if ( BaseComp >= 0 )
    Mw += kOut * pGas[BaseComp]->Mw;
  }
///////////////////////////////////////////////////////////////////////////
CСульфиды  Сульфиды;
CСульфиды::CСульфиды()
  {
  Tb = -273.;
  Mw = 0.;
  Alt_Name = Name = "Сульфиды";
  }
// S + 2 * H2 = 2 * B + H2S
void CСульфиды::Init()
  {
  pGas[G_Сульфиды] = this;
  Numb = G_Сульфиды;
  kH2 = 2.;
  kH2S = 1.;
  kOut = 2.;
  Tb = 80.;
  GetBaseComp ( Tb );
  Mw = kH2S * Mw_H2S - kH2 * Mw_H2;
  if ( BaseComp >= 0 )
    Mw += kOut * pGas[BaseComp]->Mw;
  }
///////////////////////////////////////////////////////////////////////////
CТиофены  Тиофены;
CТиофены::CТиофены()
  {
  Tb = -273.;
  Mw = 0.;
  Alt_Name = Name = "Тиофены";
  }
// T + 4 * H2 = B + H2S
void CТиофены::Init()
  {
  pGas[G_Тиофены] = this;
  Numb = G_Тиофены;
  kH2 = 4.;
  kH2S = 1.;
  kOut = 1.;
  Tb = 180.;
  GetBaseComp ( Tb );
  Mw = kH2S * Mw_H2S - kH2 * Mw_H2;
  if ( BaseComp >= 0 )
    Mw += kOut * pGas[BaseComp]->Mw;
  }
///////////////////////////////////////////////////////////////////////////
CДисульфиды  Дисульфиды;
CДисульфиды::CДисульфиды()
  {
  Tb = -273.;
  Mw = 0.;
  Alt_Name = Name = "Дисульфиды";
  }

// O + H2 -> B
void CДисульфиды::Init()
  {
  pGas[G_Дисульфиды] = this;
  Numb = G_Дисульфиды;
  kH2 = 1.;
  kH2S = 0.;
  kOut = 1.;
  BaseComp = G_C6N;
  Mw = kH2S * Mw_H2S - kH2 * Mw_H2;
  Tb = 80.;
  if ( BaseComp >= 0 )
    {
    Tb = pGas[BaseComp]->Tb;
    Mw += kOut * pGas[BaseComp]->Mw;
    }
  }
///////////////////////////////////////////////////////////////////////////
CОлефины  Олефины;
CОлефины::CОлефины()
{
	Tb = -273.;
	Mw = 0.;
	Alt_Name = Name = "Олефины";
}
// O + H2 -> B
void CОлефины::Init()
{
	pGas[G_Дисульфиды] = this;
	Numb = G_Дисульфиды;
	kH2 = 1.;
	kH2S = 0.;
	kOut = 1.;
  BaseComp = G_C6N;
  Mw = kH2S * Mw_H2S - kH2 * Mw_H2;
  Tb = 80.;
  if ( BaseComp >= 0 )
    {
    Tb = pGas[BaseComp]->Tb;
    Mw += kOut * pGas[BaseComp]->Mw;
    }
}
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

#endif

