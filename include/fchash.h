
#ifndef FCHASH_H
#define FCHASH_H

#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>

inline double fchash(const std::string &txt, const int &fras) {
	if (txt.size() == 0) return 0.0;
	unsigned int csum = 0;
	for (char c : txt) {
		unsigned int req = ((int)c >= 97)   ?
		          ((int)c - 96) : ((int)c >= 65) ?
		          ((int)c - 64) : (int)c;
		csum = (csum * 3) + req;
	}
	std::cout<<std::fixed<<std::setprecision(fras);
	return (csum / (std::pow(10, fras)));
}

#endif
