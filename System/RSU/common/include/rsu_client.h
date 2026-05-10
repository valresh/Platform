#pragma once
#include "rsu_mio.h"
#include <rsus/rsu_api.h>
#include <vector>
#include <map>
#include <memory>
#include <cstring>
#include <dylib.hpp>
#include "rsu_ex.h"
#include "rsu_err.h"
#include "rsu_cfg_ldr.h"
#include "SetDataTypes.h"

namespace rsu_cp {

#define RS_CLIENT_WS_TOUT 100
#define RS_CLIENT_RS_TOUT 100

    class RsuClient {
        RsuStdObjNames m_ObjNames;
        RsuExchangeClient m_xcl;
        int m_LastError = to_underlying(RSU_ERR::SUCCESS);
        std::string m_ErrorMessage = "";
    protected:
        RsuExchangeClient* io() {
            return &m_xcl;
        }

        int send_cmd(RSU_CMD a_cmd, const void* a_snd_data, size_t a_snd_data_sz, void* a_rcv_data, size_t& a_rcv_data_sz)
        {
            int ret = to_underlying(RSU_ERR::ERR);
            m_ErrorMessage = "";
            if (m_xcl.state())
            {
                RsuRequestCmd v_cmd(a_cmd, a_snd_data, a_snd_data_sz);
                if (m_xcl.send_cmd(&v_cmd))
                {
                    if (v_cmd.cmd == to_underlying(RSU_ERR::SUCCESS))
                    {
                        if (v_cmd.size <= a_rcv_data_sz)
                        {
                            if (v_cmd.size > 0)
                            {
                                a_rcv_data_sz = v_cmd.size;
                                memcpy(a_rcv_data, v_cmd.data, a_rcv_data_sz);
                            }
                            ret = to_underlying(RSU_ERR::SUCCESS);
                        }
                        else
                            m_ErrorMessage = "Invalid size of server data.";
                    }
                    else
                    {
                        if (v_cmd.size > 1)
                            m_ErrorMessage = "Server return error code with description: " + std::string((char*)v_cmd.data);
                        else
                            m_ErrorMessage = "Server return error code withot description.";
                    }
                }
            }
            else
                m_ErrorMessage = "Exchange initialization error.";
            m_LastError = ret;
            return ret;

        }
        const RsuStdObjNames* ObjNames() { return &m_ObjNames; };

        virtual int DoInit(int) = 0;
        virtual int DoStep0() = 0;
        virtual int DoStep1() = 0;
        virtual int DoStepT(double dt) = 0;
        virtual int DoCleanup() = 0;
        virtual int DoSaveState(const char* data) = 0;
        virtual int DoSaveParams(const char* data) = 0;
        virtual int DoRestoreState(const char* data) = 0;
        virtual int DoRestoreParams(const char* data) = 0;
#ifdef _WIN32

#endif 
        unsigned TypeDataCmd(int TypeData)
        {
            switch (TypeData)
            {
            case SetDataTypes::sd_SaveRSUParams:
                return to_underlying(RSU_CMD::PARAMS_SAVE);
            case SetDataTypes::sd_RestoreRSUParams:
                return to_underlying(RSU_CMD::PARAMS_RESTORE);
            case SetDataTypes::sd_SaveRSUState:
                return to_underlying(RSU_CMD::STATE_SAVE);
            case SetDataTypes::sd_RestoreRSUState:
                return to_underlying(RSU_CMD::STATE_RESTORE);
            }
            return 0;
        }
    public:
        virtual int InitCmd(int ret)
        {
            size_t rd = 0;
            return send_cmd(RSU_CMD::INIT, NULL, 0, NULL, rd);
        }

        virtual int Step0Cmd()
        {
            size_t rd = 0;
            return send_cmd(RSU_CMD::STEP_0, NULL, 0, NULL, rd);
        }

        virtual int Step1Cmd()
        {
            size_t rd = 0;
            return send_cmd(RSU_CMD::STEP_1, NULL, 0, NULL, rd);
        }

