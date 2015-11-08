//
//File ConsoleInterface.h
//

#ifndef MATERIAL_ACCOUNTING_CONSOLE_INTERFACE_CLASS
#define MATERIAL_ACCOUNTING_CONSOLE_INTERFACE_CLASS

#include <string>
#include <map>


/////////////////////////////////////

using namespace std;

/////////////////////////////////////

class Parameters{
public:
	virtual ~Parameters(){}
	virtual void action(){}
	virtual void setParameters(const string & buffer)=0;
};
class CommandLoadFile : public Parameters{
protected:
	string fileName;
public:
	void action();
	void setParameters(const string & buffer);
};

class CommandSaveFile : public CommandLoadFile{
public:
	//const char *filename;
	void setParameters(const string & buffer){}
	void action();
};
class CommandExitProgramm : public Parameters{
	int code;
public:
	void action(){exit(code);}
	void setParameters(const string & buffer){return;}
	CommandExitProgramm():code(0){}
};

/////////////////////////////////////////

class ConsoleInterface{
private:

	typedef string typeCommand;

	enum CommandValue{CommandLoadValue=7766, CommandSaveValue, CommandExitValue};
	typeCommand command;

	map<typeCommand, CommandValue> commandList;

	void init();

	Parameters * genericParameters(CommandValue &cv);

public:

	void getCommand();


};


#endif