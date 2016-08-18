#include "doggabase.h"
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

//Function declarations
void help();
void lowercase(string& s);
int to_bool(const string& response);
void run(const string& data_file);

//Main driver
int main(int argc, char* const argv[]) {
	ios_base::sync_with_stdio(false);

	if(argc == 2) {
		if(string(argv[1]) == "-h" || string(argv[1]) == "--help") {
			help();
		}
		else {
			run(argv[1]);
		}
	}
	else {
		cerr << "Error: invalid executable arguments" << endl;
		exit(1);
	}
	return 0;
}



void doggabase::read_data(const string& input_file) {
	//Open file
	ifstream fin;
	fin.open(input_file);
	if(!fin.is_open()) {
		cerr << "Failed to open " << input_file << endl;
		exit(1);
	}
	string name, guard, kids, train, trash;
	char hair;
	double weight;
	for(int i = 0; getline(fin, name, '|'); ++i) {
		fin >> hair >> guard >> kids >> train >> weight;
		catalog.push_back(dog(i, hair, to_bool(guard), to_bool(kids)
			, to_bool(train), weight, name));
		update_dog(catalog[i]);
		getline(fin, trash);
	}//for
	cout << "Read in " << catalog.size() << " entries." << endl;
}

void doggabase::update_dog(dog& d) {
	//Array '1' corresponds to a 'yes' response list
	// while array '0' corresponds to a 'no' response list
	if(d.guard) { guard_list[1].push_back(d.id); }
		else { guard_list[0].push_back(d.id); }
	if(d.kids) { kids_list[1].push_back(d.id); }
		else { kids_list[0].push_back(d.id); }
	if(d.train) { train_list[1].push_back(d.id); }
		else { train_list[0].push_back(d.id); }
	//Array '0' corresponds to short, '1' to medium
	// and '2' to long hair categories
	if(d.hair == 's') { hair_list[0].push_back(d.id); }
		else if(d.hair == 'm') { hair_list[1].push_back(d.id); }
			else { hair_list[2].push_back(d.id); }
	add_to_keymap(d.name, d.id);
}
//Adds keys to the keyword map by parsing the given string
void doggabase::add_to_keymap(const string& name, int idx) {
	string word;
	stringstream ss(name);
	while(ss >> word) {
		lowercase(word);
		auto& v = keyword_map[word];
		if(v.empty() || v.back() != idx) {
			v.push_back(idx);
		}
	}
}//add_to_keymap

void doggabase::search_by_trait() {
	cout << "";
}

//HELPER FUNCTIONS
void help() {

}

void welcome() {
	cout << "┈╭━╱▔▔▔╱▔▔▔╲┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈\n"
		 << "┈┃╭▏┊┊┊▏▏▕▎▍╮┈┈ WELCOME TO THE WORLD FAMOUS ┈\n"
		 << "┈╰┛┃┏━┓╲▏┳▕▉┃┈┈┈┈┈┈┈┈ DOG-DATABASE ┈┈┈┈┈┈┈┈┈┈\n"
		 << "┈┈┈┃┗╮┃┗╮╰┳┳╯┈┈┈┈┈┈┈┈ \"DOGGABASE\" ┈┈┈┈┈┈┈┈┈┈┈\n"
		 << "┈┈┈┗┻╯┗┻╯┈╰╯┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈\n";
	cout << "This program is designed to process data of various dog \n"
		 << "breeds and perform advanced searches to help the user find \n"
		 << "the appropriate dog. For further questions, please refer \n"
		 << "to the help section.\n";
}

//Converts string to lowercase
void lowercase(string& s) {
	transform(s.begin(), s.end(), s.begin(), [](char c) { 
    	return tolower(c); 
    });
}

void run(const string& data_file) {
	//Initialize doggabase
	doggabase master(data_file);

	//User interface
	welcome();
	int selection = 1;
	while(selection != 0) {
		cout << "-----------------------------------------------------------\n"
			 << "(1) Search by trait 		(2) Find the perfect match\n"
			 << "(3) Add a dog 			(4) Remove a dog\n"
			 << "(5) Browse entire catalog 	(0) Quit\n"
			 << "-----------------------------------------------------------\n"
			 << "Please enter the number corresponding to the desired operation: ";
		cin >> selection;
	}



}

int to_bool(const string& response) {
	if(response == "yes") {
		return 1;
	}
	else if(response == "no") {
		return 0;
	}
	else { /// only needed for user IO not data input
		//cout << "Invalid response. Please enter yes or no:" << endl;
		return -1;
	}
}