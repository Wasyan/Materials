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
#include <list>
#include <sstream>

//
//
//

#include "Errors.h"
#include "ConsoleInterface.h"

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
	virtual UINT64 putFile(fstream &out, UINT64 location=0)const;
	virtual UINT64 getFile(fstream &out, UINT64 location=0);
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
	UINT64 putFile(fstream &out, UINT64 location=0)const;
	UINT64 getFile(fstream &out, UINT64 location=0);
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

	void remove(const Material &m);
	void remove(string name);

	void print(ostream &out)const;
	UINT64 putFile(fstream &in, UINT64 location=0)const;
	UINT64 getFile(fstream &out, UINT64 location=0);

	TreeMaterial *getTree(MaterialAccounting *item)const;
	SingleMaterial *getSingle(MaterialAccounting *item)const;

	TreeMaterial* get(){return this;}	//?
	
	Material operator[] (const string & name){return *search(name); } // !!add thorow

};


/*
class TreeNode{
private:
	MaterialAccounting *ma;
public:

};*/

//Singletone class for management tree
class TreeManager{
private:

	typedef list<Material> ParentStack;

	TreeMaterial *treeObj;
	Material tree;
	ParentStack iterator;
	
	//blocked all possibility create object beyond this class
	TreeManager():treeObj(new TreeMaterial("")), tree(treeObj) {}
	TreeManager(const TreeManager& );
	TreeManager & operator= (const TreeManager&t);

	void instIterator();

public:
	
	friend TreeManager&  getManager();		//get instance Manager
	
	//operation for tree
	void printTree(const char *file_name);		//print tree in txt
	void putFileTree(const char *file_name);		//into binary
	void getFileTree(const char *file_name);		//from binary
	
	//operation for iterator
	void addChild(const string &name, UINT64 amount);
	void go(const string &name);
	void back();
	void deleteChild(const string &name);
	void deleteThis();//by deleteChild

};


/////////////////////

#endif