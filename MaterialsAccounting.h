//
//
//

#ifndef MATERIALS_ACCOUNTING_CLASSSES_FILE
#define MATERIALS_ACCOUNTING_CLASSSES_FILE

#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#include <map>
#include <memory>	//shared_ptr

//
//
//

using namespace std;

typedef string Symbol;					//type for txt data
typedef unsigned long long UINT64;		//type for txt data


class MaterialAccounting{
private:
	string name;						// name material or group materials

protected:

	virtual ~MaterialAccounting(){}
	MaterialAccounting(string name) : name(name){}

public:

	const char * getName()const{return name.c_str();}
	virtual void print(ostream &out)=0;
};

typedef shared_ptr<MaterialAccounting> Material;

/////

class SingleMaterial : public MaterialAccounting{
private:
	UINT64 amount;						// count measure this materials
	/*///////////////////////////////////////////////////////
	later it will be implemented Date List :)
	//*///////////////////////////////////////////////////////
public:

	SingleMaterial(string name, UINT64 amount) : MaterialAccounting(name), amount(amount){}
	UINT64 getAmount()const{return amount;}
	void addAmount(UINT64 inc){amount+=inc;}
	void print(ostream& out){out << getName() << " - " << amount << '\n';}
};

//////////////////////////////////////////////

class TreeMaterial : public MaterialAccounting{
private:
	
	map<string, Material> children;		//children this item
	void printTree(string full_name, ostream &out);

public:
	
	explicit TreeMaterial(string name) : MaterialAccounting(name){}

	void add(string name, UINT64 amount){children.insert( make_pair(name, Material(new SingleMaterial(name,amount)) ) );}
	bool remove(string name);			// if fuction return false then item not remove

	void doGroup(string name);			//single material convert in group material (TreeMaterial)

	void print(ostream &out);

	Material & operator[] (string name);

	TreeMaterial* getTree(MaterialAccounting *ma){return dynamic_cast<TreeMaterial*>(ma);}
	TreeMaterial* getTree(string name){return getTree(children[name].get());}

};


//Is not developed
class TreeManager{
private:

	TreeMaterial tree;	
	Material current;

	//map

public:
	TreeManager():tree(""){}
	void printTree(char *file_name);
	void putFileTree(char *file_name){}
	void getFileTree(char *file_name){}
};

/*////////////////////////////////

typedef struct BaseFile{

	char name[256];						// full name this file
	unsigned long long size;			// size this file

	fstream file;						// stream this file

} BaseData;

////////////////////////////////////////////////////////////////

void CreateNewBase(const char *filename);		//Function create new file for base date


////////////////////////////////////////////////////////////////
*/
#endif