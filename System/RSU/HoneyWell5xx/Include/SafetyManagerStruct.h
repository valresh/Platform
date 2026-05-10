#pragma once
#include <SmBaseType_.h>
#include <SM_Declare.h>
#include <SmBridge2SysParam.h>

struct UTILSSM5XX_API W_SMANALOG_W
{
#include <SafetyManager/SmAnalog.h>
};
struct UTILSSM5XX_API W_SMANALOG_P
{
#include <SafetyManager/SmAnalog_P.h>
};
struct UTILSSM5XX_API W_SMANALOG : CSmBase, W_SMANALOG_P, W_SMANALOG_W
{
  static LPCSTR ClassName;
  enum { TypeID = id_SMANALOG };
  static void BuildList();
  int GetParams( KSmBridge2SysParam &params );
};
//////////////////////////////////////////////////////////////////////////
struct UTILSSM5XX_API W_SMDISCRET_W
{
#include <SafetyManager/SmDiscret.h>
};
struct UTILSSM5XX_API W_SMDISCRET_P
{
#include <SafetyManager/SmDiscret_P.h>
};
struct UTILSSM5XX_API W_SMDISCRET : CSmBase, W_SMDISCRET_P, W_SMDISCRET_W
{
  static LPCSTR ClassName;
  enum { TypeID = id_SMDISCRET };
  static void BuildList();
  int GetParams( KSmBridge2SysParam &params );
};
//////////////////////////////////////////////////////////////////////////
struct UTILSSM5XX_API W_SMFLD_W
{
#include <SafetyManager/SmFld.h>
};
struct UTILSSM5XX_API W_SMFLD_P
{
#include <SafetyManager/SmFld_P.h>
};
struct UTILSSM5XX_API W_SMFLD : CSmBase, W_SMFLD_P, W_SMFLD_W
{
  static LPCSTR ClassName;
  enum { TypeID = id_SMFLD };
  static void BuildList();
  int GetParams( KSmBridge2SysParam &params );
};
//////////////////////////////////////////////////////////////////////////
struct UTILSSM5XX_API W_SMAI_W
{
#include <SafetyManager/SmAI.h>
};
struct UTILSSM5XX_API W_SMAI_P
{
#include <SafetyManager/SmAI_P.h>
};
struct UTILSSM5XX_API W_SMAI : CSmBase, W_SMAI_P, W_SMAI_W
{
  static LPCSTR ClassName;
  enum { TypeID = id_SMAI };
  static void BuildList();
  int GetParams( KSmBridge2SysParam &params );
};
//////////////////////////////////////////////////////////////////////////
struct UTILSSM5XX_API W_SMAO_W
{
#include <SafetyManager/SmAO.h>
};
struct UTILSSM5XX_API W_SMAO_P
{
#include <SafetyManager/SmAO_P.h>
};
struct UTILSSM5XX_API W_SMAO : CSmBase, W_SMAO_P, W_SMAO_W
{
  static LPCSTR ClassName;
  enum { TypeID = id_SMAO };
  static void BuildList();
  int GetParams( KSmBridge2SysParam &params );
};
//////////////////////////////////////////////////////////////////////////
struct UTILSSM5XX_API W_SMDI_W
{
#include <SafetyManager/SmDI.h>
};
struct UTILSSM5XX_API W_SMDI_P
{
#include <SafetyManager/SmDI_P.h>
};
struct UTILSSM5XX_API W_SMDI : CSmBase, W_SMDI_P, W_SMDI_W
{
  static LPCSTR ClassName;
  enum { TypeID = id_SMDI };
  static void BuildList();
  int GetParams( KSmBridge2SysParam &params );
};
//////////////////////////////////////////////////////////////////////////
struct UTILSSM5XX_API W_SMDO_W
{
#include <SafetyManager/SmDO.h>
};
struct UTILSSM5XX_API W_SMDO_P
{
#include <SafetyManager/SmDO_P.h>
};
struct UTILSSM5XX_API W_SMDO : CSmBase, W_SMDO_P, W_SMDO_W
{
  static LPCSTR ClassName;
  enum { TypeID = id_SMDO };
  static void BuildList();
  int GetParams( KSmBridge2SysParam &params );
};
//////////////////////////////////////////////////////////////////////////
struct UTILSSM5XX_API W_SMBI_W
{
#include <SafetyManager/SmBI.h>
};
struct UTILSSM5XX_API W_SMBI_P
{
#include <SafetyManager/SmBI_P.h>
};
struct UTILSSM5XX_API W_SMBI : CSmBase, W_SMBI_P, W_SMBI_W
{
  static LPCSTR ClassName;
  enum { TypeID = id_SMBI };
  static void BuildList();
  int GetParams( KSmBridge2SysParam &params );
};
//////////////////////////////////////////////////////////////////////////
struct UTILSSM5XX_API W_SMBO_W
{
#include <SafetyManager/SmBO.h>
};
struct UTILSSM5XX_API W_SMBO_P
{
#include <SafetyManager/SmBO_P.h>
};
struct UTILSSM5XX_API W_SMBO : CSmBase, W_SMBO_P, W_SMBO_W
{
  static LPCSTR ClassName;
  enum { TypeID = id_SMBO };
  static void BuildList();
  int GetParams( KSmBridge2SysParam &params );
};
//////////////////////////////////////////////////////////////////////////
struct UTILSSM5XX_API W_SMBICOM_W
{
#include <SafetyManager/SmBICOM.h>
};
struct UTILSSM5XX_API W_SMBICOM_P
{
#include <SafetyManager/SmBICOM_P.h>
};
struct UTILSSM5XX_API W_SMBICOM : CSmBase, W_SMBICOM_P, W_SMBICOM_W
{
  static LPCSTR ClassName;
  enum { TypeID = id_SMBICOM };
  static void BuildList();
  int GetParams( KSmBridge2SysParam &params );
};
//////////////////////////////////////////////////////////////////////////
struct UTILSSM5XX_API W_SMBOCOM_W
{
#include <SafetyManager/SmBOCOM.h>
};
struct UTILSSM5XX_API W_SMBOCOM_P
{
#include <SafetyManager/SmBOCOM_P.h>
};
struct UTILSSM5XX_API W_SMBOCOM : CSmBase, W_SMBOCOM_P, W_SMBOCOM_W
{
  static LPCSTR ClassName;
  enum { TypeID = id_SMBOCOM };
  static void BuildList();
  int GetParams( KSmBridge2SysParam &params );
};
//////////////////////////////////////////////////////////////////////////
struct UTILSSM5XX_API W_SMDOCOM_W
{
#include <SafetyManager/SmDOCOM.h>
};
struct UTILSSM5XX_API W_SMDOCOM_P
{
#include <SafetyManager/SmDOCOM_P.h>
};
struct UTILSSM5XX_API W_SMDOCOM : CSmBase, W_SMDOCOM_P, W_SMDOCOM_W
{
  static LPCSTR ClassName;
  enum { TypeID = id_SMDOCOM };
  static void BuildList();
  int GetParams( KSmBridge2SysParam &params );
};
//////////////////////////////////////////////////////////////////////////
struct UTILSSM5XX_API W_SMDICOM_W
{
#include <SafetyManager/SmDICOM.h>
};
struct UTILSSM5XX_API W_SMDICOM_P
{
#include <SafetyManager/SmDICOM_P.h>
};
struct UTILSSM5XX_API W_SMDICOM : CSmBase, W_SMDICOM_P, W_SMDICOM_W
{
  static LPCSTR ClassName;
  enum { TypeID = id_SMDICOM };
  static void BuildList();
  int GetParams( KSmBridge2SysParam &params );
};
