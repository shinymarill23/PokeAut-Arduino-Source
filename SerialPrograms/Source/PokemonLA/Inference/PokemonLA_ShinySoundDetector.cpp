/*  Shiny Sound Detector
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include "CommonFramework/Globals.h"
#include "CommonFramework/Inference/SpectrogramMatcher.h"
#include "CommonFramework/Tools/AudioFeed.h"
#include "CommonFramework/Tools/ConsoleHandle.h"
#include "PokemonLA_ShinySoundDetector.h"
#include <QString>

#include <sstream>
#include <cfloat>
#include <iostream>
using std::cout;
using std::endl;

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonLA{



ShinySoundDetector::~ShinySoundDetector(){}
ShinySoundDetector::ShinySoundDetector(LoggerQt& logger, int sampleRate, bool stop_on_detected)
    : AudioInferenceCallback("ShinySoundDetector")
    , m_logger(logger)
    , m_stop_on_detected(stop_on_detected)
{
    if (sampleRate == 0){
        logger.log("Invalid Sample Rate: " + std::to_string(sampleRate), COLOR_RED);
        return;
    }

    QString shinyFilename = RESOURCE_PATH() + "PokemonLA/shiny." + QString::number(sampleRate) + ".wav";

    m_matcher = std::make_unique<SpectrogramMatcher>(shinyFilename, SpectrogramMatcher::Mode::SPIKE_CONV, sampleRate);
}
ShinySoundDetector::ShinySoundDetector(ConsoleHandle& console, bool stop_on_detected)
    : ShinySoundDetector(console, console.audio().sample_rate(), stop_on_detected)
{}


bool ShinySoundDetector::process_spectrums(
    const std::vector<std::shared_ptr<const AudioSpectrum>>& newSpectrums,
    AudioFeed& audioFeed
){
    // Feed spectrum one by one to the matcher:
    // newSpectrums are ordered from newest (largest timestamp) to oldest (smallest timestamp).
    // To feed the spectrum from old to new, we need to go through the vector in the reverse order:
    
    for(auto it = newSpectrums.rbegin(); it != newSpectrums.rend(); it++){
        std::vector<std::shared_ptr<const AudioSpectrum>> singleSpectrum = {*it};
        float matcherScore = m_matcher->match(singleSpectrum);

        if (matcherScore == FLT_MAX){
            continue; // error or not enough spectrum history
        }

        const float threshold = 0.76f;
        bool found = matcherScore <= threshold;
//        cout << matcherScore << endl;

        size_t curStamp = m_matcher->latestTimestamp();
        // std::cout << "(" << curStamp+1-m_matcher->numTemplateWindows() << ", " <<  curStamp+1 << "): " << matcherScore << 
        //     (found ? " FOUND!" : "") << std::endl;

        if (found) {
            std::ostringstream os;
            os << "Shiny sound find, score " << matcherScore << "/" << threshold << ", scale: " << m_matcher->lastMatchedScale();
            m_logger.log(os.str(), COLOR_RED);
            audioFeed.add_overlay(curStamp+1-m_matcher->numTemplateWindows(), curStamp+1, COLOR_RED);
            // Tell m_matcher to skip the remaining spectrums so that if `process_spectrums()` gets
            // called again on a newer batch of spectrums, m_matcher is happy.
            m_matcher->skip(std::vector<std::shared_ptr<const AudioSpectrum>>(newSpectrums.begin(), 
                newSpectrums.begin() + std::distance(it+1, newSpectrums.rend())));
            return m_stop_on_detected;
        }
    }

    return false;
}

void ShinySoundDetector::clear(){
    m_matcher->clear();
}



}
}
}
