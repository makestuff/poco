#ifndef Data_Sybase_SybaseStatementImpl_INCLUDED
#define Data_Sybase_SybaseStatementImpl_INCLUDED

#include "Poco/Data/Sybase/Sybase.h"
#include "Poco/Data/Sybase/SessionImpl.h"
#include "Poco/Data/Sybase/Extractor.h"
#include "Poco/Data/Sybase/StatementExecutor.h"
#include "Poco/Data/StatementImpl.h"
#include "Poco/SharedPtr.h"
#include "Poco/Format.h"


namespace Poco {
namespace Data {
namespace Sybase {


class Sybase_API SybaseStatementImpl: public Poco::Data::StatementImpl
	/// Implements statement functionality needed for Sybase
{
public:
	SybaseStatementImpl(SessionImpl& sessionImpl);
		/// Creates the SybaseStatementImpl.

	~SybaseStatementImpl();
		/// Destroys the SybaseStatementImpl.

protected:

	virtual std::size_t columnsReturned() const;
		/// Returns number of columns returned by query.

	virtual int affectedRowCount() const;
		/// Returns the number of affected rows.
		/// Used to find out the number of rows affected by insert, delete or update.

	virtual const MetaColumn& metaColumn(std::size_t pos, std::size_t dsNum) const;
		/// Returns column meta data.

	virtual bool hasNext();
		/// Returns true if a call to next() will return data.

	virtual std::size_t next();
		/// Retrieves the next row from the resultset.
		/// Will throw, if the resultset is empty.

	virtual bool canBind() const;
		/// Returns true if a valid statement is set and we can bind.

	virtual bool canCompile() const;
		/// Returns true if another compile is possible.

	virtual void compileImpl();
		/// Compiles the statement, doesn't bind yet

	virtual void bindImpl();
		/// Binds parameters

	virtual Poco::Data::AbstractExtractor::Ptr extractor();
		/// Returns the concrete extractor used by the statement.

	virtual Poco::Data::AbstractBinder::Ptr binder();
		/// Returns the concrete binder used by the statement.

private:
	enum NextState
	{
		NEXT_DONTKNOW,
		NEXT_TRUE,
		NEXT_FALSE
	};

	StatementExecutor _statementExecutor;

	Extractor::Ptr    _pExtractor;
	NextState         _hasNext;
};


} } } // namespace Poco::Data::Sybase


#endif // Data_Sybase_SybaseStatementImpl_INCLUDED
