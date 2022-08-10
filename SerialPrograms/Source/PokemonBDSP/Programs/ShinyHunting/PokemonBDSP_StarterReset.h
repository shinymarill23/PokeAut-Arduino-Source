/*  Starter Reset
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_PokemonBDSP_StarterReset_H
#define PokemonAutomation_PokemonBDSP_StarterReset_H

#include "CommonFramework/Notifications/EventNotificationsTable.h"
#include "NintendoSwitch/Options/StartInGripMenuOption.h"
#include "NintendoSwitch/Options/GoHomeWhenDoneOption.h"
#include "NintendoSwitch/Framework/NintendoSwitch_SingleSwitchProgram.h"
#include "Pokemon/Options/Pokemon_NameSelectOption.h"

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonBDSP{


class StarterReset_Descriptor : public SingleSwitchProgramDescriptor{
public:
    StarterReset_Descriptor();

    struct Stats;
    virtual std::unique_ptr<StatsTracker> make_stats() const override;
};


class StarterReset : public SingleSwitchProgramInstance2{
public:
    StarterReset();
    virtual void program(SingleSwitchProgramEnvironment& env, BotBaseContext& context) override;


private:
    void run_trigger(BotBaseContext& context) const;
    bool find_encounter(SingleSwitchProgramEnvironment& env) const;

private:
//    StartInGripOrGameOption START_IN_GRIP_MENU;
    GoHomeWhenDoneOption GO_HOME_WHEN_DONE;

    Pokemon::PokemonNameSelect STARTER;

    BooleanCheckBoxOption USE_SOUND_DETECTION;
    BooleanCheckBoxOption VIDEO_ON_SHINY;
    EventNotificationOption NOTIFICATION_NONSHINY;
    EventNotificationOption NOTIFICATION_SHINY;
    EventNotificationsOption NOTIFICATIONS;
};



}
}
}
#endif
