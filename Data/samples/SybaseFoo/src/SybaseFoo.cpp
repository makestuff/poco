#include "Poco/SharedPtr.h"
#include "Poco/DateTime.h"
#include "Poco/Data/SessionFactory.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/RecordSet.h"
#include "Poco/Data/Column.h"
#include "Poco/Data/Phylum/Connector.h"
#include <iostream>

using namespace Poco::Data::Keywords;
using namespace std;
using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::Data::RecordSet;

int main(int argc, char* argv[])
{
	string name[2], address[2];
	int age[2];
	//Poco::DateTime dob[2];
	try
	{
		Session session("sybase", "SERVER=MOBIX CS_USERNAME=sa CS_PASSWORD=mun789");
		
		//session << "drop table if exists person", now;
		//session << "create table person (name varchar(30), address varchar(30), age int, dob date)", now;

		//DateTime bd(1980, 4, 1);
		//DateTime ld(1982, 5, 9);
		//session << "insert into person values('Bart Simpson', 'Springfield', 12, $1)", use(bd), now;
		//session << "insert into person values('Lisa Simpson', 'Springfield', 10, $1)", use(ld), now;
		
		Statement select(session);
		select << "select name, address, age from pubs2..person";
		select.execute();
		
		RecordSet rs(select);
		//const size_t cols = rs.columnCount();
		rs.moveFirst();
		name[0] = rs[0].convert<string>();
		address[0] = rs[1].convert<string>();
		age[0] = rs[2].convert<Poco::Int32>();
		//dob[0] = rs[3].convert<Poco::DateTime>();
		rs.moveNext();
		name[1] = rs[0].convert<string>();
		address[1] = rs[1].convert<string>();
		age[1] = rs[2].convert<Poco::Int32>();
		//dob[1] = rs[3].convert<Poco::DateTime>();
		for ( int i = 0; i < 2; ++i )
			cout << name[i] << " " << address[i] << " " << age[i] << "\n";
	}
	catch ( const Poco::Exception& ex )
	{
		cerr << ex.message() << endl;
	}
	return 0;
}
