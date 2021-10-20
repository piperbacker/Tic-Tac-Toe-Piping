   
p1: p1.cpp my_const.h
	g++ -g p1.cpp -o p1

p2: p2.cpp my_const.h
	g++ -g p2.cpp -o p2

clean:
	rm p1 p2

zip:
	zip named_pipe.zip *.cpp *.h Makefile