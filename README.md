## [libharu](https://github.com/libharu/libharu) program: mypdf


### libraries installed as packages under mint 20.2

libhpdf-2.3.0  
libhpdf-dev


### option to show syntax
mypdf -h 


### charset
The pdf is created by giving a .ptx file as argument. Its charset must be ISO8859-9

didn't work for danish æøåÆØÅ out of the box - therefore a 'decoding' using this workaround is done. all.

```
/**
ISO8859-1 to MacRomanEncoding encoding 
**/
const char tr[][2] = {
	 {char(230),char(207)} //æ
	,{char(198),char(174)} //Æ
	,{char(248),char(191)} //ø
	,{char(216),char(175)} //Ø
	,{char(229),char(140)} //å
	,{char(197),char(129)} //Å
};

string decodeHack(string s) {
	for (int tri=0; tri < sizeof(tr)/sizeof(tr[0]);tri++) {
		size_t pos=0;
		while ((pos = s.find(tr[tri][0],pos)) != string::npos) 
			s.replace(pos,1,1,tr[tri][1]);
	}
	return s;
}
```

### compiling and viewing from editor

Bash script 'compAndViewPdf',  compiles and opens a viewer - handsome invoked from a macro in text editor.  

