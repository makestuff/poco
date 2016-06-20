#include "Poco/Data/Sybase/StatementExecutor.h"
#include "Poco/Format.h"
#include "Poco/UUID.h"
#include "Poco/UUIDGenerator.h"
#include "Poco/NumberParser.h"
#include "Poco/NumberParser.h"
#include "Poco/RegularExpression.h"  // TODO: remove after C++ 11 implementation

//#include <regex> // saved for C++ 11 implementation
#include <algorithm>
#include <set>
#include <sstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include "CTLibNames.h"

namespace
{
	const char* const stateNames[] =
	{
		"STMT_INITED",
		"STMT_COMPILED",
		"STMT_EXECUTED"
	};

	Poco::Data::MetaColumn::ColumnDataType mapType(CS_INT ctlibType)
	{
		// See table at ctref.pdf:306
		switch ( ctlibType )
		{
		case CS_CHAR_TYPE:
			return Poco::Data::MetaColumn::FDT_STRING;
		case CS_INT_TYPE:
			return Poco::Data::MetaColumn::FDT_INT32;
		default:
			// TODO: how to avoid std::cerr here?
			std::cerr << Poco::Data::Sybase::dataTypeName(ctlibType) << " is unsupported!\n";
			return Poco::Data::MetaColumn::FDT_UNKNOWN;
		}
	}
} // namespace

