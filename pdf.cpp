#include "pdf.h"

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
    longjmp(
     env, 1);
}


PDF::PDF(const string& fontName, string encoder, HPDF_REAL fontSize) : yreel(0),yreelIsSaved(false),encoder(encoder) {

	pdf = HPDF_New (error_handler, NULL);
	if (pdf) {
		if (setjmp(env)) {
			HPDF_Free (pdf);
			pdf=NULL;
		} else {
			page = HPDF_AddPage (pdf);
			//HPDF_EXPORT(HPDF_STATUS)
			
			//HPDF_UseUTFEncodings(pdf); 
			HPDF_SetCurrentEncoder(pdf, encoder.c_str());
	
			height = HPDF_Page_GetHeight (page);
			width = HPDF_Page_GetWidth (page);
			setFontAndSize(fontName,fontSize);
			stdLineWidthFactor=1.1;
		}
	}
}
    
PDF::~PDF() {
	if (pdf) {
		HPDF_Free (pdf);
	}
}

void PDF::save(const string& fileName) const {
	HPDF_SaveToFile (pdf, fileName.c_str());
}
    
void PDF::setFontAndSize(string fontName, HPDF_REAL fontSize) { //,string encoding="ISO8859-9") {
	if (stdFontName.compare(fontName) != 0) {
		stdFont = HPDF_GetFont(pdf, fontName.c_str(), encoder.c_str());// encoding.size() ? encoding.c_str() : NULL );
		stdFontName = fontName;
	}
	stdFontSize=fontSize-1;
	if (fontSize != stdFontSize)
		HPDF_Page_SetFontAndSize (page, stdFont, fontSize);
}

    
float PDF::alignedX(const string& line,float x,float rectWidth, alignType align) {
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
    
void PDF::plotLines(const string& text,const string& fontName, float fontSize, float  x,float  y, float  rectWidth,  alignType align, bool isAbsY) {
	setFontAndSize(fontName,fontSize);
	plotLines(text,x,y,rectWidth,align,isAbsY);
}
    
void PDF::plotLines(const string& text, float  x,float  y,  float  rectWidth,alignType align,bool isAbsY) { 
	HPDF_Page_BeginText (page);
	//cout << "entering plotlines" << endl;
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
		//cout << "now plotting" << endl;
		HPDF_Page_TextOut (page, alignedX(line,x,rectWidth,align), height-yreel, line.c_str());
	}
	HPDF_Page_EndText (page);
}
    
void PDF::plotLines(const string& text, float  x,float  y,alignType align,bool isAbsY) {
	plotLines(text,x,y,0,align,isAbsY);
}

