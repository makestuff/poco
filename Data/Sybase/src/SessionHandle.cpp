#include <sstream>
#include "Poco/Data/Sybase/SessionHandle.h"
#include "CTLibNames.h"

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


#define CHECK_STATUS(funcName) \
	if ( ret != CS_SUCCEED ) \
	{ \
		std::ostringstream msg; \
		if ( cmd ) \
			ct_cmd_drop(cmd); \
		msg << funcName " failed: ret=" << ret;	\
		throw ConnectionFailedException(msg.str()); \
	}

#define RESULT_EXPECT(funcName, expRet, expResultType) \
	if ( ret != expRet || resultType != expResultType ) \
	{ \
		std::ostringstream msg; \
		if ( cmd ) \
			ct_cmd_drop(cmd); \
		msg << funcName " failed: ret=" << ret << "; resultType=" << resultTypeName(resultType); \
		throw ConnectionFailedException(msg.str()); \
	}

void SessionHandle::use(const std::string& dbName) const
{
	std::ostringstream sql;
	sql << "use " << dbName;
	CS_COMMAND* cmd = 0;
	CS_INT resultType;
	CS_RETCODE ret = ct_cmd_alloc(_connection, &cmd);
	CHECK_STATUS("ct_cmd_alloc()");
	ret = ct_command(cmd, CS_LANG_CMD, const_cast<CS_CHAR*>(sql.str().c_str()), CS_NULLTERM, CS_UNUSED);
	CHECK_STATUS("ct_command()");
	ret = ct_send(cmd);
	CHECK_STATUS("ct_send()");
	ret = ct_results(cmd, &resultType);
	RESULT_EXPECT("[1]ct_results(USE_DB)", CS_SUCCEED, CS_CMD_SUCCEED);
	ret = ct_results(cmd, &resultType);
	RESULT_EXPECT("[2]ct_results(USE_DB)", CS_SUCCEED, CS_CMD_DONE);
	ret = ct_results(cmd, &resultType);
	RESULT_EXPECT("[3]ct_results(USE_DB)", CS_END_RESULTS, CS_CMD_DONE);
	ct_cmd_drop(cmd);
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

	// Maybe switch databases
	const ConstIter databaseIter = optionsMap.find("DATABASE");
	if ( databaseIter != mapEnd )
		use(databaseIter->second);
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
