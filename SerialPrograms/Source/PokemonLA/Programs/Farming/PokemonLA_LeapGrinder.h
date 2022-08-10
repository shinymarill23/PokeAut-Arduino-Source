/*  Tree Leap Grinder
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_PokemonLA_LeapGrinder_H
#define PokemonAutomation_PokemonLA_LeapGrinder_H

#include "CommonFramework/Notifications/EventNotificationsTable.h"
#include "NintendoSwitch/Framework/NintendoSwitch_SingleSwitchProgram.h"
#include "PokemonLA/Options/PokemonLA_ShinyDetectedAction.h"
#include "PokemonLA/Inference/PokemonLA_MountDetector.h"
#include "PokemonLA/Inference/PokemonLA_UnderAttackDetector.h"
#include "CommonFramework/Options/SimpleIntegerOption.h"
#include "PokemonLA/Programs/PokemonLA_LeapPokemonActions.h"
#include "CommonFramework/OCR/OCR_LanguageOptionOCR.h"
#include "Pokemon/Options/Pokemon_NameSelectOption.h"

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonLA{

class LeapGrinder_Descriptor : public SingleSwitchProgramDescriptor{
public:
    LeapGrinder_Descriptor();

    class Stats;
    virtual std::unique_ptr<StatsTracker> make_stats() const override;
};

class LeapGrinder : public SingleSwitchProgramInstance2{
public:
    LeapGrinder();
    virtual void program(SingleSwitchProgramEnvironment& env, BotBaseContext& context) override;

private:
    bool run_iteration(SingleSwitchProgramEnvironment& env, BotBaseContext& context);
    bool quick_check(SingleSwitchProgramEnvironment& env, BotBaseContext& context);

private:
    class RunRoute;

    OCR::LanguageOCR LANGUAGE;
    Pokemon::PokemonNameSelect POKEMON;
    SimpleIntegerOption<uint8_t> LEAPS;
    EnumDropdownOption STOP_ON;
    EnumDropdownOption EXIT_METHOD;
    ShinyDetectedActionOption SHINY_DETECTED_ENROUTE;
    ShinyDetectedActionOption MATCH_DETECTED_OPTIONS;

    EventNotificationOption NOTIFICATION_STATUS;
    EventNotificationsOption NOTIFICATIONS;
};





}
}
}
#endif
