#include "doggabase.h"
#include <iostream>

using namespace std;

//Compiled using:
//g++ -std=c++14 -Wconversion -Wall -Werror -Wextra -pedantic doggabase.cpp main.cpp -o dog
//Ran using: ./dog dog_data.txt

//Function declarations
void help();

//Main function - processes executable arguments and runs program
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

//Prints help message
void help() {
	cout << " // Doggabase Help Section //\n\n"
		 << "This program provides the user with the ability to search\n"
		 << "through a database of dogs by certain criteria and perform\n"
		 << "minor modifications to the database. \"Search by trait\"\n"
		 << "allows for searches by various categories. A weight search\n"
		 << "will find dogs within a given weight range. A breed search\n"
		 << "will find all dogs with a given keyword in their name.\n"
		 << "E.g. A search for \"Bull\" would return \"Bull Terrier\".\n"
		 << "Browsing the catalog will print the entire database.\n"
		 << "In addition, users may add or remove a dog from the database.\n"
		 
		 << "File input: database input is read in from a text file of\n"
		 << "entries with the following format: breed, pipe-symbol "|",\n"
		 << "hair length (s/m/l signifying short/medium/long), good guard\n"
		 << " skills (yes/no), good with kids (yes/no), easily trained\n"
		 << "(yes/no). Each dog entry is on a separate line.\n"
		 << "Executable must be run with the input filename as an argument.\n";
}//help