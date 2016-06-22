#include "Poco/CppUnit/TestCase.h"
#include "SQLExecutor.h"
#include "Poco/String.h"
#include "Poco/ByteOrder.h"
#include "Poco/Format.h"
#include "Poco/Tuple.h"
#include "Poco/Types.h"
#include "Poco/DateTime.h"
#include "Poco/Any.h"
#include "Poco/Exception.h"
#include "Poco/Data/LOB.h"
#include "Poco/Data/Date.h"
#include "Poco/Data/Time.h"
#include "Poco/Data/StatementImpl.h"
#include "Poco/Data/RecordSet.h"
#include "Poco/Data/Transaction.h"
#include "Poco/Data/Sybase/SybaseException.h"

#include <iostream>
#include <limits>


using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using Poco::Data::Sybase::SybaseException;
using Poco::Data::Sybase::ConnectionException;
using Poco::Data::Sybase::StatementException;
using Poco::format;
using Poco::ByteOrder;
using Poco::Tuple;
using Poco::DateTime;
using Poco::Any;
using Poco::AnyCast;
using Poco::NotFoundException;
using Poco::InvalidAccessException;
using Poco::BadCastException;
using Poco::RangeException;


SQLExecutor::SQLExecutor(const std::string& name, Poco::Data::Session* session): 
	CppUnit::TestCase(name),
	_session(session)
{ }


SQLExecutor::~SQLExecutor()
{ }


void SQLExecutor::simpleAccess()
{
	const std::string func = "simpleAccess()";
	const std::string expectedLastName = "Simpson";
	const int expectedAge = 12;
	int intResult = 0;
	std::string strResult;

	// Insert single row
	try { *_session << "insert into person values('Simpson', 'Bart', 'Springfield', 12)", now; }
	catch ( const ConnectionException& ce ) { std::cout << ce.displayText() << std::endl; fail(func); }
	catch ( const StatementException& se ) { std::cout << se.displayText() << std::endl; fail(func); }

	// Query row count
	try { *_session << "select count(*) from person", into(intResult), now; }
	catch ( const ConnectionException& ce ) { std::cout << ce.displayText() << std::endl; fail(func); }
	catch ( const StatementException& se ) { std::cout << se.displayText() << std::endl; fail(func); }
	assert(intResult == 1);

	// Verify lastName
	try { *_session << "select lastName from person", into(strResult), now; }
	catch ( const ConnectionException& ce ) { std::cout << ce.displayText() << std::endl; fail(func); }
	catch ( const StatementException& se ) { std::cout << se.displayText() << std::endl; fail(func); }
	assert(strResult == expectedLastName);

	// Verify age
	try { *_session << "select age from person", into(intResult), now; }
	catch ( const ConnectionException& ce ) { std::cout << ce.displayText() << std::endl; fail(func); }
	catch ( const StatementException& se ) { std::cout << se.displayText() << std::endl; fail(func); }
	assert(intResult == expectedAge);
}