namespace Poco {
namespace Data {
namespace Sybase {


#define CHECK_STATUS(funcName) \
	if ( ret != CS_SUCCEED ) \
	{ \
		std::ostringstream msg; \
		msg << funcName " failed: ret=" << ret;	\
		throw StatementException(msg.str()); \
	}

#define RESULT_EXPECT(funcName, expRet, expResultType) \
	if ( ret != expRet || resultType != expResultType ) \
	{ \
		std::ostringstream msg; \
		msg << funcName " failed: ret=" << ret << "; resultType=" << resultTypeName(resultType); \
		throw StatementException(msg.str()); \
	}


StatementExecutor::StatementExecutor(SessionHandle& sessionHandle):
	_sessionHandle(sessionHandle),
	_state(STMT_INITED),
	_query(""),
	_affectedRowCount(0),
	_command(0)
{
	CS_CONNECTION* const conn = _sessionHandle.getConnection();
	const CS_RETCODE ret = ct_cmd_alloc(conn, &_command);
	CHECK_STATUS("ct_cmd_alloc()");
}


StatementExecutor::~StatementExecutor()
{
	if ( _command )
	{
		CS_INT resultType;
		CS_CHAR* const dynStmt = const_cast<CS_CHAR*>("s0000");
		CS_RETCODE ret;
		ct_dynamic(_command, CS_DEALLOC, (CS_CHAR*)dynStmt, CS_NULLTERM, NULL, CS_UNUSED);
		ct_send(_command);
		ret = ct_results(_command, &resultType);
		if ( ret == CS_SUCCEED && resultType == CS_CMD_SUCCEED )
		{
			ret = ct_results(_command, &resultType);
			if ( ret == CS_SUCCEED && resultType == CS_CMD_DONE )
				ct_results(_command, &resultType);
		}
		ct_cmd_drop(_command);
	}
}


StatementExecutor::State StatementExecutor::state() const
{
	return _state;
}
	
void StatementExecutor::prepare(const std::string& query)
{
	CS_RETCODE ret;
	CS_INT resultType = CS_CMD_FAIL;
	CS_CHAR* const dynStmt = const_cast<CS_CHAR*>("s0000");
	CS_CHAR* const queryText = const_cast<CS_CHAR*>(query.c_str());
	CS_INT numColumns, i;
	
	if ( !_sessionHandle.isConnected() )
		throw NotConnectedException();
	if ( _state != STMT_INITED )
	{
		std::ostringstream msg;
		msg << "Poco::Data::Sybase::StatementExecutor::prepare() called in illegal state: " << stateNames[_state];
		throw StatementException(msg.str());
	}

	clearResults();
	_colPocoFmt.clear();

	// TODO: need to lock a mutex, and supply a UUID for the query name rather than "s0000"
	ret = ct_dynamic(_command, CS_PREPARE, dynStmt, CS_NULLTERM, queryText, CS_NULLTERM);
	CHECK_STATUS("ct_dynamic(CS_PREPARE)");
	ret = ct_send(_command);
	CHECK_STATUS("ct_send()");
	ret = ct_results(_command, &resultType);
	RESULT_EXPECT("[1]ct_results(CS_PREPARE)", CS_SUCCEED, CS_CMD_SUCCEED);
	ret = ct_results(_command, &resultType);
	RESULT_EXPECT("[2]ct_results(CS_PREPARE)", CS_SUCCEED, CS_CMD_DONE);
	ret = ct_results(_command, &resultType);
	RESULT_EXPECT("[3]ct_results(CS_PREPARE)", CS_END_RESULTS, CS_CMD_DONE);
	
	// Describe output
	ret = ct_dynamic(_command, CS_DESCRIBE_OUTPUT, dynStmt, CS_NULLTERM, NULL, CS_UNUSED);
	CHECK_STATUS("ct_dynamic(CS_DESCRIBE_OUTPUT)");
	ret = ct_send(_command);
	CHECK_STATUS("ct_send(CS_DESCRIBE_OUTPUT)");
	ret = ct_results(_command, &resultType);
	RESULT_EXPECT("[1]ct_results(CS_DESCRIBE_OUTPUT)", CS_SUCCEED, CS_DESCRIBE_RESULT);
	ret = ct_res_info(_command, CS_NUMDATA, &numColumns, sizeof(numColumns), NULL);
	CHECK_STATUS("ct_res_info()");
	_colSybFmt.resize(numColumns);
	_rowData.resize(numColumns);
	_rowNumBytes.resize(numColumns);
	for ( i = 0; i < numColumns; ++i )
	{
		CS_DATAFMT& colFmt = _colSybFmt[i];
		ret = ct_describe(_command, i+1, &colFmt);
		CHECK_STATUS("ct_describe()");
		_colPocoFmt.push_back(
			MetaColumn(
				i,                            // index
				colFmt.name,                  // name
				mapType(colFmt.datatype),     // type
				colFmt.maxlength,             // number of bytes
				colFmt.precision,             // number of decimal digits
				colFmt.status | CS_CANBENULL  // nullable?
			)
		);
		_rowData[i] = std::malloc(colFmt.maxlength + 1);  // TODO: free this
	}
	ret = ct_results(_command, &resultType);
	RESULT_EXPECT("[2]ct_results(CS_DESCRIBE_OUTPUT)", CS_SUCCEED, CS_CMD_DONE);
	ret = ct_results(_command, &resultType);
	RESULT_EXPECT("[3]ct_results(CS_DESCRIBE_OUTPUT)", CS_END_RESULTS, CS_CMD_DONE);
	_query = query;
	_state = STMT_COMPILED;
}


void StatementExecutor::execute()
{
	CS_RETCODE ret;
	CS_CHAR* const dynStmt = const_cast<CS_CHAR*>("s0000");
	CS_INT resultType = CS_CMD_FAIL;
	const std::size_t countColumns = _colPocoFmt.size();
	if ( !_sessionHandle.isConnected() )
		throw NotConnectedException();
	if ( _state != STMT_COMPILED )
	{
		std::ostringstream msg;
		msg << "Poco::Data::Sybase::StatementExecutor::execute() called in illegal state: " << stateNames[_state];
		throw StatementException(msg.str());
	}

	// Execute the previously-prepared statement
	ret = ct_dynamic(_command, CS_EXECUTE, dynStmt, CS_NULLTERM, NULL, CS_UNUSED);
	CHECK_STATUS("ct_dynamic(CS_EXECUTE)");
	ret = ct_send(_command);
	CHECK_STATUS("ct_send(CS_EXECUTE)");
	ret = ct_results(_command, &resultType);
	CHECK_STATUS("[1]ct_results(CS_EXECUTE)");
	if ( resultType == CS_ROW_RESULT )
	{
		// Got some rows; bind variables ready for fetching. The remaining ct_results() will be gathered in fetch().
		for ( std::size_t i = 0; i < countColumns; ++i )
		{
			CS_DATAFMT& colFmt = _colSybFmt[i];
			CS_VOID* const colData = _rowData[i];
			CS_INT& colNumBytes = _rowNumBytes[i];
			ret = ct_bind(_command, i+1, &colFmt, colData, &colNumBytes, NULL);  // TODO: NULL-indicator
			CHECK_STATUS("ct_bind()");
		}
	}
	else if ( resultType == CS_CMD_SUCCEED )
	{
		// No rows returned, therefore fetch() will NOT be called, so cleanup here
		CS_INT rowCount;
		ret = ct_res_info(_command, CS_ROW_COUNT, &rowCount, sizeof(rowCount), NULL);
		_affectedRowCount = static_cast<std::size_t>(rowCount);
		CHECK_STATUS("ct_res_info()");
		ret = ct_results(_command, &resultType);
		RESULT_EXPECT("[2]ct_results(CS_EXECUTE)", CS_SUCCEED, CS_CMD_DONE);
		ret = ct_results(_command, &resultType);
		RESULT_EXPECT("[3]ct_results(CS_EXECUTE)", CS_END_RESULTS, CS_CMD_DONE);
	}
	else
	{
		// We're expecting either CS_ROW_RESULT or CS_CMD_SUCCEED; anything else is an error
		std::ostringstream msg;
		msg << "[1]ct_results(CS_EXECUTE) misbehaving: resultType=" << resultTypeName(resultType);
		throw StatementException(msg.str());
	}
	_state = STMT_EXECUTED;
}


bool StatementExecutor::fetch()
{
	if ( !_sessionHandle.isConnected() )
		throw NotConnectedException();
	if ( _state != STMT_EXECUTED )
	{
		std::ostringstream msg;
		msg << "Poco::Data::Sybase::StatementExecutor::fetch() called in illegal state: " << stateNames[_state];
		throw StatementException(msg.str());
	}

	CS_RETCODE ret = ct_fetch(_command, CS_UNUSED, CS_UNUSED, CS_UNUSED, NULL);
	if ( ret == CS_FAIL )
		throw StatementException("ct_fetch() returned CS_FAIL");
	else if ( ret == CS_FAIL || ret == CS_CANCELED )
		throw StatementException("ct_fetch() returned CS_CANCELED");
	else if ( ret != CS_END_DATA )
	{
		// Not yet at the end, so this is a real row
		const std::size_t numColumns = _colSybFmt.size();
		for ( std::size_t i = 0; i < numColumns; ++i )
		{
			if ( _colSybFmt[i].datatype == CS_CHAR_TYPE )
			{
				char* const buffer = (char*)_rowData[i];
				const CS_INT len = _rowNumBytes[i];
				buffer[len] = '\0';  // add NUL-terminator
			}
		}
		return true;  // there may be more data
	}
	else
	{
		// We're at the end of this result-set
		CS_INT resultType;
		ret = ct_results(_command, &resultType);
		RESULT_EXPECT("[2]ct_results(CS_EXECUTE)", CS_SUCCEED, CS_CMD_DONE);
		ret = ct_results(_command, &resultType);
		RESULT_EXPECT("[3]ct_results(CS_EXECUTE)", CS_SUCCEED, CS_CMD_SUCCEED);
		ret = ct_results(_command, &resultType);
		RESULT_EXPECT("[4]ct_results(CS_EXECUTE)", CS_SUCCEED, CS_CMD_DONE);
		ret = ct_results(_command, &resultType);
		RESULT_EXPECT("[5]ct_results(CS_EXECUTE)", CS_END_RESULTS, CS_CMD_DONE);
		return false;  // no, there's definitely not any more data
	}
}


std::size_t StatementExecutor::getAffectedRowCount() const
{
	return _affectedRowCount;
}


std::size_t StatementExecutor::columnsReturned() const
{
	return static_cast<std::size_t>(_colPocoFmt.size());
}


const MetaColumn& StatementExecutor::metaColumn(std::size_t pos) const
{
	if ( pos >= columnsReturned() )
		throw StatementException("Invalid column number for metaColumn");

	return _colPocoFmt.at(pos);
}


void StatementExecutor::clearResults()
{
	_affectedRowCount = 0;
}


}}}
