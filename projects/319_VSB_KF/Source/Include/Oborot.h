#pragma once
#include "MM.h"

struct CWodward_Data
  {
  enum eWDState { Стоит, Работает };
  eWDState State;
  double OborotTurbine;
  double OborotZ;
  //
  double Pos_Reg_Klap;
  double Max_Reg_Klap;
  double OborotNom;
  bool Work;
  bool АБ;
  bool CanSteam40;
  bool SetАБ;
  double Pos_Reg_Steam_40;
  double P_Steam_40;
  double P_Steam_40Nom;
  double P_Steam_40Z;
  double Param[10];
  };

#define ID_CtrlOborot 0x347F215D

struct COborot
  {
  bool On;
  double Oborot;
  double Pow;
  double Угол_атаки, O_Res2;
  double Shift;
  bool DefectStop;
  COborot()
    {
    memset ( this, 0, sizeof ( *this ));
    Угол_атаки = 1.;
    }
  };

struct CExternPow
  {
  double Pow_Rel, Pow_кВт;
  CExternPow()
    {
    memset ( this, 0, sizeof ( *this ));
    }
  };

struct CKlapConnect
  {
  enum { ID = 0x24FE7148 };
  double Flow_Z_mol;
  double Flow_mol;
//
  double Vent_Z;
  double Vent;
  bool Ctrl_Vent;
  CKlapConnect()
    {
    memset ( this, 0, sizeof ( *this ));
    }
  };

/*
struct COborotPnt : public IMM
  {
  enum { TypeID = 0x347F215D };
  COborot * pOborot;
  COborotPnt( COborot * pO, int _IO )
    {
    ID = TypeID;
    IO = _IO;
    pOborot = pO;
    }
  void Put( IMM * pTo )
    {
    ASS(pTo->ID == TypeID)
    ASS(IO == 1)
    ASS(pTo->IO == -1)
    
    memcpy( ((COborotPnt*)pTo)->pOborot, pOborot, sizeof ( COborot ) );
    }
  void Get( IMM * pFrom )
    {
    ASS(pFrom->ID == TypeID)
    ASS(IO == -1)
    ASS(pFrom->IO == 1)
    memcpy( pOborot, ((COborotPnt*)pFrom)->pOborot, sizeof ( COborot ) );
    }
  };
*/