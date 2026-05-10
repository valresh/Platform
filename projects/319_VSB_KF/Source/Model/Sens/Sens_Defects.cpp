#include "stdafx.h"
#include "Sens.h"
#include "Err.h"
#include "SysDataTypes.h"


ON_DEFECT(CSens)
	{
  SET_BP
  return SetDefect ( pDefect );
	return 0;
  }

#undef SET_DEF
#define SET_DEF(DefectVar,Comment) \
  {\
  LastDefect = NewDefect( );\
  DefectVar = GetDefect( LastDefect );\
  DefectVar->Parent = RootNode;\
  strcpy_s ( DefectVar->Name, Comment );\
  strcpy_s ( DefectVar->ObjName, (char*)ObjName );\
  DefectVar->Flags = CDef::Defects|CDef::Defect|CDef::Active;\
  DefectVar->pModel = this;\
  }

#undef SET_PARM
#define SET_PARM(Var, vName, vDef, vMin, vMax ) \
  {\
  Var = NewDefectPram( LastDefect );\
  Var->Type = CDefParam::tDbl;\
  strcpy_s(Var->Name, vName );\
  Var->Min = (float)vMin;\
  Var->Max = (float)vMax;\
  Var->Dbl = (double)(vDef);\
  }

void CSens::CreateDefects( )
  {
  if ( ClassRootNode == 0 )
    {
    ClassRootNode = GiveClassRoot( "Датчики" );
    CDef * pRoot = GetDefect( ClassRootNode );
//    strcpy_s ( pRoot->Name, "Датчики" );
//    pRoot->Flags = CDef::Defects|CDef::Node|CDef::Active;
//    pRoot->Parent = GetRootDefects( );
    }
  DWORD LastDefect = 0;
  DWORD RootNode = NewDefect( );
  CDef * pNode = GetDefect( RootNode );
  pNode->Parent = ClassRootNode;
  strcpy_s ( pNode->Name, (char*)ObjName );
  strcpy_s ( pNode->ObjName, (char*)ObjName );
  pNode->Flags = CDef::Defects|CDef::Node|CDef::Active;
//
  SET_DEF(Исправен, "Исправен" );
  SET_DEF(Показания_не_изменяются, "Показания не изменяются" );
  SET_DEF(Завышение_показаний, "Завышение показаний" );
  SET_PARM( parm_Завышение_показаний, "Завышение", 1.2, 1., 10. )
    SET_DEF(Занижение_показаний, "Занижение показаний" );
  SET_PARM( parm_Занижение_показаний, "Занижение", 1.2, 1., 10. )
    SET_DEF(Уход_показаний_на_максимум, "Уход показаний на максимум" );
  //		  SET_PARM( time_Уход_показаний_на_максимум, "Время изменения с", 0., 0., 3600. )
  SET_DEF(Уход_показаний_на_начало_шкалы, "Уход показаний на начало шкалы" );
  //		  SET_PARM( time_Уход_показаний_на_начало_шкалы, "Время изменения с", 0., 0., 3600. )
  SET_DEF(Самопроизвольное_изменение_показаний, "Самопроизвольное изменение показаний" );
  SET_PARM( parm_Самопроизвольное_изменение_показаний, "Конечное значение", 0., -100., 10000. )
    SET_PARM( time_Самопроизвольное_изменение_показаний, "Время изменения с", 0., 0., 3600. )
    SET_DEF(Хаотическое_изменение_показаний, "Хаотическое изменение показаний" );
  SET_PARM( min_Хаотическое_изменение_показаний, "Минимальное значение", 0., -100., 10000. )
    SET_PARM( max_Хаотическое_изменение_показаний, "Максимальное значение", 0., -100., 10000. )
  Defect = (eDefect)-1;
  }

#undef IF_DEFECT
#define IF_DEFECT(Def,Comment) \
  if ( pDefect == NULL || pDefect == Def )\
  {\
  Defect = e##Def;

#undef END_IF
#define END_IF \
  if ( pDefect )\
  return 1;\
    }

