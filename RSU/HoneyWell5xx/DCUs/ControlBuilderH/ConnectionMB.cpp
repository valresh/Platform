#include <rsuErr.h>
#include <ConnectionMB.h>

void SConnectionMB::AssignInputRef( BYTE *pVar, eVarType type, USHORT varSize, KBmBase *outI )
{
  typeInput = type;
  varSizeI = varSize;
  AssignRefImpl( pVar, typeInput, varInput );
  objI = outI;
}

void SConnectionMB::AssignOutputRef( BYTE *pVar, eVarType type, USHORT varSize, KBmBase *outO )
{
  typeOutput = type;
  varSizeO = varSize;
  AssignRefImpl( pVar, typeOutput, varOutput );
  objO = outO;
}

void SConnectionMB::AssignRefImpl( BYTE *pVar, eVarType type, uRefs &var )
{
  switch( type )
  {
  case evtByte:
  case evtBool:
    var.pByte = pVar;
    break;
  case evtInt:
    var.pInt = (int*)pVar;
    break;
  case evtDouble:
    var.pDbl = (double*)pVar;
    break;
  case evtString:
    var.pSz = (char*)pVar;
    break;
  default:
    ASS(0);
  }
  enabledTrasfer = true;
}

void SConnectionMB::Transfer()
{
  if( !enabledTrasfer )
    return;
  if( objO && objO->IsArmAssigned() )
    return;
  ASSD( typeInput==typeOutput );
  switch( typeInput )
  {
  case evtByte:
  case evtBool:
    *varInput.pByte = *varOutput.pByte;
    break;
  case evtInt:
    *varInput.pInt = *varOutput.pInt;
    break;
  case evtDouble:
    *varInput.pDbl = *varOutput.pDbl;
    break;
  case evtString:
    strncpy_s( varInput.pSz, varSizeI, varOutput.pSz, varSizeI );
    break;
  case evtHZ:
    break;
  default:
    ASSD(0);
  }
}

void SConnectionMB::FeedBack2Output( double fdv )
{
  switch( typeInput )
  {
  /*case evtByte:
  case evtBool:
    *varOutput.pByte = fdv;
    break;
  case evtInt:
    *varOutput.pInt = fdv;
    break;*/
  case evtDouble:
    *varOutput.pDbl = fdv;
    break;
  case evtHZ:
    break;
  default:
    ASSD(0);
  }
}
