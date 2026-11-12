


#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <unordered_map>

#include "../include/iobs.h"


using namespace std;
namespace {namespace fs = std::filesystem;}

string t="\n\t";

int main() {

	fs::path fn = "data.bin";

	WriteBin wrb(fn, w_type::o);


	unordered_map<double, PathLocation>  cc_data;

	unordered_map<std::string, std::vector<std::streamoff>> db_off;

	streamoff x = 10;
	streamoff y = 20;
	streamoff z = 30;
	vector<std::streamoff> vec= {x, y};

	db_off["db1"]=vec;
	vec.push_back(z);
	db_off["db2"]=vec;
	db_off["db3"]= {28};

	PathLocation p_l= {.srp_db_offsets=db_off};
	double d = 100.0, d1 = 200.0;
	cc_data[d] =p_l;
	cc_data[d1]=p_l;
	wrb.c_c_dtob(cc_data);

	
	ReadBin rb(fn);

	unordered_map<double, PathLocation> getback =rb.btoc_cd();

	for(auto [d_k, p_l] : getback) {
		cout<<t<<d_k<<"=";
		for(auto [db, vo] : p_l.srp_db_offsets) {
			cout<<db<<"={";
			for(auto off : vo) {
				cout<<off<<",";
			}
			cout<<"}";
		}
		cout<<"\n";


	}
	



	return 0;

}
