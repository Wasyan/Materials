//
//
//
//


#ifndef MATERIALS_ACCOUNTING_ERROR_CLASSSES_FILE
#define MATERIALS_ACCOUNTING_ERROR_CLASSSES_FILE

#include <string>

////////////////////////////////////
using namespace std;

class Errors{
private:


public:
	virtual ~Errors(){}

};

class MessageErrors : public Errors{
protected:

	const string message;

public:

	const char * getMessage() const{return message.c_str(); }
	MessageErrors(const char *msg) : message(msg){}

};

class ConsoleMessageErrors : MessageErrors{

public:
	void outputMessage(){cout.write(message.c_str(), message.size());}

};

class FileNotFound : public Errors{

};
class FileNotCreate : public Errors{

};

class NotCorrectAdress : public Errors{

};

class NotCorrectIndex : public Errors{

};

class NotCorrectParameters : public Errors{

};

class NotCorrectCast : public Errors{

};

class NotDeletedObject : public Errors{

};

class NotCorrectCommand : public Errors{
};

class CommandNotExist : public NotCorrectCommand{
};

#endif

