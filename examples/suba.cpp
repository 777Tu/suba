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

	string v="inside_pro1";
	if(!b.getpath(v).empty()){
	    cout<<t<<b.getpath(v)[0];
	};
	
	v = "2w8/inside_pro3";	
	if(!b.getpath(v).empty()){
		cout<<t<<b.getpath(v)[0];
	}
	v="/inside_pro2/";
	if(!b.getpath(v).empty()){
	    cout<<t<<b.getpath(v)[0];
	}
	
	v="project2/inside_pro2";
	if(!b.getpath(v).empty()){
	    cout<<t<<b.getpath(v)[0];

	}
	v = "same_one";
	if(!b.getpath(v).empty()){
	    for(auto r : b.getpath(v)){
	      cout<<t<<"Collision Suggestion: "<<r;
    	}
	}
	cout<<n;
	
	return 0;
}

