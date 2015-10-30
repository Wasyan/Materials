//
//
//
//
//
//
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

#include "Errors.h"

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
	virtual void print(ostream &out)const=0;
	virtual UINT64 putFile(fstream &out, UINT64 location)const;
	virtual UINT64 getFile(fstream &out, UINT64 location);
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
	void print(ostream& out)const{out << getName() << " - " << amount << '\n';}
	UINT64 putFile(fstream &out, UINT64 location)const;
	UINT64 getFile(fstream &out, UINT64 location);
};

////////////////////////////////////////////////////////////////////

class TempMaterial : public MaterialAccounting{
private:
	void print(ostream &out)const{}
public:
	explicit TempMaterial(string name) : MaterialAccounting(name){}

};

///////////////////////////////////////////////////////////////////

class TreeMaterial : public MaterialAccounting{

private:

	set<Material> children;
	set<Material>::iterator search(string name);

protected:

	void print(ostream &out, string & full_name)const;
	TreeMaterial *getTree_(MaterialAccounting *ma)const{return dynamic_cast <TreeMaterial*> (ma); }
	SingleMaterial *getSingle_(MaterialAccounting *ma)const{return dynamic_cast <SingleMaterial*> (ma); }

public:

	explicit TreeMaterial(string name) : MaterialAccounting(name) { children.key_comp(); }

	void add(const string & name, UINT64 amount){ children.insert( Material(new SingleMaterial(name,amount)) );}
	void add(const Material &m){ children.insert( m );}
	void add(const TreeMaterial &tm){ children.insert( new TreeMaterial(tm) ); }

	void remove(const Material &m){ children.erase(m); }
	void remove(string name);

	void print(ostream &out)const;
	UINT64 putFile(fstream &in, UINT64 location)const;
	UINT64 getFile(fstream &out, UINT64 location);

	TreeMaterial *getTree(MaterialAccounting *item)const;
	SingleMaterial *getSingle(MaterialAccounting *item)const;

};

//bool operator == (const Material &, const Material &);

/*
class TreeMaterial : public MaterialAccounting{
private:

	//map<string, Material> children;		//children this item
	//set <Material> children;

	void print(string full_name, ostream &out);

protected:

	friend MaterialAccounting *StringToMaterial(string name);

public:
	set <Material> children;//////
	explicit TreeMaterial(string name) : MaterialAccounting(name){}

	void add(string name, UINT64 amount){ children.insert( Material(new SingleMaterial(name,amount)) );}
	bool remove(string name);			// if fuction return false then item not remove

	void doGroup(string name);			//single material convert in group material (TreeMaterial)

	void print(ostream &out);
	UINT64 putFile(fstream &out, UINT64 location);
	UINT64 getFile(fstream &out, UINT64 location, int );

	Material & operator[] (string name);

	TreeMaterial* getTree(MaterialAccounting *ma){return dynamic_cast<TreeMaterial*>(ma);}	//convert from ptr base to ptr tree
	TreeMaterial* getTree(string name){ 
		return getTree( (*children.find( static_cast<Material>(StringToMaterial(name) ) ) ).get()  ); 
	}	//convert son on parent.name

	SingleMaterial *getSingle(MaterialAccounting *ma){ return dynamic_cast<SingleMaterial*>(ma); }
	SingleMaterial* getSingle(string name){ return getSingle( (*children.find( static_cast<Material>(StringToMaterial(name) ) ) ).get()  ); }

	

	//set<Material>::iterator & find(Material &m)const{return children.find(m);}
	//set<Material>::iterator & find(string name)const;//{return find();}

};*/



//Singletone class for management tree
/*class TreeManager{
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

*/
/////////////////////

#endif