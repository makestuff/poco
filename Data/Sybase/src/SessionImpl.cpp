#include "Poco/Data/Sybase/SessionImpl.h"
#include "Poco/Data/Sybase/SybaseException.h"
#include "Poco/Data/Sybase/SybaseStatementImpl.h"
#include "Poco/Data/Session.h"
#include "Poco/NumberParser.h"
#include "Poco/String.h"
#include <map>


namespace
{
	// TODO: this was lifted from the PostgreSQL code; can it be shared?
	std::string copyStripped(std::string::const_iterator from, std::string::const_iterator to)
	{
		// skip leading spaces
		while ( from != to && isspace(*from) )
			from++;

		// skip trailing spaces
		while ( to != from && isspace(to[-1]) )
			to--;

		// Return what remains
		return std::string(from, to);
	}
}


namespace Poco {
namespace Data {
namespace Sybase {


SessionImpl::SessionImpl(
	const std::string& connStr,
	CS_CONTEXT* cslibContext,
	std::size_t timeout
):
	Poco::Data::AbstractSessionImpl<SessionImpl>(connStr, timeout),
	_sessionHandle(cslibContext),
	_timeout(timeout)
{
	setProperty("handle", static_cast<SessionHandle*>(&_sessionHandle));
	open();
}

	
SessionImpl::~SessionImpl()
{
	try
	{
		close();
	}
	catch ( ... )
	{
	}
}


void SessionImpl::setConnectionTimeout(std::size_t timeout)
{
	_timeout = timeout;
}


void SessionImpl::open(const std::string& connStr)
{
	if ( connectionString() != connStr )
	{
		if ( isConnected() )
			throw ConnectionException("Session already connected");
		if ( !connStr.empty() )
			setConnectionString(connStr);
	}

	poco_assert_dbg(!connectionString().empty());
	
	std::map<std::string, std::string> optionsMap;
	const std::string& connString = connectionString();

	for ( std::string::const_iterator start = connString.begin(); ; )
	{
		const std::string::const_iterator finish = std::find(start, connString.end(), ' '); // space is the separator between keyword=value pairs
		const std::string::const_iterator middle = std::find(start, finish, '=');

		if ( middle == finish )
			throw SybaseException("create session: bad connection string format, cannot find '='");

		optionsMap[copyStripped(start, middle)] = copyStripped(middle + 1, finish);

		if	( (finish == connString.end()) || (finish + 1 == connString.end()) )
			break;

		start = finish + 1;
	}

	// Real connect
	_sessionHandle.connect(optionsMap);

	addFeature("autoCommit",
		&SessionImpl::setAutoCommit,
		&SessionImpl::isAutoCommit);

	addFeature("asynchronousCommit",
		&SessionImpl::setAutoCommit,
		&SessionImpl::isAutoCommit);
}


void SessionImpl::close()
{
	if ( isConnected() )
		_sessionHandle.disconnect();
}


bool SessionImpl::isConnected()
{
   return _sessionHandle.isConnected();
}


Poco::Data::StatementImpl* SessionImpl::createStatementImpl()
{
	return dynamic_cast<Poco::Data::StatementImpl*>(new SybaseStatementImpl(*this));
}


bool SessionImpl::isTransaction()
{
	// TODO: implement this
	return false; //_sessionHandle.isTransaction();
}


void SessionImpl::begin()
{
	if ( isTransaction() )
		throw Poco::InvalidAccessException("Already in transaction.");

	//_sessionHandle.startTransaction();
}


void SessionImpl::commit()
{
	// Not an error to issue a COMMIT without a preceeding BEGIN
	//_sessionHandle.commit();
}


void SessionImpl::rollback()
{
	// Not an error to issue a ROLLBACK without a preceeding BEGIN
	//_sessionHandle.rollback();
}


void SessionImpl::setAutoCommit(const std::string&, bool)
{
	//_sessionHandle.setAutoCommit(val);
}


bool SessionImpl::isAutoCommit(const std::string&)
{
	return false; //_sessionHandle.isAutoCommit();
}

   
void SessionImpl::setAsynchronousCommit(const std::string&, bool)
{
	//_sessionHandle.setAsynchronousCommit(val);
}


bool SessionImpl::isAsynchronousCommit(const std::string&)
{
	return false; //_sessionHandle.isAsynchronousCommit();
}


void SessionImpl::setTransactionIsolation(Poco::UInt32)
{
	//_sessionHandle.setTransactionIsolation(val);
}


Poco::UInt32 SessionImpl::getTransactionIsolation()
{
	// TODO: implement
	return 23; //_sessionHandle.transactionIsolation();
}


bool SessionImpl::hasTransactionIsolation(Poco::UInt32)
{
	return false; //_sessionHandle.hasTransactionIsolation(mask);
}


}}}
