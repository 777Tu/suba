
#ifndef SUBA_H
#define SUBA_H

#include <filesystem>
#include <unordered_map>
#include <vector>


enum exp {
	Root, Dir
};

class suba {
private:
	std::filesystem::path r_path;
	std::filesystem::path u_path;
	std::vector<std::filesystem::directory_entry> r_paths;
	std::unordered_map<double, std::vector<unsigned int>> sh_rpidx;

public:
	suba(std::filesystem::path u_p);

	suba();
	bool isdir(std::filesystem::path p);

	void scan(enum exp ep);

	std::filesystem::directory_entry getpath(std::filesystem::path sp);

	std::vector<std::filesystem::directory_entry> getrp();
	std::unordered_map<double,std::vector<unsigned int>> gethi();

private:
	void scanp(std::filesystem::path &ep);

	std::filesystem::directory_entry getp(std::filesystem::path sp);
};

#endif
