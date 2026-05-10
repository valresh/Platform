#pragma once
#include "MqttSettings.h"

class MqttConfigurator
{
public:
    
    // @brief Any errors will be ignored
    MqttSettings ReadSettings();
    
    // @throws std::runtime_error
    MqttSettings Configure(const MqttSettings &);
};