#include "mypdf.h"
#include "logger.h"
#include "globalFuncs.h"
#include "printtask.h"

Logger toLog;




int test() {
	toLog << "compiled today";
	return 0;
}
			

int makePdfFile(string fileName) {
	PrintTask pntTask("Times-Roman","MacRomanEncoding",12,fileName.substr(0,fileName.find_last_of('.'))+".pdf");
	if (!pntTask.pdf.pdf) { 
        cout << "hpdf object creation failed\n";
    	return 1;
    }
	return pntTask.procesInfile(fileName);
}

int main(int argc, char * argv[]) {
	if (argc < 2 || argc > 3) 
    	return syntax();
	switch(getopt(argc, argv, ":a:hst")) 
	{ 
		case 'a':
			return printChrTable(string(argv[2]));
		case 'h':
			return syntax();
		case 's':
			return listFontNames();
		case 't':
			return test();
		case ':': 
                cout << "option -a needs a font name - use one of following:\n" << endl; 
                return 0; 
		case '?': 
			cout << "unknown option: " << char(optopt) << endl;
			return 0;
	}
    return makePdfFile(string(argv[1]));
}
