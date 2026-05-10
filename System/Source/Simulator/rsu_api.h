#ifndef RSU_API_H
#define RSU_API_H
#pragma once
#include <functional>
#include <string>
#include <string.h>

using RsuFnIntParamVoid = std::function<int()>;
using RsuFnIntParamInt = std::function<int(int)>;
using RsuFnIntParamPchar = std::function<int(const char*)>;
using RsuFnIntParamDbl = std::function<int(const double)>;
using RsuFnIntParamPvoid = std::function<int(const void*)>;
using RsuFnBoolParamPcharPszt = std::function<bool (char*, size_t*)>;
using RsuFnBoolParamVoid = std::function<bool()>;

#define RSU_SVR_IO "rsu_svr_io_semaphore"
#define RSU_SVR_SS "rsu_svr_ss_semaphore"
#define RSU_SVR_CS "rsu_svr_cs_semaphore"
#define RSU_SVR_IO_SZ 4096
#define RSU_API_MSG_SZ 256

namespace rsu_cp {

class RsuAPI {
    int m_CreationRes = 0;
    char m_CreationMsg[RSU_API_MSG_SZ] = { 0 };
    RsuFnBoolParamVoid m_State = NULL;
    RsuFnIntParamVoid m_LastError = NULL;
    RsuFnBoolParamPcharPszt m_ErrorMessage = NULL;
public:
    const char* CreationMsg()
    {
        return &m_CreationMsg[0];
    }
    int CreationRes()
    {
        return m_CreationRes;
    }
    void SetCreationRes(int res, const char *msg)
    {
        m_CreationRes = res;
        strncpy(m_CreationMsg, msg, RSU_API_MSG_SZ);
    }
    RsuFnBoolParamPcharPszt ErrorMessage()
    {
        return m_ErrorMessage;
    }
    RsuFnIntParamVoid LastError()
    {
        return m_LastError;
    }
    RsuFnBoolParamVoid State() {
        return m_State;
    }

    void setErrorMessage(const RsuFnBoolParamPcharPszt& f)
    {
        m_ErrorMessage = f;
    }
    void setErrorMessage(RsuFnBoolParamPcharPszt&& f)
    {
        m_ErrorMessage = std::move(f);
    }

    void setLastError(const RsuFnIntParamVoid& f)
    {
        m_LastError = f;
    }
    void setLastError(RsuFnIntParamVoid&& f)
    {
        m_LastError = std::move(f);
    }

    void setState(const RsuFnBoolParamVoid& f)
    {
        m_State = f;
    }
};

class RsuServerAPI : public RsuAPI
{
    RsuFnIntParamVoid m_Initialize = NULL;
    RsuFnIntParamVoid m_AfterInit = NULL;
    RsuFnIntParamVoid m_AfterRestored = NULL;
    RsuFnIntParamInt m_CalcStep = NULL;
    RsuFnIntParamPchar m_StateSave = NULL;
    RsuFnIntParamPchar m_StateRestore = NULL;
    RsuFnIntParamPchar m_ParamsSave = NULL;
    RsuFnIntParamPchar m_ParamsRestore = NULL;
public:
    RsuFnIntParamVoid Initialize() {
        return m_Initialize;
    }
    RsuFnIntParamVoid AfterInit() {
        return m_AfterInit;
    }
    RsuFnIntParamVoid AfterRestored() {
        return m_AfterRestored;
    }
    RsuFnIntParamInt CalcStep() {
        return m_CalcStep;
    }
    RsuFnIntParamPchar StateSave() {
        return m_StateSave;
    }
    RsuFnIntParamPchar StateRestore() {
        return m_StateRestore;
    }
    RsuFnIntParamPchar ParamsSave() {
        return m_ParamsSave;
    }
    RsuFnIntParamPchar ParamsRestore() {
        return m_ParamsRestore;
    }

    void setInitialize(const RsuFnIntParamVoid& f)
    {
        m_Initialize = f;
    }
    void setInitialize(RsuFnIntParamVoid&& f)
    {
        m_Initialize = std::move(f);
    }

