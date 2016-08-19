#ifndef DOGGABASE_H
#define DOGGABASE_H

#include <array>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <iostream>

//RUN WITH g++ -std=c++14 -Wconversion -Wall -Werror -Wextra -pedantic doggabase.cpp -o dog

class doggabase {
private:


	//Dog member class
	class dog {
	public:
		int id;
		char hair;
		bool guard; //good guard dog
		bool kids; //exceptionally good with children
		bool train; //exceptionally easy to train
		double weight;
		std::string name;

		dog(int id_in, char hair_in, bool guard_in, bool kids_in, 
			bool train_in, double weight_in, std::string name_in)
			: id(id_in), hair(hair_in), guard(guard_in), kids(kids_in)
			, train(train_in), weight(weight_in), name(name_in) {}
	};

	struct name_comp {
		bool operator()(const dog& a, const dog& b) {
			return a.name < b.name;
		}
	};

	//Member functions
	void add_dog(); 
	void add_to_keymap(const std::string& s, int idx); //X
	void browse(); //alphab or original as input
	void delete_dog();
	void find_match();
	void print_dog(dog& d);
	void read_data(const std::string& input_file); //X
	//void run(const std::string& data_file);
	void search_trait(); //int option, depending on opt, further cin
	void update_dog(dog& d); //X
	//bool name_comparator(const dog& a, const dog& b);

	//Containers
	std::vector<dog> catalog;
	std::array<std::vector<int>, 2> guard_list;
	std::array<std::vector<int>, 2> kids_list;
	std::array<std::vector<int>, 2> train_list;
	std::array<std::vector<int>, 3> hair_list;
	std::unordered_map<std::string, std::vector<int> > keyword_map;

public:
	//Database constructor
	doggabase(const std::string& input_file) {
		read_data(input_file);
		//init_maps();
	}
	void run();

};




#endif //DOGGABASE_H