all:
	g++ main.cpp -o fonograf -lpthread -lm -ldl

deps:
	@mkdir -p vendor
	@[ -f vendor/miniaudio.h ] || curl -fS -o vendor/miniaudio.h \
		https://raw.githubusercontent.com/mackron/miniaudio/master/miniaudio.h
	@[ -f vendor/boiler.h ] || curl -fS -o vendor/boiler.h \
		https://raw.githubusercontent.com/pes18fan/boiler.h/main/boiler.h
	@echo "Dependencies up to date."

clean:
	rm -f fonograf
