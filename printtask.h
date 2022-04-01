#include <iostream>
#include <map>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cctype>
#include "pdf.h"
class PrintTask {
	string fontName;
	float fontSize;
	float x;
	float y;
	bool hasAbsY;
	float rectWidth;
	alignType align;
	bool hasLayout;
	string outFile;
	map <string, string> values;
	static const char tr[][2];

	void parse(string nextLayout,string& content);
	void flush(string& content);
	string& interpolated(string& c);
	bool isSection(string& line) const;
	string decodeHack(string s) const;

public:
	PDF pdf;
	PrintTask(string fontName, string encoding, HPDF_REAL fontSize, string outFN);
	~PrintTask();
		
	int procesInfile(string fileName);		
};
		
