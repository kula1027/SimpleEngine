#pragma once

#include <iostream>
#include <string>

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

