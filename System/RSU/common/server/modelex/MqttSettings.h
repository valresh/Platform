#pragma once
#include <string>

constexpr char* DefaultMqttServer = "127.0.0.1";
constexpr int DefaultMqttPort = 1883;

struct MqttSettings final
{
private:
    std::string _host;
    int _port;
    bool _isReqAuthentication;
    std::string _user;
    std::string _password;
    std::string _cryptoKeyPath;

public:
    MqttSettings()
    {
        _isReqAuthentication = false;
        EnsureHostAndPort();
    }

    MqttSettings(const std::string& host, int port)
    {
        _host = host;
        _port = port;
        _isReqAuthentication = false;
        EnsureHostAndPort();
    }
    
    MqttSettings(
        const std::string& host,
        int port,
        const std::string& user,
        const std::string& password,
        const std::string& cryptoKeyPath,
        bool isReqAuthentication)
    {
        _host = host;
        _port = port;
        _user = user;
        _password = password;
        _cryptoKeyPath = cryptoKeyPath;
        _isReqAuthentication = isReqAuthentication;
        EnsureHostAndPort();
    }

    const std::string& Host() const
    {
        return _host;
    }

    int Port() const
    {
        return _port;
    }

    bool IsReqAuthentication() const
    {
        return _isReqAuthentication;
    }

    const std::string& User() const
    {
        return _user;
    }

    const std::string& Password() const
    {
        return _password;
    }

    const std::string& CryptoKeyPath() const
    {
        return _cryptoKeyPath;
    }

    bool IsMisconfigured() const
    {
        if (!IsReqAuthentication())
        {
            return false;
        }

        if (User().empty())
        {
            return true;
        }

        if (!CryptoKeyPath().empty() && Password().empty())
        {
            return true;
        }

        return false;
    }

private:
    void EnsureHostAndPort()
    {
        if (_host.empty())
        {
            _host = DefaultMqttServer;
        }

        if (_port <= 0)
        {
            _port = DefaultMqttPort;
        }
    }
};