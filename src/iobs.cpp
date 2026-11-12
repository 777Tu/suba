
#include <iostream>
#include <string>
#include <utility>

#include "../include/iobs.h"


WriteBin::WriteBin(fs::path wbf, enum w_type::wt_enum ew_type) : wb_file(std::move(wbf)) {
	if(ew_type==w_type::a) {
		mode = std::ios::app | std::ios::binary;
	} else {
		mode = std::ios::trunc | std::ios::binary;
	}
}

bool WriteBin::c_c_dtob(const std::unordered_map<double, PathLocation>  &cc_data) {
	std::ofstream wb_file(this->wb_file, this->mode);

	if(!(wb_file.is_open())) {
		std::cerr << "File not open.";
		return false;
	}

	size_t d_fhk_vmsvso_size = cc_data.size();

	wb_file.write((char*)&d_fhk_vmsvso_size, sizeof(d_fhk_vmsvso_size));
	for(const auto &[df_hash,p_location] : cc_data) {
		wb_file.write((char*)&df_hash, sizeof(df_hash));

		size_t s_dbk_offsvso_size = p_location.srp_db_offsets.size();
		//Map<string, vec<streamoff>>
		wb_file.write((char*)&s_dbk_offsvso_size, sizeof(s_dbk_offsvso_size));
		for(const auto &[sdb_k, vso_v] : p_location.srp_db_offsets) {
			size_t sdbk_size = sdb_k.size();
			wb_file.write((char*)&sdbk_size, sizeof(sdbk_size));
			wb_file.write((char*)sdb_k.data(), sdbk_size);
			//size-key-Map-size, key-size-off
			size_t voff_size = vso_v.size();
			wb_file.write((char*)&voff_size, sizeof(voff_size));
			for(const auto &stroff : vso_v) {
				wb_file.write((char*)&stroff, sizeof(stroff));
			}
		}

	}
	wb_file.close();

	return true;
}

ReadBin::ReadBin(fs::path rbf) {
	if(fs::exists(rbf) && fs::is_regular_file(rbf)) {
		ccb_file= std::move(rbf);
	} else {
		throw std::runtime_error("Invalid file: does not exist or is not a regular file.");
	}
}

std::unordered_map<double, PathLocation> ReadBin::btoc_cd() {

	std::ifstream rb_file(this->ccb_file, this->mode);
	if(!(rb_file.is_open())) {
		throw std::runtime_error("File not open");
	}

	std::unordered_map<double, PathLocation> cc_data;

	size_t d_fhk_vmsvso_size = 0;
	rb_file.read((char*)&d_fhk_vmsvso_size, sizeof(d_fhk_vmsvso_size));

	/*
	 p-size<double, pl>
	        key-double
	        (pl.string-vec)c-size<string, vec>
	        key-string
	        vec-size
	        vec
	  */

	for(size_t d_fh_msize=0; d_fh_msize<d_fhk_vmsvso_size; d_fh_msize++) {
		double d_fhash_k;
		rb_file.read((char*)&d_fhash_k, sizeof(d_fhash_k));

		std::unordered_map<std::string, std::vector<std::streamoff>> srp_db_offsets;
		size_t s_dbk_offsvso_size = 0;
		//size db vec
		rb_file.read((char*)&s_dbk_offsvso_size, sizeof(s_dbk_offsvso_size));
		for(size_t s_db_size = 0; s_db_size<s_dbk_offsvso_size; s_db_size++) {
			std::string sdb_k;

			size_t sdbk_size =0;
			rb_file.read((char*)&sdbk_size, sizeof(sdbk_size));
			sdb_k.resize(sdbk_size);
			rb_file.read((char*)sdb_k.data(), sdbk_size);

			std::vector<std::streamoff> vso_v;
			size_t voff_size = 0;
			rb_file.read((char*)&voff_size, sizeof(voff_size));

			for(size_t stroff_size = 0; stroff_size<voff_size; stroff_size++) {
				std::streamoff offs;
				rb_file.read((char*)&offs, sizeof(offs));
				vso_v.push_back(offs);
			}
			srp_db_offsets[std::move(sdb_k)]=std::move(vso_v);
			//vso_v= {};
		}

		PathLocation p_location ={.srp_db_offsets=std::move(srp_db_offsets)};

		cc_data[std::move(d_fhash_k)]=std::move(p_location);
	}

	return cc_data;
}
