#pragma once
#include <string>
#include <filesystem>
#include "databuffer.h"

class PasswordPolicy final
{
private:
    bool _isValidKey;
    unsigned char _key[32];

public:
    // @throws std::runtime_error
    PasswordPolicy(const std::filesystem::path& cryptoKeyPath);

    bool IsEncrypted(const std::string& password) const;

    // @throws std::runtime_error
    std::string EncryptPassword(const std::string& password);

    // @throws std::runtime_error
    std::string DecryptPassword(const std::string& encodedPassword);
};
