#include "socketclient.h"
#include "PipeClient.h"
//#include <sstream>
//#include <set>
//#include <process.h>
//#include <stdexcept>
//#include "LogFile.h"

using namespace std;

//const UINT g_nPort = 20200;
//const UINT g_nKey = 0xBD610CC9;

//set<CSocketClient*> g_SocketSet;   //все имеющиеся сокет-клиенты

//void cdecl SocketConnectThread(SocketConnectStruct *soconst)
//{
//	if(WSAConnect(soconst->hClient, (SOCKADDR*)&soconst->sinClient, sizeof(soconst->sinClient), NULL, NULL, NULL, NULL))
//	  {
//        soconst->m_SocketState = scErrorEnded;
//	  }
//	else
//	  {
//	    soconst->m_SocketState = scOkEnded;
//	  }
//	_endthread();
//}

CSocketClient::CSocketClient(managed::INetworkHub& networkHub)
	: _networkHub(networkHub)
	//://hClient(NULL)
	 //m_bSocketConnected(false)
	//, m_nActive(-1)
	//, m_nCount(0)
	//, vectorData(NULL)
	//, m_nReservedSize(0)
		//:_clientId(clientId)
{
	//_socketClient = socketClient;
	//_mqttServerUrl = cross::format(MQTT_URL, serverName);
	//buffer.len = 0;
	//buffer.buf = new char[1024];

	//m_soconst.m_SocketState = scNotinitialized;
	//m_soconst.hClient = NULL;
	//memset(&m_soconst.sinClient, 0, sizeof(m_soconst.sinClient));
	//m_hSocketConnectThread = NULL;
	//_thread = std::thread(&CSocketClient::SocketThread, this);
}

CSocketClient::~CSocketClient()
{
	//if(buffer.buf)
	//	delete[] buffer.buf;
	//CloseSocket();
	//ClearVector();
	//if (vectorData)
	//	delete vectorData;
}

//void CSocketClient::SocketThread()
//{
//	struct mg_mgr mqttManager;
//	mg_mgr_init(&mqttManager);
//	
//	//struct mg_mqtt_opts opts;
//	//memset(&opts, 0, sizeof(opts));
//	//opts.client_id = mg_str_s(_clientId.c_str());
//	//mg_mqtt_prop properties[1];
//	//properties[0].id = MQTT_PROP_TOPIC_ALIAS;
//	//properties[0].iv = _topicAlias;
//	//opts.num_props = 1;
//	//opts.qos = 0;
//	//opts.props = properties;
//	//std::cerr << "Drawing channel " << _channelName << " started" << std::endl;
//	//std::chrono::nanoseconds frameDuration{ 1000000000 / _maxFrameRate };
//
//	while (true)
//	{
//		TryMqttConnect(&mqttManager);
//		//{
//		//	auto startTime = std::chrono::high_resolution_clock::now();
//		//	auto shouldRenderAll = _shouldRenderAll.exchange(false);
//		//	auto dataBuffer = _renderer.Render(shouldRenderAll);
//		//	_shouldRenderAll = false;
//		//	if (dataBuffer.size() > 0)
//		//	{
//		//		opts.message = mg_str_n(dataBuffer.data(), dataBuffer.size());
//		//		mg_mqtt_pub(_mqttConnection, &opts);
//		//	}
//
//		//	mg_mgr_poll(&mqttManager, 1);
//		//	std::chrono::nanoseconds delta = std::chrono::high_resolution_clock::now() - startTime;
//		//	std::this_thread::sleep_for(frameDuration - delta);
//		//}
//		//else
//		//{
//			mg_mgr_poll(&mqttManager, 3000); // ms
//		//}
//	}
//
//	/*if (_mqttConnection)
//	{
//		struct mg_mqtt_opts opts;
//		memset(&opts, 0, sizeof(opts));
//		opts.client_id = mg_str_n(_clientId.c_str(), _clientId.size());
//		opts.version = 5;
//		mg_mqtt_prop properties[1];
//		properties[0].id = MQTT_PROP_REASON_STRING;
//		properties[0].val = mg_str("No more subscribers");
//		opts.num_props = 1;
//		opts.props = properties;
//		mg_mqtt_disconnect(_mqttConnection, &opts);
//		mg_mgr_poll(&mqttManager, 100);
//		_mqttConnection = nullptr;
//	}*/
//
//	mg_mgr_free(&mqttManager);
//	//std::cerr << "Drawing channel " << _channelName << " closed" << std::endl;
//}

