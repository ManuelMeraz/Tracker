/**
 * @file Database.cpp
 * @author Manuel G. Meraz
 * @date 03/13/2019
 * @brief The database singleton class is in charge all database queries
 *
 *  Due to the way SQLite works, we want a single connection to the database
 *  up and running at all times. This class keeps the connection maintained.
 */

#include "database/Database.hpp"

static std::unique_ptr<soci::session> _instance =
    std::make_unique<soci::session>(
        "sqlite3", "db=tracker.db timeout=2 shared_cache=true");

//! @copydoc Database::execute(std::string)
void Database::execute(std::string sql_command) { 
	*_instance << sql_command;
}
