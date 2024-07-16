#include "fonograf.hpp"

#include <iostream>

int main(int argc, char** argv) {
    bool song_passed_in = true;
    if (argc != 2) {
        song_passed_in = false;
    }

    if (argc == 2 && std::string(argv[1]) == "version") {
        std::cout << "fonograf 0.0.1\n";
        std::cout << "Written with <3 by pes18fan\n";
        return 0;
    }

    try {
        Fonograf f;

        if (song_passed_in) {
            f = Fonograf(argv[1]);
            f.play_track();
        }

        return f.render_ui();
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
