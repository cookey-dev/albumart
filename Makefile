main: main.cc spinners.hpp fetch.hpp ascii.hpp
	g++ -o main main.cc `GraphicsMagick++-config --cppflags --cxxflags --ldflags --libs` `curl-config --cflags --libs` -std=c++17
