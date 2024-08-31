#include "ui/tabbing/PlayerViewTab.hpp"

#include "ui/BaseFunctions.hpp"
#include "sqlite/tabledef.hpp"
#include "globals.hpp"
#include <imgui.h>
#include <iostream>

PlayerViewTab::PlayerViewTab(const std::string& tab_name)
    :NonClosableTab(tab_name) {}

void PlayerViewTab::displayContents()
{
    if (db_handle != NULL)
    {
        sqlite3_stmt* statement;
        std::string message = "SELECT * FROM " +
            std::string(PLAYER_TABLE_NAME) + " ORDER BY " +
            std::string(PLAYER_TABLE_ROW_PNAME) + " COLLATE NOCASE ASC";
        int ret = sqlite3_prepare_v2(db_handle, message.c_str(), message.size(), &statement, NULL);
        if ( ret != SQLITE_OK)
        {
            std::cerr << "sqlite3 error: " << sqlite3_errmsg(db_handle) << std::endl;
            sqlite3_close(db_handle);
            return;
        }

        while ( (ret = sqlite3_step(statement)) == SQLITE_ROW )
        {
            const unsigned char* pname = sqlite3_column_text(statement, PLAYER_TABLE_ROW_PNAME_IDX);
            ImCustom::CenteredText("Player: " + std::string((const char*)pname), true, false);
        };

        if (ret != SQLITE_DONE)
        {
            std::cerr << "sqlite3_step error: " << sqlite3_errmsg(db_handle) << std::endl;
        }
        sqlite3_finalize(statement);
    } else {
        ImCustom::CenteredText("No Database is loaded!", true, true);
    }
}