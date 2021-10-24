/*  Max Lair Notifications
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include <QJsonArray>
#include <QJsonObject>
#include "CommonFramework/Globals.h"
#include "CommonFramework/Notifications/ProgramNotifications.h"
#include "Pokemon/Resources/Pokemon_PokemonNames.h"
#include "PokemonSwSh_MaxLair_Notifications.h"

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonSwSh{
namespace MaxLairInternal{

using namespace Pokemon;


void send_nonshiny_notification(
    Logger& logger, EventNotificationOption& settings,
    const QString& program,
    const Stats& stats
){
    std::vector<std::pair<QString, QString>> embeds;
    embeds.emplace_back("Adventure Result", "No shinies found.");
    embeds.emplace_back("Session Stats", QString::fromStdString(stats.to_str()));
    send_program_notification(
        logger, settings,
        QColor(), program,
        "Max Lair Status Update",
        embeds
    );
}
void send_shiny_notification(
    Logger& logger, EventNotificationOption& settings,
    const QString& program,
    const std::set<std::string>* slugs,
    const Stats& stats,
    const QImage& image
){
    std::vector<std::pair<QString, QString>> embeds;
    if (slugs){
        QString str;
        if (slugs->empty()){
            str = "None - Unable to detect.";
        }else if (slugs->size() == 1){
//                str += get_pokemon_name(*slugs->begin()).display_name();
            const PokemonNames* names = get_pokemon_name_nothrow(*slugs->begin());
            if (names){
                str += names->display_name();
            }else{
                str += QString::fromStdString(*slugs->begin());
            }
        }else{
            str += "Ambiguous: ";
            bool first = true;
            for (const std::string& slug : *slugs){
                if (!first){
                    str += ", ";
                }
                first = false;
//                    str += get_pokemon_name(slug).display_name();
                const PokemonNames* names = get_pokemon_name_nothrow(slug);
                if (names){
                    str += names->display_name();
                }else{
                    str += QString::fromStdString(slug);
                }
            }
        }
        embeds.emplace_back(STRING_POKEMON, std::move(str));
    }
    embeds.emplace_back("Session Stats", QString::fromStdString(stats.to_str()));

    send_program_notification(
        logger, settings,
        0xffff00, program,
        "Max Lair Shiny Notification",
        embeds,
        image, true
    );
}



}
}
}
}