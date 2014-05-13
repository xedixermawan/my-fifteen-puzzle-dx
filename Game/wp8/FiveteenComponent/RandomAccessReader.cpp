//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#include "pch.h"
#include "RandomAccessReader.h"

//
// For package
//
using namespace Windows::ApplicationModel;

RandomAccessReader::RandomAccessReader(
    _In_ Platform::String^ fileName
    )
{
    Platform::String^ fullPath = Package::Current->InstalledLocation->Path;
    fullPath += "\\data\\";
    fullPath += fileName;

    HANDLE fileHandle = ::CreateFile2(
        fullPath->Data(),
        GENERIC_READ,
        FILE_SHARE_READ,
        OPEN_EXISTING,
        nullptr
        );

    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        throw ref new Platform::FailureException();
    }

    m_file = fileHandle;
}

Platform::Array<byte>^ RandomAccessReader::Read(
    _In_ size_t bytesToRead
    )
{
    Platform::Array<byte>^ fileData = ref new Platform::Array<byte>(static_cast<unsigned int>(bytesToRead));

    if (!ReadFile(
        m_file,
        fileData->Data,
        fileData->Length,
        nullptr,
        nullptr
        ))
    {
        throw ref new Platform::FailureException();
    }

    return fileData;
}

uint64 RandomAccessReader::GetFileSize()
{
    FILE_STANDARD_INFO fileStandardInfo = { 0 };
    BOOL result = ::GetFileInformationByHandleEx(
        m_file,
        FileStandardInfo,
        &fileStandardInfo,
        sizeof(fileStandardInfo)
        );

    if ((result == 0) || (fileStandardInfo.AllocationSize.QuadPart < 0))
    {
        throw ref new Platform::FailureException();
    }

    return fileStandardInfo.AllocationSize.QuadPart;
}

uint64 RandomAccessReader::GetPosition()
{
    LARGE_INTEGER position = { 0 };
    LARGE_INTEGER zero = { 0 };
    BOOL result = ::SetFilePointerEx(m_file, zero, &position, FILE_CURRENT);
    if ((result == 0) || (position.QuadPart < 0))
    {
        throw ref new Platform::FailureException();
    }

    return position.QuadPart;
}

void RandomAccessReader::SeekRelative(_In_ int64 offset)
{
    LARGE_INTEGER position;
    position.QuadPart = offset;
    BOOL result = ::SetFilePointerEx(m_file, position, nullptr, FILE_CURRENT);
    if (result == 0)
    {
        throw ref new Platform::FailureException();
    }
}

void RandomAccessReader::SeekAbsolute(_In_ int64 position)
{
    if (position < 0)
    {
        throw ref new Platform::FailureException();
    }

    LARGE_INTEGER pos;
    pos.QuadPart = position;
    BOOL result = ::SetFilePointerEx(m_file, pos, nullptr, FILE_BEGIN);
    if (result == 0)
    {
        throw ref new Platform::FailureException();
    }
}


void RandomAccessReader::SeekToStart()
{
    LARGE_INTEGER zero = { 0 };
    BOOL result = ::SetFilePointerEx(m_file, zero, nullptr, FILE_BEGIN);
    if (result == 0)
    {
        throw ref new Platform::FailureException();
    }
}

void RandomAccessReader::SeekToEnd()
{
    LARGE_INTEGER zero = { 0 };
    BOOL result = ::SetFilePointerEx(m_file, zero, nullptr, FILE_END);
    if (result == 0)
    {
        throw ref new Platform::FailureException();
    }
}


