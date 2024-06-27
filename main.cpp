#define MINIAUDIO_IMPLEMENTATION
#include "vendor/miniaudio.h"

#include <iostream>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: fonograf <filename>\n";
        return 1;
    }

    ma_result result;
    ma_engine engine;

    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        std::cerr << "Failed to initialize miniaudio engine\n";
        return 1;
    }

    ma_result ok = ma_engine_play_sound(&engine, argv[1], NULL);
    if (ok != MA_SUCCESS) {
        std::cerr << "Failed to play file " << argv[1] << ". Does it exist?\n";
        return 1;
    }

    std::cout << "Press Enter to quit..\n";
    getchar();

    ma_engine_uninit(&engine);

    return 0;
}
