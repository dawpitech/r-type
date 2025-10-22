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
    if (this->_system == nullptr || this->_soundBuffer == nullptr) {
        utils::Logger::debug("Unable to acces system and/or soundBuffer for voice chat");
        return;
    }

    auto result = this->_system->recordStart(this->_deviceIndex, this->_soundBuffer, true);
    if (result != FMOD_OK) {
        utils::Logger::debug("Unable to record for voice chat");
        return;
    }
}

void voice::VoiceChat::stopAudioCapture()
{
    if (this->_system == nullptr) {
        utils::Logger::debug("Unable to acces system to stop audio for voice chat");
        return;
    }

    this->_system->recordStop(this->_deviceIndex);
}

void voice::VoiceChat::playSound()
{
    unsigned int recordPos = 0;
    auto result = this->_system->getRecordPosition(this->_deviceIndex, &recordPos);
    if (result != FMOD_OK) {
        utils::Logger::debug("Unable to get record pos for voice chat");
        return;
    }

    result = this->_system->playSound(this->_soundBuffer, 0, false, &this->_channel);
    if (result != FMOD_OK) {
        utils::Logger::debug("Unable to get record pos for voice chat");
    }
}
