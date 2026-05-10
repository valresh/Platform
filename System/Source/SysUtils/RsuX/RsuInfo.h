#pragma once
#include "basemodel.h"

#define MAX_RSU 32
#define MAX_MAIN_NAME 32

enum eSaveGroup { Model, RSU, Yoko };

struct CRSU_Info
{
	char MainName[MAX_MAIN_NAME];
	IBaseModel * pMain;
};

extern CRSU_Info RSU_Info[MAX_RSU];

int GetRsuCount();
