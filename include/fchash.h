
#ifndef FCHASH_H
#define FCHASH_H

#include <cmath>
#include <string>

inline double fchash(const std::string &txt, const int &fras) {
	if (txt.size() == 0) return 0.0;
	unsigned int csum = 0;
	for (char c : txt) {
		int req = ((int)c >= 97)   ?
		          ((int)c - 96) : ((int)c >= 65) ?
		          ((int)c - 64) : (int)c;
		csum = (csum * 3) + req;
	}
	return (csum / (std::pow(10, fras)));
}

#endif
