#include <string>
#include <queue>
#include <map>
#include <set>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>

#include "inverter.h"

using namespace std;

void print_vector(std::vector<std::string> *v){
	std::cout << "Printing Vector" << endl;
	for (std::vector<std::string>::const_iterator i = v->begin(); i != v->end(); ++i)
    	std::cout << *i << endl;
    std::cout << "________" << endl;
}

std::string stringize_set(set<int> &theset){
	std::string outstr;

	for (std::set<int>::iterator it=theset.begin(); it != theset.end(); ++it){
		outstr += std::to_string(*it);
		if (it != std::prev(theset.end()))
			outstr += " ";
	}

	return outstr;
}

std::string print_map(std::map<std::string, set<int>> &map){
	std::string outstr;

	for (std::map<std::string, set<int>>::iterator it=map.begin(); it!=map.end(); ++it){
		std::cout << it->first << ": " << stringize_set(it->second) << std::endl;
		outstr += it->first + ": " + stringize_set(it->second) + "\n";
	}

	return outstr;
}

void parse_and_store(std::string input_str, int index, std::map<std::string, set<int>> &map){
	std::stringstream ss;
	ss.str(input_str);

	char readchar;
	std::string readword;

	while(ss.get(readchar)){
		if(!isalpha(readchar)){ //when nonalpha char is met
			if(readword.size() > 0) // insert to map at readword key and clear temp var if we read a word
			{
				map[readword].insert(index);
				readword.clear();
			}
			continue;
		}

		readword += readchar;
	}
}

string build_inverted_index(string filename){
	ifstream file;
    file.open(filename);

   	std::vector<std::string> v;

   	std::string readstr;

   	while(std::getline(file, readstr)){
   		v.push_back(readstr);
   	}

   	file.close();
   	file.clear();

   	std::vector<std::string> fileindex; //store index -> file_contents_string

   	for(uint i = 0; i < v.size(); i ++){
   		file.open(v[i]);

   		if(file.is_open()){
	   		std::string str;
	   		std::string file_contents;

			while (std::getline(file, str))
			{
			  file_contents += str;
			  file_contents.push_back('\n');
			}  

			fileindex.push_back(file_contents);
		}

		file.close();
		file.clear();
   	}

   	//print_vector(&fileindex);
   	
   	std::map<std::string, set<int>> map;

   	for(uint i = 0; i < fileindex.size(); i ++){
   		parse_and_store(fileindex[i], i, map);
   	}

   	return print_map(map);

   	//print_vector(&v);
}
