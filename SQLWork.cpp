#include "SQLWork.h"

SQL::SQL(std::string connect) {
	try
	{
		this->c = new pqxx::connection(connect);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
};

SQL::~SQL() {
	delete c;
};

void SQL::add_db() {
	pqxx::work tx{ *c };
	try
	{
		tx.exec("CREATE TABLE IF NOT EXISTS clients (ID SERIAL PRIMARY KEY, FIRST_NAME VARCHAR(30) NOT NULL, SECOND_NAME VARCHAR(30) NOT NULL);\
			CREATE TABLE IF NOT EXISTS clients_emails(ID SERIAL PRIMARY KEY, CLIENT_ID INTEGER REFERENCES clients(ID), EMAIL VARCHAR(30)); \
			CREATE TABLE IF NOT EXISTS clients_phones(ID SERIAL PRIMARY KEY, CLIENT_ID INTEGER REFERENCES clients(ID), TELEPHONE VARCHAR(30)); \
		");
		tx.commit();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
};

void SQL::add_client(std::string first_name, std::string second_name, std::string email, std::string phone) {
	pqxx::work tx{ *c };
	try
	{
		tx.exec("INSERT INTO clients(FIRST_NAME, SECOND_NAME) VALUES('" + tx.esc(first_name) + "', '" + tx.esc(second_name) + "');");
		tx.commit();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
};

void SQL::add_number(std::string first_name, std::string second_name, std::string phone) {
	pqxx::work tx{ *c };
	try
	{
		tx.exec("INSERT INTO clients_phones(CLIENT_ID, TELEPHONE) VALUES((select id from clients where first_name = '" + tx.esc(first_name) + "' AND second_name = '" + tx.esc(second_name) + "'), '" + tx.esc(phone) + "');");
		tx.commit();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
};

void SQL::add_email(std::string first_name, std::string second_name, std::string email) {
	pqxx::work tx{ *c };
	try
	{
		tx.exec("INSERT INTO clients_emails(CLIENT_ID, EMAIL) VALUES((select id from clients where first_name = '" + tx.esc(first_name) + "' AND second_name = '" + tx.esc(second_name) + "'), '" + tx.esc(email) + "');");
		tx.commit();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
};

void SQL::delete_number(std::string phone) {
	pqxx::work tx{ *c };
	try
	{
		tx.exec("DELETE FROM clients_phones WHERE TELEPHONE = '" + tx.esc(phone) + "';");
		tx.commit();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
};

void SQL::delete_client(std::string first_name, std::string second_name){
	pqxx::work tx{ *c };
	try
	{
		tx.exec("DELETE FROM clients_phones WHERE CLIENT_ID = (select id from clients where first_name = '" + tx.esc(first_name) + "' AND second_name = '" + tx.esc(second_name) + "'); \
			DELETE FROM clients_emails WHERE CLIENT_ID = (select id from clients where first_name = '" + tx.esc(first_name) + "' AND second_name = '" + tx.esc(second_name) + "');\
			DELETE FROM clients WHERE FIRST_NAME = '" + tx.esc(first_name) + "' AND SECOND_NAME = '" + tx.esc(second_name) + "'; \
	");
		tx.commit();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
};

void SQL::update_client(std::string wheres, std::string first_name, std::string second_name, std::string what) {
	pqxx::work tx{ *c };
	std::string sql;

	if (wheres == "first name") {
		sql = "UPDATE clients SET first_name = '" + tx.esc(what) + "' WHERE first_name = '" + tx.esc(first_name) + "' AND second_name = '" + tx.esc(second_name) + "' ; ";
	}

	if (wheres == "second name") {
		sql = "UPDATE clients SET second_name = '" + tx.esc(what) + "' WHERE first_name = '" + tx.esc(first_name) + "' AND second_name = '" + tx.esc(second_name) + "' ; ";
	}

	if (wheres == "telephone") {
		sql = "UPDATE clients_phones SET TELEPHONE = '" + tx.esc(what) + "' WHERE TELEPHONE = first_name AND CLIENT_ID = (select id from clients where SECONDNAME = '" + tx.esc(second_name) + "' ; ";
	}

	if (wheres == "email") {
		sql = "UPDATE clients_emails SET EMAIL = '" + tx.esc(what) + "' WHERE EMAIL = first_name AND CLIENT_ID = (select id from clients where SECONDNAME = '" + tx.esc(second_name) + "' ; ";
	}

	try
	{
		tx.exec(sql);
		tx.commit();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

};

void SQL::find_client(std::string how, std::string what) {
	pqxx::work tx{ *c };

	if (how == "first_name") {
		try
		{
			for (auto [first_name, second_name, phone, email] : tx.query<std::string, std::string, std::string, std::string>(
				"SELECT first_name, second_name, telephone, email FROM clients" \
				" LEFT JOIN clients_phones ON clients.ID = clients_phones.CLIENT_ID LEFT JOIN clients_emails ON clients.ID = clients_emails.CLIENT_ID" \
				" WHERE clients.first_name = '" + tx.esc(what) + "'"))
			{
				std::cout << first_name << " " << second_name << " " << phone << " " << email << "\n";
			}
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}

	if (how == "second_name") {
		try
		{
			for (auto [first_name, second_name, phone, email] : tx.query<std::string, std::string, std::string, std::string>(
				"SELECT first_name, second_name, telephone, email FROM clients " \
				"LEFT JOIN clients_phones ON clients.ID = clients_phones.CLIENT_ID LEFT JOIN clients_emails ON clients.ID = clients_emails.CLIENT_ID " \
				"WHERE second_name = '" + tx.esc(what) + "';"))
			{
				std::cout << first_name << " " << second_name << " " << phone << " " << email << "\n";
			}
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}

	if (how == "telephone") {
		try
		{
			for (auto [first_name, second_name, phone, email] : tx.query<std::string, std::string, std::string, std::string>(
				"SELECT first_name, second_name, telephone, email FROM clients " \
				"LEFT JOIN clients_phones ON clients.ID = clients_phones.CLIENT_ID LEFT JOIN clients_emails ON clients.ID = clients_emails.CLIENT_ID " \
				"WHERE telephone = '" + tx.esc(what) + "';"))
			{
				std::cout << first_name << " " << second_name << " " << phone << " " << email << "\n";
			}
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}

	if (how == "email") {
		try
		{
			for (auto [first_name, second_name, phone, email] : tx.query<std::string, std::string, std::string, std::string>(
				"SELECT first_name, second_name, telephone, email FROM clients " \
				"LEFT JOIN clients_phones ON clients.ID = clients_phones.CLIENT_ID LEFT JOIN clients_emails ON clients.ID = clients_emails.CLIENT_ID " \
				"WHERE email = '" + tx.esc(what) + "';"))
			{
				std::cout << first_name << " " << second_name << " " << phone << " " << email << "\n";
			}
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}

	}
};