    void setAfterInit(const RsuFnIntParamVoid& f)
    {
        m_AfterInit = f;
    }
    void setAfterInit(RsuFnIntParamVoid&& f)
    {
        m_AfterInit = std::move(f);
    }

    void setAfterRestored(const RsuFnIntParamVoid& f)
    {
        m_AfterRestored = f;
    }
    void setAfterRestored(RsuFnIntParamVoid&& f)
    {
        m_AfterRestored = std::move(f);
    }

    void setCalcStep(const RsuFnIntParamInt& f)
    {
        m_CalcStep = f;
    }
    void setCalcStep(RsuFnIntParamInt&& f)
    {
        m_CalcStep = std::move(f);
    }

    void setStateSave(const RsuFnIntParamPchar& f)
    {
        m_StateSave = f;
    }
    void setStateSave(RsuFnIntParamPchar&& f)
    {
        m_StateSave = std::move(f);
    }

    void setStateRestore(const RsuFnIntParamPchar& f)
    {
        m_StateRestore = f;
    }
    void setStateRestore(RsuFnIntParamPchar&& f)
    {
        m_StateRestore = std::move(f);
    }

    void setParamsSave(const RsuFnIntParamPchar& f)
    {
        m_ParamsSave = f;
    }
    void setParamsSave(RsuFnIntParamPchar&& f)
    {
        m_ParamsSave = std::move(f);
    }

    void setParamsRestore(const RsuFnIntParamPchar& f)
    {
        m_ParamsRestore = f;
    }
    void setParamsRestore(RsuFnIntParamPchar&& f)
    {
        m_ParamsRestore = std::move(f);
    }
};

typedef int (*t_rsu_create_server)(const char* obj_name, RsuServerAPI* api);

class RsuClientAPI : public RsuAPI
{
    RsuFnIntParamInt m_Init = NULL;
    RsuFnIntParamVoid m_Step0 = NULL;
    RsuFnIntParamVoid m_Step1 = NULL;
    //RsuFnIntParamVoid m_PreStepT = NULL;
    RsuFnIntParamDbl m_StepT = NULL;
    RsuFnIntParamPchar m_StateSave = NULL;
    RsuFnIntParamPchar m_StateRestore = NULL;
    RsuFnIntParamPchar m_ParamsSave = NULL;
    RsuFnIntParamPchar m_ParamsRestore = NULL;
public:
    RsuFnIntParamInt Init() {
        return m_Init;
    }
    RsuFnIntParamVoid Step0() {
        return m_Step0;
    }
    RsuFnIntParamVoid Step1() {
        return m_Step1;
    }
    ~RsuClientAPI()
    {

    }
    /*
        RsuFnIntParamVoid PreStepT()
        {
            return m_PreStepT;
        }
        */
    RsuFnIntParamDbl StepT()
    {
        return m_StepT;
    }
    RsuFnIntParamPchar StateSave() {
        return m_StateSave;
    }
    RsuFnIntParamPchar StateRestore() {
        return m_StateRestore;
    }
    RsuFnIntParamPchar ParamsSave() {
        return m_ParamsSave;
    }
    RsuFnIntParamPchar ParamsRestore() {
        return m_ParamsRestore;
    }
    void setInit(const RsuFnIntParamInt& f)
    {
        m_Init = f;
    }
    void setInit(RsuFnIntParamInt&& f)
    {
        m_Init = std::move(f);
    }

    void setStep0(const RsuFnIntParamVoid& f)
    {
        m_Step0 = f;
    }
    void setStep0(RsuFnIntParamVoid&& f)
    {
        m_Step0 = std::move(f);
    }

