#include "doggabase.h"
#include <iostream>

using namespace std;

void help();

int main(int argc, char* const argv[]) {
	ios_base::sync_with_stdio(false);

	if(argc == 2) {
		if(string(argv[1]) == "-h" || string(argv[1]) == "--help") {
			help();
		}
		else {
			doggabase master(argv[1]);
			master.run();
		}
	}
	else {
		cerr << "Error: invalid executable arguments" << endl;
		exit(1);
	}
	return 0;
}

void help() {

}