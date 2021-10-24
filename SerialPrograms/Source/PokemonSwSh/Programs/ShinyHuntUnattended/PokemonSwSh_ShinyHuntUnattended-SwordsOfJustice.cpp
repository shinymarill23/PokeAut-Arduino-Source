/*  ShinyHuntUnattended-SwordsOfJustice
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include "Common/Cpp/PrettyPrint.h"
#include "NintendoSwitch/Commands/NintendoSwitch_Device.h"
#include "NintendoSwitch/Commands/NintendoSwitch_PushButtons.h"
#include "NintendoSwitch/NintendoSwitch_Settings.h"
#include "PokemonSwSh/PokemonSwSh_Settings.h"
#include "PokemonSwSh/Commands/PokemonSwSh_Commands_GameEntry.h"
#include "PokemonSwSh/Commands/PokemonSwSh_Commands_DateSpam.h"
#include "PokemonSwSh_ShinyHuntTools.h"
#include "PokemonSwSh_ShinyHuntUnattended-SwordsOfJustice.h"

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonSwSh{


ShinyHuntUnattendedSwordsOfJustice_Descriptor::ShinyHuntUnattendedSwordsOfJustice_Descriptor()
    : RunnableSwitchProgramDescriptor(
        "PokemonSwSh:ShinyHuntUnattendedSwordsOfJustice",
        "Shiny Hunt Unattended - Swords Of Justice",
        "ComputerControl/blob/master/Wiki/Programs/PokemonSwSh/ShinyHuntUnattended-SwordsOfJustice.md",
        "Hunt for shiny SOJs. Stop when a shiny is found.",
        FeedbackType::NONE,
        PABotBaseLevel::PABOTBASE_12KB
    )
{}


ShinyHuntUnattendedSwordsOfJustice::ShinyHuntUnattendedSwordsOfJustice(const ShinyHuntUnattendedSwordsOfJustice_Descriptor& descriptor)
    : SingleSwitchProgramInstance(descriptor)
    , EXIT_CAMP_TO_RUN_DELAY(
        "<b>Exit Camp to Run Delay:</b><br>This needs to be carefully calibrated.",
        "1890"
    )
    , AIRPLANE_MODE(
        "<b>Airplane Mode:</b><br>Enable if airplane mode is on.",
        false
    )
    , m_advanced_options(
        "<font size=4><b>Advanced Options:</b> You should not need to touch anything below here.</font>"
    )
    , ENTER_CAMP_DELAY(
        "<b>Enter Camp Delay:</b>",
        "8 * TICKS_PER_SECOND"
    )
{
    PA_ADD_OPTION(START_IN_GRIP_MENU);
    PA_ADD_OPTION(TIME_ROLLBACK_HOURS);

    PA_ADD_OPTION(EXIT_CAMP_TO_RUN_DELAY);
    PA_ADD_OPTION(AIRPLANE_MODE);
    PA_ADD_DIVIDER(m_advanced_options);
    PA_ADD_OPTION(ENTER_CAMP_DELAY);
}



void ShinyHuntUnattendedSwordsOfJustice::program(SingleSwitchProgramEnvironment& env){
    if (START_IN_GRIP_MENU){
        grip_menu_connect_go_home(env.console);
        resume_game_no_interact(env.console, ConsoleSettings::instance().TOLERATE_SYSTEM_UPDATE_MENU_FAST);
    }else{
        pbf_press_button(env.console, BUTTON_B, 5, 5);
    }

    const uint32_t PERIOD = (uint32_t)TIME_ROLLBACK_HOURS * 3600 * TICKS_PER_SECOND;
    uint32_t last_touch = system_clock(env.console);
    for (uint32_t c = 0; ; c++){
        //  Touch the date.
        if (TIME_ROLLBACK_HOURS > 0 && system_clock(env.console) - last_touch >= PERIOD){
            pbf_press_button(env.console, BUTTON_HOME, 10, GameSettings::instance().GAME_TO_HOME_DELAY_SAFE);
            rollback_hours_from_home(env.console, TIME_ROLLBACK_HOURS, ConsoleSettings::instance().SETTINGS_TO_HOME_DELAY);
            resume_game_no_interact(env.console, ConsoleSettings::instance().TOLERATE_SYSTEM_UPDATE_MENU_FAST);
            last_touch += PERIOD;
        }

        //  Trigger encounter.
        pbf_press_button(env.console, BUTTON_X, 10, GameSettings::instance().OVERWORLD_TO_MENU_DELAY);
        pbf_press_button(env.console, BUTTON_A, 10, ENTER_CAMP_DELAY);
        if (AIRPLANE_MODE){
            pbf_press_button(env.console, BUTTON_A, 10, 100);
            pbf_press_button(env.console, BUTTON_A, 10, 100);
        }
        pbf_press_button(env.console, BUTTON_X, 10, 50);
        pbf_press_dpad(env.console, DPAD_LEFT, 10, 10);
        env.log("Starting Encounter: " + tostr_u_commas(c + 1));
        pbf_press_button(env.console, BUTTON_A, 10, EXIT_CAMP_TO_RUN_DELAY);

        //  Run away if not shiny.
        run_away_with_lights(env.console);

        //  Enter Pokemon menu if shiny.
        enter_summary(env.console, false);
    }

    pbf_press_button(env.console, BUTTON_HOME, 10, GameSettings::instance().GAME_TO_HOME_DELAY_SAFE);
    end_program_callback(env.console);
    end_program_loop(env.console);
}



}
}
}