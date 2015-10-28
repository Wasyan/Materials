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

		fstream f3("file3.bin", ios::binary | ios::out);
		f3.seekp(0,ios::beg);
		UINT64 a=7;
		f3.write( (char*)&a , 8 );
		f3.write( (char*)&a , 8 );
		f3.write( (char*)&a , 8 );
		f3.write( (char*)&a , 8 );
		f3.write( (char*)&a , 8 );
		f3.write( (char*)&a , 8 );
		f3.write( (char*)&a , 8 );

		f3.close();
		
		TreeMaterial tm("all");
		tm.add("1/2all", 2);
		tm.add("1/3all", 3);
		tm.add("1/5all", 5);
		tm.add("1/7all", 7);

		tm.doGroup("1/2all");
		TreeMaterial* tm2 = tm.getTree("1/2all");
		tm2->add("1/4all",4);
		tm2->add("1/8all",8);
		tm2->add("1/16all",16);

		tm2->doGroup("1/8all");
		TreeMaterial* tm3 = tm2->getTree("1/8all");
		tm3->add("1/64all", 64);

		TreeMaterial* tm4;
		tm.doGroup("1/5all");
		tm4 = tm.getTree("1/5all");
		tm4->add("1/25all", 25);

		tm.print(cout);
	
		
		cout << "\n\n\n";
		fstream f2("file.bin", ios::binary | ios::out);
		UINT64 aa=tm.putFile(f2, 0);
		f2.close();
		
		fstream f("file.bin", ios::binary | ios::in);
		TreeMaterial tree("all");
		UINT64 bb=tree.getFile(f,0,0);
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