bool CSocketClient::IsConnected()
{
	return _networkHub.IsConnected();
};

//bool CSocketClient::TryMqttConnect(struct mg_mgr* mqttManager)
//{
//	if (_mqttConnection)
//	{
//		return true;
//	}
//
//	// connection
//	struct mg_mqtt_opts opts;
//	memset(&opts, 0, sizeof(opts));
//	//opts.qos = 0;
//	opts.clean = true;
//	opts.client_id = mg_str_s(_clientId.c_str());
//	opts.version = 5;
//	//mg_mqtt_prop properties[1];
//	//properties[0].id = MQTT_PROP_TOPIC_ALIAS_MAXIMUM;
//	//properties[0].iv = MAX_TOPIC_ALIASES;
//	//opts.num_props = 1;
//	//opts.props = properties;
//	_mqttConnection = mg_mqtt_connect(mqttManager, _mqttServerUrl.c_str(), &opts, OnMqttEvent, this);
//	if (!_mqttConnection)
//	{
//		return false;
//	}
//
//	// subscribe for wills
//	//opts.topic = mg_str(_willChannelName.c_str());
//	//opts.num_props = 0;
//	//opts.props = nullptr;
//	//mg_mqtt_sub(_mqttConnection, &opts);
//
//	// topic alias publishing (it delivers empty message to clients also)
//	//opts.topic = mg_str(_channelName.c_str());
//	//properties[0].id = MQTT_PROP_TOPIC_ALIAS;
//	//properties[0].iv = _topicAlias;
//	//opts.num_props = 1;
//	//opts.props = properties;
//	//mg_mqtt_pub(_mqttConnection, &opts);
//	return true;
//}

//void CSocketClient::OnMqttEvent(struct mg_connection* c, int ev, void* ev_data)
//{
//	auto socketClient = (CSocketClient*)c->fn_data;
//	switch (ev)
//	{
//	case MG_EV_CLOSE:
//		socketClient->_mqttConnection = nullptr;
//		break;
//
//	case MG_EV_MQTT_MSG:
//		auto message = (mg_mqtt_message*)ev_data;
//	//	if (strcmp(message->topic.buf, socketClient->_willChannelName.c_str()) == 0)
//	//	{
//	//		// If unexpected will message received
//	//		std::lock_guard lock(socketClient->_startStopMutex);
//	//		drawingChannel->_activeConnections--;
//	//	}
//		std::string response = std::string(message->data.buf, message->data.len);
//		nlohmann::json j = nlohmann::json::parse(response);
//		//INSTR_MODEL_NOTIF nRequest;
//		//j["Request"].get_to(nRequest);
//		//nlohmann::json args;
//		//j["Arguments"].get_to(args);
//		//auto outJ = self->OnCommand(nRequest, args, mm);
//		//self->PublishResponse(outJ, mm);
//
//
//		break;
//	}
//}

//void CSocketClient::SendCommand(const std::string& resp, mg_mqtt_message* mm)
//{
//	if (resp == "")
//		return;
//	struct mg_mqtt_opts pub_opts;
//	memset(&pub_opts, 0, sizeof(pub_opts));
//	std::string topic(mm->topic.buf, mm->topic.len);
//	auto outtopic = std::regex_replace(topic, std::regex("/In/"), "/Out/");
//	pub_opts.topic = mg_str(outtopic.c_str());
//	pub_opts.message = mg_str(resp.c_str());
//	pub_opts.qos = 1;
//	mg_mqtt_pub(s_conn, &pub_opts);
//}

