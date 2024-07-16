#include "fonograf.hpp"
#include "vendor/rogueutil.h"

namespace ru = rogueutil;

/*** PRIVATE ***/

void Fonograf::print_centered_line_of_text(std::string text) {
    int cols = ru::tcols();
    int center_start = (cols / 2) - text.size();

    for (int i = 0; i < center_start; i++) {
        std::cout << " ";
    }

    std::cout << text << "\n";
}

void Fonograf::print_ui_header() {
    ru::setColor(ru::GREEN);
    print_centered_line_of_text("Fonograf.");
    ru::resetColor();

    /* Now playing section */
    if (track != "") {
        if (paused) {
            ru::setColor(ru::YELLOW);
            ru::rutil_print("Paused: ");
            ru::resetColor();

            ru::rutil_print(track + "\n");
        } else {
            ru::setColor(ru::YELLOW);
            ru::rutil_print("Now playing: ");
            ru::resetColor();

            ru::rutil_print(track + "\n");
        }
    } else {
        ru::setColor(ru::YELLOW);
        ru::rutil_print("No song chosen.");
        ru::resetColor();
    }

    ru::rutil_print("\n");

    /* Tracks list */
    ru::rutil_print("Tracks found: \n");
    int i = 1;
    for (const auto& track : tracks_in_directory) {
        ru::setColor(ru::YELLOW);
        ru::rutil_print("\t [" + std::to_string(i) + "] ");
        ru::resetColor();

        ru::rutil_print(track + "\n");
        i++;
    }

    if (tracks_in_directory.empty()) {
        ru::rutil_print("No songs found in this folder.\n");
    }

    ru::rutil_print("\n");

    /* Controls */
    ru::setColor(ru::YELLOW);
    ru::rutil_print("Controls: \n");
    ru::resetColor();

    ru::rutil_print("\t 'p' to pause or play\n"
                    "\t 'c' to choose a song to switch to\n"
                    "\t 'q' to quit\n");
}

/*** PUBLIC ***/

int Fonograf::render_ui() {
    ru::saveDefaultColor();

    ru::cls();
    print_ui_header();

    while (true) {
        if (kbhit()) {
            int key = getch();
            switch (key) {
            case 'q':
                return 0;
            case 'c': {
                int last_row = ru::trows();
                gotoxy(0, last_row);

            song_picker:
                std::string ans;
                ru::rutil_print("Enter the number of the song to switch to, or "
                                "'b' to go back: ");
                std::cin >> ans;

                try {
                    int num = std::stoi(ans);

                    // Switch to the new song
                    if (device_inited)
                        ma_device_stop(&device);
                    cleanup();

                    play_track(tracks_in_directory[num - 1]);
                    break;
                } catch (const std::invalid_argument& e) {
                    if (ans == "b") {
                        break; // Break out of the switch
                    }

                    ru::rutil_print(
                        "Invalid option! Press any key to continue.");
                    getch();

                    ru::cls();
                    print_ui_header();
                    goto song_picker;
                }
            }
            case 'p': {
                if (!paused) {
                    ma_device_stop(&device);
                } else {
                    ma_device_start(&device);
                }

                paused = !paused;
            }
            }

            ru::cls();
            print_ui_header();
        }
    }
}
