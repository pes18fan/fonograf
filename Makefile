all:
	clang++ -lpthread -lm -ldl -o fonograf main.cpp fonograf.cpp ui.cpp

clean:
	rm -f fonograf
