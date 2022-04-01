#include "globalFuncs.h"
#include "printtask.h"


const char * fontNames[] = {
     {"Courier"}
    ,{"Courier-Bold"}
    ,{"Courier-Oblique"}
    ,{"Courier-BoldOblique"}
    ,{"Helvetica"}
    ,{"Helvetica-Bold"}
    ,{"Helvetica-Oblique"}
    ,{"Helvetica-BoldOblique"}
    ,{"Times-Roman"}
    ,{"Times-Bold"}
    ,{"Times-Italic"}
    ,{"Times-BoldItalic"}
    ,{"Symbol"}
    ,{"ZapfDingbats"}
};
int fontNamesCount() {
	return sizeof(fontNames)/sizeof(fontNames[0]);
}

string& verFontName(string& guess) {
	for (int i=0; i< fontNamesCount(); i++)
		if (guess == fontNames[i])
			return guess;
	return guess = fontNames[8]; //default
}

int listFontNames() {
	for (int i=0; i< fontNamesCount(); i++)
		cout << fontNames[i] << endl;
	return 0;
}



string& trim(string& str) {
	str.erase(0,str.find_first_not_of(" \n\r"));
	str.erase(str.find_last_not_of(" \n\r")+1);
	return str;
}
  
int syntax() {
	cout << \
	"mypdf [option] | [filename]\n" \
	"\noptions:\n" \
	"\t-h\n" \
	"\t\tThis info\n" \
	"\t-s\n" \
	"\t\tlist fonts\n" \
	"\t-a font\n" \
	"\t\tshows character codes for font\n";
	
	return 0;
}


int printChrTable(string font) {

	verFontName(font);
	char buf[49];
	buf[48]='\0';
	memset(buf,32,48);
	string encode = "MacRomanEncoding";
	string numstr = "0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F  ";
	string out = font+"-"+encode+"\n\n   "+numstr+"\n";
	for (int line=64; line <256; line+=16) {
		for (int offset=0; offset <16; offset++) {
			*(buf+3*offset) = offset+line;
			//*(buf+3*offset+1) = '.';
			//*(buf+3*offset+2) = '\n';
		}	
		out += numstr.substr(3*line/16,3)+string(buf)+"\n";
	}
	//cout << out << endl;
	PrintTask pntTask("Times-Roman",encode,20,font+".pdf");
   	if (!pntTask.pdf.pdf) { 
        cout << "hpdf object creation failed\n";
    	return 1;
    }
    string layout = font+",20,10,10,0,LEFT";
    //cout << layout << endl;
    //cout << out << endl;
    string empty;
    pntTask.parse(layout,empty);
    pntTask.flush(out);
    

	return 0;
}


