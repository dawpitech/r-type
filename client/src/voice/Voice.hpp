//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Base class for voice chat
//

#include "network/UDPClient.hpp"
#include "network/datatype.hpp"
#include <fmod.hpp>
#include <string>

namespace voice {
    class VoiceChat {
       public:
        VoiceChat();
        ~VoiceChat();

        void update();

        void startAudioCapture();
        void stopAudioCapture();
        void setSoundToSend(network::UDPVoiceInfo &info);
        void playSound();
        void receiveSound(const std::string &soundReceived);

       private:
        FMOD::System *_system = nullptr;
        FMOD::Sound *_selfSound = nullptr;
        FMOD::Sound *_receivedSound = nullptr;
        FMOD::Channel *_channel = nullptr;

        FMOD_CREATESOUNDEXINFO _soundInfo = {0};
        unsigned _soundBufferLength = 0;
        int _deviceIndex = 0;
        // unsigned _samplesRecorded = 0;
        
        unsigned _lastCursorPos = 0;
        unsigned _playCursor = 0;


        std::string _getRecordData();
    };
}  // namespace voice
