#ifndef fonograf_fonograf_hpp
#define fonograf_fonograf_hpp

#include "vendor/miniaudio.h"
#include <string>
#include <vector>

class Fonograf {
    /* Miniaudio stuff. */
    ma_decoder decoder;
    ma_device device;

    /* Whether the ma_decoder is initialized. */
    bool decoder_inited;

    /* Whether the ma_device is initialized. */
    bool device_inited;

    std::string track;

    // Track length (in seconds)
    int duration;
    ma_uint64 duration_as_pcm_frames;
    int remaining_duration;

    bool paused;

    std::vector<std::string> tracks_in_directory;

    void cleanup();

    void fetch_tracks_in_directory();

    void play_track_boilerplate(std::string filepath);

    /* UI */
    void print_centered_line_of_text(std::string text);
    void print_ui_header();

  public:
    Fonograf();
    Fonograf(std::string filepath);
    ~Fonograf();

    int render_ui();

    void play_track();
    void play_track(std::string track);
};

#endif
