#include <StateSerLocal.h>
#include <rsuErr.h>
#include <thread>
#include <crossplatform.h>
#include <crossstring.h>

KStateSerLocal::KStateSerLocal(LPCSTR pszFile, bool bSave)
    : m_dwFileSize(0)
    , m_dwCurr(0)
    , m_dwLastError(0)
    , m_BytesCanRead(0)
    , m_bConvertToUtf8(false)
{
    bool res = bSave ? Create(pszFile) : Open(pszFile);
    ASS(res);
    m_bRead = !bSave;
}

KStateSerLocal::KStateSerLocal(std::fstream& hFile, bool bSave)
    : m_dwFileSize(0)
    , m_dwCurr(0)
    , m_dwLastError(0)
    , m_BytesCanRead(0)
    , m_bConvertToUtf8(false)
{
    m_bRead = !bSave;
    hFile.seekp(0, std::ios::end);
    m_dwFileSize = hFile.tellp();
    hFile.seekp(0, std::ios::beg);
    m_hFile = std::move(hFile);
    ASS(m_hFile);
}

KStateSerLocal::~KStateSerLocal()
{
    Close();
}

bool KStateSerLocal::Create(LPCSTR pszFile)
{
    m_hFile.open(pszFile, std::ios::out | std::ios::binary | std::ios::trunc);
    return m_hFile.is_open();
}

bool KStateSerLocal::Open(LPCSTR pszFile)
{
    m_hFile.open(pszFile, std::ios::in | std::ios::binary);
    if (!m_hFile.is_open())
    {
        return false;
    }

    m_hFile.seekg(0, std::ios::end);
    m_dwFileSize = m_hFile.tellg();
    m_hFile.seekg(0, std::ios::beg);
    return true;
}

bool KStateSerLocal::Close()
{
    if (!m_hFile.is_open())
    {
        return false;
    }

    int retries = 10;
    while (m_hFile.is_open() && retries > 0) {
        m_hFile.close();
        --retries;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return true;
}

void KStateSerLocal::OnWrote(DWORD size)
{
    m_dwCurr += size;
    m_dwFileSize += size;
}

void KStateSerLocal::OnRead(DWORD size)
{
    m_dwCurr += size;
    DWORD pos = m_hFile.tellg();
    if (m_dwCurr != pos)
    {
        ASS(m_dwCurr == pos);
    }
}

IFlexStateSer* KStateSerLocal::GetInterface()
{
    return this;
}

DWORD KStateSerLocal::Shift(LONG lDistanceToMove)
{
    m_hFile.seekg(lDistanceToMove, std::ios::cur);
    DWORD r = m_hFile.tellg();
    m_dwCurr += lDistanceToMove;
    ASS(m_dwCurr == r);
    return lDistanceToMove;
}

bool KStateSerLocal::IsData()
{
    ASSD(!"реализовать");
    return false;
}

DWORD KStateSerLocal::BytesCanRead()
{
    return m_BytesCanRead;
}

void KStateSerLocal::SetConvertToUtf8(bool bConvert)
{
    m_bConvertToUtf8 = bConvert;
}

void KStateSerLocal::SetReadyBytes(DWORD count)
{
    m_BytesCanRead = count;
}

DWORD KStateSerLocal::SimpleRead(LPVOID Addr, DWORD dwSize)
{
    if (!m_hFile.read((char*)Addr, dwSize))
    {
        return 0;
    }

    DWORD Lr = m_hFile.gcount();
    ASSD(Lr == dwSize);
    OnRead(dwSize);
    return dwSize;
}

DWORD KStateSerLocal::ReadLenStr(LPSTR str, DWORD dwSize)
{
    DWORD len = 0;
    if (!SimpleRead(&len, sizeof(len)))
    {
        return 0;
    }

    ASS(len < dwSize);
    if (len >= (dwSize - 1))
    {
        return 0;
    }

    memset(str, 0, dwSize);
    char tmpRead[256 * 4] = {};
    if (!SimpleRead(tmpRead, len))
    {
        return 0;
    }

    if (m_bConvertToUtf8)
    {
        cross::string::convert_to_utf8(1251, tmpRead, str, dwSize);
    }
    else
    {
        strcpy(str, tmpRead);
    }

    return len + sizeof(len);
}

DWORD KStateSerLocal::SimpleWrite(LPCVOID Addr, DWORD dwSize)
{
    DWORD Lw = 0;
    if (!m_hFile.write((char*)Addr, dwSize))
    {
        return 0;
    }

    ASSD(m_hFile);
    OnWrote(dwSize);
    return dwSize;
}

DWORD KStateSerLocal::WriteStr(LPCSTR str)
{
    char tmpBuffer[256 * 4] = {};
    if (m_bConvertToUtf8)
    {
        cross::string::convert_to_utf8(1251, str, tmpBuffer, sizeof(tmpBuffer));
    }
    else
    {
        strcpy(tmpBuffer, str);
    }

    DWORD L = (DWORD)std::strlen(tmpBuffer);
    if (!SimpleWrite(&L, sizeof(L)))
    {
        return 0;
    }

    if (L && !SimpleWrite(str, L))
    {
        return 0;
    }

    return L + sizeof(L);
}

DWORD KStateSerLocal::GetPosition()
{
    return m_dwCurr;
}

DWORD KStateSerLocal::WriteAtPosition(LONG shiftFromBegin, LPCVOID Addr, DWORD objSize)
{
    DWORD& curPos = m_dwCurr;
    m_hFile.seekp(shiftFromBegin);
    DWORD r = m_hFile.tellp();
    curPos = shiftFromBegin;
    DWORD l = SimpleWrite(Addr, objSize);
    m_hFile.seekp(0, std::ios::end);
    r = m_hFile.tellp();
    m_dwFileSize = m_dwCurr = r;
    return l;
}

