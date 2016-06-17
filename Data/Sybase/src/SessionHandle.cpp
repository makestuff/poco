#include "Poco/Data/Sybase/SessionHandle.h"

namespace Poco {
namespace Data {
namespace Sybase {


SessionHandle::SessionHandle(CS_CONTEXT* cslibContext):
	_cslibContext(cslibContext),
	_connection(0),
	_isConnected(false)
{
}


SessionHandle::~SessionHandle()
{
	disconnect();
}


bool SessionHandle::isConnected() const
{
	return isConnectedNoLock();
}


bool SessionHandle::isConnectedNoLock() const
{
	return _isConnected;
}


void SessionHandle::connect(const std::map<std::string, std::string>& optionsMap)
{
	if ( isConnectedNoLock() )
		throw ConnectionFailedException("Already Connected");

	typedef std::map<const std::string, std::string>::const_iterator ConstIter;
	const ConstIter mapEnd = optionsMap.end();
	CS_RETCODE ret = ct_con_alloc(_cslibContext, &_connection);

	// Maybe set username
	const ConstIter usernameIter = optionsMap.find("CS_USERNAME");
	const char* const usernameStr = (usernameIter != mapEnd) ? usernameIter->second.c_str() : 0;
	if ( usernameStr )
		ret = ct_con_props(_connection, CS_SET, CS_USERNAME, (CS_VOID*)usernameStr, CS_NULLTERM, NULL);
	else
		ret = ct_con_props(_connection, CS_CLEAR, CS_USERNAME, (CS_VOID*)0, CS_UNUSED, NULL);

	// Maybe set password
	const ConstIter passwordIter = optionsMap.find("CS_PASSWORD");
	const char* const passwordStr = (passwordIter != mapEnd) ? passwordIter->second.c_str() : 0;
	if ( passwordStr )
		ret = ct_con_props(_connection, CS_SET, CS_PASSWORD, (CS_VOID*)passwordStr, CS_NULLTERM, NULL);
	else
		ret = ct_con_props(_connection, CS_CLEAR, CS_PASSWORD, (CS_VOID*)0, CS_UNUSED, NULL);

	// Connect to the server
	const ConstIter serverIter = optionsMap.find("SERVER");
	if ( serverIter == mapEnd )
		throw ConnectionException("No SERVER specified in connect-string!");

	const char* const serverStr = serverIter->second.c_str();
	ret = ct_connect(_connection, (CS_CHAR*)serverStr, CS_NULLTERM);
	if ( ret == CS_SUCCEED )
		_isConnected = true;
	else
		throw ConnectionFailedException("Connection Error!");
}


void SessionHandle::disconnect()
{
	if ( _connection )
	{
		CS_RETCODE ret;
		if ( _isConnected )
		{
			ret = ct_close(_connection, CS_UNUSED);
			_isConnected = false;
		}
		ret = ct_con_drop(_connection);
		_connection = 0;
		(void)ret;
	}
}


}}} // Poco::Data::Sybase
