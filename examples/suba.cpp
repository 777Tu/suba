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
	cout<<t<<b.getpath("inside_pro1");
	cout<<t<<b.getpath("inside_pro2");
	cout<<t<<b.getpath("inside_pro3")<<n;
	
	/*
	for(auto [k, v] : b.gethi()){
	    cout<<t<<k<<v[0];
	}
	for(auto entry : b.getrp()){
	    cout<<t<<entry;
	}
	*/

	return 0;
}

