/*  Activate Menu Glitch (Poketch)
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_PokemonBDSP_ActivateMenuGlitchPoketch_H
#define PokemonAutomation_PokemonBDSP_ActivateMenuGlitchPoketch_H

#include "NintendoSwitch/Options/TimeExpressionOption.h"
#include "NintendoSwitch/Framework/NintendoSwitch_SingleSwitchProgram.h"

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonBDSP{


class ActivateMenuGlitch112_Descriptor : public SingleSwitchProgramDescriptor{
public:
    ActivateMenuGlitch112_Descriptor();
};



class ActivateMenuGlitch112 : public SingleSwitchProgramInstance2{
public:
    ActivateMenuGlitch112();

    virtual void program(SingleSwitchProgramEnvironment& env, BotBaseContext& context) override;

private:
    TimeExpressionOption<uint16_t> FLY_A_TO_X_DELAY;
};



}
}
}
#endif
