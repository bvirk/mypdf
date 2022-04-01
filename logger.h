#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;


class Logger {
	ofstream myfile;
	bool isOpened;
public:
	Logger();
	~Logger();
	string hhmmss();
	Logger& operator <<(string message);
};

    
