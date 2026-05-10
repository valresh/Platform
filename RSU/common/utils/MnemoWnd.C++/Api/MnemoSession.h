#pragma once
#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include "managed.h"
#include "BaseType.h"
#include "squeue.h"

#undef CLIENTDLL_API
#if defined(_WIN32)
    #ifdef QUEUEAPM_EXPORTS
        #define CLIENTDLL_API __declspec(dllexport)
    #else
        #define CLIENTDLL_API __declspec(dllimport)
    #endif
#else
    #ifdef UTILS_EXPORTS
        #define CLIENTDLL_API __attribute((visibility("default")))
    #else
        #define CLIENTDLL_API
    #endif
#endif

class CPipeClient;
class CSocketClient;

namespace mnemownd
{
    namespace api
    {
        struct IWatchingCallbacks
        {
            void (*InvalidateVisual)();
        };

        class CLIENTDLL_API MnemoSession
        {
        private:
            static int s_Id;
            int _id;
            std::shared_ptr<CPipeClient> _pipeClient;
            std::shared_ptr<CSocketClient> _socketClient;
            std::thread _watchingThread;
            std::atomic<bool> _isWatching;
            std::atomic<bool> _isStopping;
            std::condition_variable _waitVariable;
            std::mutex _mutex;
            std::string _elementName;
            EDataTypes _elementType;
            EDataTypes _groupElementType;
            SQueue _queue;

        public:
            //
            // @param elementName  Can be assigned if mnemo dialog for the certain element is being opened
            // @param elementType  Can be assigned if mnemo dialog for the certain element is being opened
            // @throws std::invalid_argument    
            //
            MnemoSession(
                managed::INetworkHub& networkHub,
                const char* elementName,
                EDataTypes elementType,
                EDataTypes groupElementType);

            ~MnemoSession();

            int GetId() const;

            std::shared_ptr<CPipeClient> GetPipeClient() const;

            std::shared_ptr<CSocketClient> GetSocketClient() const;

            SQueue& GetQueue();

            bool TryStartWatching(IWatchingCallbacks callbacks);

        private:
            void StopWatching();
            void AddRef(const char* elementName, EDataTypes elementType);
            void Release(const char* elementName, EDataTypes elementType);
            void DownloadSocketData(const IWatchingCallbacks& callbacks);
            void DownloadPipeData(const IWatchingCallbacks& callbacks);
        };
    };
};
