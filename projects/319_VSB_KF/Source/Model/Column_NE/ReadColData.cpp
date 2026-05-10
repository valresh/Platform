#include "stdafx.h"
#define DLL_COLUMN
#include "Column_NE.h"
#include "Err.h"
#include "Split.h"
//#include "Data.h"
#include "SetData.h"
#include "Connect.h"

void CColumn_NE::InitCol( )
  {
  // Опции;параметры опций;
  Char<256>Path;
  Path.Prt ( "%sDATA\\Equipment\\%s.csv", PROJECT_ROOT, ObjName );
  CSplit<6,32> Data;
  bool NH3Col = false;
  bool Azeotrope = false;
  if ( Data.Open( Path ) )
    {
    while ( Data.Next( ))
      {
      if ( strcmp ( Data.Cols[0], "Вода" ) == 0 )
        {
//        WaterCol = true;
        break;
        }
      if ( strcmp ( Data.Cols[0], "Азеотроп" ) == 0 )
        {
        Azeotrope = true;
        break;
        }
      }
    Data.Close();
    }
  ////////////////////////////
  Column.InitCol( kStage );
  // Чтение размеров
  // Размер;n1;n2;D;H; - размеры тарелок
  if ( Data.Open( Path ) )
    {
    while ( Data.Next( ))
      {
      if ( strcmp ( Data.Cols[0], "Размер" ) == 0 )
        {
        int n1 = atoi ( Data.Cols[1] );
        int n2 = atoi ( Data.Cols[2] );
        ASS( n1 >= 1 && n1 <= n2 )
          ASS( n1 <= kStage )
          double D = atof ( Data.Cols[3] ) * 1e-3;
        double H = atof ( Data.Cols[4] ) * 1e-3;
        double h_st = atof ( Data.Cols[5] ) * 1e-3;
        double S = _PI_4 * D * D;
        double V = S * H * 0.95;
        for ( int n = n1; n <= n2; n++ )
          {
          Column.pStage[n]->Real_Vol_Stage = V;
          Column.pStage[n]->Real_S_Stage = S;
          Column.pStage[n]->Real_H_Stage = H;
          Column.pStage[n]->h_st = h_st;
          }
        continue;
        }
      if ( strcmp ( Data.Cols[0], "Тип входа" ) == 0 )
        {
        bool OK = false;
        for ( int n = 0; n < kIO; n++ )
          {
          if ( strcmp ( pNodes[n].Name, Data.Cols[1] ) == 0 )
            {
            if ( Data.Cols[2][0] == 'г' || Data.Cols[2][0] == 'Г' )
              {
              OK = true;
              pNodes[n].Type = 'G';
              break;
              }
            if ( Data.Cols[2][0] == 'ж' || Data.Cols[2][0] == 'Ж' )
              {
              OK = true;
              pNodes[n].Type = 'L';
              break;
              }
            ModMsg ( "В %s задан неправильный тип входа '%s'", ObjName, Data.Cols[2] );
            }
          }
        if ( !OK )
          {
          ModMsg ( "В %s задано неправильное название входа '%s'", ObjName, Data.Cols[1] );
          }
        continue;
        }
      if ( strcmp ( Data.Cols[0], "Вода" ) == 0 )
        {
        continue;
        }
      if ( strcmp ( Data.Cols[0], "Азеотроп" ) == 0 )
        {
        continue;
        }
      //if ( strcmp ( Data.Cols[0], "Поглощение" ) == 0 )
      //  {
      //  Column.CalcAdsDes = true;
      //  bool OK = false;
      //  if ( strcmpi ( Data.Cols[1], "H2S" ) == 0 )
      //    {
      //    Column.AdsType = AdsH2S_MEA;
      //    Column.pAdsGas_1 = new CGamma();
      //    Column.pAdsGas_1->G_GAS = G_H2S;
      //    Column.pAdsGas_1->G_ABS = G_MDEA;
      //    OK = true;
      //    }
      //  if ( strcmpi ( Data.Cols[1], "CO2" ) == 0 )
      //    {
      //    Column.AdsType = AdsCO2_MEA;
      //    Column.pAdsGas_1 = new CGamma();
      //    Column.pAdsGas_1->G_GAS = G_CO2;
      //    Column.pAdsGas_1->G_ABS = G_MDEA;
      //    OK = true;
      //    }
      //  if ( strcmpi ( Data.Cols[1], "CO+CO2+Карсол" ) == 0 )
      //    {
      //    Column.AdsType = AdsCO2_K2CO3;
      //    Column.pAdsGas_1 = new CGamma();
      //    //          Column.pAdsGas_2 = new CGamma();
      //    Column.pAdsGas_1->G_GAS = G_CO2;
      //    Column.pAdsGas_1->G_ABS = G_K2CO3;
      //    //          Column.pAdsGas_2->G_GAS = G_CO;
      //    //          Column.pAdsGas_2->G_ABS = G_K2CO3;
      //    OK = true;
      //    }
      //  if ( !OK )
      //    {
      //    ModMsg ( "В '%s' Неправильно задан тип поглощаемого газа '%s'", ObjName, Data.Cols[1] );
      //    continue;
      //    }
      //  continue;
      //  }

      //if ( strcmp ( Data.Cols[0], "Расширение" ) == 0 )
      //  {
      //  char * Name = Data.Cols[1];
      //  char * DLL = Data.Cols[2];
      //  char * Params = Data.Cols[3];
      //  I_Addition * pAdd = LoadAddition( DLL, Name );
      //  if ( pAdd == NULL )
      //    {
      //    ModMsg ( "Не загружено расширение '%s'(%s)", Name, DLL );
      //    continue;
      //    }
      //  if ( Column.pColAdditions == NULL )
      //    {
      //    Column.pColAdditions = new I_ColAdditionList();
      //    Column.pColAdditions->Init();
      //    }
      //  Column.pColAdditions->Add( pAdd );
      //  continue;
      //  }
      if ( strcmp ( Data.Cols[0], "Нагреватель" ) == 0 )
        {
        int Numb = atoi ( Data.Cols[1] );        
        int n_from = atoi ( Data.Cols[2] );
        int n_to = atoi ( Data.Cols[3] );
        ASS( Numb >= 1 && Numb <= 3 )
          Numb--;
        Char<32> Name;
        Name.Prt( "Вход нагрева %d", Numb + 1 );
        Column.pHeat_In[Numb] = new CExtHeat( Name, this );
        Column.pHeat_In[Numb]->nGroup = Numb;
        Column.pHeat_In[Numb]->KS = KSHeatStage[Numb];
        Name.Prt( "Выход нагрева %d", Numb + 1 );
        Column.pHeat_Out[Numb] = new CExtHeat( Name, this );
        Column.pHeat_Out[Numb]->nGroup = Numb;
        CExtHeat * pPrevHeat = Column.pHeat_In[Numb];
        CExtHeat * pH = NULL;
        if ( n_from <= n_to )
          {
          for ( int n = n_from; n <= n_to; n++ )
            { 
            Name.Prt( "Нагрев %d т. %d", Numb + 1, n );
            pH = new CExtHeat( Name, this );
            pPrevHeat->pNextHeat = pH;
            pH->pPrevHeat = pPrevHeat;
            pPrevHeat = pH;
            pH->nGroup = Numb;
            pH->nStage = n;
            }
          pPrevHeat->pNextHeat = Column.pHeat_Out[Numb];
          Column.pHeat_Out[Numb]->pPrevHeat = pH;
          }
        else
          {
          for ( int n = n_from; n >= n_to; n-- )
            { 
            Name.Prt( "Нагрев т. %d", n );
            pH = new CExtHeat( Name, this );
            pPrevHeat->pNextHeat = pH;
            pH->pPrevHeat = pPrevHeat;
            pPrevHeat = pH;
            pH->nGroup = Numb;
            pH->nStage = n;
            }
          pPrevHeat->pNextHeat = Column.pHeat_Out[Numb];
          Column.pHeat_Out[Numb]->pPrevHeat = pH;
          }
        continue;
        }
      ASS(0) //- неизвестный параметр
      }
    Data.Close( );
    }
  }
