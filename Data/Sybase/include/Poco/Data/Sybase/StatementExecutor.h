#ifndef Data_Sybase_StatementHandle_INCLUDED
#define Data_Sybase_StatementHandle_INCLUDED

#include "Poco/Data/Sybase/SybaseException.h"
#include "Poco/Data/Sybase/SessionHandle.h"
#include "Poco/Data/MetaColumn.h"
#include <string>
#include <vector>
#include <ctpublic.h>


namespace Poco {
namespace Data {
namespace Sybase {


class StatementExecutor
	/// Sybase statement executor.
{
public:
	enum State
	{
		STMT_INITED,
		STMT_COMPILED,
		STMT_EXECUTED
	};

	explicit StatementExecutor(SessionHandle& sessionHandle);
		/// Creates the StatementExecutor.

	~StatementExecutor();
		/// Destroys the StatementExecutor.

	State state() const;
		/// Returns the current state.

	void prepare(const std::string& query);
		/// Prepares the statement for execution.

	void execute();
		/// Executes the statement.

	bool fetch();
		/// Fetches the data for the current row

	std::size_t getAffectedRowCount() const;
		/// get the count of rows affected by the statement

	std::size_t columnsReturned() const;
		/// get the count of columns returned by the statement

	const MetaColumn& metaColumn(std::size_t pos) const;
		/// Returns the reference to the specified metacolumn - 0 based

	const void* getBuffer(std::size_t pos) const;
		/// Returns a pointer to the data for the specified column in the current row
	
	std::size_t getNumBytes(std::size_t pos) const;
		/// Returns the number of bytes of data returned for the specified column in the current row

private:
	void clearResults();

	SessionHandle& _sessionHandle;
	State _state;
	std::string _query;
	std::size_t _affectedRowCount;
	CS_COMMAND* _command;
	std::vector<MetaColumn> _colPocoFmt;  // column formats, from the Poco perspective
	std::vector<CS_DATAFMT> _colSybFmt;   // column formats, from the Sybase perspective
	std::vector<CS_VOID*> _rowData;       // this row's raw bytes
	std::vector<CS_INT> _rowNumBytes;     // for this row, the number of bytes read for each column
};

inline const void* StatementExecutor::getBuffer(std::size_t pos) const
{
	return _rowData[pos];
}

inline std::size_t StatementExecutor::getNumBytes(std::size_t pos) const
{
	return _rowNumBytes[pos];
}

	
}}}

#endif // Data_Sybase_StatementHandle_INCLUDED
