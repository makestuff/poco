#ifndef Data_Sybase_SessionImpl_INCLUDED
#define Data_Sybase_SessionImpl_INCLUDED


#include "Poco/Data/Sybase/Sybase.h"
#include "Poco/Data/Sybase/SessionHandle.h"
#include "Poco/Data/AbstractSessionImpl.h"
#include <string>


namespace Poco {
namespace Data {
namespace Sybase {


class Sybase_API SessionImpl: public Poco::Data::AbstractSessionImpl<SessionImpl>
	/// Implements SessionImpl interface
{
public:
	SessionImpl(
		const std::string& connStr,
		CS_CONTEXT* ctlibContext,
		std::size_t timeout = LOGIN_TIMEOUT_DEFAULT);
		/// Creates the SessionImpl. Opens a connection to the database.
		///
		/// Connection string format:
		/// <connStr> == <assignment> | <assignment> ' ' <connStr>
		/// <assignment> == <name> '=' <value>
		/// <name> == 'CS_USERNAME' | 'CS_PASSWORD' | 'SERVER'
		/// <value> == [~;]*

	~SessionImpl();
		/// Destroys the SessionImpl.

	void setConnectionTimeout(std::size_t timeout);
		/// Sets the session connection timeout value.

	std::size_t getConnectionTimeout();
		/// Returns the session connection timeout value.

	void open(const std::string& connStr = std::string());
		/// Opens a connection to the database.

	void close();
		/// Closes the connection.

	bool isConnected();
		/// Returns true if connected, false otherwise.

	Poco::Data::StatementImpl* createStatementImpl();
		/// Returns an Sybase StatementImpl

	void begin();
		/// Starts a transaction

	void commit();
		/// Commits and ends a transaction		

	void rollback();
		/// Aborts a transaction

	bool canTransact();
		/// Returns true if session has transaction capabilities.

	bool isTransaction();
		/// Returns true iff a transaction is a transaction is in progress, false otherwise.

	void setTransactionIsolation(Poco::UInt32 x);
		/// Sets the transaction isolation level.

	Poco::UInt32 getTransactionIsolation();
		/// Returns the transaction isolation level.

	bool hasTransactionIsolation(Poco::UInt32 mask);
		/// Returns true iff the transaction isolation level corresponding
		/// to the supplied bitmask is supported.

	bool isTransactionIsolation(Poco::UInt32 mask);
		/// Returns true iff the transaction isolation level corresponds
		/// to the supplied bitmask.

	void setAutoCommit(const std::string&, bool val);
		/// Sets autocommit property for the session.

	bool isAutoCommit(const std::string& name = std::string());
		/// Returns autocommit property value.

	void setAsynchronousCommit(const std::string&, bool val);
		/// Sets asynchronousCommit property for the session.

	bool isAsynchronousCommit(const std::string& name = std::string());
		/// is the connection in Asynchronous commit mode?

	SessionHandle& handle();
		/// Get handle

	const std::string& connectorName() const;
		/// Returns the name of the connector.

private:
	std::string		_connectorName;
	SessionHandle	_sessionHandle;

	std::size_t		_timeout;
};


//
// inlines
//
inline bool SessionImpl::canTransact()
{
	return true;
}

inline SessionHandle& SessionImpl::handle()
{
	return _sessionHandle;
}

inline const std::string& SessionImpl::connectorName() const
{
	return _connectorName;
}

inline bool SessionImpl::isTransactionIsolation(Poco::UInt32 mask)
{
	return getTransactionIsolation() == mask;
}
	

inline std::size_t SessionImpl::getConnectionTimeout()
{
	return _timeout;
}


} } } // namespace Poco::Data::Sybase


#endif // Data_Sybase_SessionImpl_INCLUDED
