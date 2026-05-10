#include <limits.h>
#include "MqttConfigurator.h"
#include "PasswordPolicy.h"
#include "Prop.h"
#include "ConsoleHelper.h"
#include "LogFile.h"

MqttSettings MqttConfigurator::ReadSettings()
{
    char mqttServer[256] = "";
    char mqttUsername[100] = {};
    int mqttPort = 0;
    char mqttPassword[100] = {};
    char mqttPasswordKey[256] = {};
    bool mqttReqAuth;
    if (PROPS.ReadProp())
    {
        // Example:
        //1;SB#Сервер MQTT;ED#127.0.0.1;ED#
        //1;SB#Порт MQTT;ED#1883;ED#
        //1;SB#Пользователь MQTT;ED#mqtt;ED#
        //1;SB#Пароль MQTT;ED#password;ED#
        //1;SB#MQTT Авторизация;ED#1;ED#
        PROPS.Get(PROPKEY_MQTT_SERVER, mqttServer);
        PROPS.Get(PROPKEY_MQTT_PORT, mqttPort);
        PROPS.Get(PROPKEY_MQTT_REQAUTH, mqttReqAuth);
        PROPS.Get(PROPKEY_MQTT_USER, mqttUsername);
        PROPS.Get(PROPKEY_MQTT_PASSWORD, mqttPassword);
        PROPS.Get(PROPKEY_MQTT_PASSWORDKEY, mqttPasswordKey);
        std::string password = mqttPassword;
        if (mqttReqAuth && *mqttPasswordKey)
        {
            try
            {
                PasswordPolicy passwordPolicy(mqttPasswordKey);
                if (passwordPolicy.IsEncrypted(password))
                {
                    password = passwordPolicy.DecryptPassword(password);
                }
            }
            catch (std::runtime_error& e)
            {
                std::string errorMessage = std::string("Сбой восстановления пароля MQTT (") + e.what() + ")";
                CLogFile::LogEx(LOGLEVEL_URGENT, errorMessage.c_str());
            }
        }

        std::string server = mqttServer;
        if (server.empty())
        {
            server = DefaultMqttServer;
        }

        if (mqttPort <= 0)
        {
            mqttPort = DefaultMqttPort;
        }

        return MqttSettings(server, mqttPort, mqttUsername, password, mqttPasswordKey, mqttReqAuth);
    }
    else
    {
        return MqttSettings(DefaultMqttServer, DefaultMqttPort);
    }
}

static void clearstdin()
{
    std::cin.clear();
    std::cin.ignore(INT_MAX, '\n');
}

MqttSettings MqttConfigurator::Configure(const MqttSettings& mqttSettings)
{
#ifdef _WIN32
    auto console = console::AllocConsole();
#endif
    std::string username;
    std::string password;
    std::string cryptoKeyPath;
    std::cout << "-- MQTT security ---" << std::endl << std::endl;
    while (true)
    {
        std::cout << "Пользователь: ";
        std::getline(std::cin, username);
        console::SetStdinEcho(false);
        std::cout << "Пароль: ";
        std::getline(std::cin, password);
        console::SetStdinEcho(true);
        std::cout << std::endl;
        std::string response;
        if (username.empty() && password.empty())
        {
            std::cout << "Предупреждение: Пользователь/Пароль не указаны. Продолжать? [y/n]";
            std::cin >> response;
            clearstdin();
            if (response != "y" && response != "Y")
            {
                continue;
            }
        }

        if (!username.empty() && !password.empty())
        {
            std::cout << std::endl << "Путь ключа шифрования пароля: ";
            std::getline(std::cin, cryptoKeyPath);
        }

        if (!password.empty() && cryptoKeyPath.empty())
        {
            std::cout << "Предупреждение: Ключ шифрования пароля не указан. Продолжить без шифрования пароля? [y/n]";
            std::cin >> response;
            clearstdin();
            if (response != "y" && response != "Y")
            {
                continue;
            }
        }

        break;
    }

    try
    {
        if (!password.empty() && !cryptoKeyPath.empty())
        {
            PasswordPolicy passwordPolicy(cryptoKeyPath);
            password = passwordPolicy.EncryptPassword(password);
        }

        PROPS.StoreSystemProperties(
        {
            {PROPKEY_MQTT_USER, username},
            {PROPKEY_MQTT_PASSWORD, password},
            {PROPKEY_MQTT_REQAUTH, !username.empty() ? "1" : "0"},
            {PROPKEY_MQTT_PASSWORDKEY, cryptoKeyPath}
        });
    }
    catch (std::runtime_error& e)
    {
        std::string errorMessage = std::string("Ошибка: Не могу сохранить (") + e.what() + ")";
        std::cout << errorMessage << std::endl;
#ifdef _WIN32
        std::this_thread::sleep_for(std::chrono::seconds(3));
        console::CloseConsole(console);
#endif
        throw std::runtime_error(e);
    }

    std::cout << "Пользователь/Пароль и путь к ключу шифрования сохранены в Properties.csv" << std::endl;
#ifdef _WIN32
    console::CloseConsole(console);
#endif
    return MqttSettings(
        mqttSettings.Host(),
        mqttSettings.Port(),
        username,
        password,
        cryptoKeyPath,
        !username.empty());
}