#ifndef fonograf_fonograf_hpp
#define fonograf_fonograf_hpp

#include "vendor/miniaudio.h"
#include <string>
#include <vector>

enum class PlayerState {
    NO_TRACK_CHOSEN,
    TRACK_LOADING,
    PLAYING,
    PAUSED,
};

struct Track {
    std::string filepath;
    bool played;

    Track(std::string p) : filepath(p), played(false) {}
};

class Fonograf {
    /* Miniaudio stuff. */
    ma_decoder decoder;
    ma_device device;

    /* Whether the ma_decoder is initialized. */
    bool decoder_inited;

    /* Whether the ma_device is initialized. */
    bool device_inited;

    Track* current_track;

    // Track length (in seconds)
    int duration;
    ma_uint64 duration_as_pcm_frames;
    int remaining_duration;

    PlayerState player_state;

    std::vector<Track> tracks_in_directory;

    void cleanup();

    void fetch_tracks_in_directory();

    void play_track_boilerplate(Track& track);

    /* UI */
    void print_centered_line_of_text(std::string text);
    void print_ui_header();

  public:
    Fonograf();
    ~Fonograf();

    int render_ui();

    void play_track();
    void play_track(Track& track);
};

#endif
