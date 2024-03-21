#include <pqxx/pqxx>
#include "SQLWork.h"
#include <Windows.h>
#pragma execution_character_set( "utf-8" )

int main()
{
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	setvbuf(stdout, nullptr, _IOFBF, 1000);

	SQL db_conn("user=postgres password=postgres host=127.0.0.1 port=5432 dbname=test");

	db_conn.add_db();
	db_conn.add_client("Sergey", "Belousov", "sb@gm.com", "+58159345508");
	db_conn.add_client("Petr", "Brinza", "pb@gm.com", "+581593455123");
	db_conn.add_client("Igor", "Ivanov", "ii@gm.com", "+581593455152");

	db_conn.add_number("Sergey", "Belousov", "+58159345509");
	db_conn.add_number("Sergey", "Belousov", "+58159345510");
	db_conn.add_email("Sergey", "Belousov", "sb2@gm.com");

	db_conn.delete_number("+58159345510");

	db_conn.update_client("first name", "Sergey", "Belousov", "Pavel");

	db_conn.delete_client("Petr", "Brinza");

	db_conn.find_client("first_name", "Pavel");

	return 0;
}