
#ifndef SUBA_H
#define SUBA_H

#include <filesystem>
#include <ios>
#include <unordered_map>
#include <vector>


constexpr std::streamoff MAXO = 1 * 1024 * 1024;
enum exp {Root, Dir};

class suba {
private:
    bool dfh_exists = false;
	std::filesystem::path sab_file;
	std::filesystem::path r_path;
	std::filesystem::path u_path;
	std::vector<std::filesystem::directory_entry> r_paths;
	std::unordered_map<double, PathLocation> sh_location;

public:
	suba(std::filesystem::path u_p, std::filesystem::path sabf="suba_fl.bin");

	suba();
	
	void scan(enum exp ep);
	
	std::vector<std::filesystem::directory_entry> getpath(std::filesystem::path &&sp);
	
	std::unordered_map<double,PathLocation> getshl();

    void rbinf(std::filesystem::path sabf="suba_fl.bin");

    void fngen();

private:
	void scanp(std::filesystem::path &ep);

	std::vector<std::filesystem::directory_entry> getp(std::filesystem::path &sp);
	
	bool findp(double &hsp);
	
	bool sppath(std::filesystem::path &spp);
	
	bool isdir(std::filesystem::path &p);
   
    bool findf(std::string &f, std::unordered_map<std::string, std::vector<std::streamoff>> &f_s);

    bool wbinf(std::filesystem::path b);

};

#endif
