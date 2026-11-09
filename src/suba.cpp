
#include <iostream>
#include <string>
#include <utility>

#include "../include/fchash.h"
#include "../include/suba.h"


suba::suba(std::filesystem::path u_p) {
	if(!(std::filesystem::exists(u_p) && isdir(u_p))){exit(1);}
	this->u_path=std::move(u_p);
	this->r_path = std::filesystem::current_path();
}

suba::suba(){this->r_path = std::filesystem::current_path();}

bool suba::isdir(std::filesystem::path &p) {
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
    return (sh_idx == this->sh_rpidx.end())? false : true;
}

bool suba::sppath(std::filesystem::path &spp){
    return (spp.parent_path().empty());
}
	
void suba::scan(enum exp ep) {
	switch(ep) {
	case Root : scanp(this->r_path); break;
	case Dir : scanp(this->u_path); break;
	default : exit(1);
	}
}

void suba::scanp(std::filesystem::path &ep) {
	unsigned int rpidx = 0;
	for(auto &entry : std::filesystem::recursive_directory_iterator(ep)) {
	    if(!entry.is_directory()){continue;}

	    std::string fn = entry.path().filename().string();
	    std::filesystem::directory_entry rentry(entry.path().parent_path());
	    this->r_paths.push_back(rentry);
	    double cfh = fchash(fn, 5);
	    if(!(findp(cfh))){
	        this->sh_rpidx.insert({cfh, {rpidx}});
	        rpidx++; continue;
	    }
	    std::vector<unsigned int> rpidxs;
	    for(unsigned int idx = 0; idx < (this->sh_rpidx.at(cfh).size()) ; idx++){
	        rpidxs.push_back((this->sh_rpidx.at(cfh).at(idx)));
	    }
	    rpidxs.push_back(rpidx);
	    this->sh_rpidx.at(cfh)=rpidxs;
	           
	    rpidx++;	
	}
}

std::vector<std::filesystem::directory_entry> suba::getpath(std::filesystem::path &&sp) {return getp(sp);}

std::vector<std::filesystem::directory_entry> suba::getp(std::filesystem::path &sp) {
    std::vector<std::filesystem::directory_entry> collsugg;
	double hsp = fchash(sp.string(), 5);
	bool f = true;
	       /*
	       sp - folder
	       sp - coll
	       sp - path/
	       */
	if(!findp(hsp)) {
	    if(sppath(sp)){/*cout<<"folder not found";*/exit(1);}
	    hsp = fchash(sp.filename().string(), 5);
	    f = false;
	}
	if(!f){
	    if(!findp(hsp)){/*cout<<"folder not found";*/exit(1);}
	}

	auto sh_pidx = this->sh_rpidx.at(hsp);

	if (sh_pidx.size() == 1) {
	    std::string fn = (f) ? sp.string() : sp.filename().string();
	    unsigned int pidx = sh_pidx.at(0);
	    std::filesystem::directory_entry rentry((this->r_paths.at(pidx).path() / fn));
	    collsugg.push_back(rentry);
	    return collsugg;
    }

    for (auto pidxs : sh_pidx){
        std::filesystem::directory_entry rentry((this->r_paths.at(pidxs).path() / sp.filename()));
        if(!f){
            if((this->r_paths.at(pidxs).path().filename() == sp.parent_path().filename())) {
                rentry.assign((this->r_paths.at(pidxs).path() / sp.filename()));
                collsugg.push_back(rentry);
                return collsugg;
            }
            continue;
        }  
        collsugg.push_back(rentry);
    }
    return collsugg; 
}

