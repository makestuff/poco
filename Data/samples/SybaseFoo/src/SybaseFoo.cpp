#include "Poco/Data/Sybase/Connector.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/RecordSet.h"
#include <iostream>


using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::Data::RecordSet;


int main(int argc, char* argv[])
{
	try
	{
		// create a session
		const char* const connStr = "SERVER=MOBIX DATABASE=pubs2 CS_USERNAME=sa CS_PASSWORD=mun789";
		Session session(Poco::Data::Sybase::Connector::KEY, connStr);

		// drop sample table, if it exists
		session << "if exists (select 1 from sysobjects where name='person') drop table person", now;

		// (re)create table
		session << "create table person (name varchar(30), address varchar(30), age int, dob date)", now;

		// insert some rows
		session << "insert into person values('Bart Simpson', 'Springfield', 12, '1980-04-01')", now;
		session << "insert into person values('Lisa Simpson', 'Springfield', 10, '1982-05-09')", now;
		session << "insert into person values('Maggie Simpson', 'Springfield', 1, '1986-04-19')", now;
		
		// a simple query
		Statement select(session);
		select << "select name, address, age from person";
		select.execute();

		// create a RecordSet
		RecordSet rs(select);
		const std::size_t cols = rs.columnCount();

		// print all column names
		for ( std::size_t col = 0; col < cols; ++col )
			std::cout << rs.columnName(col) << " ";
		std::cout << std::endl;

		// iterate over all rows and columns
		bool more = rs.moveFirst();
		while ( more )
		{
			for ( std::size_t col = 0; col < cols; ++col )
				std::cout << rs[col].convert<std::string>() << " ";
			std::cout << std::endl;
			more = rs.moveNext();
		}
	}
	catch ( const Poco::Exception& ex )
	{
		std::cerr << ex.message() << std::endl;
	}
	return 0;
}
