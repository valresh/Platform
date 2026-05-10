#include <HcdStruct.h>
#include <HcdShData.h>
#include "ListValue.h"
#include <stddef.h>


#include "HCD_Sheme.h"
#pragma warning(push)
#pragma warning(disable : 4101)

void W_ANAVAR::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_ANAVAR shareObj;
  typedef CShANAVAR netObj;
#include <HCD/AnalogVar.h>
}

void W_DIGVAR::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_DIGVAR shareObj;
  typedef CShDIGVAR netObj;
#include <HCD/DigitalVar.h>
}

void W_ANASIG::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_ANASIG shareObj;
  typedef CShANASIG netObj;
#include <HCD/AnalogSignal.h>
}

void W_DIGSIG::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_DIGSIG shareObj;
  typedef CShDIGSIG netObj;
#include <HCD/DigitalSignal.h>
}
//////////////////////////////////////////////////////////////////////////

void W_8DI::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_8DI shareObj;
  typedef CSh8DI netObj;
#include <HCD/8DI.h>
}

void W_AI::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_AI shareObj;
  typedef CShAI netObj;
#include <HCD/AI.h>
}

void W_AO::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_AO shareObj;
  typedef CShAO netObj;
#include <HCD/AO.h>
}

void W_DO::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_DO shareObj;
  typedef CShDO netObj;
#include <HCD/DO.h>
}

void W_8DO::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_8DO shareObj;
  typedef CSh8DO netObj;
#include <HCD/8DO.h>
}

#pragma warning(pop)
