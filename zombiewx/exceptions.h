#pragma once
#include <exception>

// Macro because noexcept is NYI in VS 2013
#ifdef _WIN32
#define NOEXCEPT
#else
#define NOEXCEPT noexcept
#endif

class InvalidOp : public std::exception
{
	const char* what() const NOEXCEPT override { return "Invalid OP code or parameters."; }
};

class AccessViolation : public std::exception
{public:
	const char* what() const NOEXCEPT override { return "Machine tried to access outside available memory."; }
};

class ParseExcept : public std::exception
{
	const char* what() const NOEXCEPT override{ return "Parse ZOM file except"; }
};

class InvalidGoto : public std::exception{
	const char* what() const NOEXCEPT override{ return "Goto Line Number is invalid"; }
};

class FileLoadExcept : public std::exception{
	const char* what() const NOEXCEPT override{ return "ZOM file invalid"; }
};

class ZomFileNotLoadExcept : public std::exception{
public:
	const char* what() const NOEXCEPT override{ return "ZOM file not load"; }
};

class ZombieInvalidOperation : public std::exception{
public:
	const char* what() const NOEXCEPT override{ return "Zombie cannot perform ranged_attack or access memory"; }
};