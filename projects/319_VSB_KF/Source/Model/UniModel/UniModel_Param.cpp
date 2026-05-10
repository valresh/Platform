#include "stdafx.h"
#include "UniModel.h"
#include "Err.h"
#include "CommProc.h"
#include "Param.h"
#include "Work.h"
#include "HydroStruct.h"


_W::_W()
	{
  memset ( this, 0, sizeof ( _W ));
  k_Flow = 1.;
	}


int CUniModel::GetParams( char * )
	{
#include "IO_Parms.h"
  if ( pCurrentModel )
    pCurrentModel->GetParams( NULL );
//#include "Points.h"
////  if ( Тип == 9 ) //Состав
//  TAB("Состав", 2)
//    {
//    PARM( Mass, "Массовые доли" )
//    Char<256>Name;
//    for ( int c = 0; c < LAST_GAS; c++ )
//      {
//      Name.Prt( "%s", (char*)pGas[c]->Name );
//      PARM( MEA.Cmol[c], Name );
//      }
//    }
////  if ( Тип == 10 )
//    {
//    Char<256>Name;
//    for ( int c = 0; c < LAST_GAS; c++ )
//      {
//      Name.Prt( "k фильтра %s", (char*)pGas[c]->Name );
//      PARM( k_Comp[c], Name );
//      }
//    }
//  ETAB
//  TAB("Неисправности",1 )
//  if ( Тип == 0 )
//    {
//    DEFECT(Восстановление,"Восстановление")
//    DEFECT(Засорение,"Засорение дросселя")
//    DEF_D (Засор, "Степень засорения, %", 50, 0, 100 )
//	DEFECT(Загазованность,"Загазованность")
//    DEF_D (Уровень_загазованности, "Уровень загазованности, %", 50, 0, 100 )
//	
//    }
//  if ( Тип == 4 )
//    {
//    DEFECT(Восстановление,"Восстановление")
//    DEFECT(Засорение,"Засорение фильтра")
//    DEF_D (Засор, "Степень засорения, %", 50, 0, 100 )
//    }
//  if( Тип == 14 )
//  {
//
//	  DEFECT(Восстановление,"Восстановление")
//	  DEFECT(Опустошение,"Опустошение")
//	  DEFECT(Переполнение,"Переполнение")
//  }
//  ETAB
  ACS_Params( "АСУ", this );
////  TAB("Модули",5)
	return 0;
	}

int CUniModel::UpdateParam( CParams & Param )
  {
  //if ( Тип == 9 ) //Состав
  //  {
  //  for ( int c = 0; c < LAST_GAS; c++ )
  //    if ( MEA.Cmol[c] >= 0. )
  //      MEA.IsComp[c] = true;
  //    else
  //      MEA.IsComp[c] = false;
  //  SetComp( );
  //  return 0;
  //  }
  return 0;
  }

//#include "ShowParams.h"

int CUniModel::ShowParams( DWORD DataType, struct CShowData * pSD )
  {
/*
	switch ( DataType )
		{
		case sp_Params:
		  GetParams( "" );
		return 1;
		}
*/
  return 0;
  }

char * Types[] = {
  "Дроссель",
  "Гидростат",
  "Нагреватель",
  "Уплотнения",
  "Фильтр",
  "Очистка",
  "Люк",
  "Продувка",
  "МЭА",
  "Состав",
  "Фильтр состава",
  "Поток",
  "dT",
  "Электроподогрев",
  "Сборник",
  NULL
  };

int CUniModel::SetProp ( struct CObjProps * pProp )
  {
  if ( pProp->PropName != "Тип" )
    return 1;
  if ( pProp->Value == NULL )
    return 0;
  int n = 0;
  while ( Types[n] )
    {
    if ( pProp->Value == Types[n] )
      {
      Тип = n;
      break;
      }
    n++;
    }
  return 0;
  }

#include "IO_Props.h"
int CUniModel::GetProp( )
  {
  PARM( "Тип", 'L', Types ); // Список
  return 0;
  }
#ifdef LINUX
#define EXP Q_DECL_EXPORT
#endif

extern "C"
  {
  EXP CProps Properties[] = {
    'L', "Тип", "Тип", "Дроссель;Гидростат;Нагреватель;Уплотнения;Фильтр;Очистка;Люк;Продувка;МЭА;Состав;Фильтр;Поток;",
    0, NULL, 
    };
  }


BEGIN_PROPS(CUniModel)
  PROP('L', "Тип", "Тип", "Дроссель;Гидростат;Нагреватель;Уплотнения;Фильтр;Очистка;Люк;Продувка;МЭА;Состав;Фильтр;Поток;", Тип )
END_PROPS
