
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <sstream>
#include <utility>

#include "../include/fchash.h"
#include "../include/iobs.h"
#include "../include/suba.h"


suba::suba(std::filesystem::path u_p , std::filesystem::path sabf) {
	if(!(std::filesystem::exists(u_p) && isdir(u_p))){
		throw std::runtime_error("suba: Path doesn't exists.");
	}
	this->u_path=std::move(u_p);
	this->r_path = std::filesystem::current_path();
	rbinf(std::move(sabf));
}

suba::suba(){
	this->sab_file = "suba_fl.bin";
	this->r_path = std::filesystem::current_path();
}

bool suba::isdir(std::filesystem::path &p) {
	return std::filesystem::directory_entry(p).is_directory();
}

std::unordered_map<double, PathLocation> suba::getshl(){
	return this->sh_location;
}

bool suba::findp(double &hsp){
    auto sh_idx = this->sh_location.find(hsp);
    return (sh_idx == this->sh_location.end())? false : true;
}

bool suba::sppath(std::filesystem::path &spp){
    return (spp.parent_path().empty());
}

bool suba::findf(std::string &f, std::unordered_map<std::string, std::vector<std::streamoff>> &f_s){
	    auto fn_so = f_s.find(f);
	    return (fn_so==f_s.end())? false : true;
}
	
void suba::scan(enum exp ep) {
	switch(ep) {
	case Root : scanp(this->r_path); break;
	case Dir : scanp(this->u_path); break;
	default : exit(1);
	}
}


void suba::scanp(std::filesystem::path &ep) {
	std::string gsa_file =  fngen();
	std::ofstream wsa_file(gsa_file);

	if(!wsa_file.is_open()) {
		throw std::runtime_error("suba: File couldn't open.");
	}

	PathLocation p_location;
	std::unordered_map<std::string, std::vector<std::streamoff>> srp_db_offsets;
	std::vector<std::streamoff> rp_offsets;
	std::streamoff offset;

	for (auto &entry : std::filesystem::recursive_directory_iterator(ep)) {
		if (!entry.is_directory()) {continue;}

		std::string f_name = entry.path().filename().string();

		if(wsa_file.tellp()>=MAXO) {
			wsa_file.close();
			gsa_file=fngen();
			wsa_file.open(gsa_file, std::ios::out | std::ios::app);
			if(!(wsa_file.is_open())) {
				throw std::runtime_error("suba: File couldn't open");
			}
		}
		//std::filesystem::directory_entry rentry(entry.path()/*.parent_path()*/);
		double cfh = fchash(f_name, 5);
		offset = wsa_file.tellp();
		wsa_file<<entry<<std::endl;

		if(!(findp(cfh) ) ) {
			rp_offsets= {offset};
			srp_db_offsets.insert({gsa_file, std::move(rp_offsets)});
			p_location.srp_db_offsets = std::move(srp_db_offsets);
			this->sh_location.insert({cfh, std::move(p_location)});
			continue;
		}

		PathLocation p_locate= this->sh_location.at(cfh);
		if(!(findf(gsa_file,p_locate.srp_db_offsets))) {
			rp_offsets= {offset};
			p_locate.srp_db_offsets.insert({gsa_file, std::move(rp_offsets)});
			this->sh_location.at(cfh) =std::move(p_locate);
			continue;
		}

		rp_offsets=p_locate.srp_db_offsets.at(gsa_file);
		rp_offsets.push_back(offset);
		p_locate.srp_db_offsets.at(gsa_file)=rp_offsets;
		this->sh_location.at(cfh)=std::move(p_locate);
	}
	wsa_file.close();
	wbinf(this->sab_file);
}

std::vector<std::filesystem::directory_entry> suba::getpath(std::filesystem::path &&sp) {return getp(sp);}

std::vector<std::filesystem::directory_entry> suba::getp(std::filesystem::path &sp) {

    std::vector<std::filesystem::directory_entry> collsugg;
    /*
	double hsp = fchash(sp.string(), 5);
	bool f = true;
	       /
	       sp - folder
	       sp - coll
	       sp - path/
	       /
	if(!findp(hsp)) {
	    if(sppath(sp)){/cout<<"folder not found";/exit(1);}
	    hsp = fchash(sp.filename().string(), 5);
	    f = false;
	}
	if(!f){
	    if(!findp(hsp)){/cout<<"folder not found";/exit(1);}
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
    */
    return collsugg;
    
}

std::string suba::fngen() {
    auto now = std::chrono::system_clock::now();
    std::time_t timestamp = std::chrono::system_clock::to_time_t(now);
    std::tm datetime = *std::localtime(&timestamp);
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(
        now.time_since_epoch()
    ).count() % 1000000;

    char buf[50];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", &datetime);

    std::ostringstream oss;
    oss << buf << "-" << std::setw(6) << std::setfill('0') << us << ".sa";
    return oss.str();
}

void suba::rbinf(std::filesystem::path sabf) {
	if((std::filesystem::exists(sabf))) {
		ReadBin ccd((sabf.string()));
		this->sh_location =ccd.btoc_cd();
		this->dfh_exists = true;
	}
	this->sab_file = sabf;
}

bool suba::wbinf(std::filesystem::path b){
	WriteBin wbf(b, w_type::wt_enum::o);
	return (wbf.c_c_dtob(std::move(this->sh_location))==true) ? true : false;	        	    
}
