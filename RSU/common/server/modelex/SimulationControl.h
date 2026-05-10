#pragma once
#include "thread"
#include "api.h"
#include "Prop.h"
#include "tray.h"
#include "LogFile.h"
#include "MqttSettings.h"

namespace SimulatorControl 
{
    static int StartModelExServer(const MqttSettings& mqttSettings)
    {
        return modelex::StartModelExServer(
            mqttSettings.Host(),
            mqttSettings.Port(),
            mqttSettings.User(),
            mqttSettings.Password(),
            true);
    }

    static void StopModelExServer()
    {
        modelex::StopModelExServer();
    }

#ifdef _DEBUG
    static void OnStartPausedSimulation(struct tray_menu* item = nullptr)
    {
        Defs::MODEL_START modelStart =
        {
            .sStartFile = "N",
            .sConfgFile = "N",
            .sStateFile = "",
            .sParamFile = "",
            .AccelZ = 1.0,
            .IsPause = true
        };
        std::thread([modelStart]()
        {
            modelex::StartSimulation(modelStart);
        }).detach();
    }

    static void OnStartSimulation(struct tray_menu* item = nullptr)
    {
        Defs::MODEL_START modelStart =
        {
            .sStartFile = "N",
            .sConfgFile = "N",
            .sStateFile = "",
            .sParamFile = "",
            .AccelZ = 1.0,
            .IsPause = false
        };
        std::thread([modelStart]()
        {
            modelex::StartSimulation(modelStart);
        }).detach();
    }

    static void OnRestartModelEx(struct tray_menu* item = nullptr)
    {
        modelex::RestartModelExServer();
    }
#endif
};