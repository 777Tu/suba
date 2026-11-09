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

	fs::path p= "project/";
	suba b(p);
	
	b.scan(Dir);
	
	cout<<t<<b.getpath("inside_pro1")[0];
	cout<<t<<b.getpath("inside_pro2")[0];
	cout<<t<<b.getpath("inside_pro3")[0];

	for(auto r : b.getpath("same_one")){
	    cout<<t<<"Collision Suggestion: "<<r;
	}



	/*
	for(auto [k, v] : b.gethi()){
	    if(v.size()==1){cout<<t<<k<<" = "<<v[0];continue;}
	    for(auto vi : v){
	        cout<<t<<k<<" = "<<vi<<",";
	    }
	}
	for(auto entry : b.getrp()){
	    cout<<t<<entry;
	}
	*/

	cout<<n;

	return 0;
}

