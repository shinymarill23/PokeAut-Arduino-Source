/*  Item Drop Sound Detector
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_PokemonLA_ItemDropSoundDetector_H
#define PokemonAutomation_PokemonLA_ItemDropSoundDetector_H

#include <functional>
#include "CommonFramework/Inference/AudioPerSpectrumDetectorBase.h"

#include <memory>

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonLA{


class ItemDropSoundDetector : public AudioPerSpectrumDetectorBase{
public:
    //  Warning: The callback will be called from the audio inference thread.
    ItemDropSoundDetector(ConsoleHandle& console, DetectedCallback detected_callback);

    // Implement AudioPerSpectrumDetectorBase::get_score_threshold()
    virtual float get_score_threshold() const override;

protected:
    // Implement AudioPerSpectrumDetectorBase::build_spectrogram_matcher()
    virtual std::unique_ptr<SpectrogramMatcher> build_spectrogram_matcher(size_t sample_rate) override;
};




}
}
}
#endif
