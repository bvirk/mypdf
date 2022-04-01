#include "logger.h"


Logger::Logger() : isOpened(false) {} 
Logger::~Logger() {  
	if (isOpened) 
		myfile.close(); 
}
string Logger::hhmmss() {
	time_t now = time(0);
	string date(ctime(& now));
	return date.substr(11,8);
}
Logger& Logger::operator <<(string message) {
	if (!isOpened) {
		myfile.open ("/tmp/cdebug.log",ios::app);
		isOpened=true;
	}
	myfile << hhmmss() << " " << message << endl;
	return *this;
}


