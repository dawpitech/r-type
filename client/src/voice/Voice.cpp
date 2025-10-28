//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Voice class method implementation to handle audio
//

#include "voice/Voice.hpp"
#include "fmod_common.h"
#include "network/UDPClient.hpp"
#include "network/datatype.hpp"
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
    this->_soundBufferLength = this->_soundInfo.length;

    result = this->_system->createSound(
        nullptr, FMOD_LOOP_NORMAL | FMOD_OPENUSER, &this->_soundInfo, &this->_selfSound);
    if (result != FMOD_OK) {
        utils::Logger::debug("Unable to create sound for voice chat");
        this->_system = nullptr;
        return;
    }

    result = this->_system->createSound(
        nullptr, FMOD_LOOP_NORMAL | FMOD_OPENUSER, &this->_soundInfo, &this->_receivedSound);
    if (result != FMOD_OK) {
        utils::Logger::debug("Unable to create sound for received voice chat");
        this->_system = nullptr;
        return;
    }

    result = this->_selfSound->getLength(&this->_soundBufferLength, FMOD_TIMEUNIT_RAWBYTES);
    if (result != FMOD_OK) {
        utils::Logger::debug("Unable to get sound length");
        return;
    }
}

voice::VoiceChat::~VoiceChat()
{
    if (this->_selfSound) {
        this->_selfSound->release();
    }
    if (this->_receivedSound) {
        this->_receivedSound->release();
    }
    if (this->_system) {
        this->_system->close();
        this->_system->release();
    }
}

void voice::VoiceChat::update()
{
    if (this->_system) {
        this->_system->update();
    }
}

void voice::VoiceChat::startAudioCapture()
{
    if (this->_system == nullptr || this->_selfSound == nullptr) {
        utils::Logger::debug("Unable to acces system and/or soundBuffer for voice chat");
        return;
    }

    auto result = this->_system->recordStart(this->_deviceIndex, this->_selfSound, true);
    if (result != FMOD_OK) {
        utils::Logger::debug("Unable to record for voice chat");
        return;
    }
    this->_lastCursorPos = 0;
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
    if (this->_system == nullptr || this->_receivedSound == nullptr) {
        utils::Logger::debug("Unable to find system to play sound");
        return;
    }

    auto result = this->_system->playSound(this->_receivedSound, 0, false, &this->_channel);
    if (result != FMOD_OK) {
        utils::Logger::debug("Unable to play sound for voice chat");
        return;
    }

}

void voice::VoiceChat::setSoundToSend(network::UDPVoiceInfo &info)
{
    auto soundToSend = this->_getRecordData();
    if (soundToSend.empty())
        return;
    info.serializedData = soundToSend;
}

std::string voice::VoiceChat::_getRecordData()
{
    if (!this->_system || !this->_selfSound) {
        return "";
    }

    unsigned int recordPos = 0;
    this->_system->getRecordPosition(this->_deviceIndex, &recordPos);

    if (recordPos == this->_lastCursorPos) {
        return "";
    }

    int dataSize = 0;
    if (recordPos > this->_lastCursorPos) {
        dataSize = recordPos - this->_lastCursorPos;
    } else {
        dataSize = (this->_soundBufferLength - this->_lastCursorPos) + recordPos;
    }

    if (dataSize <= 0) {
        return "";
    }

    void *ptr1 = nullptr;
    void *ptr2 = nullptr;
    unsigned int len1 = 0;
    unsigned int len2 = 0;

    auto result = this->_selfSound->lock(
        this->_lastCursorPos, dataSize, &ptr1, &ptr2, &len1, &len2);
    if (result != FMOD_OK) {
        return "";
    }

    std::vector<char> tempBuffer(dataSize);
    
    std::memcpy(tempBuffer.data(), ptr1, len1);
    if (ptr2 != nullptr) {
        std::memcpy(tempBuffer.data() + len1, ptr2, len2);
    }

    this->_selfSound->unlock(ptr1, ptr2, len1, len2);

    this->_lastCursorPos = recordPos;

    return std::string(tempBuffer.begin(), tempBuffer.end());
}

void voice::VoiceChat::receiveSound(const std::string &soundReceived)
{
    if (this->_receivedSound == nullptr || soundReceived.empty()) {
        return;
    }

    void *ptr1 = nullptr;
    void *ptr2 = nullptr;
    unsigned int len1 = 0;
    unsigned int len2 = 0;

    unsigned int dataSize = soundReceived.length();

    auto result = this->_receivedSound->lock(
        this->_playCursor, dataSize, &ptr1, &ptr2, &len1, &len2);
    if (result != FMOD_OK) {
        return;
    }

    std::memcpy(ptr1, soundReceived.data(), len1);
    if (ptr2 != nullptr) {
        std::memcpy(ptr2, soundReceived.data() + len1, len2);
    }

    this->_receivedSound->unlock(ptr1, ptr2, len1, len2);

    this->_playCursor = (this->_playCursor + dataSize) % this->_soundBufferLength;
}
