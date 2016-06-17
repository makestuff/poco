#ifndef Data_Sybase_SybaseException_INCLUDED
#define Data_Sybase_SybaseException_INCLUDED

#include "Poco/Data/Sybase/Sybase.h"
#include "Poco/Data/DataException.h"
#include <typeinfo>
#include <string>

namespace Poco {
namespace Data {
namespace Sybase {

// End-user include this file and use in code ConnectionException/StatementException
// So it need not know 

class Sybase_API SybaseException: public Poco::Data::DataException
	/// Base class for all Sybase exceptions
{
public:

	SybaseException(const std::string& msg);
		/// Creates SybaseException.

	SybaseException(const SybaseException& exc);
		/// Creates SybaseException.

	~SybaseException() throw();
		/// Destroys Sybaseexception.

	SybaseException& operator=(const SybaseException& exc);
		/// Assignment operator.

	const char* name() const throw();
		/// Returns exception name.

	const char* className() const throw();
		/// Returns the name of the exception class.

	Poco::Exception* clone() const;
		/// Creates an exact copy of the exception.
		///
		/// The copy can later be thrown again by
		/// invoking rethrow() on it.

	void rethrow() const;
		/// (Re)Throws the exception.
		///
		/// This is useful for temporarily storing a
		/// copy of an exception (see clone()), then
		/// throwing it again.
};


class ConnectionException : public SybaseException
	/// ConnectionException
{
public:

	ConnectionException(const std::string& msg);
		/// Creates ConnectionException from string.

};


class TransactionException : public ConnectionException
	/// TrabsactionException
{
public:

	TransactionException(const std::string& msg);
		/// Creates TransactionException from string.
};


class StatementException : public SybaseException
	/// StatementException
{
public:

	StatementException(const std::string& msg);
		/// Creates StatementException from string.
};


//
// inlines
//

inline SybaseException& SybaseException::operator=(const SybaseException& exc)
{
	Poco::Data::DataException::operator=(exc);
	return *this;
}

inline const char* SybaseException::name() const throw()
{
	return "Sybase";
}

inline const char* SybaseException::className() const throw()
{
	return typeid(*this).name();
}

inline Poco::Exception* SybaseException::clone() const
{
	return new SybaseException(*this);
}

inline void SybaseException::rethrow() const
{
	throw *this;
}


} } } // namespace Poco::Data::Sybase

#endif //Data_Sybase_SybaseException_INCLUDED
