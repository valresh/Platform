//#include <WinSock2.h>
#include <chrono>
#include <cassert>
#include <exception>
#include "MnemoSession.h"
#include "LogFile.h"
#include "PipeClient.h"
#include "socketclient.h"

using namespace mnemownd::api;
using namespace std::chrono_literals;

int MnemoSession::s_Id = 100;

MnemoSession::MnemoSession(managed::INetworkHub & networkHub, const char* elementName, EDataTypes elementType, EDataTypes groupElementType)
{
    _id = s_Id++;
    _socketClient = std::make_shared<CSocketClient>(networkHub);
    _pipeClient = std::make_shared<CPipeClient>(networkHub);
    _isStopping = false;
    _isWatching = false;
    _elementType = elementType;
    _groupElementType = groupElementType;
    memset(&_queue, 0, sizeof(_queue));
    _queue.m_pPipe = _pipeClient.get();
    _queue.m_pSocket = _socketClient.get();
    if (!elementName)
    {
        return;
    }

    _elementName = elementName;

    //Load remote data before parsing
    // (Switch is stick here: it assumes nobody has to make a prediction about pipe or socket should be used. Rework is required)
    switch (_groupElementType)
    {
    case id_Value:
        _pipeClient->ActObj(_elementName.c_str());
        break;

    case id_Forsunka:
    case id_GasAnalyz:
        _pipeClient->FindObj(_groupElementType, _elementName.c_str(), _elementName.c_str());
        _pipeClient->End();
        break;

    case id_Library:
        break;

    default:
        if (_elementType != id_Unknown)
        {
            _socketClient->FindObj(_groupElementType, _elementName.c_str(), _elementName.c_str());
            _socketClient->Refresh();
        }

        break;
    }
}

MnemoSession::~MnemoSession()
{
    StopWatching();
    CLogFile::Log("MnemoSession %i: Destroyed", _id);
}

int MnemoSession::GetId() const
{
    return _id;
}

std::shared_ptr<CPipeClient> MnemoSession::GetPipeClient() const
{
    return _pipeClient;
}

std::shared_ptr<CSocketClient> MnemoSession::GetSocketClient() const
{
    return _socketClient;
}

SQueue& MnemoSession::GetQueue()
{
    return _queue;
}

bool MnemoSession::TryStartWatching(IWatchingCallbacks callbacks)
{
    if (_isWatching)
    {
        return false;
    }

    auto countPipeData = _pipeClient->Count();
    auto countSocketData = _socketClient->Count();
    if (countPipeData == 0 && countSocketData == 0)
    {
        CLogFile::Log("MnemoSession %i: Nothing to watch, no controls require network updating", _id);
        return false;
    }

    CLogFile::Log("MnemoSession %i: Watch for %i pipe item(s) and %i socket item(s) started", _id, countPipeData, countSocketData);
    if (!_elementName.empty())
    {
        AddRef(_elementName.c_str(), _elementType);
    }

    _isWatching = true;
    _watchingThread = std::thread([=]
    {
        _pipeClient->End();
        while (!_isStopping)
        {
            if (_socketClient->Count())
            {
                // Download CShXXXX data from ModelEx
                DownloadSocketData(callbacks);
            }

            if (!_isStopping && _pipeClient->Count())
            {
                // Download CShXXXX data from ModelEx
                DownloadPipeData(callbacks);
            }

            if (!_isStopping)
            {
                callbacks.InvalidateVisual();
            }

            // Sleep/wait for _isStopping
            if (!_isStopping)
            {
                std::unique_lock<std::mutex> lock(_mutex);
                auto isStopping = _waitVariable.wait_for(lock, 1s, [&]
                {
                    return (bool)_isStopping;
                });
                if (isStopping)
                {
                    break;
                }
            }
        }

        if (!_elementName.empty())
        {
            Release(_elementName.c_str(), _elementType);
        }

        _isWatching = false;
    });
    return true;
}

void MnemoSession::DownloadSocketData(const IWatchingCallbacks& callbacks)
{
    _socketClient->Refresh();
}

void MnemoSession::DownloadPipeData(const IWatchingCallbacks& callbacks)
{
    _pipeClient->Refresh();
}

void MnemoSession::StopWatching()
{
    _isStopping = true;
    _waitVariable.notify_one();
    if (_watchingThread.joinable())
    {
        _watchingThread.join();
    }

    assert(_isWatching == false);
    _isStopping = false;
}

void MnemoSession::AddRef(const char* elementName, EDataTypes elementType)
{
    if (!elementName)
        throw std::invalid_argument("elementName");

    _pipeClient->ChangeBusy(elementType, elementName, true);
}

void MnemoSession::Release(const char* elementName, EDataTypes elementType)
{
    if (!elementName)
        throw std::invalid_argument("elementName");

    _pipeClient->ChangeBusy(elementType, elementName, false);
}
