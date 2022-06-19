#ifndef EXEC_HPP
#define EXEC_HPP

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

using namespace std;

string exec(const string cmd) {
	const char* ccmd = cmd.c_str();
	array<char, 128> buf;
	string res;
	unique_ptr<FILE, decltype(&pclose)> pipe(popen(ccmd, "r"), pclose);
	if (!pipe) {
		throw runtime_error("popen failed");
	}
	while (fgets(buf.data(), buf.size(), pipe.get()) != nullptr) {
		res += buf.data();
	}
	return res;
}

#endif
