#include <iostream>
#include <sstream>
#include <csetjmp>
#include "hpdf.h"



using namespace std;

enum alignType {LEFT=0, CENTER, RIGHT};

class PDF {
     
    HPDF_Page page;
    HPDF_REAL height;
    HPDF_REAL width;

    HPDF_Font stdFont;
    HPDF_REAL stdFontSize;
    
    string  stdFontName;
    float  stdLineWidthFactor;
    float 	yreel;
    float  savedYreel;
    bool   yreelIsSaved;
    string encoder;

    void setFontAndSize(string fontName, HPDF_REAL fontSize);
    float alignedX(const string& line,float x,float rectWidth, alignType align);

public:
    HPDF_Doc pdf;

    PDF(const string& fontName, string encoder, HPDF_REAL fontSize);
    ~PDF();
    void save(const string& fileName) const;
    void plotLines(const string& text,const string& fontName, float fontSize, float  x,float  y, float  rectWidth,  alignType align, bool isAbsY);
    void plotLines(const string& text, float  x,float  y,  float  rectWidth,alignType align,bool isAbsY);
    void plotLines(const string& text, float  x,float  y,alignType align,bool isAbsY);
};
