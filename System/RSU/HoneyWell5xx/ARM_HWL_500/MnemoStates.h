#pragma once

#include <HoneyMnemoExtend.h>
#include <string>

#define MAX_SENSORMNEMO 80
struct SAlarmsOnMnemo2
{
    bool bRedNotAck;
    bool bYellowNotAck;
	bool bBlueNotAck;
	bool bRedBlink;
    bool bYellowBlink;
	bool bBlueBlink;
};

class CMnemoStates
{
    //KHoneyPipeClient &mPipe;
 //   KHoneyPipeClient mPipe;
    std::string m_OperName;   
    int m_countManemoData;
public:  
    struct
    {
        std::string mnemoName;
        int mnemoState;
        int m_nNumber;
    }
    m_ManemoData[MAX_SENSORMNEMO];

    //CMnemoStates(char *OperName, KHoneyPipeClient &mmPipe);
    CMnemoStates(char *OperName);
    virtual ~CMnemoStates(void);
    void OnTimer(void);
};
