#include "Poco/Data/Sybase/SybaseStatementImpl.h"
#include "Poco/Data/Sybase/SybaseException.h"


namespace Poco {
namespace Data {
namespace Sybase {


SybaseStatementImpl::SybaseStatementImpl(SessionImpl& sessionImpl):
	Poco::Data::StatementImpl(sessionImpl),
	_statementExecutor(sessionImpl.handle()),
	_pExtractor(new Extractor(_statementExecutor)),
	_hasNext(NEXT_DONTKNOW)
{
}


SybaseStatementImpl::~SybaseStatementImpl()
{
}


std::size_t SybaseStatementImpl::columnsReturned() const
{
	return _statementExecutor.columnsReturned();
}


int SybaseStatementImpl::affectedRowCount() const
{
	return _statementExecutor.getAffectedRowCount();
}


const MetaColumn& SybaseStatementImpl::metaColumn(
	std::size_t pos, std::size_t dsNum) const
{
	// Disallow multiple result-sets for now
	poco_assert_dbg(dsNum == 0);

	return _statementExecutor.metaColumn(pos);
}


bool SybaseStatementImpl::hasNext()
{
	if ( _hasNext == NEXT_DONTKNOW )
	{
		if ( columnsReturned() == 0 )
			return false;
		if ( _statementExecutor.fetch() )
		{
			_hasNext = NEXT_TRUE;
			return true;
		}
		_hasNext = NEXT_FALSE;
		return false;
	}
	else
	{
		if ( _hasNext == NEXT_TRUE )
			return true;
	}
	return false;
}


std::size_t SybaseStatementImpl::next()
{
	if ( !hasNext() )
		throw StatementException("No data received");

	Poco::Data::AbstractExtractionVec::iterator it = extractions().begin();
	const Poco::Data::AbstractExtractionVec::const_iterator itEnd = extractions().end();
	std::size_t position = 0;
	for ( ; it != itEnd; ++it )
	{
		(*it)->extract(position);
		position += (*it)->numOfColumnsHandled();
	}

	_hasNext = NEXT_DONTKNOW;

	return 1;
}


bool SybaseStatementImpl::canBind() const
{
	return false;
}


bool SybaseStatementImpl::canCompile() const
{
	return (_statementExecutor.state() < StatementExecutor::STMT_COMPILED);
}


void SybaseStatementImpl::compileImpl()
{
	_statementExecutor.prepare(toString());
}


void SybaseStatementImpl::bindImpl()
{
	Poco::Data::AbstractBindingVec& binds = bindings();
	std::size_t position = 0;
	Poco::Data::AbstractBindingVec::iterator it = binds.begin();
	const Poco::Data::AbstractBindingVec::const_iterator itEnd = binds.end();
	for ( ; it != itEnd && (*it)->canBind(); ++it )
	{
		(*it)->bind(position);
		position += (*it)->numOfColumnsHandled();
	}

	//_pBinder->updateBindVectorToCurrentValues();
	//_statementExecutor.bindParams(_pBinder->getBindArray(), _pBinder->size());
	_statementExecutor.execute();
	_hasNext = NEXT_DONTKNOW;
}


Poco::Data::AbstractExtractor::Ptr SybaseStatementImpl::extractor()
{
	return _pExtractor;
}


Poco::Data::AbstractBinder::Ptr SybaseStatementImpl::binder()
{
	// TODO: implement
	return 0;
}


} } } // namespace Poco::Data::Sybase
