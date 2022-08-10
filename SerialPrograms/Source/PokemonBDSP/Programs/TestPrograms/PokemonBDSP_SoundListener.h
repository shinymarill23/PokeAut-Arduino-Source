/*  Sound Listener
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *  
 *  Debug program to test all kinds of sound detectors.
 */

#ifndef PokemonAutomation_PokemonBDSP_SoundListener_H
#define PokemonAutomation_PokemonBDSP_SoundListener_H

#include "CommonFramework/Options/BooleanCheckBoxOption.h"
#include "NintendoSwitch/Framework/NintendoSwitch_SingleSwitchProgram.h"
#include "NintendoSwitch/Options/GoHomeWhenDoneOption.h"
#include "CommonFramework/Options/EnumDropdownOption.h"

namespace PokemonAutomation{ 
namespace NintendoSwitch{
namespace PokemonBDSP{


class SoundListener_Descriptor : public SingleSwitchProgramDescriptor{
public:
    SoundListener_Descriptor();
};


class SoundListener : public SingleSwitchProgramInstance2{
public:
    SoundListener();

    virtual void program(SingleSwitchProgramEnvironment& env, BotBaseContext& context) override;

private:

    EnumDropdownOption SOUND_TYPE;
    BooleanCheckBoxOption STOP_ON_DETECTED_SOUND;
};




}
}
}
#endif
