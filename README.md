## [libharu](https://github.com/libharu/libharu) program: mypdf


### libraries installed as packages under mint 20.2

libhpdf-2.3.0  
libhpdf-dev


### option to show syntax
mypdf -h 


### charset
The pdf is created by giving a .ptx file as argument. Its charset must be ISO8859-9

didn't work for danish æøåÆØÅ out of the box - therefore a 'decoding' workaround is used.

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

### syntax of 'pdf script'
The file consists of to section seperated by an empty line
```
pName=Benny Andersen
pAdrL1=Kisserup Strand Vest 13
pAdrL2=4300 Holbæk
pPhone=12345678
pEmail=a.mail.user@gmail.com

```
The first section is a variable block. It could be a presentaion of a record from a database query where the variable name is field names.

The following sections follows the pattern

[ fontname,fontsize,xpos,ypos,width,align] 
```
line 1
...
line n
```

#### fontname
any of those listet by 'mypdf -s'. Any font name not spelt correct default to 'Times-Roman' - even the empty string. Convenient for not writting to much.

#### fontsize,xpos,ypos and width
are all real numbers. 

The first drawed line of first section after the variable block is drawed y lines below the top - where lines is measured in the chosen font size.
In any following sections y lines is the space to prior section, measured in the fontsize the line has to be drawed in.

If ypos is followed by a letter - like in '1a' the ypos is absolute. In that way areas away from the  line below line progress can be drawed.
A true number ypos again keeps up after the latest true number.

#### align
is one of LEFT, CENTER OR RIGHT. if align=center, width is the size line is centered in. If align is LEFT or RIGHT width has no effect and can be set til zero. x is from LEFT or RIGHT margin.

#### lines
is subject for interpolation - the replacement mechanism known from php - but ${} surrounding is mandatory.