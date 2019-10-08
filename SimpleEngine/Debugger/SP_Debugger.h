#pragma once

#include <iostream>
#include <string>
#include <EngineDef.h>

#ifdef DEBUG
#define DebugLog(string) SP_Debugger::Log(string)
#define DebugError(string) SP_Debugger::Error(string)
#define DebugWarning(string) SP_Debugger::Warning(string)
#else
#define DebugLog(string) 
#define DebugError(string)
#define DebugWarning(string)
#endif


using namespace::std;
class SP_Debugger {
public:
	static int logLevel;

	static void Log(string str_);	
	static void Log(string str_, int logLevel_);//logLevel_ <= logLevel  ->  Ãâ·Â

	static void Error(string str_);
	static void Warning(string str_);

private:
	SP_Debugger();
	~SP_Debugger();
};

