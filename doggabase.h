#ifndef DOGGABASE_H
#define DOGGABASE_H

#include <array>
#include <string>
#include <unordered_map>
#include <vector>

class doggabase {
private:
	//Dog member class
	class dog {
	public:
		int id;
		char hair;
		bool guard;
		bool kids;
		bool train;
		double weight;
		std::string breed;

		dog(int id_in, char hair_in, bool guard_in, bool kids_in, 
			bool train_in, double weight_in, std::string breed_in)
			: id(id_in), hair(hair_in), guard(guard_in), kids(kids_in)
			, train(train_in), weight(weight_in), breed(breed_in) {}
	};
	// breed comparator
	struct breed_comp {
		bool operator()(const dog& a, const dog& b) {
			return a.breed < b.breed;
		}
	};
	//ID comparator
	struct id_comp {
		bool operator()(const dog& a, const dog& b) {
			return a.id < b.id;
		}
	};
	//Weight comparator
	struct weight_comp {
		bool operator()(const dog& a, const dog& b) {
			return a.weight < b.weight;
		}
	};

	//Private member functions
	void add_dog(); 
	void add_to_keymap(const std::string& s, int idx);
	void browse();
	void delete_dog();
	void find_match();
	void print_by_id(std::vector<int>& print);
	void print_dog(dog& d);
	void read_data(const std::string& input_file);
	void search_guard(std::string& response, std::vector<int>*& v_out);
	void search_hair(std::string& hair, std::vector<int>*& v_out);
	void search_keyword(std::string& s, std::vector<int>*& v_out);
	void search_kids(std::string& response, std::vector<int>*& v_out);
	void search_train(std::string& response, std::vector<int>*& v_out);
	void search_trait();
	void search_trait_promt(int s, std::vector<int>*& v_out);
	void search_weight(double lower, double upper, std::vector<int>*& v_out);
	void update_dog(dog& d);

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
	}
	//Public member functions
	void run();
};

#endif //DOGGABASE_H