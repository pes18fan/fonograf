#include "fonograf.hpp"

#include <iostream>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: fonograf <filename>\n";
        return 1;
    }

    try {
        Fonograf f(argv[1]);
        f.play_track();

        return f.render_ui();
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
