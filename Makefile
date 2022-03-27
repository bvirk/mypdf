object = mypdf
$(object) : $(object).o
	g++  -o $(object) $(object).o -O2 -Wall -L/usr/local/lib -lhpdf -lm
%.o : %.cpp
	g++ -c $<

.PHONY : clean
clean :
	rm -f $(object) *.o
	