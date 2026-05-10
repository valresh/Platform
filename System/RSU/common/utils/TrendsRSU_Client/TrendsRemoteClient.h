#pragma once
#include <basemodel.h>
#include <SysDataTypes.h>
#include <TrendsSup.h>
#include <rsu_client.h>
#include "../TrendsKernel/TrendsRsuKernel.h"

struct SPluginImpl
{
    tTrendInfoPoint pInfo;
    tEnumIo_Objs pEnumObjs;
    tEnumIo_Objs pEnumIOs;
    char szDllName[64 * 4];
    SPluginImpl()
        : pInfo(NULL)
        , pEnumObjs(NULL)
        , pEnumIOs(NULL)
    {
        szDllName[0] = 0;
    }
};

class KTrendsRemoteClient : public KTrendsRsuKernel
{
    void LoadPlugins();
    SPluginImpl m_Plugins[30];
    std::vector<dylib*> m_loadedLibs;
    std::string m_ObjName = "";
    rsu_cp::RsuClientProxy m_proxy;
public:
    KTrendsRemoteClient(LPSTR ObjName, rsu_cp::RsuClient* a_cl);
    ~KTrendsRemoteClient();
    // Интерфейс IBaseModel
    virtual int Init(int);
    virtual int Step0();
    virtual int Step1();
    virtual int StepT(double dt);
    NEW
protected:
    void EnumPointsCount(tEnumIo_Objs pEnumerator, tTrendInfoPoint pInfo, int& pointCount, std::vector<STrendPointInfo>& pointsInfo);
    void AddPoints(tEnumIo_Objs pEnumerator, tTrendInfoPoint pInfo, std::vector<STrendPointInfo>& pointsInfo);
    virtual void FillArray();
    virtual void FillHeader();
    void WriteTrends();
};