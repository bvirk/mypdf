installdir = /home/bvirk/bin
object = mypdf.o logger.o pdf.o printtask.o globalFuncs.o
out = mypdf
$(out) : $(object)
	g++  -o $(out) $(object) -O2 -Wall -L/usr/local/lib -lhpdf -lm
	cp $(out) $(installdir)
%.o : %.cpp
	g++ -c $<

.PHONY : clean
clean :
	rm -f $(object) *.o
	