//bool CSocketClient::InitAndSendKey()
//{
//	return InitAndSendKey(CPipeClient::ms_szServer.c_str());
//}
//
//bool CSocketClient::InitAndSendKey(const char* szHostName)
//{
//    CLogFile::LogEx (LOGLEVEL_HIGH, "CSocketClient::(%s) InitAndSendKey_Start", m_szSocketName);
//	switch(InitSocket(szHostName))
//      {
//    case istWaiting:
//        m_bSocketConnected = false;
//        break;
//    case istOk:
//        m_bSocketConnected = true;
//        SendKey();
//        SendOperator();
//        CLogFile::LogEx (LOGLEVEL_HIGH, "CSocketClient::(%s) InitAndSendKey CONNECTED", m_szSocketName);
//        break;
//    default:
//        m_bSocketConnected = false;
//        CloseSocket();	
//        break;
//      }
//	CLogFile::LogEx (LOGLEVEL_HIGH, "CSocketClient::(%s) InitAndSendKey_End", m_szSocketName);
//	return m_bSocketConnected;
//}

//INIT_STATE CSocketClient::InitSocket(const char* szHostName)
//{
//	if(!szHostName)
//		return istError;
//
//    switch(m_soconst.m_SocketState)
//    {
// case scNotinitialized:
//     {
//		 CLogFile::LogEx (LOGLEVEL_HIGH, "CSocketClient::InitSocket_11: g_nPort %d", g_nPort);
//	hostent* clientInfo = gethostbyname(szHostName);
//	if(!clientInfo)
//		return istError;
//	hClient = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, NULL, 0);
//	CLogFile::LogEx (LOGLEVEL_HIGH, "CSocketClient::InitSocket_13");
//	if(!hClient)
//		return istError;
//	SOCKADDR_IN sinClient;
//	sinClient.sin_family = clientInfo->h_addrtype;
//	sinClient.sin_port = htons(g_nPort);
//	sinClient.sin_addr = **(IN_ADDR**)clientInfo->h_addr_list;
//    m_soconst.sinClient = sinClient;
//    m_soconst.m_SocketState = scProcessing;
//    m_soconst.hClient = hClient;
//    m_hSocketConnectThread = (HANDLE)_beginthread((void (__cdecl *)(void *))SocketConnectThread, 0, (LPVOID)&m_soconst);
//	CLogFile::LogEx (LOGLEVEL_HIGH, "CSocketClient::InitSocket_14");
//     }
//    return istWaiting;
//
// case scProcessing:
//	 CLogFile::LogEx (LOGLEVEL_HIGH, "CSocketClient::InitSocket_2");
//    return istWaiting;
//
// case scOkEnded:
//	 CLogFile::LogEx (LOGLEVEL_HIGH, "CSocketClient::InitSocket_3");
//    m_soconst.m_SocketState = scNotinitialized;
//    m_hSocketConnectThread = NULL;
//    return istOk;
//
// default:
//    m_soconst.m_SocketState = scNotinitialized;
//    m_hSocketConnectThread = NULL;
//	return istError;
//    }
//}

//bool CSocketClient::SendKey()
//{
//	memcpy(buffer.buf, &g_nKey, sizeof(g_nKey));
//	DWORD nByte;
//	buffer.len = sizeof(g_nKey);
//	if (hClient!=NULL)
//		WSASend(hClient, &buffer, 1, &nByte, 0, NULL, 0);
//	else
//		return false;
//	return true;
//}

