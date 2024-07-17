#define MINIAUDIO_IMPLEMENTATION
#include "vendor/miniaudio.h"

#include "fonograf.hpp"

/* I needed to do this because for whatever reason because the non-experimental
   filesystem header didn't have std::filesystem with it, but the experimental
   one did */
#if __has_include(<experimental/filesystem>)
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#elif __has_include(<filesystem>)
#include <filesystem>
namespace fs = std::filesystem;
#else
#error "No <filesystem> header present."
#endif

#include <fstream>
#include <iostream>

#ifdef _WIN32
#include <conio.h>
#endif

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
    if (device_inited) {
        ma_device_uninit(&device);
        device_inited = false;
    }

    if (decoder_inited) {
        ma_decoder_uninit(&decoder);
        decoder_inited = false;
    }
}

void Fonograf::play_track_boilerplate(std::string filepath) {
    ma_result result = ma_decoder_init_file(filepath.c_str(), NULL, &decoder);
    if (result != MA_SUCCESS)
        throw std::runtime_error("Failed to open file " + filepath + ".");
    decoder_inited = true;

    ma_device_config device_config =
        ma_device_config_init(ma_device_type_playback);
    device_config.playback.format = decoder.outputFormat;
    device_config.playback.channels = decoder.outputChannels;
    device_config.sampleRate = decoder.outputSampleRate;
    device_config.dataCallback = data_callback;
    device_config.pUserData = &decoder;

    if (ma_device_init(NULL, &device_config, &device) != MA_SUCCESS) {
        throw std::runtime_error("Failed to open playback device.");
    }
    device_inited = true;

    if (ma_decoder_get_length_in_pcm_frames(
            &decoder, &duration_as_pcm_frames) != MA_SUCCESS) {
        throw std::runtime_error("Failed to get duration of track.");
    }

    duration = duration_as_pcm_frames / decoder.outputSampleRate;
    remaining_duration = duration;

    if (ma_device_start(&device) != MA_SUCCESS) {
        throw std::runtime_error("Failed to start playback device.");
    }
    player_state = PlayerState::PLAYING;
}

void Fonograf::fetch_tracks_in_directory() {
    std::string path = ".";

    try {
        for (const auto& entry : fs::directory_iterator(path)) {
            if (entry.path().extension() == ".mp3" ||
                entry.path().extension() == ".wav" ||
                entry.path().extension() == ".flac") {
                tracks_in_directory.push_back(entry.path().string());
            }
        }
    } catch (const fs::filesystem_error& e) {
        throw;
    }
}

/***** PUBLIC *****/

Fonograf::Fonograf()
    : player_state(PlayerState::NO_TRACK_CHOSEN), decoder_inited(false),
      device_inited(false) {
    track = "";
    fetch_tracks_in_directory();
}

Fonograf::Fonograf(std::string filepath)
    : player_state(PlayerState::NO_TRACK_CHOSEN), decoder_inited(false),
      device_inited(false) {
    std::ifstream ifile;
    ifile.open(filepath);
    if (!ifile) {
        throw std::invalid_argument("File " + filepath + " doesn't exist.");
    }

    track = filepath;
    player_state = PlayerState::PAUSED;

    fetch_tracks_in_directory();
}

Fonograf::~Fonograf() { cleanup(); }

void Fonograf::play_track() {
    if (track == "") {
        throw std::invalid_argument("No track path set.");
    }

    // Deallocate any previous allocations
    cleanup();
    play_track_boilerplate(track);
}

void Fonograf::play_track(std::string filepath) {
    // Deallocate any previous allocations
    cleanup();
    play_track_boilerplate(filepath);
    track = filepath;
}
