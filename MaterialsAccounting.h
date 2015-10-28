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
#include <set>
#include <memory>	//shared_ptr
#include <stack>

//
//
//

using namespace std;

typedef string Symbol;					//type for txt data
typedef unsigned long long UINT64;		//short record type


class MaterialAccounting{
private:
	string name;						// name material or group materials

protected:

	MaterialAccounting(string name) : name(name){}

public:

	virtual ~MaterialAccounting(){}

	bool operator == (const MaterialAccounting & obj)const{return name==obj.name;}
	bool operator > (const MaterialAccounting & obj)const{return name>obj.name;}

	const char * getName()const{return name.c_str();}
	virtual void print(ostream &out)=0;
	virtual UINT64 putFile(fstream &out, UINT64 location);
	virtual UINT64 getFile(fstream &out, UINT64 location, int);
};

/*
bool operator == (const Material & a, const Material &b);
bool operator > (const Material & a, const Material &b);
bool operator < (const Material & a, const Material &b);*/

typedef shared_ptr<MaterialAccounting> Material;

/////////////////////////////////////////////////////////////////////

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
	UINT64 putFile(fstream &out, UINT64 location);
	UINT64 getFile(fstream &out, UINT64 location, int);
};

////////////////////////////////////////////////////////////////////


class TreeMaterial : public MaterialAccounting{
private:

	map<string, Material> children;		//children this item
	
	void print(string full_name, ostream &out);

public:
	
	explicit TreeMaterial(string name) : MaterialAccounting(name){}

	void add(string name, UINT64 amount){children.insert( make_pair(name, Material(new SingleMaterial(name,amount)) ) );}
	bool remove(string name);			// if fuction return false then item not remove

	void doGroup(string name);			//single material convert in group material (TreeMaterial)

	void print(ostream &out);
	UINT64 putFile(fstream &out, UINT64 location);
	UINT64 getFile(fstream &out, UINT64 location, int );

	Material & operator[] (string name);

	TreeMaterial* getTree(MaterialAccounting *ma){return dynamic_cast<TreeMaterial*>(ma);}	//convert from ptr base to ptr tree
	TreeMaterial* getTree(string name){return getTree(children[name].get());}	//convert son on parent.name
	SingleMaterial *getSingle(MaterialAccounting *ma){return dynamic_cast<SingleMaterial*>(ma);}
	SingleMaterial* getSingle(string name){return getSingle(children[name].get());}

};



//Singletone class for management tree
class TreeManager{
private:

	TreeMaterial tree;	
	Material current;

	
	//blocked all possibility create object beyond class
	TreeManager():tree(""){}
	TreeManager(const TreeManager& ):tree(""){}
	TreeManager & operator= (const TreeManager&t);

public:
	
	friend TreeManager&  getManager();		//get instance Manager
	
	void printTree(char *file_name);		//print tree in txt
	void putFileTree(char *file_name);		//into binary
	void getFileTree(char *file_name){}		//from binary
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