//void CSocketClient::SendOperator()
//{	
	//if ( hClient!=NULL && buffer.buf!=NULL)
 //   { //посылаем оператора на сервер
	//  //для CENTUM было User@HIS0164, в журнал надо писать только User
		//std::string sOperatorFullName(CPipeClient::ms_szOperator);
	//	std::string sOperator=sOperatorFullName;
	//	int nFind=sOperatorFullName.find("@HIS");
	//	if ((nFind!=std::string::npos)&&(nFind!=0))
	//		sOperator=sOperatorFullName.substr(0,nFind);
 //       lstrcpyA(buffer.buf + sizeof(int), "Operator"); 
	//	lstrcpyA(buffer.buf + sizeof(int) + sizeof("Operator"), sOperator.c_str());	 
 //       buffer.len = (u_long)(sizeof("Operator") + sOperator.length() + 1);
 //       *((int*)buffer.buf) = buffer.len;
 //       buffer.len += sizeof(int);
 //       DWORD nByte;
	//	WSASend(hClient, &buffer, 1, &nByte, 0, NULL, 0);
 //   }
//}


//void CSocketClient::SendOperatorToAllSocket()
//{	
//	CLogFile::LogEx (LOGLEVEL_HIGH, "CSocketClient::SendOperatorToAllSocket_1");
//    for (set<CSocketClient*>::iterator iter = g_SocketSet.begin(); iter != g_SocketSet.end(); ++iter)
//    {
//         if(!(*iter)->IsConnected())
//           {
//			 if(!(*iter)->InitAndSendKey())
//               continue;
//           }
//		 (*iter)->SendOperator();
//    }
//	CLogFile::LogEx (LOGLEVEL_HIGH, "CSocketClient::SendOperatorToAllSocket_2");
//} 

//void CSocketClient::ChangeBusy(int nType, const char* szName, bool bIncrement)
//{
//	//TODO SERG Здесь надо послать увеличение счетчика при bIncrement true
//	//TODO SERG Здесь надо послать уменьшение счетчика при bIncrement false
//}
//
//int CSocketClient::GetBusy(int nType,const char* szName)
//{
//	//TODO SERG Здесь надо получить значение счетчика по имени
//	return 0;
//}

//void CSocketClient::GetParam(const char* szObjName, const char* szParam)
//{
//
//	//struct mg_mqtt_opts opts = {};
//	//auto topic = cross::format(MQTT_TOPIC, _clientId);
//	//opts.topic = mg_str(topic.c_str());
//	//auto command = cross::format(
//	//	"{{""Command"":{},""Params"":[""{}"", ""{}""]}}",
//	//	0, // GETPARAM
//	//	szObjName,
//	//	szParam);
//	//opts.message = mg_str(command.c_str());
//	//opts.qos = MQTT_QOS_EXACTLY_ONCE;
//	//{
//	//	std::lock_guard<std::mutex> lock(_mutex);
//	//	mg_mqtt_pub(_mqttConnection, &opts);
//	//}
//
//	//CLogFile::LogEx (LOGLEVEL_HIGH, "CSocketClient::(%s) GetParam_1: szObjName %s", m_szSocketName, szObjName);
//	//buffer.len = sizeof(int);
//	//const char* szGetParam = "GetParam";
//	//lstrcpy(&buffer.buf[buffer.len], szGetParam);
//	//buffer.len += lstrlen(szGetParam) + 1;
//	//lstrcpy(&buffer.buf[buffer.len], szObjName);
//	//buffer.len += lstrlen(szObjName) + 1;
//	//lstrcpy(&buffer.buf[buffer.len], szParam);
//	//buffer.len += lstrlen(szParam) + 1;
//	//int nSize = buffer.len - sizeof(int);
//	//memcpy(buffer.buf, &nSize, sizeof(nSize));
//	//DWORD nByte;
//	//if(WSASend(hClient, &buffer, 1, &nByte, 0, NULL, 0))
// //     {
//	//	  CLogFile::LogEx (LOGLEVEL_HIGH, "Calling CloseSocket 1");
// //      CloseSocket();
// //      return;
// //     }
//	////
//	//buffer.len = sizeof(int);
//	//DWORD nFlags = 0;
//	//if(WSARecv(hClient, &buffer, 1, &nByte, &nFlags, NULL, 0))
// //     {
//	//	  CLogFile::LogEx (LOGLEVEL_HIGH, "Calling CloseSocket 2");
// //      CloseSocket();
// //      return;
// //     }
//	//buffer.len = *(int*)buffer.buf;
//	//if(WSARecv(hClient, &buffer, 1, &nByte, &nFlags, NULL, 0))
// //     {
//	//	  CLogFile::LogEx (LOGLEVEL_HIGH, "Calling CloseSocket 3");
// //      CloseSocket();
// //      return;
// //     }
//	//CLogFile::LogEx (LOGLEVEL_HIGH, "CSocketClient::(%s) GetParam_2", m_szSocketName, szObjName);
//}

