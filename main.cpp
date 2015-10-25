//
//
//
//

#include "MaterialsAccounting.h"

//
//
//

int main(int argc, char *argv[]){

	//*//temp version, simulation the program is started from console.
	char temp[]="console";
	*argv=temp;
	argc=2;
	////////////////////////////////////////////////////////*/
	
	if( argc > 1 && strcmp("console", *argv )==0 ){ // check for request terminale mode

		TreeMaterial tm("all");
		tm.add("1/2all", 2);
		tm.add("1/3all", 3);
		tm.add("1/5all", 5);
		tm.add("1/7all", 7);

		tm.doGroup("1/2all");
		TreeMaterial* tm2 = tm.getTree("1/2all");
		tm2->add("1/4all",4);
	
		tm.print(cout);
		
		cout << "\n\n\n";
		fstream f2("file.bin", ios::binary | ios::out);
		UINT64 aa=tm.putFile(f2, 0);
		f2.close();

		fstream f("file.bin", ios::binary | ios::in);
		TreeMaterial tree("all");
		UINT64 bb=tree.getFile(f,0);
		f.close();

		tree.print(cout);

		cout << '\n' << aa << ' ' << bb << '\n';
		//getManager();
	}
	else{ //terminale mode not selected

		//create window, after program works in windows mode
		//----------------------------------------------///
	}

	return 0;
}