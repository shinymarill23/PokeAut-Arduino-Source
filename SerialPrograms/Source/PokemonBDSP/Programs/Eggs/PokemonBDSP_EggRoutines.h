/*  Egg Routines
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_PokemonBDSP_EggRoutines_H
#define PokemonAutomation_PokemonBDSP_EggRoutines_H

#include "ClientSource/Connection/BotBase.h"

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonBDSP{


void egg_spin(BotBaseContext& context, uint16_t duration);
void egg_spin_with_A(BotBaseContext& context, uint16_t duration);

void pickup_column(BotBaseContext& context);
void party_to_column(BotBaseContext& context, uint8_t column);
void column_to_party(BotBaseContext& context, uint8_t column);

void withdraw_1st_column_from_overworld(BotBaseContext& context);
void deposit_party_to_column(BotBaseContext& context, uint8_t column);
void swap_party(BotBaseContext& context, uint8_t current_column);


}
}
}
#endif
