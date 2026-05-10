#include "RSU.h"
#include "CommProc.h"
#include "Err.h"
#include "Split.h"
#include "RSU/rsu_basemodel.h"

#define countof(array) (sizeof(array)/sizeof(array[0]))
static rsu_cp::RsuClientsHolder g_RsuHolder;
extern IBaseModel *g_RuntimeModels[eRM_COUNT];

IBaseModel* Load_ObjectRSU( const char * DllName, const char * ObjName);

void LoadRsuModelsCP(int& iRuntimeModel)
{
    char Path[1024];
    Sprintf ( Path, "%sDATA/Acy/Models.csv", PROJECT_ROOT );
    CSplit<3,64> Split;
    if ( !Split.Open(Path ))
    {
        SysMSG( "Файл '%s' не найден ", Path );
        return;
    }
    while( Split.Next() )
        {
        if ( Split.Cols[0][0] == '#' )
            continue;
        ASS( iRuntimeModel < eRM_COUNT );
        IBaseModel* pO = FindBaseModel(Split.Cols[1]);
        if (!pO)
            {
            char * Model = Split.Cols[0];
            char * Obj = Split.Cols[1];
            pO = Load_ObjectRSU( Model, Obj );
            }
        ASS(pO);
        pO->PropsWasRead = strcmp( Split.Cols[2], "Save") == 0 ;
        g_RuntimeModels[iRuntimeModel++] = pO;
        pO->ModelFlags = Flag_NoAll;
        }
    Split.Close();
    }

IBaseModel * Load_ObjectEx ( const char * DllName, const char * ObjName );

IBaseModel* Load_ObjectRSU( const char * DllName, const char * ObjName)
    {
    bool v_ValidLib = false;
    IBaseModel* ret = g_RsuHolder.create_client (DllName, "", ObjName, v_ValidLib );
    if ( !ret && !v_ValidLib)
        ret = Load_ObjectEx ( DllName, ObjName );
    if (!ret)
        {
        std::string v_msg = "Load rsu library error: " + g_RsuHolder.get_last_error();
        SysMSG(v_msg.c_str());
        }
    return ret;
    }

