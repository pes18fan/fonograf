#include "fonograf.hpp"
#include "vendor/rogueutil.h"
#include <thread>

namespace ru = rogueutil;

/*** PRIVATE ***/

void Fonograf::print_centered_line_of_text(std::string text) {
    int cols = ru::tcols();
    int center_start = (cols / 2) - (text.size() / 2);

    for (int i = 0; i < center_start; i++) {
        std::cout << " ";
    }

    std::cout << text << "\n";
}

void Fonograf::print_ui_header() {
    ru::setColor(ru::GREEN);
    print_centered_line_of_text("Fonograf.");
    ru::resetColor();

    ru::rutil_print("\n");

    /* Now playing section */
    if (player_state != PlayerState::NO_TRACK_CHOSEN) {
        if (player_state == PlayerState::PAUSED) {
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

        ru::setColor(ru::YELLOW);
        ru::rutil_print("Duration: ");
        ru::resetColor();

        std::stringstream ss;
        ss << duration / 60 << " min " << (duration - ((duration / 60) * 60))
           << " sec\n";
        ru::rutil_print(ss.str());

        ru::setColor(ru::YELLOW);
        ru::rutil_print("Remaining: ");
        ru::resetColor();

        std::stringstream ts;
        ts << remaining_duration / 60 << " min "
           << (remaining_duration - ((remaining_duration / 60) * 60))
           << " sec\n";
        ru::rutil_print(ts.str());
    } else {
        ru::setColor(ru::YELLOW);
        ru::rutil_print("No track chosen.\n");
        ru::resetColor();
    }

    ru::rutil_print("\n");

    /* Tracks list */
    ru::setColor(ru::YELLOW);
    ru::rutil_print("Tracks found: \n");
    ru::resetColor();

    int i = 1;
    for (const auto& track : tracks_in_directory) {
        ru::setColor(rogueutil::MAGENTA);
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

    if (player_state != PlayerState::NO_TRACK_CHOSEN) {
        ru::rutil_print("\t 'p' to pause or play\n");
    }
    ru::rutil_print("\t 'c' to choose a song to switch to\n"
                    "\t 'q' to quit\n");
}

/*** PUBLIC ***/

int Fonograf::render_ui() {
    ru::saveDefaultColor();

    ru::cls();
    print_ui_header();

    while (true) {
        if (player_state != PlayerState::NO_TRACK_CHOSEN) {
            ma_uint64 cursor;
            if (ma_decoder_get_cursor_in_pcm_frames(&decoder, &cursor) !=
                MA_SUCCESS) {
                throw std::runtime_error(
                    "Failed to retrieve current cursor position");
            }

            int remaining_frames = duration_as_pcm_frames - cursor;
            remaining_duration = remaining_frames / decoder.outputSampleRate;
            if (remaining_frames <= 0) {
                ru::rutil_print("Bye bye :3");
                return 0;
            }
        }

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
                if (player_state == PlayerState::PLAYING) {
                    ma_device_stop(&device);
                    player_state = PlayerState::PAUSED;
                } else if (player_state == PlayerState::PAUSED) {
                    ma_device_start(&device);
                    player_state = PlayerState::PLAYING;
                }
            }
            }

            continue;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));

        ru::cls();
        print_ui_header();
    }
}
