VENDOR_DIR = vendor
MINIAUDIO_HEADER = $(VENDOR_DIR)/miniaudio.h
ROGUEUTIL_HEADER = $(VENDOR_DIR)/rogueutil.h
MINIAUDIO_URL = https://raw.githubusercontent.com/mackron/miniaudio/master/miniaudio.h
ROGUEUTIL_URL = https://raw.githubusercontent.com/sakhmatd/rogueutil/master/rogueutil.h

all:
	mkdir -p $(VENDOR_DIR) || true
	[ -f $(MINIAUDIO_HEADER) ] || curl -o $(MINIAUDIO_HEADER) $(MINIAUDIO_URL)
	[ -f $(ROGUEUTIL_HEADER) ] || curl -o $(ROGUEUTIL_HEADER) $(ROGUEUTIL_URL)
	clang++ -std=c++17 -lpthread -lm -ldl -o fonograf main.cpp fonograf.cpp ui.cpp

.PHONY: clean
clean:
	rm -f fonograf
	rm -rf $(VENDOR_DIR)
