#include "fonograf.hpp"

#include <iostream>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: fonograf <filename>\n";
        return 1;
    }

    Fonograf f(argv[1]);
    f.play_track();

    return f.render_ui();
}
