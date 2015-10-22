//
//
//

#include "MaterialsAccounting.h"

//
//
//

bool TreeMaterial::remove(string name){

	bool good; //variable for success, true - item found
	auto tmp( children.find(name) );
	if(tmp==children.end()){ // needed item nod found
		good=false;
	}else{
		children.erase(tmp);
		good=true;
	}
	return good;
}

void TreeMaterial::doGroup(string name){

	//Material material( children.find(name)->second.get() );
	auto tmp( children.find(name) );
	if( tmp!=children.end() ){
		SingleMaterial *single=dynamic_cast<SingleMaterial*> (tmp->second.get());
			// if convert in tree material failure then this item single material
		if(single){
			//do group
			TreeMaterial* tree= new TreeMaterial(name);
			//cout << tree->getName() << '\n';
			tree->add(" ", single->getAmount() );
			children.erase(tmp);
			children.insert( make_pair(name, tree) );
			/*cout << "tree space:\n" << (*tree)[" "]->getName() << "\n";
			cout << "\ntree->name:\n" <<  tree->getName() << "\n";
			cout << "\nname:\n" <<  name << "\n";
			cout << "\nchildren->getname():\n" << children[name]->getName() << "\n";
			cout << "\nchildren->getname():\n" << (*dynamic_cast <TreeMaterial*> (children[name].get() ))[" "]->getName() << "\n";
			cout << "tree space:\n" << (*tree)[" "]->getName() << "\n";*/
			//children
			//children[name].reset( tree );
			//cout << children[name]->getName() <<'\n';
		}

	}else{
		/*
		this 
		*/
	}

}

void TreeMaterial::print(ostream & out){
		printTree(getName(),out);
}

void TreeMaterial::printTree(string full_name, ostream & out){

	for( auto i=children.begin()  ; i!=children.end() ; ++i ){
		TreeMaterial *tree=dynamic_cast<TreeMaterial*> ( (i->second).get() );
		if(tree!=0){
			
			tree->printTree(full_name+' '+tree->getName(),out);
			
		}else{
			out << full_name+' ';
			i->second->print(out);
			out << '\n';
		}
	}
}

Material & TreeMaterial::operator[](string name){

	auto i=children.find(name);

	if(i!=children.end()){
		
		return i->second;
	}
	else{
		//throw
		//later
		cout << "Not found this items\n";
		//ERROR!
		Material m;
		return m;
		////return static_cast<Material&> (*this);
	}
}

void TreeManager::printTree(char *filename){
	fstream f(filename, ios::binary | ios::out);
	tree.print(f);
	f.close();
}

/*
void InputMaterial(Materials & input){

	Symbol s[256];
	Counter c;
	cin >> s >> c;

	input.count=c;
	input.length=strlen(s);
	input.childrenBytes=0;
	input.nChildren=0;
	input.childrenBytes=0;

}

////////////////////////////////////////////////

void CreateNewBase(const char *name){
	fstream f(name, ios::binary | ios::out);
	unsigned long long size=sizeof (long long);
	f.write( (const char*)&size, sizeof(long long));
	f.close();
}*/