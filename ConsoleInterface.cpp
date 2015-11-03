//
// File ConsoleInerface.cpp
//

#include "ConsoleInterface.h"

//

void CommandLoadFile::action(){
	getManager().getFileTree(filename);
}


void ConsoleInterface::init(){

	commandList.insert( make_pair("load", CommandLoadValue) );
	commandList.insert( make_pair("save", CommandSaveValue) );
	commandList.insert( make_pair("exit", CommandExitValue) );

}

Parameters* ConsoleInterface::genericParameters(CommandValue &cv){

	switch(cv){
	case CommandLoadValue:{return new CommandLoadFile();}
	case CommandSaveValue:{return new CommandSaveFile();}
	case CommandExitValue:{return new CommandExitProgramm();}
	default: {return 0;}
	};

}

void ConsoleInterface::getCommand(){

	string buffer;
	ostringstream stream;
	//while(getline(cin,stream.)){

		Parameters *param=genericParameters(commandList[buffer]);
	//}
}