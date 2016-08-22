#include "doggabase.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

//Helper function declarations
bool check_binary_input(string& in, bool& var);	
bool check_hair_input(string& in);
void from_bool(bool b);
void lowercase(string& s);
int to_bool(const string& response);
void welcome();

////////////////////////////
///// MEMBER FUNCTIONS /////
////////////////////////////

void doggabase::add_dog() {
	string response = "yes";

	while(response == "yes" || response == "y") {
		char hair_sym;
		bool flag = 0;
		bool guard, kids, train;
		double weight;
		string name, hair, string_weight, trash;
		cout << "Please enter in the following data "
			 << "or '0' if you wish to cancel.\n"
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
			 << "Enter short, medium, or long: ";
		flag = 0;
		cin >> hair;
		while (!flag) {
			flag = check_hair_input(hair);
		}
		if(hair == "short") {
			hair_sym = 's';
		}
		else if(hair == "medium") {
			hair_sym = 'm';
		}
		else {
			hair_sym = 'l';
		}

		//Guard
		cout << "Is this dog an exceptionally good guard dog? (yes/no): ";
		flag = 0;
		cin >> response;
		while(!flag) {
			flag = check_binary_input(response, guard);
		}

		//Kids
		cout << "Is this dog exceptionally good with kids? (yes/no): ";
		flag = 0;
		cin >> response;
		while(!flag) {
			flag = check_binary_input(response, kids);
		}

		//Train
		cout << "Is this dog exceptionally easy to train? (yes/no): ";
		flag = 0;
		cin >> response;
		while(!flag) {
			flag = check_binary_input(response, train);
		}

		catalog.push_back(dog(int(catalog.size()), hair_sym, guard, kids
			, train, weight, name));
		update_dog(catalog.back());

		cout << "Would you like to add another dog? ";
		cin >> response;
		lowercase(response);
	}//while
}//add_dog

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

void doggabase::browse() {
	string response;
	bool flag = 0;
	cout << "Would you like to view the catalog"
		 << " in alphabetical or original order? ";
	while(!flag) {
		cin >> response;
		if(response == "alphabetical") {
			name_comp n_cmp;
			sort(catalog.begin(), catalog.end(), n_cmp);
			flag = 1;
		}
		else if(response == "original") {
			id_comp i_cmp;
			sort(catalog.begin(), catalog.end(), i_cmp);
			flag = 1;
		}
		else {
			cout << "Please enter 'alphabetical' or 'original': ";
		}
	}
	if(catalog.size() > 60) {
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
	string hair, response;
	double weight_lower, weight_upper;
	vector<int> v, results;
	vector<int>* collector = &v;

	//Hair
	cout << "Answer the following questions to find your perfect dog."
		 << "\nWhat hair length do you prefer? Short, medium, or long? ";
	cin >> hair;
	search_hair(hair, collector);
	results.assign(collector->begin(), collector->end());

	//Weight
	cout << "Enter minimum preferred weight (lbs): ";
	cin >> weight_lower;
	cout << "Enter maximum preferred weight (lbs): ";
	cin >> weight_upper;

	search_weight(weight_lower, weight_upper, collector);
	sort(collector->begin(), collector->end());
	auto it = set_intersection(results.begin(), results.end()
		, collector->begin(), collector->end(), results.begin());
	results.resize(it - results.begin());

	//Guard
	cout << "Answer yes or no to the following preferences: \n"
		 << "Good guard dog: ";
	cin >> response;
	search_guard(response, collector);
	it = set_intersection(results.begin(), results.end()
		, collector->begin(), collector->end(), results.begin());
	results.resize(it - results.begin());

	//Kids
	cout << "Good with kids: ";
	cin >> response;
	search_kids(response, collector);
	it = set_intersection(results.begin(), results.end()
		, collector->begin(), collector->end(), results.begin());
	results.resize(it - results.begin());

	//Train
	cout << "Easy to train: ";
	cin >> response;
	search_train(response, collector);
	it = set_intersection(results.begin(), results.end()
		, collector->begin(), collector->end(), results.begin());
	results.resize(it - results.begin());

	print_by_id(results);
}//find_match

void doggabase::print_by_id(vector<int>& print) {
	id_comp i_cmp;
	sort(catalog.begin(), catalog.end(), i_cmp);

	cout << '\n' << print.size() << " dog(s) found:\n";
	for(unsigned i = 0; i < print.size(); ++i) {
		dog& d = catalog[print[i]];
		print_dog(d);
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
}//read_data

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
			 << "Please enter the number corresponding"
			 << " to the desired operation: ";
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
			default: cout << "Invalid command.\n"
						  << "Please enter a number between 0 - 5: ";
		}//switch
	}//while
}//run

void doggabase::search_guard(string& response, vector<int>*& v_out) {
	bool guard;
	bool flag = 0;

	while(!flag) {
		flag = check_binary_input(response, guard);
	}
	if(guard == 0) {
		v_out = &guard_list[0];
	}
	else {
		v_out = &guard_list[1];
	}
}

void doggabase::search_hair(string& hair, vector<int>*& v_out) {
	bool flag = 0;
	while(!flag) {
		flag = check_hair_input(hair);
	}
	if(hair == "short") {
		v_out = &hair_list[0];
	}
	else if(hair == "medium") {
		v_out = &hair_list[1];
	}
	else {
		v_out = &hair_list[2];
	}
}

