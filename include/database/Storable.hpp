/**
 * @file Storable.hpp
 * @author Manuel G. Meraz
 * @date 03/11/2019
 * @brief The base class that all storable data will inherit from.
 */

#pragma once

#include "database/Data.hpp"

#include <string>

/**
 * @brief Organizes all databasing related classes and functions
 */
namespace database {

/*
 * @brief The base class that all storable data will inherit from.
 */
class Storable {
public:
  /**
   *  @return The unique ID of the Storable object
   */
  virtual auto id() const -> int = 0;

  /**
   * @return The name of the storable
   */
  virtual auto name() const -> std::string const = 0;

  /**
   * @param A new name for the storable
   */
  virtual void set_name(std::string_view name) = 0;

  /**
   * @return string representation of the name and data, the same way sqlite
   *         displays table data
   */
  virtual std::string str() const = 0;

  /**
   * @brief All data will be retrieved from a storable object using this
   *        function.
   *
   * @return This A pair containing the column where the data will be store
   *         and the data itself.
   */
  virtual auto get_data() const -> Data const = 0;

  /**
   * @brief When creating new storable objects from data retrieved from the
   *        database, this function will be used to set the data for the
   *        Storable object.
   *
   * @param schema A vector containing the properties of each column that make
   *               up a schema
   *
   * @param data A row of data to set the object data
   */
  virtual void set_data(std::vector<ColumnProperties> const &schema,
                        Row const &row) = 0;

  virtual ~Storable() = default;
};
} // namespace database
