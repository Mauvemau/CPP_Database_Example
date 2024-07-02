#pragma once

#include <iostream>
#include "mysql/mysql.h"

using namespace std;

/// <summary>
/// Tools and functions for MySQL.
/// </summary>
class MySQLUtils {
public:
	/// <summary>
	/// Connects to a database.
	/// </summary>
	/// <param name="host"> IP of the database server </param>
	/// <param name="user"> Username needed to access the server </param>
	/// <param name="password"> Password needed to access the server </param>
	/// <param name="database"> Name of the database to be used </param>
	/// <param name="port"> Port of the database server </param>
	static MYSQL* connectToDatabase(const char* host, const char* user, const char* password, const char* database, int port);
	/// <summary>
	/// Performs a query in the specified database, handles exceptions.
	/// </summary>
	static MYSQL_RES* executeQuery(MYSQL* mysql, const char* query);
	/// <summary>
	/// Escapes a string, should be used on values to be chained into a query string.
	/// </summary>
	static string escapeString(MYSQL* mysql, const string input);
};
