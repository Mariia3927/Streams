#pragma once
#include "stdafx.h"



class IBaseStream
{
public:
	enum class StreamType
	{
		in, out
	};

	explicit IBaseStream(StreamType type) : m_Type(type) {}
	virtual ~IBaseStream() {}

	virtual IBaseStream& Write(const char* buffer, int numberOfBytes) = 0;
	virtual IBaseStream& Read(char* buffer, int numberOfBytes) = 0;

	bool AddStream(IBaseStream* object);
	bool RemoveStream(IBaseStream* object);

	StreamType GetType() const { return m_Type; }

protected:
	bool WriteToAllStreams(const char* buffer, int numberOfBytes);
	bool ReadFromAllStreams(char* buffer, int numberOfBytes);

	bool AreTypesEqual(StreamType type);
	bool CheckParameters(const char* buffer, int numberOfBytes);

protected:
	StreamType m_Type;
	std::vector<IBaseStream*> m_Streams{};
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FileStream : public IBaseStream
{
public:
	FileStream(std::string fileName, StreamType type);
	~FileStream();

	IBaseStream& Write(const char* buffer, int numberOfBytes) override;
	IBaseStream& Read(char* buffer, int numberOfBytes) override;

private:
	std::fstream m_Object;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class MemoryStream : public IBaseStream
{
public:
	MemoryStream(std::string buffer, StreamType type);

	IBaseStream& Write(const char* buffer, int numberOfBytes) override;
	IBaseStream& Read(char* buffer, int numberOfBytes) override;

	void Show();

private:
	std::string m_Buffer;
};

class ConsoleStream : public IBaseStream
{
public:
	explicit ConsoleStream(StreamType type);

	IBaseStream& Write(const char* buffer, int numberOfBytes) override;
	IBaseStream& Read(char* buffer, int numberOfBytes) override;
};
