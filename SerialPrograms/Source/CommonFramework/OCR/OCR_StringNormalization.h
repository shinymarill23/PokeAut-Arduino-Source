/*  String Normalization
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_OCR_StringNormalization_H
#define PokemonAutomation_OCR_StringNormalization_H

#include <string>

namespace PokemonAutomation{
namespace OCR{


std::u32string remove_non_alphanumeric(const std::u32string& text);
std::u32string run_character_reductions(const std::u32string& text);

std::u32string normalize_utf32(const std::string& text);



}
}
#endif