    void setStep1(const RsuFnIntParamVoid& f)
    {
        m_Step1 = f;
    }
    void setStep1(RsuFnIntParamVoid&& f)
    {
        m_Step1 = std::move(f);
    }
    /*
        void setPreStepT(const RsuFnIntParamVoid& f)
        {
            m_PreStepT = f;
        }
        void setPreStepT(RsuFnIntParamVoid&& f)
        {
            m_PreStepT = std::move(f);
        }
        */
    void setStepT(const RsuFnIntParamDbl& f)
    {
        m_StepT = f;
    }
    void setStepT(RsuFnIntParamDbl&& f)
    {
        m_StepT = std::move(f);
    }
    void setStateSave(const RsuFnIntParamPchar& f)
    {
        m_StateSave = f;
    }
    void setStateSave(RsuFnIntParamPchar&& f)
    {
        m_StateSave = std::move(f);
    }
    void setStateRestore(const RsuFnIntParamPchar& f)
    {
        m_StateRestore = f;
    }
    void setStateRestore(RsuFnIntParamPchar&& f)
    {
        m_StateRestore = std::move(f);
    }
    void setParamsSave(const RsuFnIntParamPchar& f)
    {
        m_ParamsSave = f;
    }
    void setParamsSave(RsuFnIntParamPchar&& f)
    {
        m_ParamsSave = std::move(f);
    }
    void setParamsRestore(const RsuFnIntParamPchar& f)
    {
        m_ParamsRestore = f;
    }
    void setParamsRestore(RsuFnIntParamPchar&& f)
    {
        m_ParamsRestore = std::move(f);
    }
    void clear()
    {
        m_Init = nullptr;
        m_Step0 = nullptr;
        m_Step1 = nullptr;
        m_StepT = nullptr;
        m_StateSave = nullptr;
        m_StateRestore = nullptr;
        m_ParamsSave = nullptr;
        m_ParamsRestore = nullptr;
    }
};

inline std::string obj_prefix(std::string lib_name, std::string lib_cfg, std::string obj_name )
{
    return lib_name + "_" + lib_cfg + "_" + obj_name;
}

inline std::string obj_sem_ss(std::string lib_name, std::string lib_cfg, std::string obj_name)
{
    return obj_prefix(lib_name, lib_cfg, obj_name) + "_ss";
}

inline std::string obj_sem_cs(std::string lib_name, std::string lib_cfg, std::string obj_name)
{
    return obj_prefix(lib_name, lib_cfg, obj_name) + "_cs";
}

inline std::string obj_mio(std::string lib_name, std::string lib_cfg, std::string obj_name)
{
    return obj_prefix(lib_name, lib_cfg, obj_name) + "_io";
}

typedef int (*t_rsu_create_client)(const char* DllName, const char* DllCfg, const char* ObjName, const char* DllPath, RsuClientAPI* api, bool& o_ValidLib);
typedef void (*t_rsu_clients_cleanup)();
typedef void (*t_rsu_show_object)(const char* DllName, const char* DllCfg, const char* ObjName);
typedef const char* (*t_rsu_project_path)();

inline int RsuAPILastError(RsuAPI* api)
{
    int ret = -1;
    if (api)
    {
        auto f = api->LastError();
        if (f)
            ret = f();
    }
    return ret;
}

inline std::string RsuAPIErrorMsg(RsuAPI* api)
{
    char* buf = NULL;
    size_t sz = 0;
    std::string ret = "";
    if (api)
    {
        auto f = api->ErrorMessage();
        if (f)
        {
            if (!f(buf, &sz))
            {
                buf = (char*) malloc(sz);
                f(buf, &sz);
                if (buf)
                {
                    ret = buf;
                    free(buf);
                }
            }
        }
    }
    return ret;
}

inline std::string RsuAPICreationMsg(RsuAPI* api)
{
    std::string ret = api ? api->CreationMsg():"";
    return ret;
}

}

inline std::string char2str(const char* str)
{
    return str ? std::string(str) : std::string("");
}

//#define RSU_CLIENT_API_FN "rsu_create_client"
//#define RSU_SERVER_API_FN "rsu_create_server_object"
#endif // RSU_API_H
