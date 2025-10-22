//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Base class for voice chat
//

#include <fmod.hpp>

namespace voice {
    class VoiceChat {
       public:
        VoiceChat();
        ~VoiceChat();

        void startAudioCapture();
        void stopAudioCapture();
        void compressAudio();
        void decompressAudio();
        void sendAudio();
        void playSound();

       private:
        FMOD::System *_system = nullptr;
        FMOD::Sound *_soundBuffer = nullptr;
        FMOD::Channel *_channel = nullptr;
        // unsigned _soundLength = 0;
        // unsigned _samplesRecorded = 0;


        FMOD_CREATESOUNDEXINFO _soundInfo = {0};
        int _deviceIndex = 0;
    };
}  // namespace voice