void CSocketClient::SetParam(const char* szObjName, const char* szParam, const char* szParamSource,
	const char* szValue, const char* szValueOld, bool bMessage, EDataTypes eType, EValueType evType)
{
	_networkHub.SetParameter(szObjName, szParam, szParamSource, szValue, szValueOld, bMessage, eType, evType);
	//buffer.len = sizeof(int);
	//const char* szSetParam = "SetParam";
	//lstrcpy(&buffer.buf[buffer.len], szSetParam);
	//buffer.len += lstrlen(szSetParam) + 1;
	//lstrcpy(&buffer.buf[buffer.len], szObjName);
	//buffer.len += lstrlen(szObjName) + 1;
	//lstrcpy(&buffer.buf[buffer.len], szParam);
	//buffer.len += lstrlen(szParam) + 1;
	//lstrcpy(&buffer.buf[buffer.len], szParamSource);
	//buffer.len += lstrlen(szParamSource) + 1;
	//lstrcpy(&buffer.buf[buffer.len], szValue);
	//buffer.len += lstrlen(szValue) + 1;
	//lstrcpy(&buffer.buf[buffer.len], szValueOld);
	//buffer.len += lstrlen(szValueOld) + 1;
 //   char Buf[32];
 //   sprintf_s(Buf, "%d", bMessage);
 //   lstrcpy(&buffer.buf[buffer.len], Buf);
 //   buffer.len += lstrlen(Buf) + 1;
 //   sprintf_s(Buf, "%d", eType);
 //   lstrcpy(&buffer.buf[buffer.len], Buf);
 //   buffer.len += lstrlen(Buf) + 1;
 //   sprintf_s(Buf, "%d", evType);
 //   lstrcpy(&buffer.buf[buffer.len], Buf);
 //   buffer.len += lstrlen(Buf) + 1;

	//int nSize = buffer.len - sizeof(int);
	//memcpy(buffer.buf, &nSize, sizeof(nSize));
	//DWORD nByte;
	//if(WSASend(hClient, &buffer, 1, &nByte, 0, NULL, 0))
 //     {
	//	  CLogFile::LogEx (LOGLEVEL_HIGH, "Calling CloseSocket 4");
 //      CloseSocket();
 //      return;
 //     }
	////
	//buffer.len = sizeof(int);
	//DWORD nFlags = 0;
	//if(WSARecv(hClient, &buffer, 1, &nByte, &nFlags, NULL, 0))
 //     {
	//	  CLogFile::LogEx (LOGLEVEL_HIGH, "Calling CloseSocket 5");
 //      CloseSocket();
 //      return;
 //     }
	//buffer.len = *(int*)buffer.buf;
	//if(WSARecv(hClient, &buffer, 1, &nByte, &nFlags, NULL, 0))
 //     {
	//	  CLogFile::LogEx (LOGLEVEL_HIGH, "Calling CloseSocket 6");
 //      CloseSocket();
 //      return;
 //     }
	//CLogFile::LogEx (LOGLEVEL_HIGH, "CSocketClient::SetParam_2");
}

void CSocketClient::GetParam(const char* szObjName, const char* szParam, char* value)
{
	_networkHub.GetString(szObjName, szParam, value);
	//GetParam(szObjName, szParam);
	//if(!value)
	//	return;		
	//lstrcpy(value, buffer.buf);
}

void CSocketClient::GetParam(const char* szObjName, const char* szParam, bool& value)
{
	_networkHub.GetBool(szObjName, szParam, value);
	//GetParam(szObjName, szParam);
	//int nValue=atoi(buffer.buf);
	//value = (nValue != 0);
}

