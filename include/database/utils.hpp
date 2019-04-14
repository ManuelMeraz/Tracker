/**
 * @file utils.hpp
 * @author Manuel G. Meraz
 * @date 03/30/2019
 * @brief Utility functions and objects for databasing
 */

#pragma once

#include "database/Storable.hpp"

#include <string_view>
#include <type_traits>
#include <vector>

/**
 * @brief Organizes all databasing related classes and functions
 */
namespace database {
/**
 * @brief Utility functions to insert, retrieve, and manipulate objects in
 *        database.
 */
namespace utils {

/**
 * @brief Finds and returns the object if it exists in the stl container
 * @param first A forward iterator for the start of the search
 * @param last A forward iterator for the end of the search
 * @param comp A less than comparison function for the object to be found
 * @return An iterator to the found object, will be the end iterator if now
 *         found
 *
 * Source: https://en.cppreference.com/w/cpp/algorithm/lower_bound
 *
 * Requirements: Container must be sorted in relation to the comparison
 *               function
 *
 * Usage:
 * @n auto const compare = [](Storable const &lhs, Storable const &rhs) {
 * @n  return lhs.id() < rhs.id();
 * @n };
 *
 * @n auto found = utils::binary_find(begin(storables), end(storables));
 * @n if(found != end(storables) {
 * @n   // Do something with found object
 * @n }
 */
template <class ForwardIt, class T, class Compare = std::less<>>
auto binary_find(ForwardIt first, ForwardIt last, const T &value, Compare comp)
    -> ForwardIt;

/**
 * @brief Count the number of Storable type in the database
 * @param Storable Any type that is a base of Storable
 * @return The number of rows in the table, 0 if table does not exist
 *
 * Creates the following SQLite3 command:
 * @n SELECT count(*) from table_name;
 *
 * Usage:
 * @n size_t quantity = database::utils::count_rows<food::Food>();
 */
template <
    typename Storable,
    typename std::enable_if_t<
        std::is_base_of_v<database::Storable, std::decay_t<Storable>>, int> = 0>
auto count_rows() -> size_t;

/**
 * @brief Create table of Storable if not exists
 * @param Storable Any type that is a base of Storable
 * @param schema The schema to be used to create the table
 *
 * Creates the following SQLite3 command:
 * @n CREATE TABLE IF NOT EXISTS table_name (
 * @n  column_1 data_type PRIMARY KEY,
 * @n  column_2 data_type NOT NULL,
 * @n  column_3 data_type DEFAULT 0,
 * @n  ...
 * @n  );
 *
 * Usage:
 * @n database::utils::create_table<food::Food>();
 */
template <
    typename Storable,
    typename std::enable_if_t<
        std::is_base_of_v<database::Storable, std::decay_t<Storable>>, int> = 0>
void create_table(std::vector<ColumnProperties> const &schema);

/**
 * @brief Delete Storable object from datbase and cache of storables
 * @param storable Any type that is a base of Storable
 *
 * Creates the following SQLite3 command:
 * @n DELETE
 * @n FROM
 * @n  table
 * @n WHERE
 * @n  search_condition;
 *
 * Usage:
 * @n database::utils::delete_storable(food);
 *
 * Note: delete is a keyword, could not use that.
 */
template <
    typename Storable,
    typename std::enable_if_t<
        std::is_base_of_v<database::Storable, std::decay_t<Storable>>, int> = 0>
void delete_storable(Storable const &storable);

/**
 * @brief Deletes the Storable table from the database
 * @param Storable Any type that is a base of Storable
 *
 * Creates the following SQLite3 command:
 * DROP TABLE table_name;
 *
 * Usage:
 * @n drop_table<food::Food>();
 */
template <
    typename Storable,
    typename std::enable_if_t<
        std::is_base_of_v<database::Storable, std::decay_t<Storable>>, int> = 0>
void drop_table();

/**
 * @brief to_string function for data enum types
 * @param data_enum DataEnum type that is either a constraint type or sql type
 * @return a string view of the string version of the enum appropriate for a
 *         database
 *
 * Does not convert to exam string value, it converts to the the string
 * that will be used by the database.
 *
 * e.g. NULL_ -> "NULL", PRIMARY_KEY -> "PRIMARY KEY"
 */
template <typename DataEnum,
          typename std::enable_if_t<std::is_enum_v<DataEnum>, int> = 0>
auto enum_to_string(DataEnum const &data_enum) -> std::string_view;

/**
 * @brief Generates new unique ID for the type being asked for
 * @param Storable Any type that is a base of Storable
 * @return A new id that is not being used by the database for this Storable
 *         type
 *
 * Usage:
 * @n int new_id = database::utils::get_new_id<food::Food>();
 */
template <
    typename Storable,
    typename std::enable_if_t<
        std::is_base_of_v<database::Storable, std::decay_t<Storable>>, int> = 0>
auto get_new_id() -> int;

/**
 * @brief Gets data from any storable object and inserts it into relevant
 *        database
 *
 * @param storable The object containing data to be stored in a database. Must
 *                 inherit from Storable.
 *
 * Creates the following SQLite3 command:
 * @n INSERT INTO table1 (
 * @n column1,
 * @n column2 ,..)
 * @n VALUES
 * @n (
 * @n value1,
 * @n value2 ,...);
 *
 * Usage:
 * @n food::Food taco("taco", macros);
 * @n database::utils::insert(taco);
 */
template <
    typename Storable,
    typename std::enable_if_t<
        std::is_base_of_v<database::Storable, std::decay_t<Storable>>, int> = 0>
void insert(Storable const &storable);

/**
 * @brief Generates a new Storable object stores it in the cache, inserts it
 *        into the database, and returns a reference to tht new object.
 *
 * @param The arguments to construct the storable object.
 * @return A reference to the new object, copies are not allowed.
 *
 * Usage:
 * @n auto &some_food = database::utils::make<food::Food>("taco", macros);
 */
template <
    typename Storable, typename... Args,
    typename std::enable_if_t<
        std::is_base_of_v<database::Storable, std::decay_t<Storable>>, int> = 0>
auto make(Args &&... args) -> Storable &;

/**
 * @brief Retrieves all database objects that match the Storable that is passed
 * in
 * @param Storable The type of storable object being retrieved
 * @return A reference to the vector containing all the storable objects of tht
 *         type
 *
 * Retrieves all objects of the type requested that contain that name.
 *
 * Creates the following SQLite3 command:
 * @n SELECT * from Storable;
 *
 * Note: Copies of storable objects not allowed
 *
 * Usage:
 * @n auto all_food = database::utils::retrieve_all<food::Food>();
 * @n for(auto const &food: all_food) {
 * @n   // do something
 * @n }
 */
template <
    typename Storable,
    typename std::enable_if_t<
        std::is_base_of_v<database::Storable, std::decay_t<Storable>>, int> = 0>
auto retrieve_all() -> std::vector<Storable, struct Storable::Allocator> &;

/**
 * @brief Check if Storable table exists in database
 * @param Storable Any type that is a base of Storable
 * @return true if the table exists, false otherwise
 *
 * Creates the following SQLite3 command:
 * @n SELECT name FROM sqlite_master WHERE type='table' AND name='table_name';
 *
 * Usage:
 * @n if(table_exists<food::Food>()) {
 * @n   // do something
 * @n }
 */
template <
    typename Storable,
    typename std::enable_if_t<
        std::is_base_of_v<database::Storable, std::decay_t<Storable>>, int> = 0>
auto table_exists() -> bool;

/**
 * @brief Converts a type to a string and trims the namespaces off.
 *                 (e.g. namespace::other_namespace::ClassName -> "ClassName")
 *
 * @param T The template parameter T could be any possible type.
 * @return A std::string of that type passed in as a template parameter
 *
 * Usage:
 * @n std::string type_string = database::utils::type_to_string<food::Food>();
 * @n type_string == "Food" // true
 */
template <typename T> auto type_to_string() -> std::string;

/**
 * @brief Updates the Storable objects data within the database
 * @param storable A storable object that already exists within the database
 *                 and will update the data
 *
 * Creates the following SQLite3 command:
 * @n UPDATE Storable
 * @n SET column_1 = new_value_1,
 * @n     colimn_2 = new_value_2,
 * @n     ...
 * @n WHERE Storable_id = Storable_id_value;
 *
 * Usage:
 * @n auto all_food = database::utils::retrieve_all<food::Food>();
 * @n if(all_food && all_food.size() > 0) {
 * @n   auto my_food = all_food.back();
 * @n   food.set_protein(100);
 * @n   database::utils::update(my_food);
 * @n  }
 *
 * Will throw a runtime error if food is not in the database!
 */
template <
    typename Storable,
    typename std::enable_if_t<
        std::is_base_of_v<database::Storable, std::decay_t<Storable>>, int> = 0>
void update(Storable const &storable);

/**
 * @brief Updates the Storable objects data within the database
 * @param handler A handler that will handle for the row_data_t once
 *                it passes the data into it.
 *
 * @param row_data A std::variant containing multiple data types. See
 *                 Row implementation or the implementation of this
 *                 function for all the types it contains.
 *
 * Usage:
 * @n std::stringstream column_values;
 * @n utils::visit_row_data(
 * @n     [&column_values = column_values](auto const &row_data) {
 * @n       column_values << row_data;
 * @n     },
 * @n     row_data);
 *
 * In this example we're using a capture initializer to prevent a dangling
 * reference.
 */
template <typename Lambda>
void visit_row_data(Lambda const &handler, Row::row_data_t const &row_data);

} // namespace utils
} // namespace database

#include "utils.tpp"
