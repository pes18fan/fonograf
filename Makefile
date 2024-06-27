all:
	g++ main.cpp -o fonograf -lpthread -lm -ldl

deps:
	mkdir vendor || true
	curl -fS -o vendor/miniaudio.h \
		https://raw.githubusercontent.com/mackron/miniaudio/master/miniaudio.h

clean:
	rm -f fonograf
