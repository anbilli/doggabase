#include "doggabase.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

using namespace std;


//Function declarations
void help();
void lowercase(string& s);
int to_bool(const string& response);
bool check_hair_input(char in);
bool check_binary_input(string& in, bool& var);	
void welcome();
void from_bool(bool b);

///////////////////Implementation of member functions ///////////////////
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


void doggabase::add_dog() {
	string response = "yes";
	//cout << "You have selected option 3: Add a dog\n";

	while(response == "yes" || response == "y") {
		char hair;
		bool flag = 0;
		bool guard, kids, train;
		double weight;
		string name, string_weight, trash;
		cout << "Please enter in the following data or '0' if you wish to cancel.\n"
			 << "Dog's Breed: ";

		//Name	
		getline(cin, trash); 
		getline(cin, name, '\n');
		if(name == "0") {
			break;
		}

		//Weight
		cout << "Weight in lbs: ";
		cin >> string_weight;
		while(!flag) {
			for(auto i: string_weight) {
				if(isdigit(i) || i == '.') {
					weight = stod(string_weight);
					flag = 1;
				}
				else {
					cout << "Incorrect input. Please enter a positive number: ";
					cin >> string_weight;
				}
			}
		}
		//Hair
		cout << "What hair length does this dog have?\n"
			 << "Enter short/medium/long as s/m/l: ";
		flag = 0;
		while (!flag) {
			cin >> hair;
			hair = tolower(hair); ///int??
			flag = check_hair_input(hair);
		}

		//Guard
		cout << "Is this dog an exceptionally good guard dog? (yes/no): ";
		flag = 0;
		while(!flag) {
			cin >> response;
			flag = check_binary_input(response, guard);
		}

		//Kids
		cout << "Is this dog exceptionally good with children? (yes/no): ";
		flag = 0;
		while(!flag) {
			cin >> response;
			flag = check_binary_input(response, kids);
		}

		//Train
		cout << "Is this dog exceptionally easy to train? (yes/no): ";
		flag = 0;
		while(!flag) {
			cin >> response;
			flag = check_binary_input(response, train);
		}

		catalog.push_back(dog(int(catalog.size()), hair, guard, kids
			, train, weight, name));
		update_dog(catalog.back());

		cout << "Would you like to add another dog? ";
		cin >> response;
		lowercase(response);
	}//while

}

void doggabase::delete_dog() {
	string kill, trash;
	cout << "Please enter the name of the dog you would like\n"
		 << "to remove from the database or '0' to cancel: ";
	getline(cin, trash);
	getline(cin, kill);
	if(kill != "0") {
		bool flag = 0;
		for(unsigned i = 0; i < catalog.size(); ++i) {
			if(catalog[i].name == kill) {
				cout << catalog[i].name << " removed.\n";
				catalog.erase(catalog.begin() + i);
				flag = 1;
				break;
			}
		}
		if(flag == 0) {
			cout << kill << " could not be found.\n";
		}
	}
}

void doggabase::find_match() {
	bool flag = 0;
	char hair;
	double weight_lower, weight_upper;

	cout << "Answer the following questions to find your perfect dog."
		 << "What hair length do you prefer? Enter short/medium/long as s/m/l: ";
	while(!flag) {
		cin >> hair;
		flag = check_hair_input(hair);
	}

	cout << "Enter minimum preferred weight (lbs): ";
	cin >> weight_lower;
	cout << "Enter maximum preferred weight (lbs): ";
	cin >> weight_upper;

	cout << "Answer yes or no to the following preferences: \n"
		 << "Good guard dog: ";
	flag = 0;
	while(!flag) {
		cin >> guard;
		flag = check_binary_input(guard);
	}
	cout << "Good with kids: ";
	flag = 0;
	while(!flag) {
		cin >> kids;
		flag = check_binary_input(kids);
	}
	cout << "Easy to train: ";
	flag = 0;
	while(!flag) {
		cin >> train;
		flag = check_binary_input(train);
	}
}

