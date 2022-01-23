#include "attribute.h"
#include "condition.h"
#include "interpreter.h"
#include "API.h"
#include <cstdlib>
#include <time.h>
#include <fstream>


int main(int argc, char* argv[]) {
	API api;
	CatalogManager cm;
	api.cm = &cm;
	RecordManager rm(&api);
	api.rm = &rm;
	IndexManager im(&api);
	api.im = &im;
	Interpreter ip(&api);
	fstream file;


	string s;
	clock_t begin, finish;
	cout << "Welcome to Minisql!" << endl;
	int state= TRUE_SYNTAX;
	while(true) {
		
		cout << ">>>";
	   if(state == TRUE_SYNTAX || state == ERROR_SYNTAX) {
			getline(cin, s, ';');
			begin = clock();
			state = ip.mainInterpreter(s);
			finish = clock();
			if(state != ERROR_SYNTAX) {

				double t = (double)(finish - begin) / CLOCKS_PER_SEC;
//				cout << "\n\truntime: ";
//				cout.width(5);
//				cout << t <<  endl;
			}
			if (state ==QUIT) {
				//			system("pause");
				break;
			}
		}
		
	}
}
