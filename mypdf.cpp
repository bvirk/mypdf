#include <iostream>
#include <iterator>
#include <utility>
#include <list>
#include <map>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <csetjmp>
#include <unistd.h>
#include "hpdf.h"


jmp_buf env;

#ifdef HPDF_DLL
void  __stdcall
#else
void
#endif
error_handler (HPDF_STATUS   error_no,
               HPDF_STATUS   detail_no,
               void         *user_data) {
    printf ("ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT)error_no,
                (HPDF_UINT)detail_no);
    longjmp(env, 1);
}


using namespace std;

enum alignType {LEFT=0, CENTER, RIGHT};


class PDF {
     
    HPDF_Page page;
    HPDF_REAL height;
    HPDF_REAL width;

    HPDF_Font stdFont;
    HPDF_REAL stdFontSize;
    
	public:
    
    string  stdFontName;
    float  stdLineWidthFactor;
    float 	yreel;
    float  savedYreel;
    bool   yreelIsSaved;
    
    HPDF_Doc pdf;

    PDF(const string& fontName, HPDF_REAL fontSize) : yreel(0),yreelIsSaved(false) {
    
        pdf = HPDF_New (error_handler, NULL);
        if (pdf) {
            if (setjmp(env)) {
                HPDF_Free (pdf);
                pdf=NULL;
            } else {
                page = HPDF_AddPage (pdf);
                //HPDF_EXPORT(HPDF_STATUS)
                
                //HPDF_UseUTFEncodings(pdf); 
                HPDF_SetCurrentEncoder(pdf, "ISO8859-9");
        
                height = HPDF_Page_GetHeight (page);
                width = HPDF_Page_GetWidth (page);
                setFontAndSize(fontName,fontSize);
                stdLineWidthFactor=1.1;
            }
        }
    }
    
    ~PDF() {
        if (pdf) {
            HPDF_Free (pdf);
        }
    }

    void save(const string& fileName) {
        HPDF_SaveToFile (pdf, fileName.c_str());
    }
    
    void setFontAndSize(string fontName, HPDF_REAL fontSize,string encoding="") {
        if (stdFontName.compare(fontName) != 0) {
            stdFont = HPDF_GetFont(pdf, fontName.c_str(), encoding.size() ? encoding.c_str() : NULL );
            stdFontName = fontName;
        }
        stdFontSize=fontSize-1;
        if (fontSize != stdFontSize)
            HPDF_Page_SetFontAndSize (page, stdFont, fontSize);
    }

    
    float alignedX(const string& line,float x,float rectWidth, alignType align) {
    	float xEvaled = x;
        if ( align != LEFT) {
           if (rectWidth==0)
               rectWidth = width-x;
            float tw = HPDF_Page_TextWidth (page, line.c_str());
            xEvaled = align == RIGHT
                ? width-x-tw
                : x + (rectWidth-tw)/2;
        }
        return xEvaled;
    }
    
    void plotLines(const string& text,const string& fontName, float fontSize, float  x,float  y, float  rectWidth,  alignType align, bool isAbsY) { 
    	setFontAndSize(fontName,fontSize);
        plotLines(text,x,y,rectWidth,align,isAbsY);
    }
    
    void plotLines(const string& text, float  x,float  y,  float  rectWidth,alignType align,bool isAbsY) { 
        HPDF_Page_BeginText (page);
        cout << "entering plotlines" << endl;
        istringstream stream(text);
        string line;
        if (isAbsY) {
        	if (!yreelIsSaved) {
        		yreelIsSaved=true;
        		savedYreel=yreel;
        	}
        	yreel = stdLineWidthFactor*stdFontSize*y;
        } else {
        	if (yreelIsSaved) {
        		yreelIsSaved=false;
        		yreel=savedYreel;
        	}
        	yreel += stdLineWidthFactor*stdFontSize*y;
        	
        }	
        while (getline(stream, line)) {
        	yreel += stdLineWidthFactor*stdFontSize;
        	cout << "now plotting" << endl;
        	HPDF_Page_TextOut (page, alignedX(line,x,rectWidth,align), height-yreel, line.c_str());
        }
       HPDF_Page_EndText (page);
    }
    
    void plotLines(const string& text, float  x,float  y,alignType align,bool isAbsY) {
        plotLines(text,x,y,0,align,isAbsY);
    }
};

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

class Logger {
	ofstream myfile;
	bool isOpened;
public:
	Logger() : isOpened(false) {} 
	~Logger() {  
		if (isOpened) 
			myfile.close(); 
	}
	string hhmmss() {
		time_t now = time(0);
		string date(ctime(& now));
		return date.substr(11,8);
	}
	Logger& operator <<(string message) {
		if (!isOpened) {
			myfile.open ("/tmp/cdebug.log",ios::app);
			isOpened=true;
		}
		myfile << hhmmss() << " " << message << endl;
		return *this;
	}
	//ToLogC* operator <<(ToLogC* dummy) {
	//	return this;
	//}
} toLog;

string& trim(string& str) {
	str.erase(0,str.find_first_not_of(" \n\r"));
	str.erase(str.find_last_not_of(" \n\r")+1);
	return str;
}
string& interpolated(string& c); 

