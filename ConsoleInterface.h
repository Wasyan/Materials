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
// Pattern prototype, command
class Parameters{
public:
	virtual ~Parameters(){}
	virtual void action()=0;
	virtual void setParameters(istringstream & buffer)=0;
	virtual Parameters *clone()=0;
};
class CommandLoadFile : public Parameters{
protected:
	string fileName;
public:
	void action();
	void setParameters(istringstream & buffer);
	Parameters *clone(){ return new CommandLoadFile(*this); }
};

class CommandSaveFile : public CommandLoadFile{
public:
	//const char *filename;
	//void setParameters(istringstream & buffer){}
	void action();
	Parameters *clone(){ return new CommandSaveFile(*this); }
};
class CommandExitProgram : public Parameters{
	int code;
public:
	void action(){exit(code);}
	void setParameters(istringstream & buffer){return;}
	CommandExitProgram():code(0){}
	Parameters *clone(){ return new CommandExitProgram(*this); }
};

/////////////////////////////////////////

class ConsoleInterface{
private:

	typedef string typeCommand;

	enum CommandValue{CommandLoadValue=7766, CommandSaveValue, CommandExitValue};
	typeCommand command;


	map<typeCommand, shared_ptr<Parameters> > commandList;

	void init();

	//Parameters * genericParameters(CommandValue &cv);

public:

	//loop program
	void getCommand();

};


#endif