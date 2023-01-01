#pragma once
#include "Win32Includes.h"
#include <exception>
#include <string>
#include <sstream>

class BaseException : public std::exception
{
private:
	int line;
	std::string file;
	std::string note;
protected:
	mutable std::string whatBuffer = "";
public:
	BaseException() = delete;
	BaseException(int line, std::string file, std::string _note = "") noexcept;
	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	int GetLine() const noexcept;
	std::string GetFile() const noexcept;
	std::string GetNote() const noexcept;
public:
	static std::string GetErrorCodeDesc(HRESULT hr) noexcept;
};
#define EXCPT BaseException(__LINE__, __FILE__)
#define EXCPT_NOTE(note) BaseException(__LINE__, __FILE__, note)


