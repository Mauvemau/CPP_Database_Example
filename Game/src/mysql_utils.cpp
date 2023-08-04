#include "mysql_utils.h"

MYSQL* MySQLUtils::connectToDatabase(const char* host, const char* user, const char* password, const char* database, int port) {
    MYSQL* mysql = mysql_init(nullptr);
    if (!mysql) {
        throw runtime_error("Failed to initialize MySQL connection.");
    }

    if (mysql_real_connect(mysql, host, user, password, database, port, nullptr, 0) == nullptr) {
        string errorMessage = "Connection Error: ";
        errorMessage += mysql_error(mysql);
        mysql_close(mysql);
        throw runtime_error(errorMessage);
    }

    return mysql;
}

MYSQL_RES* MySQLUtils::executeQuery(MYSQL* mysql, const char* query) {
    if (mysql_query(mysql, query) != 0) {
        string errorMessage = "Query Execution Error: ";
        errorMessage += mysql_error(mysql);
        throw runtime_error(errorMessage);
    }

    MYSQL_RES* result = mysql_store_result(mysql);
    if (!result && mysql_errno(mysql)) {
        string errorMessage = "Error fetching results: ";
        errorMessage += mysql_error(mysql);
        throw runtime_error(errorMessage);
    }

    return result;
}

string MySQLUtils::escapeString(MYSQL* mysql, const string input) {
    char* escapedBuffer = new char[2 * input.length() + 1];
    mysql_real_escape_string(mysql, escapedBuffer, input.c_str(), static_cast<unsigned long>(input.length()));
    string escapedString(escapedBuffer);
    delete[] escapedBuffer;
    return escapedString;
}