void CSocketClient::GetParam(const char* szObjName, const char* szParam, int& value)
{
	_networkHub.GetInt(szObjName, szParam, value);
	//GetParam(szObjName, szParam);
	//value = atoi(buffer.buf);
}

void CSocketClient::GetParam(const char* szObjName, const char* szParam, double& value)
{
	_networkHub.GetDouble(szObjName, szParam, value);
	//GetParam(szObjName, szParam);
	//value = atof(buffer.buf);
}

//void CSocketClient::CloseSocket()
//{
//	//closesocket(hClient);
// //   hClient = NULL;
// //   m_bSocketConnected = false;
// //   CLogFile::LogEx (LOGLEVEL_HIGH, "CSocketClient::(%s) CLOSE", m_szSocketName);
//}

void CSocketClient::Refresh()
{
	//  int nSize = 0;
	//  CLogFile::LogEx (LOGLEVEL_HIGH, "CSocketClient::(%s) Refresh_1: connected %d, nSize %d", m_szSocketName, IsConnected(), nSize);
	//if ( !IsConnected() )
 //     { // Если нет соединения - пытаемся соединиться
 //       if(!InitAndSendKey())
 //         return;
 //     }
 //   else
 //     {//проверяем соединение на случай отсутствия операций ввода/вывода
 //      int error_code;
 //      int error_code_size = sizeof(error_code);
 //      int retval = getsockopt(hClient, SOL_SOCKET, SO_ERROR, (char*)&error_code, &error_code_size);
 //      if(retval)
	//   {
	//	   CLogFile::LogEx (LOGLEVEL_HIGH, "Calling CloseSocket 7");
 //         CloseSocket();
	//   }
 //     }

	if (IsConnected())
	{
		FillBaseStructsFromSocket();
	}
	//CLogFile::LogEx (LOGLEVEL_HIGH, "CSocketClient::(%s) Refresh_2: nSize %d", m_szSocketName, nSize);
}

void CSocketClient::FillBaseStructsFromSocket()
{
	UINT   nCount = Count();
	for (UINT n = 0; n < nCount; n++)
	{
		SSterver* obj = Item(n);
		if (obj != NULL)
			FillBaseStructFromSocket(obj);
	}
}

