all:
	clang++ -lpthread -lm -ldl -o fonograf main.cpp fonograf.cpp 

clean:
	rm -f fonograf
