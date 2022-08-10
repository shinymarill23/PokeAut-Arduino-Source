/*  Prevent Sleep
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_NintendoSwitch_PreventSleep_H
#define PokemonAutomation_NintendoSwitch_PreventSleep_H

#include "NintendoSwitch/Framework/NintendoSwitch_SingleSwitchProgram.h"

namespace PokemonAutomation{
namespace NintendoSwitch{


class PreventSleep_Descriptor : public SingleSwitchProgramDescriptor{
public:
    PreventSleep_Descriptor();
};


class PreventSleep : public SingleSwitchProgramInstance2{
public:
    PreventSleep();

    virtual void program(SingleSwitchProgramEnvironment& env, BotBaseContext& context) override;


};



}
}
#endif



