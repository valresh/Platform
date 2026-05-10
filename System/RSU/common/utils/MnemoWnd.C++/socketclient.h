#pragma once
//#include <stdio.h>
//#include <tchar.h>
//#include <conio.h>
#include <string>
#include <thread>
#include <mutex>
#include "managed.h"
#include "Sterver.h"

//#include <vector>
	
//enum SOCKET_STATE
//	{
//		scNotinitialized = 0,
//        scProcessing,
//		scErrorEnded,
//        scOkEnded
//	};	
//
//typedef struct _SocketConnectStruct
//    {
//	    SOCKET_STATE m_SocketState;
//	    SOCKET hClient;
//		SOCKADDR_IN sinClient;
//    } SocketConnectStruct;
//
//enum INIT_STATE
//    {
//     istWaiting = 0,
//     istError,
//     istOk
//    };

class CLIENTDLL_API CSocketClient: public CSterver
{
private:
	//const std::string _clientId;
	//SOCKET hClient;
	//WSABUF buffer;
	//bool m_bSocketConnected;   
    //SocketConnectStruct m_soconst;
    //HANDLE m_hSocketConnectThread; 
	//std::thread _thread;
	//std::string _mqttServerUrl;
	//std::mutex _mutex;
	managed::INetworkHub & _networkHub;

public:
	CSocketClient(managed::INetworkHub & networkHub);
	virtual ~CSocketClient();
	//bool InitAndSendKey();
	void GetParam(const char* szObjName, const char* szParam, char* value);
	void GetParam(const char* szObjName, const char* szParam, bool &value);
    void GetParam(const char* szObjName, const char* szParam, int &value);
	void GetParam(const char* szObjName, const char* szParam, double &value);
    void SetParam(const char* szObjName, const char* szParam, const char* szParamSource, const char* szValue, const char* szValueOld, bool bMessage, EDataTypes eType, EValueType evType);
	bool IsConnected();
	//void CloseSocket();
	bool SetSocketValue(EDataTypes nType, const char* szName, const char* szFieldOld, double dValue, double dValueOld, bool bMessage, EValueType evType);
	bool GetData(UINT type, LPCTSTR name, CShBase*);
	void Refresh();
	const char* GetModelPseudonym(UINT nType, const char* szFieldOld);

private:
	//void GetParam(const char* szObjName, const char* szParam);
	bool InitAndSendKey(const char* szHostName);
	void FillBaseStructsFromSocket();
	void FillBaseStructFromSocket(SSterver* obj);
	// void SendOperatorToAllSocket();
	// bool GetModelData(UINT nType, int nSocketNumber, LPCTSTR szName, LPCTSTR szParam);
	// void ChangeBusy(int nType, const char* szName, bool bIncrement);
	// int GetBusy(int nType, const char* szName);
	// void Begin();
	// INIT_STATE InitSocket(const char* szHostName);
	//bool SendKey();
	//void SendOperator();
	
	//void SocketThread();
	//bool TryMqttConnect(struct mg_mgr* mqttManager);
	//static void OnMqttEvent(struct mg_connection* c, int ev, void* ev_data);
};
