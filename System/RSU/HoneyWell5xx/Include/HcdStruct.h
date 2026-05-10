#pragma once
#include <HCDBaseType_.h>
#include <HcdVarTypes.h>
#include <HCD_Declare.h>

struct UTILSHCD_API W_ANAVAR_W
{
#include <HCD/AnalogVar.h>
};

struct UTILSHCD_API W_ANAVAR : CHcdBase, W_ANAVAR_W
{
  static LPCSTR ClassName;
  enum { TypeID = id_ANAVAR };
  enum
  {
    PVtype = ehtAnalog32
  };
  static void BuildList();
};
//////////////////////////////////////////////////////////////////////////
struct UTILSHCD_API W_DIGVAR_W
{
#include <HCD/DigitalVar.h>
};

struct UTILSHCD_API W_DIGVAR : CHcdBase, W_DIGVAR_W
{
  static LPCSTR ClassName;
  enum { TypeID = id_DIGVAR };
  enum
  {
    PVtype = ehtDigital32
  };
  static void BuildList();
};
//////////////////////////////////////////////////////////////////////////
struct UTILSHCD_API W_DIGSIG_W
{
#include <HCD/DigitalSignal.h>
};

struct UTILSHCD_API W_DIGSIG : CHcdBase, W_DIGSIG_W
{
  static LPCSTR ClassName;
  enum { TypeID = id_DIGSIG };
  static void BuildList();
};
//////////////////////////////////////////////////////////////////////////
struct UTILSHCD_API W_ANASIG_W
{
#include <HCD/AnalogSignal.h>
};

struct UTILSHCD_API W_ANASIG : CHcdBase, W_ANASIG_W
{
  static LPCSTR ClassName;
  enum { TypeID = id_ANASIG };
  static void BuildList();
};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
struct UTILSHCD_API W_8DI_W
{
#include <HCD/8DI.h>
};

struct UTILSHCD_API W_8DI : CHcdBase, W_8DI_W
{
  static LPCSTR ClassName;
  enum { TypeID = id_8DI };
  static void BuildList();
};
//////////////////////////////////////////////////////////////////////////
struct UTILSHCD_API W_AI_W
{
#include <HCD/AI.h>
};

struct UTILSHCD_API W_AI : CHcdBase, W_AI_W
{
  static LPCSTR ClassName;
  enum { TypeID = id_AI };
  static void BuildList();
};
//////////////////////////////////////////////////////////////////////////
struct UTILSHCD_API W_AO_W
{
#include <HCD/AO.h>
};

struct UTILSHCD_API W_AO : CHcdBase, W_AO_W
{
  static LPCSTR ClassName;
  enum { TypeID = id_AO };
  static void BuildList();
};
//////////////////////////////////////////////////////////////////////////
struct UTILSHCD_API W_DO_W
{
#include <HCD/DO.h>
};

struct UTILSHCD_API W_DO : CHcdBase, W_DO_W
{
  static LPCSTR ClassName;
  enum { TypeID = id_DO };
  static void BuildList();
};
//////////////////////////////////////////////////////////////////////////
struct UTILSHCD_API W_8DO_W
{
#include <HCD/8DO.h>
};

struct UTILSHCD_API W_8DO : CHcdBase, W_8DO_W
{
  static LPCSTR ClassName;
  enum { TypeID = id_8DO };
  static void BuildList();
};