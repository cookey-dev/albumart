#ifndef FETCH_HPP
#define FETCH_HPP

#include <string>
#include <unistd.h>
#include <random>
#include <iostream>
#include <stdio.h>
#include <curl/curl.h>
#include "exec.hpp"

using namespace std;

bool fileExists(string path) {
	return (access(path.c_str(), F_OK) == 0);
}

string randomString(int len) {
	static const char an[] =
	"0123456789"
	"ABCDEFGHIJKLMNOPQRSTUVQXYZ"
	"abcdefghijklmnopqrstuvwxyz";

	mt19937 rg{random_device{}()};
	uniform_int_distribution<string::size_type> pick(0, sizeof(an) - 2);

	string res;
	res.reserve(len);
	while (len--) res += an[pick(rg)];
	return res;
}

string randomFile() {
	string res;

	do {
		string s = "/tmp/" + randomString(6);
		res = s;
	} while (fileExists(res));

	return res;
}

string fetch(string url) {
	string path = randomFile();

	string cmd = "curl " + url + " -o " + path;

	exec(cmd);

	return path;
}

#endif
