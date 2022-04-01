#include "printtask.h"
#include "globalFuncs.h"

/**
	*ISO8859-1 to MacRoman encoding 
**/
const char PrintTask::tr[][2] = {
	{char(230),char(207)} //æ
,{char(198),char(174)} //Æ
,{char(248),char(191)} //ø
,{char(216),char(175)} //Ø
,{char(229),char(140)} //å
,{char(197),char(129)} //Å
};


PrintTask::PrintTask(string fontName, string encoding, HPDF_REAL fontSize, string outFN) : 	
		hasLayout(false)
	,pdf(fontName
		,encoding
		,fontSize)
	,outFile(outFN)
	,hasAbsY(false) 
	{}
PrintTask::~PrintTask() { 
	if (hasLayout)
		pdf.save(outFile);
}
int PrintTask::procesInfile(string fileName) {
	ifstream is(fileName.c_str());
    if (!is) {
        cout << "file: " << fileName << " not found" << endl; 
        return 1;
    }
	
	string line;
    bool inArrayBlock=true;
   	string section;
   	
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
				parse(trim(ut),skippedLeadNewline);
				section.clear();
			} else	
				section += "\n"+line;
		}
		
    }
    string skippedLeadNewline = section.substr(1);
    flush(skippedLeadNewline);
    return 0;
}

void PrintTask::parse(string nextLayout,string& content) {
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
void PrintTask::flush(string& content) {
	if (hasLayout) {
		string text = decodeHack(interpolated(content));
		pdf.plotLines(text,fontName, fontSize, x, y, rectWidth, align,hasAbsY);
	}
}
string& PrintTask::interpolated(string& c) {
	int pos;
    while ((pos = c.find_first_of("${")) != string::npos) {
    	int endpos = c.find_first_of("}",pos);
    	if (endpos == string::npos)
    		endpos = pos+2;
    	c= c.replace(pos,endpos-pos+1,values[c.substr(pos+2,endpos-pos-2)]);
    }
    return c;
}

bool PrintTask::isSection(string& line) const {
	string t = trim(line);
	return ((t[0] == '[') & (t[t.length()-1] == ']'));
}

string PrintTask::decodeHack(string s) const{
	for (int tri=0; tri < sizeof(tr)/sizeof(tr[0]);tri++) {
		size_t pos=0;
		while ((pos = s.find(tr[tri][0],pos)) != string::npos) 
			s.replace(pos,1,1,tr[tri][1]);
	}
	return s;
}


