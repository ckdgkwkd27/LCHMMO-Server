#pragma once
class CircularBuffer
{
public:
	CircularBuffer(uint32 _bufferSize);
	~CircularBuffer();

	void Reposition();
	bool OnRead(uint32 bytes);
	bool OnWrite(uint32 bytes);

	char* ReadPos();
	char* WritePos();
	uint32 DataSize();
	uint32 FreeSize();

	char* data()
	{
		return buffer.data();
	}

private:
	uint32 capacity = 0;
	uint32 bufferSize = 0;
	uint32 readPos = 0;
	uint32 writePos = 0;
	std::vector<char> buffer;
};

