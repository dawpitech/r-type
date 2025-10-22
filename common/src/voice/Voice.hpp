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

        void captureAudio();
        void compressAudio();
        void decompressAudio();
        void sendAudio();
       private:
    };
}  // namespace voice
