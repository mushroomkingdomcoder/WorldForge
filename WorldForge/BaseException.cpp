#include "BaseException.h"

BaseException::BaseException(int line, std::string file, std::string _note) noexcept
	:
	line(line),
	file(file),
	note(_note)
{
	if (note.empty())
	{
		note = "[None available]";
	}
}

const char* BaseException::what() const noexcept
{
	std::ostringstream wht;
	wht << "Exception Type: " << GetType() << std::endl
		<< "File Name: " << GetFile() << std::endl
		<< "Line Number: " << GetLine() << std::endl
		<< "Additional Info: " << GetNote() << std::endl;
	whatBuffer = wht.str();
	return whatBuffer.c_str();
}

const char* BaseException::GetType() const noexcept
{
	return "FantasyForge Runtime Exception";
}

int BaseException::GetLine() const noexcept
{
	return line;
}

std::string BaseException::GetFile() const noexcept
{
	return file;
}

std::string BaseException::GetNote() const noexcept
{
	return note;
}

std::string BaseException::GetErrorCodeDesc(HRESULT hr) noexcept
{
	char* pDescBuffer = nullptr;
	DWORD length = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&pDescBuffer), 0, nullptr
	);
	if (length == 0)
	{
		return "Unidentified error code.";
	}
	std::string desc = pDescBuffer;
	LocalFree(pDescBuffer);
	return desc;
}