        virtual int StepTCmd(double dt)
        {
            size_t rd = 0;
            return send_cmd(RSU_CMD::STEP_T, &dt, sizeof(dt), NULL, rd);
        }
#ifdef _WIN32
        virtual int ShowObjectCmd(const char* a_name)
        {
            size_t rd = 0;
            return send_cmd(RSU_CMD::SHOW_OBJECT, a_name, strlen(a_name) + 1, NULL, rd);
        }
#endif // _WIN32
        virtual int SetDataCmd(int TypeData, const char* pData)
        {
            size_t rd = 0;
            unsigned cmd = TypeDataCmd(TypeData);
            if (cmd)
                return send_cmd(static_cast<RSU_CMD>(cmd), pData, strlen(pData) + 1, NULL, rd);
            return -1;
        }

        virtual int CleanupCmd()
        {
            size_t rd = 0;
            return send_cmd(RSU_CMD::CLEANUP, NULL, 0, NULL, rd);
        }

        bool state() {
            return m_xcl.state();
        }
        std::string ErrorMessageStr()
        {
            return m_ErrorMessage;
        }
        bool ErrorMessage(char* buf, size_t* buf_sz)
        {
            bool ret = m_ErrorMessage.length() == 0;
            if (!ret && buf_sz)
            {
                if (buf && m_ErrorMessage.size() < *buf_sz)
                {
                    strcpy(buf, m_ErrorMessage.c_str());
                    ret = true;;
                }
            }
            if (buf_sz)
                *buf_sz = m_ErrorMessage.size() + 1;
            return ret;
        }
        int LastError()
        {
            return m_LastError;
        }
        void set_last_error(int a_err, std::string a_msg)
        {
            m_LastError = a_err;
            m_ErrorMessage = a_msg;
        }
        int Init(int i)
        {
            return DoInit(i);
        }
        int Step0()
        {
            return DoStep0();
        }
        int Step1()
        {
            return DoStep1();
        }
        int StepT(double dt)
        {
            return DoStepT(dt);
        }
        int SaveState(const char* data)
        {
            return DoSaveState(data);
        }
        int SaveParams(const char* data)
        {
            return DoSaveParams(data);
        }
        int RestoreState(const char* data)
        {
            return DoRestoreState(data);
        }
        int RestoreParams(const char* data)
        {
            return DoRestoreParams(data);
        }
        int Cleanup()
        {
            return DoCleanup();
        }
        bool reset(const RsuStdObjNames& a_names)
        {
            set_last_error(to_underlying(RSU_ERR::SUCCESS), "");
            return m_xcl.reset_config(RsuExchangeConfig(a_names.io, RSU_IO_SZ, a_names.cs, a_names.ss, RSU_IO_TOUT, RSU_IO_TOUT));
        }
        void Bind(RsuClientAPI* api)
        {
            if (api)
            {
                api->setInit(std::bind(&RsuClient::Init, this, std::placeholders::_1));
                api->setStep0(std::bind(&RsuClient::Step0, this));
                api->setStep1(std::bind(&RsuClient::Step1, this));
                api->setStepT(std::bind(&RsuClient::StepT, this, std::placeholders::_1));
                api->setStateSave(std::bind(&RsuClient::SaveState, this, std::placeholders::_1));
                api->setStateRestore(std::bind(&RsuClient::RestoreState, this, std::placeholders::_1));
                api->setParamsSave(std::bind(&RsuClient::SaveParams, this, std::placeholders::_1));
                api->setParamsRestore(std::bind(&RsuClient::RestoreParams, this, std::placeholders::_1));
                api->setErrorMessage(std::bind(&RsuClient::ErrorMessage, this, std::placeholders::_1, std::placeholders::_2));
                api->setLastError(std::bind(&RsuClient::LastError, this));
                api->setState(std::bind(&RsuClient::state, this));
            }
        }
        RsuClient(const RsuStdObjNames& a_names) :
            m_ObjNames{ a_names }, m_xcl{ RsuExchangeConfig(a_names.io, RSU_IO_SZ, a_names.cs, a_names.ss, RSU_IO_TOUT, RSU_IO_TOUT) }
        {

        }
        ~RsuClient()
        {

        }
    };

