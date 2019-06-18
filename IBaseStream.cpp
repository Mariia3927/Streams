#include "stdafx.h"
#include "IBaseStream.h"


bool IBaseStream::AddStream(IBaseStream* object)
{
	if (!object)
	{
		std::cout << "Error! The stream object is incorrect!" << std::endl;
		return false;
	}

	if (m_Type != object->GetType())
	{
		std::cout << "Error! The types of streams are incompatible!" << std::endl;
		return false;
	}

	m_Streams.push_back(object);
	return true;
}

bool IBaseStream::RemoveStream(IBaseStream* object)
{
	if (!object)
	{
		std::cout << "Error! The stream object is incorrect!" << std::endl;
		return false;
	}

	std::remove(m_Streams.begin(), m_Streams.end(), object);
	return true;
}

bool IBaseStream::WriteToAllStreams(const char* buffer, int numberOfBytes)
{
	if (m_Streams.size())
	{
		for (unsigned int i = 0; i < m_Streams.size(); i++)
		{
			m_Streams[i]->Write(buffer, numberOfBytes);
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool IBaseStream::ReadFromAllStreams(char* buffer, int numberOfBytes)
{
	if (m_Streams.size())
	{
		for (unsigned int i = 0; i < m_Streams.size(); i++)
		{
			strcpy_s(buffer, numberOfBytes, "");
			m_Streams[i]->Read(buffer, numberOfBytes);

		}
		return true;
	}
	else
	{
		return false;
	}
}

bool IBaseStream::AreTypesEqual(StreamType type)
{
	if (m_Type == type)
	{
		return true;
	}
	else
	{
		std::cout << "Error! The type of stream is incorrect for this operation!" << std::endl;
		return false;
	}
}

bool IBaseStream::CheckParameters(const char* buffer, int numberOfBytes)
{
	if (numberOfBytes <= 0)
	{
		std::cout << "Error! The number of bytes for reading / writing cannot be less than or equal zero!" << std::endl;
		return false;
	}
	if (!buffer)
	{
		std::cout << "Error! The buffer is incorrect!" << std::endl;
		return false;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FileStream::FileStream(std::string fileName, StreamType type) : IBaseStream(type)
{
	if (m_Type == StreamType::in)
	{
		m_Object.open(fileName.c_str(), std::ios::in);
	}
	else
	{
		m_Object.open(fileName.c_str(), std::ios::out);
	}

	if (!m_Object)
	{
		std::cout << "Error! The filestream object hasn't been created successfully!" << std::endl;
	}
}

FileStream::~FileStream()
{
	if (m_Object)
	{
		m_Object.close();
	}
}

IBaseStream& FileStream::Write(const char* buffer, int numberOfBytes)
{
	if (!AreTypesEqual(StreamType::out) || !CheckParameters(buffer, numberOfBytes))
	{
		return *this;
	}

	m_Object.write(buffer, numberOfBytes);

	WriteToAllStreams(buffer, numberOfBytes);

	return *this;
}

IBaseStream& FileStream::Read(char* buffer, int numberOfBytes)
{
	if (!AreTypesEqual(StreamType::in) || !CheckParameters(buffer, numberOfBytes))
	{
		return *this;
	}

	m_Object.read(buffer, numberOfBytes);

	ReadFromAllStreams(buffer, numberOfBytes);

	return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MemoryStream::MemoryStream(std::string buffer, StreamType type) : IBaseStream(type), m_Buffer(buffer) { }

IBaseStream& MemoryStream::Write(const char* buffer, int numberOfBytes)
{
	if (!AreTypesEqual(StreamType::out) || !CheckParameters(buffer, numberOfBytes))
	{
		std::cout << "Error! The type of stream is incorrect for this operation!" << std::endl;
		return *this;
	}

	m_Buffer.assign(buffer, numberOfBytes);

	WriteToAllStreams(buffer, numberOfBytes);

	return *this;
}

IBaseStream& MemoryStream::Read(char* buffer, int numberOfBytes)
{
	if (!AreTypesEqual(StreamType::in) || !CheckParameters(buffer, numberOfBytes))
	{
		std::cout << "Error! The type of stream is incorrect for this operation!" << std::endl;
		return *this;
	}

	strcpy_s(buffer, numberOfBytes, m_Buffer.c_str());

	ReadFromAllStreams(buffer, numberOfBytes);

	return *this;
}

void MemoryStream::Show()
{
	std::cout << "Memory buffer: " << m_Buffer << std::endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ConsoleStream::ConsoleStream(StreamType type) : IBaseStream(type)
{
}

IBaseStream & ConsoleStream::Write(const char* buffer, int numberOfBytes)
{
	if (!AreTypesEqual(StreamType::out) || !CheckParameters(buffer, numberOfBytes))
	{
		std::cout << "Error! The type of stream is incorrect for this operation!" << std::endl;
		return *this;
	}

	std::cout.write(buffer, numberOfBytes);
	std::cout << std::endl;

	WriteToAllStreams(buffer, numberOfBytes);

	return *this;
}

IBaseStream & ConsoleStream::Read(char* buffer, int numberOfBytes)
{
	if (!AreTypesEqual(StreamType::in) || !CheckParameters(buffer, numberOfBytes))
	{
		std::cout << "Error! The type of stream is incorrect for this operation!" << std::endl;
		return *this;
	}

	std::cout << "Enter anything to the console:\n";
	std::cin.read(buffer, numberOfBytes);

	ReadFromAllStreams(buffer, numberOfBytes);

	return *this;
}
