#pragma once

struct SLinkOTWR
{
  DWORD Station;
  DWORD Serial;
  DWORD fake1[2];
  char name[128];
};

struct SLinkSnd
{
  DWORD BitNo;
  DWORD SelfNull;
  DWORD fake1[2];
  char name[128];
};

typedef SLinkSnd SLinkSLWR;