    class RsuClientProxy {
        RsuClient* m_cl;
    public:
        virtual int Init(int i) { return m_cl ? m_cl->InitCmd(i) : -1; }
        virtual int Step0() { return m_cl ? m_cl->Step0Cmd() : -1; }
        virtual int Step1() { return m_cl ? m_cl->Step1Cmd() : -1; }
        virtual int StepT(double dt) { return m_cl ? m_cl->StepTCmd(dt) : -1; }
#ifdef _WIN32
        virtual int ShowObject(const char* a_name) { return m_cl ? m_cl->ShowObjectCmd(a_name) : -1; }
#endif // _WIN32		
        virtual int Cleanup() { return m_cl ? m_cl->CleanupCmd() : -1; }
        virtual int SetDataCmd(int TypeData, const char* pData)
        {
            return m_cl ? m_cl->SetDataCmd(TypeData, pData) : -1;
        }
        void reset_client(RsuClient* a_cl) { m_cl = a_cl; }
        RsuClientProxy(RsuClient* a_cl) : m_cl(a_cl) {}
    };

    class RsuClientList {
        std::string m_svr_lib;
        std::string m_cfg_name;
        RsuExchangeClient m_ex;
        std::vector <std::unique_ptr<RsuClient>> m_clients;
    protected:
        virtual std::unique_ptr<RsuClient> create_client(const RsuSvrObjNames& a_ObjNames) = 0;
    public:
        bool state()
        {
            return m_ex.state();
        }

        std::string srv_lib()
        {
            return m_svr_lib;
        }

        std::string cfg_name()
        {
            return m_cfg_name;
        }

        int rsu_create_client(const char* a_ObjName, RsuClientAPI* api)
        {
            int ret = to_underlying(RSU_ERR::ERR);
            if (a_ObjName && api)
            {
                RsuSvrObjNames v_ObjNames(m_svr_lib, m_cfg_name, a_ObjName);
                auto cl = create_client(v_ObjNames);
                cl->Bind(api);
                if (m_ex.state())
                {
                    if (true) //cl->cfg())
                    {
                        RsuRequestCmd v_cmd(RSU_CMD::GET_OBJECT, &v_ObjNames, sizeof(v_ObjNames));
                        if (m_ex.send_cmd(&v_cmd))
                        {
                            if (v_cmd.cmd == to_underlying(RSU_ERR::SUCCESS))
                            {
                                if (v_cmd.size == sizeof(v_ObjNames))
                                {
                                    memcpy(&v_ObjNames, v_cmd.data, sizeof(v_ObjNames));
                                    if (cl->reset(v_ObjNames))
                                    {
                                        ret = to_underlying(RSU_ERR::SUCCESS);
                                        api->SetCreationRes(ret, m_ex.ErrorStr().c_str());
                                    }
                                    else
                                    {
                                        std::string err = "Client object exchange initialization error. "
                                            + cl->ErrorMessageStr();
                                        api->SetCreationRes(ret, err.c_str());
                                    }
                                }
                                else
                                    api->SetCreationRes(ret, "Invalid size of server data.");
                            }
                            else
                            {
                                std::string err = "";
                                if (!v_ObjNames.lib_name[0])
                                    err = "Server library not found.";
                                else if (!v_ObjNames.obj_name[0])
                                    err = "Object name isn't in server config.";
                                else if (!v_ObjNames.io_name[0] || !v_ObjNames.CS_name[0] || !v_ObjNames.SS_name[0])
                                    err = "Server object exchange initialization error.";
                                else
                                    err = "Unknown server side error.";
                                api->SetCreationRes(ret, err.c_str());
                            }
                        }
                        else
                            api->SetCreationRes(ret, "");
                    }
                    else
                        api->SetCreationRes(ret, "Object name isn't in client config.");
                }
                else
                {
                    std::string err = "RsuClientList error." + m_ex.ErrorStr();
                    api->SetCreationRes(ret, err.c_str());
                }
                cl->set_last_error(api->CreationRes(), api->CreationMsg());
                m_clients.push_back(std::move(cl));
            }
            else
            {
                if (api)
                    api->SetCreationRes(ret, "Object name is null.");
            }

            return ret;
        }

        RsuClientList(std::string a_svr_lib, std::string a_cfg_name, std::string a_svr_io, std::string a_CS, std::string a_SS, size_t a_svr_io_sz) :
            m_ex{ RsuExchangeConfig(a_svr_io, a_svr_io_sz, a_CS, a_SS, RSU_IO_TOUT, RSU_IO_TOUT) }, m_svr_lib{ a_svr_lib }, m_cfg_name{ a_cfg_name }
        {

        }
    };
};
