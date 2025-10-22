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

       private:
        FMOD::System *_system = nullptr;
        FMOD::Sound *_soundBuffer = nullptr;
        FMOD_CREATESOUNDEXINFO _soundInfo = {0};
        int _deviceIndex = 0;
    };
}  // namespace voice