void doggabase::search_trait() {
	int search;
	bool flag = 0;
	cout << "Search by:\n"
		 << "(1) Weight 		(2) Hair length\n"
		 << "(3) Guard skills 	(4) Good with kids\n"
		 << "(5) Easy to train\n"
		 << "Please enter the number corresponding to the search you"
		 << "would like to perform: ";
	
	while(!flag) {
		cin >> search;
		if(search > 5 || search < 1) {
			cout << "Please enter a valid number: ";
		}
		else { flag = 1; }
	}
	search_trait_helper(search);

}

void search_trait_helper(int s) {

	switch(s) {
		case 1: {
			
			break;
		}
		case 2: {
			break;
		}
		case 3: {
			break;
		}
		case 4: {
			break;
		}
		case 5: {

			break;
		}
	}
}

/*bool doggabase::name_comparator(const dog& a, const dog& b) {
	return a.name < b.name;
}*/


void doggabase::browse() {
	string response;
	bool flag = 0;
	cout << "Would you like to view the catalog in alphabetical or original order? ";
	while(!flag) {
		cin >> response;
		if(response == "alphabetical") {
			name_comp cmp;
			sort(catalog.begin(), catalog.end(), cmp);
			flag = 1;
		}
		else if(response == "original") {
			flag = 1;
		}
		else {
			cout << "Please enter 'alphabetical' or 'original': ";
		}
	}
	if(!flag) {
		if(catalog.size() > 100) {
			string response;
			cout << "Are you sure you want to do this? There are "
				 << catalog.size() << " entries. (yes/no): ";
			cin >> response;
			lowercase(response);
			if(to_bool(response)) { 
				for(auto& i: catalog) {
					print_dog(i);
				}
			}
		}//if
		else {
			for(auto& i: catalog) {
				print_dog(i);
			}
		}
	}
}
void doggabase::print_dog(dog& d) {
		cout << endl << d.name
			 << "\nAve. Weight: " << d.weight << " lbs"
			 << "\nHair Length: ";
		if(d.hair == 's') { cout << "short"; }
		else if(d.hair == 'm') { cout << "medium"; }
		else if(d.hair == 'l') { cout << "long"; }
		cout << "\nGood guard dog: ";
		from_bool(d.guard);
		cout << "\nGood with kids: ";
		from_bool(d.kids);
		cout << "\nEasy to train: ";
		from_bool(d.train);
		cout << endl;
}

void from_bool(bool b) {
	if(b) {	cout << "yes"; }
	else { cout << "no"; }
}

bool check_hair_input(char h) {
	if(h != 's' && h != 'm' && h != 'l') {
		cout << "Incorrect input. Please enter s/m/l: ";
		return 0;
	}
	return 1;
}

void doggabase::run() {

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

		switch(selection) {
			case 1: search_trait();
				break;
			case 2: find_match();
				break;
			case 3: add_dog();
				break;
			case 4: delete_dog();
				break;
			case 5: browse();
				break;
			case 0: 
				break;
			default: cout << "Invalid command. Please enter a number between 0 - 5: ";
		}//switch
	}//while
}//run

/////////DOGGABASE HELPER FUNCS///////

bool check_binary_input(string& in, bool& var) {
	lowercase(in);
	if(to_bool(in) == 1) { 
		var = 1;
		return 1; }
	else if(to_bool(in) == 0) { 
		var = 0;
		return 1; }
	else {
		cout << "Incorrect input. Please enter yes/no: ";
		return 0;
	}
}


int to_bool(const string& response) {
	if(response == "yes") { return 1; }
	else if(response == "no") { return 0; }
	else { return -1; }
}

//////////////PUT IN MAIN FILE /////////////////


//Main driver
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



///////////////////HELPER FUNCTIONS////////////////////////////
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
    	return tolower(c); });
}
