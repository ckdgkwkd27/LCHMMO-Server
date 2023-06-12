#include "pch.h"
#include "CircularBuffer.h"

CircularBuffer::CircularBuffer(uint32 _bufferSize)
{
    capacity = _bufferSize * 10;
    buffer.resize(capacity);
}

CircularBuffer::~CircularBuffer()
{
    buffer.clear();
}

//데이터가 없으면 비워주고, 끝까지 있으면 [][][][r][][w] 맨 앞으로 이동
void CircularBuffer::Reposition()
{
    size_t dataSize = buffer.size();
    if (dataSize == 0)
    {
        readPos = 0;
        writePos = 0;
    }

    else
    {
        if(FreeSize() < bufferSize)
        memcpy(&buffer[0], &buffer[readPos], dataSize);
        readPos = 0;
        writePos = 0;
    }
}

bool CircularBuffer::OnRead(uint32 bytes)
{
    if (bytes > DataSize())
        return false;

    readPos += bytes;
    return true;
}

bool CircularBuffer::OnWrite(uint32 bytes)
{
    if (bytes > FreeSize())
        return false;

    writePos += bytes;
    return true;
}

char* CircularBuffer::ReadPos()
{
    return &buffer[readPos];
}

char* CircularBuffer::WritePos()
{
    return &buffer[writePos];
}

uint32 CircularBuffer::DataSize()
{
    return writePos - readPos;
}

uint32 CircularBuffer::FreeSize()
{
    return capacity - writePos;
}
