//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Voice class method implementation to handle audio
//

#include "voice/Voice.hpp"
#include "fmod_common.h"
#include "utils/logger.hpp"
#include <fmod.hpp>
#include <format>

voice::VoiceChat::VoiceChat()
{
    auto result = FMOD::System_Create(&this->_system);
    if (result != FMOD_OK) {
        utils::Logger::debug("Unable to create system for voice chat");
        this->_system = nullptr;
        return;
    }

    result = this->_system->init(512, FMOD_INIT_NORMAL, 0);
    if (result != FMOD_OK) {
        utils::Logger::debug("Unable to init system for voice chat");
        this->_system = nullptr;
        return;
    }
    int numConnected = 0;
    this->_system->getRecordNumDrivers(nullptr, &numConnected);
    utils::Logger::debug(std::format("NumConnected: {}", numConnected));

    int nativeRate = 0;
    int nativeChannels = 0;
    result = this->_system->getRecordDriverInfo(
        this->_deviceIndex, NULL, 0, NULL, &nativeRate, NULL, &nativeChannels, NULL);

    this->_soundInfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
    this->_soundInfo.numchannels = nativeChannels;
    this->_soundInfo.format = FMOD_SOUND_FORMAT_PCM16;
    this->_soundInfo.defaultfrequency = nativeRate;
    this->_soundInfo.length = nativeRate * sizeof(short) * nativeChannels;

    result = this->_system->createSound(
        nullptr, FMOD_LOOP_NORMAL | FMOD_OPENUSER, &this->_soundInfo, &this->_soundBuffer);
    if (result != FMOD_OK) {
        utils::Logger::debug("Unable to create sound for voice chat");
        this->_system = nullptr;
        return;
    }
}

voice::VoiceChat::~VoiceChat() {}

void voice::VoiceChat::startAudioCapture()
{
    this->_system->recordStart(this->_deviceIndex, this->_soundBuffer, true);
}

void voice::VoiceChat::stopAudioCapture()
{
    this->_system->recordStop(this->_deviceIndex);
}
