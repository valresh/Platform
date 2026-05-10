#include <SafetyManagerStruct.h>
#include <SmShData.h>
#include "ListValue.h"
#include <stddef.h>


#include "SM_Sheme.h"
#pragma warning(push)
#pragma warning(disable : 4101)

void W_SMANALOG::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_SMANALOG shareObj;
  typedef CShSMANALOG netObj;
#include "SafetyManager/SmAnalog.h"
#include "SafetyManager/SmAnalog_P.h"
}

void W_SMDISCRET::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_SMDISCRET shareObj;
  typedef CShSMDISCRET netObj;
#include "SafetyManager/SmDiscret.h"
#include "SafetyManager/SmDiscret_P.h"
}

void W_SMFLD::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_SMFLD shareObj;
  typedef CShSMFLD netObj;
#include "SafetyManager/SmFld.h"
#include "SafetyManager/SmFld_P.h"
}

void W_SMAI::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_SMAI shareObj;
  typedef CShSMAI netObj;
#include "SafetyManager/SmAI.h"
#include "SafetyManager/SmAI_P.h"
}

void W_SMAO::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_SMAO shareObj;
  typedef CShSMAO netObj;
#include "SafetyManager/SmAO.h"
#include "SafetyManager/SmAO_P.h"
}

void W_SMDI::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_SMDI shareObj;
  typedef CShSMDI netObj;
#include "SafetyManager/SmDI.h"
#include "SafetyManager/SmDI_P.h"
}

void W_SMDO::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_SMDO shareObj;
  typedef CShSMDO netObj;
#include "SafetyManager/SmDO.h"
#include "SafetyManager/SmDO_P.h"
}

void W_SMBI::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_SMBI shareObj;
  typedef CShSMBI netObj;
#include "SafetyManager/SmBI.h"
#include "SafetyManager/SmBI_P.h"
}

void W_SMBO::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_SMBO shareObj;
  typedef CShSMBO netObj;
#include "SafetyManager/SmBO.h"
#include "SafetyManager/SmBO_P.h"
}

void W_SMBICOM::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_SMBICOM shareObj;
  typedef CShSMBICOM netObj;
#include "SafetyManager/SmBICOM.h"
#include "SafetyManager/SmBICOM_P.h"
}

void W_SMBOCOM::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_SMBOCOM shareObj;
  typedef CShSMBOCOM netObj;
#include "SafetyManager/SmBOCOM.h"
#include "SafetyManager/SmBOCOM_P.h"
}

void W_SMDOCOM::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_SMDOCOM shareObj;
  typedef CShSMDOCOM netObj;
#include "SafetyManager/SmDOCOM.h"
#include "SafetyManager/SmDOCOM_P.h"
}
void W_SMDICOM::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_SMDICOM shareObj;
  typedef CShSMDICOM netObj;
#include "SafetyManager/SmDICOM.h"
#include "SafetyManager/SmDICOM_P.h"
}
#pragma warning(pop)
