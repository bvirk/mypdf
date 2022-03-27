## [libharu](https://github.com/libharu/libharu) program: mypdf


### libraries installed as packages under mint 20.2

libhpdf-2.3.0
libhpdf-dev


### option to show syntax
mypdf -h 


### charset
The pdf is created by giving a .ptx file as argument. Its charset must be ISO8859-9

didn't work for danish æøÆØ out of box therefore a 'decoding' using this workaround is done. åÅ didn't exists at all.

```
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
```

### compiling and viewing from editor

Bash script 'compAndViewPdf',  compiles and opens a viewer - handsome invoked from a macro in text editor.  

