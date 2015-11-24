//
// File ConsoleInerface.cpp
//

#include "MaterialsAccounting.h"

//


void ConsoleInterface::init(){
	/*
	commandList.insert( make_pair("load", CommandLoadValue) );
	commandList.insert( make_pair("save", CommandSaveValue) );
	commandList.insert( make_pair("exit", CommandExitValue) );
	*/
	commandList.insert( make_pair("load", new CommandLoadFile() ) );
	commandList.insert( make_pair("save", new CommandSaveFile() ) );
	commandList.insert( make_pair("exit", new CommandExitProgram() ) );
}

/*Parameters* ConsoleInterface::genericParameters(CommandValue &cv){

	switch(cv){
	case CommandLoadValue:{return new CommandLoadFile();}
	case CommandSaveValue:{return new CommandSaveFile();}
	case CommandExitValue:{return new CommandExitProgramm();}
	default: {throw NotCorrectAdress();}
	};

}*/

void CommandLoadFile::setParameters(istringstream & str){

	str >> fileName;
	if(fileName.size()==0){throw NotCorrectParameters();}
	//fileName=str;

}

void CommandLoadFile::action(){
	
	getManager().getFileTree( fileName.c_str() );

}
/*
void CommandSaveFile::setParameters(const string & str){

	fileName=str;

}*/

void CommandSaveFile::action(){

	getManager().putFileTree( fileName.c_str() );

}

void ConsoleInterface::getCommand(){

	init();

	istringstream stream;
	string buffer;
	shared_ptr<Parameters> param;

	while(getline(cin, buffer)){

		stream.str(buffer);
		string name_command;
		stream >> name_command;
		
		auto i = commandList.find(name_command);
		if( i==commandList.end() ){throw NotCorrectIndex(); }
		else{
			param.reset( i->second->clone() );
			param->setParameters( stream );
			try{
				param->action();
			}
			catch(ConsoleMessageErrors &me){
				me.outputMessage();
			}
		}

		stream.clear();
		buffer="";

	}


	/*
	while(getline(cin,buffer)){
		
		stream.str(buffer);
		string name_command;
		buffer.clear();
		stream >> name_command >> buffer;
		cout << buffer;
		auto i = commandList.find(name_command);
		if( i==commandList.end() ){throw NotCorrectIndex(); }
		param.reset( i->second->clone() );
		//if( dynamic_cast<CommandLoadFile*> ( param.get() ) ){cout << "aaa";}
		//getline(stream,buffer);
		param->setParameters( buffer );
		param->action();
		stream.clear();
		stream.str("");
		
		buffer="";
	}
		*/
}