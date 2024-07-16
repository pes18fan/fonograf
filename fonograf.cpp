#define BOILER_IMPL
#include "vendor/boiler.h"

#include "fonograf.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

ma_uint64 FRAMES_READ = -1;

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput,
                   ma_uint32 frameCount) {
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL)
        return;

    ma_result r =
        ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, &FRAMES_READ);
    (void)pInput;
}

/***** PRIVATE *****/

void Fonograf::cleanup() {
    if (device_inited)
        ma_device_uninit(&device);
    if (decoder_inited)
        ma_decoder_uninit(&decoder);
}

void Fonograf::die(std::string why) {
    std::cerr << why << "\n";
    cleanup();
    exit(1);
}

void Fonograf::print_ui_header() {
    GREEN();
    center_text("Fonograf.\n\n");
    RESET();

    std::ostringstream oss;
    if (paused) {
        oss << "Paused: " << track << "\n";
    } else {
        oss << "Now playing: " << track << "\n";
    }
    center_text(oss.str().c_str());

    center_text("Press p to pause or play, and q to quit.\n");
}

/***** PUBLIC *****/

Fonograf::Fonograf(std::string t)
    : paused(true), decoder_inited(false), device_inited(false) {
    std::ifstream ifile;
    ifile.open(t);
    if (!ifile) {
        die("File " + t + " doesn't exist.");
    }

    track = t;
}

Fonograf::~Fonograf() { cleanup(); }

void Fonograf::play_track() {
    // Deallocate any previous allocations
    cleanup();

    ma_result result = ma_decoder_init_file(track.c_str(), NULL, &decoder);
    if (result != MA_SUCCESS)
        die("Failed to open file " + track + ".");
    decoder_inited = true;

    ma_device_config device_config =
        ma_device_config_init(ma_device_type_playback);
    device_config.playback.format = decoder.outputFormat;
    device_config.playback.channels = decoder.outputChannels;
    device_config.sampleRate = decoder.outputSampleRate;
    device_config.dataCallback = data_callback;
    device_config.pUserData = &decoder;

    if (ma_device_init(NULL, &device_config, &device) != MA_SUCCESS) {
        die("Failed to open playback device.");
    }
    device_inited = true;

    if (ma_device_start(&device) != MA_SUCCESS) {
        die("Failed to start playback device.");
    }
    paused = false;
}

int Fonograf::render_ui() {
    clrscr();
    print_ui_header();

    while (true) {
        if (__kbhit()) {
            int key = __getch();
            switch (key) {
            case 'q':
                return 0;
            case 'p': {
                if (!paused) {
                    ma_device_stop(&device);
                } else {
                    ma_device_start(&device);
                }

                paused = !paused;
            }
            }

            clrscr();
            print_ui_header();
        }
    }
}
