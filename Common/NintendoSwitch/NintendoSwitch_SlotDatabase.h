/*  Slot Database
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_NintendoSwitch_SlotDatabase_H
#define PokemonAutomation_NintendoSwitch_SlotDatabase_H

#include "Common/Cpp/EnumDatabase.h"

namespace PokemonAutomation{
namespace NintendoSwitch{


inline const IntegerEnumDatabase& GameSlot_Database(){
    static const IntegerEnumDatabase database({
        {1, "game1", "Game 1"},
        {2, "game2", "Game 2"},
    });
    return database;
}
inline const IntegerEnumDatabase& UserSlot_Database(){
    static const IntegerEnumDatabase database({
        {1, "user1", "User 1"},
        {2, "user2", "User 2"},
        {3, "user3", "User 3"},
        {4, "user4", "User 4"},
        {5, "user5", "User 5"},
        {6, "user6", "User 6"},
        {7, "user7", "User 7"},
        {8, "user8", "User 8"},
    });
    return database;
}


}
}
#endif