int CSens::SetDefect ( struct CDef * pDefect )
  {
  IF_DEFECT(Исправен, "Исправен" );
  END_IF
    //
    IF_DEFECT(Показания_не_изменяются, "Показания не изменяются" );
  Finish = Показания;
  END_IF
  IF_DEFECT(Завышение_показаний, "Завышение показаний" );
  p1 = parm_Завышение_показаний->Dbl;
  p2 = 0.;
  END_IF
  IF_DEFECT(Занижение_показаний, "Занижение показаний" );
  p1 = parm_Занижение_показаний->Dbl;
  p2 =  0.;
  END_IF
  IF_DEFECT(Уход_показаний_на_максимум, "Уход показаний на максимум" );
  Finish = 100000.;//SH * 1.001;
  END_IF
  IF_DEFECT(Уход_показаний_на_начало_шкалы, "Уход показаний на начало шкалы" );
  Defect = eУход_показаний_на_начало_шкалы;
  Finish = 0.;////SL * 0.999;
  END_IF
  IF_DEFECT(Самопроизвольное_изменение_показаний, "Самопроизвольное изменение показаний" );
  Alfa = 0.;
  Start = Показания;
  Finish = parm_Самопроизвольное_изменение_показаний->Dbl;// "Конечное значение", 0., 0., 300. )
  Time = time_Самопроизвольное_изменение_показаний->Dbl;// "Время изменения с", 0., 0., 3600. )
  END_IF
  IF_DEFECT(Хаотическое_изменение_показаний, "Хаотическое изменение показаний" );
  p1 = min_Хаотическое_изменение_показаний->Dbl;// "Минимальное значение", 0., 0., 1000. )
  p2 = max_Хаотическое_изменение_показаний->Dbl;// "Максимальное значение", 0., 0., 1000. )
  Prop = 0.9;
  if ( p1 > p2 )
    {
    double w = p1;
    p1 = p2;
    p2 = w;
    }
  Finish = Показания;
  END_IF
  return 0;
  }

#undef IF_DEFECT
#define IF_DEFECT(Def,Comment) \
  if ( Defect == e##Def )\
  {

#undef END_IF
#define END_IF \
    }

double CSens::SetDefect( double V )
  {
  if ( Defect < 0 )
    return V;
  IF_DEFECT(Исправен, "Исправен" );
  return V;
  END_IF
    //
    IF_DEFECT(Показания_не_изменяются, "Показания не изменяются" );
  return Finish;
  END_IF
    IF_DEFECT(Завышение_показаний, "Завышение показаний" );
  return V * p1;
  END_IF
    IF_DEFECT(Занижение_показаний, "Занижение показаний" );
  return V / p1;
  END_IF
    IF_DEFECT(Уход_показаний_на_максимум, "Уход показаний на максимум" );
  return Finish;
  END_IF
    IF_DEFECT(Уход_показаний_на_начало_шкалы, "Уход показаний на начало шкалы" );
  return Finish;
  END_IF
    IF_DEFECT(Самопроизвольное_изменение_показаний, "Самопроизвольное изменение показаний" );
  double dt = pSys->dt * 3600.;
  if ( Alfa < 1. )
    {
    if ( Time <= dt )
      Alfa = 1.;
    else
      {
      Alfa += dt / Time;
      if ( Alfa > 1. )
        Alfa = 1.;
      }
    }
  return Start + Alfa * ( Finish - Start );
  END_IF
    IF_DEFECT(Хаотическое_изменение_показаний, "Хаотическое изменение показаний" );
  double Ksi = r01();
  double V = Finish;
  double A = 0.2;
  if ( Ksi < Prop )
    V = ( 1. - A ) * V + A * (double)p1;
  else
    V = ( 1. - A ) * V + A * (double)p2;
  Finish = V;
  if ( fabs ( V - p1 ) < 0.05 * ( p2 - p1 ))
    Prop = 0.1;
  if ( fabs ( V - p2 ) < 0.05 * ( p2 - p1 ))
    Prop = 0.9;
  return Finish;
  END_IF
/**********
  IF_DEFECT(Сигнализация_HH, "Сигнализация HH" );
  return Finish;
  END_IF
    IF_DEFECT(Сигнализация_H, "Сигнализация HI" );
  return Finish;
  END_IF
    IF_DEFECT(Сигнализация_L, "Сигнализация LO" );
  return Finish;
  END_IF
    IF_DEFECT(Сигнализация_LL, "Сигнализация LL" );
  return Finish;
  END_IF
    IF_DEFECT(Сигнализация_IOPp, "Сигнализация IOP+" );
  return Finish;
  END_IF
    IF_DEFECT(Сигнализация_IOPm, "Сигнализация IOP-" );
  return Finish;
  END_IF
**********/
  return 0.;
  }
