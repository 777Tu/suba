

#include <iostream>
#include <string>
#include <utility>

#include "../include/fchash.h"
#include "../include/suba.h"


suba::suba(std::filesystem::path u_p) {
	if(std::filesystem::exists(u_p) && isdir(u_p)) {
		this->u_path=std::move(u_p);
		this->r_path = std::filesystem::current_path();


	} else {
		exit(1);
	}
}

suba::suba() {
	this->r_path = std::filesystem::current_path();
}
bool suba::isdir(std::filesystem::path p) {
	return std::filesystem::directory_entry(p).is_directory();
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

std::filesystem::directory_entry suba::getpath(std::filesystem::path sp) {
	return getp(sp);
}

std::vector<std::filesystem::directory_entry> suba::getrp() {
	return this->r_paths;
}
std::unordered_map<double,std::vector<unsigned int>> suba::gethi() {
	return this->sh_rpidx;
}


void suba::scanp(std::filesystem::path &ep) {
	unsigned int rpidx = 0;
	for(auto &entry : std::filesystem::recursive_directory_iterator(ep)) {

		std::string fn = entry.path().filename().string();
		this->r_paths.push_back(entry);
		this->sh_rpidx.insert({fchash(fn, 5), {rpidx}});
		rpidx++;

	}

}

std::filesystem::directory_entry suba::getp(std::filesystem::path sp) {

	double hsp = fchash(sp.string(), 5);
	auto sh_idx = this->sh_rpidx.find(hsp);
	if(sh_idx == this->sh_rpidx.end()) {
		exit(1);
	}

	unsigned int pidx = sh_idx->second[0];
	return this->r_paths.at(pidx);

}