bool CSocketClient::GetData(UINT nType, LPCTSTR szName, CShBase* shBase)
{
	if (!IsConnected())
		return false;
	if (nType == id_Zadv)
	{
		CShZadv* sh = (CShZadv*)shBase;
		sh->RetCode = CShBase::rcOK;
		GetParam(szName, "#Положение", sh->VentZ);
		sh->Vent = sh->VentZ;
		bool bZaglushka = false;
		GetParam(szName, "#Заглушка", bZaglushka);
		sh->bZaglushka = bZaglushka;
		bool bStopWorkDeffect = false;
		bool bStartWorkDeffect = false;
		GetParam(szName, "#+Заклинивание поворотного механизма", bStopWorkDeffect);
		if (bStopWorkDeffect)
			sh->On_Blk(CZadv::ZADV_BROKEN);
		else
			sh->Off_Blk(CZadv::ZADV_BROKEN);
		GetParam(szName, "#+Исправна", bStartWorkDeffect);
		if (bStartWorkDeffect)
			sh->Off_Blk(CZadv::ZADV_BROKEN);
		return true;
	}
	else
		if (nType == id_Pump)
		{
			CShPump* sh = (CShPump*)shBase;
			sh->RetCode = CShBase::rcOK;
			bool bPusk = false;
			GetParam(szName, "#Пуск/Стоп", bPusk);
			if (bPusk)
				sh->On_State(CPumpData::Work);
			else
				sh->Off_State(CPumpData::Work);
			return true;
		}
		else
			if (nType == id_Klapan)
			{
				CShKlapan* sh = (CShKlapan*)shBase;
				sh->RetCode = CShBase::rcOK;
				char strType[64] = {};
				GetParam(szName, "#Тип клапана", strType);
				sh->Type_Flags = 0;
				if (strcmp(strType, "EV") == 0)
				{
					sh->On_Type(CKlapan::IS_ELECTRO_ZADV);
					GetParam(szName, "#Положение колеса дублера", sh->Vent_Dubler);
					GetParam(szName, "#Зацепление", sh->m_bZacepka);
					GetParam(szName, "#Мест-Дист", sh->LocalRemoteDouble);
				}
				else
					if (lstrcmp(strType, "CVFA") == 0)
					{
						sh->On_Type(CKlapan::IN_ZADV);
						sh->On_Type(CKlapan::OUT_ZADV);
						sh->On_Type(CKlapan::BP_ZADV);
						GetParam(szName, "#Положение колеса дублера", sh->Vent_Dubler);
						GetParam(szName, "#Зацепление", sh->m_bZacepka);
					}
					else
						if ((lstrcmp(strType, "CV") == 0) || (lstrcmp(strType, "IVFC") == 0)
							|| (lstrcmp(strType, "IVFO") == 0))
						{
							GetParam(szName, "#Положение колеса дублера", sh->Vent_Dubler);
							GetParam(szName, "#Зацепление", sh->m_bZacepka);
						}
				GetParam(szName, "#Положение", sh->Vent);
				GetParam(szName, "#Вход", sh->Vent_In);
				GetParam(szName, "#Выход", sh->Vent_Out);
				GetParam(szName, "#Байпас", sh->Vent_BP);
				return true;
			}
			else
				if (nType == id_KVO)
				{
					CShKVO* sh = (CShKVO*)shBase;
					sh->RetCode = CShBase::rcOK;
					bool bWork = false;
					GetParam(szName, "#Работает", bWork);
					if (bWork)
						sh->On_State(CKVO::On);
					else
						sh->Off_State(CKVO::On);
					return true;
				}
				else
					if (nType == id_Sensor)
					{
						CShSensor* sh = (CShSensor*)shBase;
						sh->RetCode = CShBase::rcOK;
						GetParam(szName, "#Показания(разм)", sh->ShowValue);
						GetParam(szName, "#Показания(физ)", sh->PhysValue);
						//Для уровня
						GetParam(szName, "#Уровень в объекте", sh->SensValue);
						GetParam(szName, "Единицы измерения", sh->Mes);
						return true;
					}
					else
						if (nType == id_HS)
						{
							CShHS* sh = (CShHS*)shBase;
							sh->RetCode = CShBase::rcOK;
							GetParam(szName, "#Открыта", sh->mOn);
							return true;
						}
						else
							if (nType == id_PHS)
							{
								CShPolyHS* sh = (CShPolyHS*)shBase;
								sh->RetCode = CShBase::rcOK;
								GetParam(szName, "#Открыт", sh->SP);
								return true;
							}
	if (nType == id_ModelData)
	{
		CShModelData* sh = (CShModelData*)shBase;
		sh->RetCode = CShBase::rcOK;
		//данные для этого типа берутся отдельно в методе GetModelData, передается еще и параметр
		return true;
	}
	else
		if (nType == id_GasAnalyz)
		{
			throw std::runtime_error("rework is required: GetBasePath() can't be used ");

			//      CShGasAnalyz* sh = (CShGasAnalyz*)shBase;
				  //sh->RetCode=CShBase::rcOK;
			//      char Path[512] = {""}; 
			//      lstrcpy ( Path, GetBasePath() );
			//      lstrcat ( Path, "Data\\Equipment\\Анализаторы\\" );
			//      lstrcat ( Path, szName );
			//      lstrcat ( Path, ".csv" );
			//      FILE *fin;
			//      if(fopen_s(&fin, Path, "r") == 0)
			//      {
			//      char buf[512];
			//      int ii=0;
			//      while( fgets(buf, _countof(buf)-1, fin) )
			//         {
			//           char *ref = strchr(buf, ';');
			//           if(ref)
			//             {
			//             *ref = 0;
			//             strcpy_s(sh->Name[ii], buf);
			//             char param[512] = "";
			//             param[0]= '#';
			//             strcpy_s(&param[1], _countof(param)-2, buf);
			//             GetParam(szName, param, sh->CC[ii]);
				  //	   if (Version==LG35_8_KF)
				  //		   sh->DecimalPlace[ii]=1;
			//             ii++;
			//             }
			//         }
			//      fclose(fin);
			//      }

			return true;
		}
	return false;
}

