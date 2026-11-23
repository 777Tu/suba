#include <iostream>
#include <filesystem>
#include <vector>
#include <string>

#include "../include/suba.h"
using namespace std;

namespace {
string t = "\n\t";
string n = "\n";
}
namespace fs = std::filesystem;

int main() {

	fs::path p= "project";
	suba b(p);

	/*
	
	b.scan(Dir);	
	int i = 1;
	unordered_map<double, PathLocation> sh_locate = b.getshl();

	cout<<t<<"["<<sh_locate.size()
	<<"] size of <double, PathLocation>";
	for(auto &[folder, location] : sh_locate){
	    cout<<t<<i<<"="<<folder<<"\t ["<<location.srp_db_offsets.size()<<"] sa_Files:\n\t\t";
		for(auto &[sa_file, vec_offsets] : location.srp_db_offsets){
		    cout<<"\n\t\t: "<<sa_file;
		    for(auto offset : vec_offsets){
		        cout<<": _"<<offset;
		    }
		}
		i++;  
	}

	*/

	
	cout<<t<<b.getpath("inside_pro1")[0];
	cout<<t<<b.getpath("2w8/inside_pro3")[0];
	cout<<t<<b.getpath("/inside_pro3/")[0];
	cout<<t<<b.getpath("project2/inside_pro2")[0];
	cout<<t<<b.getpath("same_one")[0];

	for(auto r : b.getpath("same_one")){
	    cout<<t<<"Collision Suggestion: "<<r;
	}
	
	cout<<n;
	
	return 0;
}

