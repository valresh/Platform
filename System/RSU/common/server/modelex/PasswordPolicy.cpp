#include <fstream>
#include "PasswordPolicy.h"
#include "base64.hpp"
#include "aes-c/include/aes.h"
#include "LogFile.h"
#include "cutf.h"

const int Aes256KeyLength = 32;
const int AesBlockSize = 16;
const char8_t CryptSign8[] = u8"𓀧";
const uint32_t CryptSign32 = U'𓀧';

// Initial vector
const unsigned char IV_CBC[16] =
{ 
    0x5d, 0xaf, 0xba, 0x42, 0x9d, 0x9e,
    0xb4, 0x35, 0x54, 0x72, 0xaa, 0x80,
    0x2c, 0x9f, 0xac, 0x23 
};

PasswordPolicy::PasswordPolicy(const std::filesystem::path& cryptoKeyPath)
{
    _isValidKey = false;
    memset(_key, 0, sizeof(_key));
    if (cryptoKeyPath.empty())
    {
        return;
    }

    if (!std::filesystem::exists(cryptoKeyPath))
    {
        throw std::runtime_error("Ключ шифрования не найден");
    }

    std::ifstream file(cryptoKeyPath);
    if (!file.is_open())
    {
        throw std::runtime_error("Не могу открыть файл ключа шифрования");
    }

    std::string line;
    file >> line;
    file.close();
    if (line.empty())
    {
        return;
    }

    std::string key;
    try
    {
        key = base64::from_base64(line);
        if (key.length() != Aes256KeyLength)
        {
            throw std::runtime_error("Invalid key length");
        }

    }
    catch (std::runtime_error& e)
    {
        std::string errorMessage = std::string("Crypto key ") + cryptoKeyPath.string() + " is invalid or corrupted (" + e.what() + ")";
        throw std::runtime_error(errorMessage);
    }

    memcpy(_key, key.c_str(), Aes256KeyLength);
    _isValidKey = true;
}

std::string PasswordPolicy::EncryptPassword(const std::string& password)
{
    if (!_isValidKey || password.empty())
    {
        return std::string();
    }

    auto alignGap = password.length() <= AesBlockSize ?
        AesBlockSize - password.length() :
        (((password.length()/ AesBlockSize) + 1)* AesBlockSize) - password.length();
    std::unique_ptr<unsigned char[]> alignedPassword;
    if (alignGap != 0)
    {
        auto length = password.length() + alignGap;
        alignedPassword = std::make_unique<unsigned char[]>(length);
        memcpy(alignedPassword.get(), password.c_str(), password.length());
    }

    AesContext aesContext;
    aes_init(&aesContext, KEY_TYPE_AES256, _key);
    databuffer buffer(password.length() + alignGap);
    aes_cbc_encrypt(
        &aesContext,
        password.length() + alignGap,
        (unsigned char*)buffer.data(),
        alignedPassword ? alignedPassword.get() : (unsigned char*)password.c_str(),
        IV_CBC);
    auto base64string = base64::to_base64(std::string_view(buffer.data(), buffer.size()));
    return (char*)CryptSign8 + base64string;
}

std::string PasswordPolicy::DecryptPassword(const std::string& encodedPassword)
{
    auto signLength = sizeof(CryptSign8) - 1;
    if (!_isValidKey || encodedPassword.empty())
    {
        return std::string();
    }

    if (strncmp(encodedPassword.data(), (const char*)CryptSign8, signLength) != 0)
    {
        throw std::runtime_error("Невозможно расшифровать, неизвестный формат");
    }

    auto unwrapedEncodedPassword = base64::from_base64(
        std::string_view(encodedPassword.data() + signLength, encodedPassword.size() - signLength));
    AesContext aesContext;
    aes_init(&aesContext, KEY_TYPE_AES256, _key);
    databuffer buffer(unwrapedEncodedPassword.length());
    aes_cbc_decrypt(
        &aesContext, 
        unwrapedEncodedPassword.size(),
        (unsigned char*)buffer.data(),
        (unsigned char*)unwrapedEncodedPassword.data(),
        IV_CBC);
    return buffer.data();
}

bool PasswordPolicy::IsEncrypted(const std::string& password) const
{
    if (password.empty() || password.length() < 4)
    {
        return false;
    }

    uint8_t* it =(uint8_t*)password.c_str();
    auto symbol = cutf_next(&it, nullptr);
    return symbol == CryptSign32;
}
