#ifndef ASCII_HPP
#define ASCII_HPP

#include <iostream>
#include <Magick++.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>
#include <array>
#include "fetch.hpp"
#include "exec.hpp"

using namespace std;
namespace IM = Magick;

// Resize file
IM::Image resize(string path) {
	// Get stdout rows/columns
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

	// Resize
	IM::Geometry new_size = IM::Geometry(size.ws_col, size.ws_row);
	new_size.aspect(true);
	IM::Image img;
	img.read(path);
	img.resize(new_size);
	return img;
}

array<int, 4> nl = {300, 300, 300, 300};

vector<array<int, 4>> _rgby(IM::Image img) {
	vector<array<int, 4>> res;
	try {
		// Rows/Columns
		int rows = img.rows();
		int col = img.columns();
		cout << "Width: " << col << endl << "Height: " << rows << endl;

		// Iterate over pixels
		for (int y = 0; y <= rows; y++) {
			for (int x = 0; x <= col; x++) {
				IM::Color px = img.pixelColor(x, y);
				string cl = px;
				int r, g, b;
				const char *hexColor = cl.c_str();
				sscanf(hexColor, "#%02x%02x%02x", &r, &g, &b);
				int l = 0.2126 * r + 0.7152 * g + 0.0722 * b;
				array<int, 4> rgbRes = {r, g, b, l};
				res.push_back(rgbRes);
			}
			res.push_back(nl);
		}
	} catch (IM::Exception &err) {
		cerr << "Error: " << err.what() << endl;
		exit(1);
	}
	
	return res;
}

string chars[10] = {" ", ".", ":", "!", "+", "*", "e", "$", "#", "@"};

string image(vector<array<int, 4>> rgby) {
	string res;
	for (array<int, 4> i : rgby) {
		int y = i[3];
		string r = to_string(i[0]);
		string g = to_string(i[1]);
		string b = to_string(i[2]);
		string current = y <= 25 ? chars[0] :
		y <= 51 && y > 25 ? chars[1] :
		y <= 76 && y > 51 ? chars[2] :
		y <= 102 && y > 76 ? chars[3] :
		y <= 127 && y > 102 ? chars[4] :
		y <= 153 && y > 127 ? chars[5] :
		y <= 178 && y > 153 ? chars[6] :
		y <= 204 && y > 153 ? chars[7] :
		y <= 229 && y > 204 ? chars[8] :
		y <= 255 && y > 229 ? chars[9] :
		y == 300 ? "\n" : "Error";
		current = "\033[38;2;" + r + ";" + g + ";" + b + "m" + current + "\033[0m";
		res += current;
	}
	
	return res;
}

void initIM() {
	string magickOptions = exec("GraphicsMagick++-config --cppflags --cxxflags --ldflags --libs");
	IM::InitializeMagick(magickOptions.c_str());
}

string ascii(string url) {
	string res;
	string path = fetch(url);
	IM::Image img = resize(path);
	vector<array<int, 4>> orig = _rgby(img);
	string imgOut = image(orig);
	cout << imgOut << endl;
	return res;
}

#endif

