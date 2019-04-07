/**
 * @file Database.hpp
 * @author Manuel G. Meraz
 * @date 03/13/2019
 * @brief Due to the way SQLite works, we want a single connection to the
 *        database up and running at all times. This class keeps the connection
 *        maintained.
 */

#ifndef DATABASE_DATABASE_HPP
#define DATABASE_DATABASE_HPP

#include "soci-sqlite3.h"
#include "soci.h"
#include <memory>

/**
 * @brief The database singleton class is in charge all database queries
 *
 *  Due to the way SQLite works, we want a single connection to the database
 *  up and running at all times. This class keeps the connection maintained.
 */
class Database {
public:
  /**
   * @brief Returns a reference to the current database connection;
   *
   * Called by Database::utils
   */
  static auto get_connection() -> soci::session&;

  //! Deleted functions
  Database(const Database &) = delete;
  Database(Database &&) = delete;
  Database &operator=(const Database &) = delete;
  Database &operator=(Database &&) = delete;

private:
  Database() = default;
  ~Database() = default;

  static std::unique_ptr<soci::session> sql_connection;
};

#endif /* DATABASE_DATABASE_HPP */
