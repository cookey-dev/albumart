#include <iostream>
#include "ascii.hpp"

using namespace std;

int main() {
	initIM();
	string art = ascii("https://www.therip.com/wp-content/uploads/2017/01/pink-season.jpg");
	cout << art;
}
