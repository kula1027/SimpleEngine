#include "SP_Debugger.h"


int SP_Debugger::logLevel = 0;

void SP_Debugger::Log(string str_) {	
	cout << str_ << endl;
}

void SP_Debugger::Log(string str_, int logLevel_) {
	if (logLevel <= logLevel_) {
		cout << str_ << endl;
	}	
}

void SP_Debugger::Error(string str_) {
	cout << str_ << endl;
}

void SP_Debugger::Warning(string str_) {
	cout << str_ << endl;
}

SP_Debugger::SP_Debugger() {
}


SP_Debugger::~SP_Debugger() {
}
