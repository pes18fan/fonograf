#ifndef fonograf_fonograf_hpp
#define fonograf_fonograf_hpp

#include "vendor/miniaudio.h"
#include <string>

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

    void cleanup();
    void die(std::string why);

    void print_ui_header();

  public:
    Fonograf(std::string t);
    ~Fonograf();

    int render_ui();

    void play_track();
};

#endif
