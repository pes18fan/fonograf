all:
	g++ main.cpp -o fonograf -lpthread -lm -ldl -ltagparser

deps:
	@mkdir -p vendor
	@echo "Fetching miniaudio" && [ -f vendor/miniaudio.h ] || curl -fS -o vendor/miniaudio.h \
		https://raw.githubusercontent.com/mackron/miniaudio/master/miniaudio.h
	@echo "Fetching boiler.h" && [ -f vendor/boiler.h ] || curl -fS -o vendor/boiler.h \
		https://raw.githubusercontent.com/pes18fan/boiler.h/main/boiler.h
	@echo "Updating submodules" && git submodule update --init --recursive
	@echo "Dependencies up to date."

clean:
	rm -f fonograf
