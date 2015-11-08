//
// File ConsoleInerface.cpp
//

#include "MaterialsAccounting.h"

//


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
	default: {throw NotCorrectAdress();}
	};

}

void CommandLoadFile::setParameters(const string & str){

	fileName=str;

}

void CommandLoadFile::action(){
	
	getManager().putFileTree( fileName.c_str() );

}
/*
void CommandSaveFile::setParameters(const string & str){

	fileName=str;

}*/

void CommandSaveFile::action(){

	getManager().getFileTree( fileName.c_str() );

}

void ConsoleInterface::getCommand(){

	init();

	istringstream stream;
	string buffer;
	shared_ptr<Parameters> param;

	while(getline(cin,buffer)){
		
		stream.str(buffer);
		string name_command;
		stream >> name_command >> buffer;
		param.reset(genericParameters(commandList[name_command]));
		//if( dynamic_cast<CommandLoadFile*> ( param.get() ) ){cout << "aaa";}
		//getline(stream,buffer);
		param->setParameters( buffer );
		param->action();
		stream.clear();
		stream.str("");
		
		buffer="";
	}
		
}