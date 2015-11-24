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
protected:
	Parameters(){}
	class Dummy{};
public:
	virtual ~Parameters(){}
	virtual void action()=0;
	virtual void setParameters(istringstream & buffer)=0;
	virtual Parameters *clone()=0;
	static map< string, Parameters* > prototypes;
};

class CommandLoadFile : public Parameters{
private:
	static CommandLoadFile prototype;
protected:
	string fileName;
	CommandLoadFile(){}
	CommandLoadFile(Dummy){prototypes.insert( make_pair( "load", this) );}
public:
	void action();
	void setParameters(istringstream & buffer);
	Parameters *clone(){ return new CommandLoadFile(*this); }
};

class CommandSaveFile : public CommandLoadFile{
private:
	static CommandSaveFile prototype;
protected:
	CommandSaveFile(){}
	CommandSaveFile(Dummy){prototypes.insert( make_pair( "save", this) );}
public:
	//const char *filename;
	//void setParameters(istringstream & buffer){}
	void action();
	Parameters *clone(){ return new CommandSaveFile(*this); }
};
class CommandExitProgram : public Parameters{
private:
	int code;
	static CommandExitProgram prototype;
protected:
	CommandExitProgram(Dummy){prototypes.insert( make_pair( "exit", this) );}
	CommandExitProgram():code(0){}
public:
	void action(){exit(code);}
	void setParameters(istringstream & buffer){return;}
	Parameters *clone(){ return new CommandExitProgram(*this);}
};

/////////////////////////////////////////

class ConsoleInterface{
private:

	typedef string typeCommand;

	enum CommandValue{CommandLoadValue=7766, CommandSaveValue, CommandExitValue};
	typeCommand command;

	map<string, Parameters* > & commandList;
	//map<typeCommand, shared_ptr<Parameters> > commandList;

	void init();

	//Parameters * genericParameters(CommandValue &cv);

public:

	ConsoleInterface() : commandList(Parameters::prototypes) {}
	//loop program
	void getCommand();

};


#endif