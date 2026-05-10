#include <OnDiskAllocator.h>
#include <rsus/rsuPaths.h>
#include <rsuErr.h>
#include <system_error>
#include <fstream>
#include <mio.hpp>
#include <crosspath.h>

DWORD CalulateFileSize(DWORD requestedSize)
{
    return ((requestedSize / mio::page_size()) + 1) * mio::page_size();
}

bool MapFile(mio::mmap_sink*& mapper, const fs::path& filePath, DWORD requestedSize, bool createNew, bool& isNew)
{
    std::error_code error;
    mapper = new mio::mmap_sink();
    auto fileSize = requestedSize;
    bool isNewFile = false;
    if (fs::exists(filePath))
    {
        if (createNew)
        {
            auto realFileSize = fs::file_size(filePath);
            if (realFileSize != requestedSize)
            {
                fileSize = CalulateFileSize(requestedSize);
            }
        }
        else
        {
            fileSize = fs::file_size(filePath);
        }
    }
    else
    {
        if (createNew)
        {
            fileSize = CalulateFileSize(requestedSize);
        }
        else
        {
            delete mapper;
            mapper = nullptr;
            return false;
        }
    }

    mapper->map(filePath.c_str(), 0, fileSize, error, createNew);
    if (error)
    {
        delete mapper;
        mapper = nullptr;
        return false;
    }

    isNew = createNew;
    return true;
}

BYTE* OnDiskAllocator(mio::mmap_sink*& mapper, bool& isNew, DWORD requestedSeze, LPCSTR fileName, LPCSTR fileExtension, bool forceCreate)
{
    isNew = false;
    auto filePath = fs::path(fileName);
    if (!fs::exists(filePath))
    {
        auto fullFileName = std::string(fileName) + "." + std::string(fileExtension);
        auto folderPath = rsuGetMemoryPath();
        filePath = (fs::path(folderPath) / fullFileName).generic_string();
    }

    if (!MapFile(mapper, filePath, requestedSeze, forceCreate, isNew))
    {
        return nullptr;
    }

    isNew = forceCreate;
    return (BYTE*)mapper->data();
}