void doggabase::search_keyword(string& s, vector<int>*& v_out) {
	bool first = 0;
	string word;
	lowercase(s);
	stringstream ss(s);

	while(ss >> word) {
		if(keyword_map.find(word) != keyword_map.end()) {
			vector<int>& list = keyword_map[word];
			if(!first) {
				v_out->assign(list.begin(), list.end());
				first = 1;
			}
			else {
				auto it = set_intersection(v_out->begin(), v_out->end()
					, list.begin(), list.end(), v_out->begin());
				v_out->resize(it - v_out->begin());
				if(v_out->empty()) { break; }
			}
		}//if
	}//while
}//search_keyword

void doggabase::search_kids(string& response, vector<int>*& v_out) {
	bool kids;
	bool flag = 0;

	while(!flag) {
		flag = check_binary_input(response, kids);	
	}
	if(kids == 0) {
		v_out = &kids_list[0];
	}
	else {
		v_out = &kids_list[1];
	}
}

void doggabase::search_train(string& response, vector<int>*& v_out) {
	bool train;
	bool flag = 0;

	while(!flag) {
		flag = check_binary_input(response, train);
	}
	if(train == 0) {
		v_out = &train_list[0];
	}
	else {
		v_out = &train_list[1];
	}
}

void doggabase::search_trait() {
	int search;
	bool flag = 0;
	vector<int> v;
	vector<int>* results = &v;

	cout << "\nSearch by:\n"
		 << "(1) Weight 		(2) Hair length\n"
		 << "(3) Guard skills 	(4) Good with kids\n"
		 << "(5) Easy to train  	(6) Breed\n"
		 << "(0) Quit\n"
		 << "Please enter the number corresponding to the search\n"
		 << "you would like to perform: ";
	
	while(!flag) {
		cin >> search;
		if(search > 6 || search < 0) {
			cout << "Please enter a valid number: ";
		}
		else { flag = 1; }
	}
	if(search != 0) {
		search_trait_promt(search, results);
		print_by_id(*results);
	}
}//search_trait

void doggabase::search_trait_promt(int s, vector<int>*& v_out) {
	switch(s) {
		case 1: {
			double lower, upper;
			cout << "Please enter a minimum weight (lbs): ";
			cin >> lower;
			cout << "Please enter a maximum weight (lbs): ";
			cin >> upper;
			search_weight(lower, upper, v_out);
			break;
		}
		case 2: {
			string hair;
			cout << "What type of hair length are you searching for?\n"
				 << "Short, medium, or long? ";
			cin >> hair;
			search_hair(hair, v_out);
			break;
		}
		case 3: {
			string response;
			cout << "Are you searching for a dog with strong guard skills?" 
				 << "\n(yes/no): ";
			cin >> response;
			search_guard(response, v_out);
			break;
		}
		case 4: {
			string response;
			cout << "Are you searching for a dog compatible with kids?"
				 << "\n(yes/no): ";
			cin >> response;
			search_kids(response, v_out);
			break;
		}
		case 5: {
			string response;
			cout << "Are you searching for a dog with strong training skills?"
				 << "\n(yes/no): ";
			cin >> response;
			search_train(response, v_out);
			break;
		}
		case 6: {
			string response, trash;
			cout << "Enter keywords to describe the breed of the dog: ";
			getline(cin, trash);
			getline(cin, response);
			search_keyword(response, v_out);
			break;
		}
	}//switch
}//search_trait_prompt

void doggabase::search_weight(double lower, double upper, vector<int>*& v_out) {
	weight_comp w_cmp;
	dog lower_dog = dog(0, '0', 0, 0, 0, lower, "0");
	dog upper_dog = dog(0, '0', 0, 0, 0, upper, "0");
	sort(catalog.begin(), catalog.end(), w_cmp);
	v_out->resize(catalog.size());
	auto out_it = v_out->begin();

	auto dog_start = lower_bound(catalog.begin(), catalog.end()
		, lower_dog, w_cmp);
	auto dog_end = upper_bound(catalog.begin(), catalog.end()
		, upper_dog, w_cmp);
	for(auto dog_it = dog_start; dog_it < dog_end; ++dog_it, ++out_it) {
		*out_it = dog_it->id;
	}
	v_out->resize(out_it - v_out->begin());
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

////////////////////////////
///// HELPER FUNCTIONS /////
////////////////////////////

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
		cin >> in;
		return 0;
	}
}

bool check_hair_input(string& h) {
	lowercase(h);
	if(h != "short" && h != "medium" && h != "long") {
		cout << "Incorrect input. Please enter short, medium or long: ";
		cin >> h;
		return 0;
	}
	return 1;
}

void from_bool(bool b) {
	if(b) {	cout << "yes"; }
	else { cout << "no"; }
}

//Converts string to lowercase
void lowercase(string& s) {
	transform(s.begin(), s.end(), s.begin(), [](char c) { 
    	return tolower(c); });
}

int to_bool(const string& response) {
	if(response == "yes") { return 1; }
	else if(response == "no") { return 0; }
	else { return -1; }
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

