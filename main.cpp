#include <tagparser/tag.h>
#include <tagparser/tagvalue.h>
#define MINIAUDIO_IMPLEMENTATION
#include "vendor/miniaudio.h"

#define BOILER_IMPL
#include "vendor/boiler.h"

#include <tagparser/diagnostics.h>
#include <tagparser/mediafileinfo.h>
#include <tagparser/progressfeedback.h>

#include <iostream>
#include <sstream>

class State {
    ma_engine engine;
    ma_sound sound;
    bool sound_initialized;

    void cleanup() {
        ma_engine_uninit(&engine);
        ma_sound_uninit(&sound);
    }

    void die(std::string why) {
        std::cerr << why << "\n";
        cleanup();
        exit(1);
    }

  public:
    State() {
        ma_result result = ma_engine_init(NULL, &engine);
        if (result != MA_SUCCESS) {
            die("Failed to initialize miniaudio engine.");
        }

        sound_initialized = false;
    }

    ~State() { cleanup(); }

    void init_sound(std::string filepath) {
        if (sound_initialized)
            ma_sound_uninit(&sound);
        sound_initialized = false;

        ma_result ok = ma_sound_init_from_file(&engine, filepath.c_str(), 0,
                                               NULL, NULL, &sound);
        if (ok != MA_SUCCESS) {
            std::ostringstream oss;
            oss << "Failed to open file " << filepath << ". Does it exist?";
            die(oss.str());
        }
        sound_initialized = true;
    }

    void play_sound() { ma_sound_start(&sound); }
    void pause_sound() { ma_sound_stop(&sound); }

    bool is_playing() { return bool(ma_sound_is_playing(&sound)); }
};

void print_header(const char* track) {
    GREEN();
    center_text("Fonograf.\n\n");
    RESET();

    char* str = new char[sizeof(track)];
    sprintf(str, "Now playing: %s\n", track);
    center_text(str);

    center_text("Press p to pause or play, and q to quit.\n");

    delete[] str;
}

void print_track_info(const char* track) {
    auto file_info = TagParser::MediaFileInfo();
    auto diag = TagParser::Diagnostics();

    // create handle to abort gracefully and get feedback during during long
    // operations
    auto progress = TagParser::AbortableProgressFeedback(
        [](TagParser::AbortableProgressFeedback& feedback) {
            // callback for status update
            std::clog << "At step: " << feedback.step() << '\n';
        },
        [](TagParser::AbortableProgressFeedback& feedback) {
            // callback for percentage-only updates
            std::clog << "Step percentage: " << feedback.stepPercentage()
                      << '\n';
        });

    file_info.setPath(std::string_view(track));
    file_info.open();
    file_info.parseEverything(diag, progress);

    if (!file_info.tags().empty()) {
        auto tag = file_info.tags().at(0);
        auto title = tag->value(TagParser::KnownField::Title)
                         .toString(TagParser::TagTextEncoding::Utf8);
        auto artist = tag->value(TagParser::KnownField::Artist)
                          .toString(TagParser::TagTextEncoding::Utf8);

        GREEN();
        std::cout << "\n\n";
        std::cout << "Playing: ";
        RESET();
        std::cout << " " << title << "\n"
                  << "by " << artist << "\n";
    } else {
        GREEN();
        std::cout << "\n\n";
        std::cout << "Playing: ";
        RESET();
        std::cout << track << "\n";
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: fonograf <filename>\n";
        return 1;
    }

    State state;
    state.init_sound(argv[1]);
    state.play_sound();

    clrscr();
    print_header(argv[1]);
    print_track_info(argv[1]);

    while (true) {
        if (__kbhit()) {
            int key = __getch();

            clrscr();
            print_header(argv[1]);
            print_track_info(argv[1]);
            switch (key) {
            case 'q':
                goto end;
            case 'p': {
                if (state.is_playing()) {
                    state.pause_sound();
                } else {
                    state.play_sound();
                }
            }
            }
        }
    }

end:
    return 0;
}
