#pragma once
#include <pqxx/pqxx>
#include <iostream>
#include <string>

class SQL {
public:
	pqxx::connection* c;
	SQL(std::string connect);
	~SQL();
	void add_db();
	void add_client(std::string first_name, std::string second_name, std::string email, std::string phone);
	void add_number(std::string first_name, std::string second_name, std::string phone);
	void add_email(std::string first_name, std::string second_name, std::string email);
	void delete_number(std::string phone);
	void delete_client(std::string first_name, std::string second_name);
	void update_client(std::string wheres, std::string first_name, std::string second_name, std::string what);
	void find_client(std::string how, std::string what);
};
