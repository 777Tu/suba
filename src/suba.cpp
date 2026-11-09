
#include <iostream>
#include <string>
#include <utility>

#include "../include/fchash.h"
#include "../include/suba.h"


suba::suba(std::filesystem::path u_p) {
	if(std::filesystem::exists(u_p) && isdir(u_p)) {
		this->u_path=std::move(u_p);
		this->r_path = std::filesystem::current_path();
	} else {exit(1);}
}

suba::suba(){this->r_path = std::filesystem::current_path();}

bool suba::isdir(std::filesystem::path p) {
	return std::filesystem::directory_entry(p).is_directory();
}

std::vector<std::filesystem::directory_entry> suba::getrp() {
	return this->r_paths;
}

std::unordered_map<double,std::vector<unsigned int>> suba::gethi(){
	return this->sh_rpidx;
}

bool suba::findp(double &hsp){
    auto sh_idx = this->sh_rpidx.find(hsp);
    if(sh_idx == this->sh_rpidx.end()){return false;}
    return true;
}

void suba::scan(enum exp ep) {
	switch(ep) {
	case Root:
		scanp(this->r_path);
		break;
	case Dir:
		scanp(this->u_path);
		break;
	default:
		exit(1);
	}
}

void suba::scanp(std::filesystem::path &ep) {
	unsigned int rpidx = 0;
	for(auto &entry : std::filesystem::recursive_directory_iterator(ep)) {
	    if(!entry.is_directory()){continue;}

	    std::string fn = entry.path().filename().string();
	    this->r_paths.push_back(entry);
	    double cfh = fchash(fn, 5);
	    if(findp(cfh)){
	        std::vector<unsigned int> rpidxs;
	        for(int idx = 0; idx < (this->sh_rpidx[cfh].size()) ; idx++){
	            rpidxs.push_back(this->sh_rpidx.at(cfh)[idx]);
	        }
	        rpidxs.push_back(rpidx);
	        this->sh_rpidx.at(cfh)=rpidxs; 	         
	    }else{this->sh_rpidx.insert({cfh, {rpidx}});}
	    rpidx++;	
	}
}

std::vector<std::filesystem::directory_entry> suba::getpath(std::filesystem::path sp) {return getp(sp);}

std::vector<std::filesystem::directory_entry> suba::getp(std::filesystem::path sp) {
    std::vector<std::filesystem::directory_entry> collsugg;
	double hsp = fchash(sp.string(), 5);
	if(!findp(hsp)){exit(1);}
	auto sh_pidx = this->sh_rpidx.at(hsp);
	if(sh_pidx.size() ==1){
	    unsigned int pidx = sh_pidx[0];
		collsugg.push_back(this->r_paths.at(pidx));
		return collsugg;
	}
    for(auto pidx : sh_pidx){
	    collsugg.push_back(this->r_paths.at(pidx));
	}
	return collsugg;
}