//bool CSocketClient::GetModelData(UINT nType, int nSocketNumber, LPCTSTR szName,LPCTSTR szParam)
//{
//	if (!IsConnected())
//		return false;
//	if (nSocketNumber<0)
//		return false;
//	CShBase* shBase = Sh( nSocketNumber );
//	if (nType==id_ModelData)
//	{
//		CShModelData* sh = (CShModelData*)shBase;
//		sh->RetCode=CShBase::rcOK;
//		GetParam(szName,szParam,sh->dValue);
//		return true;
//	}
//	return false;
//}

void CSocketClient::FillBaseStructFromSocket(SSterver* obj)
{
	char* szName = obj->szName + 1;
	GetData(obj->nType, szName, Base(*obj));
}

bool CSocketClient::SetSocketValue(EDataTypes nType, const char* szName, const char* szFieldOld, double dValue, double dValueOld, bool bMessage, EValueType evType)
{
	if (IsConnected())
	{
		std::ostringstream strs;
		strs << dValue;
		std::string strValue = strs.str();
		const char* szField = GetModelPseudonym(nType, szFieldOld);
		char szValueOld[32];
		sprintf_s(szValueOld, "%0.5f", dValueOld);
		SetParam(szName, szField, szFieldOld, strValue.c_str(), szValueOld, bMessage, nType, evType);
		return true;
	}

	return false;
}

const char* CSocketClient::GetModelPseudonym(UINT nType, const char* szFieldOld)
{
	switch (nType)
	{
	case id_Zadv:if (lstrcmp(szFieldOld, "Задание") == 0) return "#Положение"; break;
	case id_Klapan:
		if (lstrcmp(szFieldOld, "Дублёр") == 0) return "#Задание";
		if (lstrcmp(szFieldOld, "Зацепление") == 0) return "#Зацепление";
		if (lstrcmp(szFieldOld, "Зацепленный дублёр") == 0) return "#Положение колеса дублера";
		if (lstrcmp(szFieldOld, "Автозацеп дублёр") == 0) return "#Положение колеса дублера";
		if (lstrcmp(szFieldOld, "Открыть") == 0) return "#Команда 'Открыть'";
		if (lstrcmp(szFieldOld, "Закрыть") == 0) return "#Команда 'Закрыть'";
		if (lstrcmp(szFieldOld, "Остановить") == 0) return "#Команда 'Стоп'";
		if (lstrcmp(szFieldOld, "Дистанц.") == 0) return "#Мест-Дист";
		if (lstrcmp(szFieldOld, "Вход") == 0) return "#Вход";
		if (lstrcmp(szFieldOld, "Выход") == 0) return "#Выход";
		if (lstrcmp(szFieldOld, "Байпас") == 0) return "#Байпас";
		break;
	case id_Pump:if (lstrcmp(szFieldOld, "Вкл/Выкл") == 0) return "#Пуск/Стоп"; break;
	case id_KVO:if (lstrcmp(szFieldOld, "Вкл/Выкл") == 0) return "#Работает"; break;
	case id_HS:
		if (lstrcmp(szFieldOld, "MV") == 0) return "#Заглушка";
		if (lstrcmp(szFieldOld, "Заглушка") == 0) return "#Заглушка";
		break;
	}
	const char* szField = szFieldOld;
	return szField;
}

//void CSocketClient::Begin()
//  {
//    m_nCurrent = 0, mData.m_nCurrent = 0;
//  }