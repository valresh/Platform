#include <ConnectY.h>
#include <crossplatform.h>

tAlarm pAlarm = NULL;
////////////////////////////////////////////////
tShowLogic pShowLogic = NULL;
//tCreateLCWnd pCreateLCWnd = NULL;
//tCreateST16Wnd pCreateST16Wnd = NULL;
//tCreateCalcuBaseWnd pCreateCalcuBaseWnd = NULL;
tOpenDialSYSObjParams pOpenDialSYSObjParams = NULL;

tSet_Y_Graf_Data pSet_Y_Graf_Data = NULL;
tAlarmSignal pAlarmSignal = NULL;
tFindYObj pFindYObj = NULL;
tFindYObjN pFindYObjN = NULL;
tFindYObjTyped pFindYObjTyped = NULL;
tFindDObj pFindDObj = NULL;
tFindDI pFindDI = NULL;
tGetDR pGetDR = NULL;
tExecConnectionTransfer pExecConnectionTransfer = NULL;
//tFindAndShowYBlk pFindAndShowYBlk = NULL;
//tShowCalcuCDebugWindow pShowCalcuCDebugWindow = NULL;
tGetYBaseWrapper pGetYBaseWrapper = NULL;
tGetYBaseByBaseModel pGetYBaseByBaseModel = NULL;
tFindObjByNumb pFindObjByNumb = NULL;
tSendOG pSendOG = NULL;
tSendOGraw pSendOGraw = NULL;
tDialogue pDialogue = NULL;
tSendRQ pSendRQ = NULL;
tRecipeLoad pRecipeLoad = NULL;
////////////////////////////////////////////////

tRegisterYWstructs pRegisterYWstructs = NULL;

tYokoStructSaveState pYokoStructSaveState = NULL;
tYokoStructRestoreState pYokoStructRestoreState = NULL;

IYokoStateSer * pYokoStateSer = NULL;
int g_CalcStep = 0;
