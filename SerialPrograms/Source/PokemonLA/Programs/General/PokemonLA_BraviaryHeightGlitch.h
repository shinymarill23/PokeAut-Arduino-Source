/*  Braviary Height Glitch
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_PokemonLA_BraviaryHeightGlitch_H
#define PokemonAutomation_PokemonLA_BraviaryHeightGlitch_H

#include "NintendoSwitch/Framework/NintendoSwitch_SingleSwitchProgram.h"

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonLA{


class BraviaryHeightGlitch_Descriptor : public SingleSwitchProgramDescriptor{
public:
    BraviaryHeightGlitch_Descriptor();
};


class BraviaryHeightGlitch : public SingleSwitchProgramInstance2{
public:
    BraviaryHeightGlitch();

    virtual void program(SingleSwitchProgramEnvironment& env, BotBaseContext& context) override;
};





}
}
}
#endif
