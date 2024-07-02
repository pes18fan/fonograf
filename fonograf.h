#define BOILER_IMPL
#include "vendor/boiler.h"

#define MINIAUDIO_IMPLEMENTATION
#include "vendor/miniaudio.h"

#include <string>

class Fonograf {
    ma_decoder decoder;
    ma_device device;

    bool decoder_inited;
    bool device_inited;

    std::string track;
    int duration;

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
