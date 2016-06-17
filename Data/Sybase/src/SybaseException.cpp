#include "Poco/Data/Sybase/SybaseException.h"

namespace Poco {
namespace Data {
namespace Sybase {


SybaseException::SybaseException(const std::string& msg):
	Poco::Data::DataException(std::string("[Sybase]: ") + msg)
{
}


SybaseException::SybaseException(const SybaseException& ex):
	Poco::Data::DataException(ex)
{
}


SybaseException::~SybaseException() throw()
{
}


/////
//
// ConnectionException
//
/////


ConnectionException::ConnectionException(const std::string& msg):
	SybaseException(msg)
{
}


/////
//
// TransactionException
//
/////

TransactionException::TransactionException(const std::string& msg):
	ConnectionException(msg)
{
}


/////
//
// StatementException
//
/////


StatementException::StatementException(const std::string& msg):
	SybaseException(msg)
{
}


} } } // namespace Poco::Data::Sybase
