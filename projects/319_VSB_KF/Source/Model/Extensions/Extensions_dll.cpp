#include "stdafx.h"
#include "Extensions.h"
#include "Info.h"
#include "CommProc.h"
#include "Err.h"

#ifdef LINUX
#else
BOOL APIENTRY DllMain(_HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
  return TRUE;
}
#endif


IExtension * ReadExtension(struct IBaseModel * pModel)
{
	char * ParamsName[] = {"Dll", "Sub", NULL};
	char Params[2][256];
	CLEAR(Params)
	int K = Split(pModel->Model.Str, "Model", (const char **)ParamsName, Params);
	if(K != 2) return NULL;
	if(Params[0] == 0)
	{
		ModelMsg(pModel, "Не задано имя DLL расширения модели '%s'", pModel->Model.Str);
		return NULL;
	}
	if(Params[1] == 0)
	{
		ModelMsg(pModel, "Не задано имя входа DLL расширения модели '%s'", pModel->Model.Str);
		return NULL;
	}
	IExtension * pExt = Get_Ext( Params[0], Params[1] );
	if(pExt == NULL)
	{
		ModelMsg(pModel, "Ошибка загрузки DLL расширения модели '%s'", pModel->Model.Str);
		return NULL;
	}
	if(!pExt->ReadData(pModel))
	{
		ModelMsg(pModel, "Ошибка чтения данных расширения модели '%s'", pModel->Model.Str);
		return NULL;
	}
	return pExt;
}

typedef IExtension * (*tGetExtension)(const char * Name);

IExtension * Get_Ext(char * Dll, char * Point)
{
    // HMODULE hLib = NULL;
    // hLib = (HMODULE)Load_Dll(Dll, "PROJECT");
    // if(hLib == NULL) return NULL;
    // tGetExtension pGetExtension = (tGetExtension)GetProcAddress(hLib, "GetExtension");
    // if(pGetExtension == NULL)
    // {
    // 	pGetExtension = (tGetExtension)GetProcAddress(hLib, "GetModelExtension");
    // 	if(pGetExtension == NULL) return NULL;
    // }
    // IExtension * pExt = (*pGetExtension)(Point);
    // return pExt;
    return NULL;
}
