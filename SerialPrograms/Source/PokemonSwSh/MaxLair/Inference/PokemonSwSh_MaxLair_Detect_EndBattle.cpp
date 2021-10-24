/*  Max Lair Detect Battle Menu
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include "Common/Compiler.h"
#include "CommonFramework/ImageTools/SolidColorTest.h"
#include "PokemonSwSh_MaxLair_Detect_EndBattle.h"

#include <iostream>
using std::cout;
using std::endl;

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonSwSh{
namespace MaxLairInternal{


PokemonCaughtMenuDetector::PokemonCaughtMenuDetector()
    : m_top_white(0.550, 0.020, 0.400, 0.020)
    , m_caught_left(0.500, 0.080, 0.050, 0.070)
    , m_caught_right(0.930, 0.080, 0.050, 0.070)
    , m_middle_pink(0.930, 0.300, 0.050, 0.200)
    , m_bottom_white(0.550, 0.900, 0.400, 0.020)
    , m_bottom_black(0.100, 0.970, 0.700, 0.020)
    , m_bottom_options(0.920, 0.970, 0.070, 0.020)
{
    add_box(m_top_white, Qt::magenta);
    add_box(m_caught_left, Qt::magenta);
    add_box(m_caught_right, Qt::magenta);
    add_box(m_middle_pink, Qt::magenta);
    add_box(m_bottom_white, Qt::magenta);
    add_box(m_bottom_black, Qt::magenta);
    add_box(m_bottom_options, Qt::magenta);
}
bool PokemonCaughtMenuDetector::process_frame(
    const QImage& frame,
    std::chrono::system_clock::time_point timestamp
){
    return detect(frame);
}


bool PokemonCaughtMenuDetector::detect(const QImage& screen){
    ImageStats top_white = image_stats(extract_box(screen, m_top_white));
//    cout << top_white.average << ", " << top_white.stddev << endl;
    if (!is_solid(top_white, {0.316068, 0.341966, 0.341966})){
        return false;
    }
    ImageStats caught_left = image_stats(extract_box(screen, m_caught_left));
    if (!is_black(caught_left)){
        return false;
    }
    ImageStats caught_right = image_stats(extract_box(screen, m_caught_right));
    if (!is_black(caught_right)){
        return false;
    }
    if (euclidean_distance(caught_left.average, caught_right.average) > 10){
        return false;
    }
    ImageStats middle_pink = image_stats(extract_box(screen, m_middle_pink));
    if (!is_solid(middle_pink, {0.485975, 0.0980567, 0.415969})){
        return false;
    }
    ImageStats bottom_white = image_stats(extract_box(screen, m_bottom_white));
//    cout << bottom_white.average << ", " << bottom_white.stddev << endl;
    if (!is_solid(bottom_white, {0.331264, 0.332167, 0.336569}, 0.1, 20)){
        return false;
    }
    ImageStats bottom_black = image_stats(extract_box(screen, m_bottom_black));
    if (!is_black(bottom_black)){
        return false;
    }
    ImageStats bottom_options = image_stats(extract_box(screen, m_bottom_options));
    if (bottom_options.stddev.sum() < 30){
        return false;
    }

    return true;
}



size_t count_catches(VideoOverlay& overlay, const QImage& screen){
    InferenceBoxScope box0(overlay, 0.760, 0.400 + 0*0.133, 0.050, 0.030, Qt::blue);
    InferenceBoxScope box1(overlay, 0.760, 0.400 + 1*0.133, 0.050, 0.030, Qt::blue);
    InferenceBoxScope box2(overlay, 0.760, 0.400 + 2*0.133, 0.050, 0.030, Qt::blue);
    InferenceBoxScope box3(overlay, 0.760, 0.400 + 3*0.133, 0.050, 0.030, Qt::blue);

    size_t count = 0;
    if (is_black(image_stats(extract_box(screen, box0)))){
        count++;
    }
    if (is_white(image_stats(extract_box(screen, box1)))){
        count++;
    }
    if (is_white(image_stats(extract_box(screen, box2)))){
        count++;
    }
    if (is_white(image_stats(extract_box(screen, box3)))){
        count++;
    }

    return count;
}

















}
}
}
}