const char tr[][2] = {
	 {char(230),char(241)}
	,{char(198),char(225)}
	,{char(216),char(233)}
	,{char(248),char(249)}
};

string decodeHack(string s) {
	for (int tri=0; tri < sizeof(tr)/sizeof(tr[0]);tri++) {
		size_t pos=0;
		while ((pos = s.find(tr[tri][0],pos)) != string::npos) 
			s.replace(pos,1,1,tr[tri][1]);
	}
	return s;
}

	
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
public:
	PDF pdf;
	
	PrintTask(string outFN) : hasLayout(false),pdf("Times-Roman",12),outFile(outFN),hasAbsY(false) {}
	~PrintTask() { 
		if (hasLayout)
			pdf.save(outFile);
	}
	
	
	void parse(string nextLayout,string& content) {
		if (content.length()) 
			flush(content);
		
		istringstream line(nextLayout);
	
		string trword;
		string word;
		int pos=0;
		while (getline(line, word, ',') ) {
			switch(pos) {
			case 0:
				fontName = verFontName(trim(word));
				break;
			case 1:
				fontSize = atof(word.c_str());
				break;
			case 2:
				x = atof(word.c_str());
				break;
			case 3: {
				const char* word_cstr = word.c_str();
				y = atof(word_cstr);
				hasAbsY=!isdigit(*(const_cast<char *>(word_cstr)+word.length()-1));
				} break; 
			case 4:
				rectWidth = atof(word.c_str());
				break;
			case 5:
				trword = trim(word);
				align = strcasecmp("CENTER",trword.c_str())==0 
					? CENTER
					: strcasecmp("RIGHT",trword.c_str())==0 
						? RIGHT
						: LEFT;
				break;
			default:
				break;
			}
			pos++;
		}
		hasLayout=true;
	}
	void flush(string& content) {
		if (hasLayout) {
			string text = decodeHack(interpolated(content));
			pdf.plotLines(text,fontName, fontSize, x, y, rectWidth, align,hasAbsY);
		}
	}
};
		

map <string, string> values;
    
string& interpolated(string& c) {
	int pos;
    while ((pos = c.find_first_of("${")) != string::npos) {
    	int endpos = c.find_first_of("}",pos);
    	if (endpos == string::npos)
    		endpos = pos+2;
    	c= c.replace(pos,endpos-pos+1,values[c.substr(pos+2,endpos-pos-2)]);
    }
    return c;
}
bool isSection(string& line) {
	string t = trim(line);
	return ((t[0] == '[') & (t[t.length()-1] == ']'));
}
    
int procesInfile(string fileName) {
	
	ifstream is(fileName.c_str());
    if (!is) {
        cout << "file: " << fileName << " not found" << endl; 
        return 1;
    }
	
	string line;
    bool inArrayBlock=true;
   	string section;
   	PrintTask pntTask(fileName.substr(0,fileName.find_last_of('.'))+".pdf");
   	if (!pntTask.pdf.pdf) { 
        cout << "hpdf object creation failed\n";
    	return 1;
    }
    
   	values.insert(pair<string, string>("", "MISSING TRAILING CURLY BRACE"));
    
    
    while (getline(is,line)) {
    	if (trim(line).length() == 0)
    		inArrayBlock = false;
    	if (inArrayBlock) {
			size_t colonPos = line.find('=');
			if (colonPos == string::npos) {
				cout << "unknown fields specification: " << line << endl;
				return 1; 
			} 
			
			string fldName = line.substr(0,colonPos);
			string fldValue = line.substr(colonPos+1);
			values.insert(pair<string, string>(fldName, fldValue));
		} else {
			if (isSection(line)) {
				string ut = line.erase(line.find(']')).substr(1);
				string skippedLeadNewline = section.substr(1);
				pntTask.parse(trim(ut),skippedLeadNewline);
				section.clear();
			} else	
				section += "\n"+line;
		}
		
    }
    string skippedLeadNewline = section.substr(1);
    pntTask.flush(skippedLeadNewline);
    return 0;
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
	string numstr = "0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F  ";
	string out = font+"\n\n   "+numstr+"\n";
	for (int line=224; line <256; line+=16) {
		for (int offset=0; offset <16; offset++) {
			*(buf+3*offset) = offset+line;
			//*(buf+3*offset+1) = '.';
			//*(buf+3*offset+2) = '\n';
		}	
		out += numstr.substr(3*line/16,3)+string(buf)+"\n";
	}
	//cout << out << endl;
	PrintTask pntTask(font+".pdf");
   	if (!pntTask.pdf.pdf) { 
        cout << "hpdf object creation failed\n";
    	return 1;
    }
    string layout = font+",10,10,10,0,LEFT";
    //cout << layout << endl;
    //cout << out << endl;
    string empty;
    pntTask.parse(layout,empty);
    pntTask.flush(out);
    

	return 0;
}


int test() {
	toLog << "compiled";
	return 0;
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
                return 0; //listFontNames();
		case '?': 
			cout << "unknown option: " << char(optopt) << endl;
			return 0;
	}
    return procesInfile(string(argv[1]));
}
