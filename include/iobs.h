
#ifndef IOBS_H
#define IOBS_H

#include <filesystem>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <ios>


namespace{ namespace fs = std::filesystem; }

namespace w_type { enum wt_enum {a, o}; }

struct PathLocation {
	std::unordered_map<std::string, std::vector<std::streamoff>> srp_db_offsets;
};

class WriteBin {
private:
	fs::path wb_file;
	std::ios::openmode mode;
	
public:
	WriteBin(fs::path wbf, enum w_type::wt_enum ew_type);
	
	bool c_c_dtob(const std::unordered_map<double, PathLocation>  &cc_data);
};

class ReadBin {
private:
	fs::path ccb_file;
	std::ios::openmode mode = std::ios::in | std::ios::binary;
public:
	ReadBin(fs::path rbf);

	std::unordered_map<double, PathLocation> btoc_cd();
};

#endif
