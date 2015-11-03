//
//File ConsoleInterface.h
//

#ifndef MATERIAL_ACCOUNTING_CONSOLE_INTERFACE_CLASS
#define MATERIAL_ACCOUNTING_CONSOLE_INTERFACE_CLASS

#include <string>
#include <map>

#include "MaterialsAccounting.h"

/////////////////////////////////////

using namespace std;

/////////////////////////////////////

class Parameters{
public:
	virtual ~Parameters(){}
	virtual void action(){}

};
class CommandLoadFile : public Parameters{
private:
	const char *filename;
public:
	virtual void action();
};

class CommandSaveFile : public CommandLoadFile{
public:
	//const char *filename;
	
};
class CommandExitProgramm : public Parameters{
public:
	void action(){exit(0);}
};

/////////////////////////////////////////

class ConsoleInterface{
private:

	typedef string typeCommand;

	enum CommandValue{CommandLoadValue, CommandSaveValue, CommandExitValue};
	typeCommand command;

	map<typeCommand, CommandValue> commandList;

	void init();

	Parameters * genericParameters(CommandValue &cv);

public:

	void getCommand();

};


#endif