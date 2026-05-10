#include "Queue.h"
#include "../Lang.h"

bool ParamKlapan(CLang& def,char* text);
bool ParamKVO   (CLang& def,char* text);
bool ParamSensor(CLang& def,char* text);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AddIF(DefineTegs)
  {
  //
  char szNo [256] = "";
  char szYes[256] = "";
  //
  STegObj obj[] =
    {
    { 'S', "0"   , szNo , 255 },
    { 'S', "1"   , szYes, 255 },
    { 'S', "no"  , szNo , 255 },
    { 'S', "yes" , szYes, 255 },
    { 'S', "да"  , szNo , 255 },
    { 'S', "нет" , szYes, 255 },
    };
  ::ParserObjScn( teg, obj, sizeof(obj)/sizeof(STegObj) );
  //
  bool bYesOrNo = true;
  bool bYes = true;
  char* text;
  if ( *teg[0].value )
    {
    if ( *teg[0].value == '!' )
    text = teg[0].value+1, bYesOrNo = false;
    else
    text = teg[0].value;
    }
  else
  if ( *szYes ) text = szYes;
  else
  if ( *szNo  ) text = szNo, bYesOrNo = false;
  else 
  {
      def.nIf++;
      ASSERT(def.nIf < MAX_IF);
      def.bIf[def.nIf] = bYesOrNo == bYes;
      return;
  }
  //
  int nActive = def.GetActiveObjSocket();
  UINT eType  = def.GetMnemoType( nActive );
  switch ( eType )
    {
    case CKlapan::TypeID: bYes = ParamKlapan(def,text);break;
    case CKVO   ::TypeID: bYes = ParamKVO   (def,text);break;
	case CSensor::TypeID: bYes = ParamSensor(def,text);break;
    default:
      bYes = false;
      break;
    };

  def.nIf++;
  ASSERT(def.nIf < MAX_IF);
  def.bIf[def.nIf] = bYesOrNo == bYes;
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AddELSE(DefineTegs)
  {
  def.bIf[def.nIf] = !def.bIf[def.nIf];
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AddENDIF(DefineTegs)
  {
  def.nIf = std::max(0,def.nIf-1);
  def.bIf[0] = true;
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
