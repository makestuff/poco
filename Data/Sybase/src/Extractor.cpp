#include "Poco/Data/Sybase/Extractor.h"
#include "Poco/Data/Date.h"
#include "Poco/Data/Time.h"
#include "Poco/NumberParser.h"
#include "Poco/DateTimeParser.h"
#include <limits>

namespace Poco {
namespace Data {
namespace Sybase {


Extractor::Extractor(StatementExecutor& st):
	_statementExecutor(st)
{
}


Extractor::~Extractor()
{
}


bool Extractor::extract(std::size_t pos, Poco::Int8&)
{
	throw NotImplementedException("Poco::Int8 extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, Poco::UInt8&)
{
	throw NotImplementedException("Poco::Int8 extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, Poco::Int16&)
{
	throw NotImplementedException("Poco::Int16 extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, Poco::UInt16&)
{
	throw NotImplementedException("Poco::Int16 extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, Poco::Int32& val)
{
	const Poco::Int32* const ptr = reinterpret_cast<const Poco::Int32*>(_statementExecutor.getBuffer(pos));
	val = *ptr;
	return true;
}


bool Extractor::extract(std::size_t pos, Poco::UInt32&)
{
	throw NotImplementedException("Poco::Int32 extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, Poco::Int64&)
{
	throw NotImplementedException("Poco::Int64 extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, Poco::UInt64&)
{
	throw NotImplementedException("Poco::Int64 extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, bool&)
{
	throw NotImplementedException("bool extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, float&)
{
	throw NotImplementedException("float extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, double&)
{
	throw NotImplementedException("double extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, char&)
{
	throw NotImplementedException("char extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::string& val)
{
	val = static_cast<const char*>(_statementExecutor.getBuffer(pos));
	return true;
}


bool Extractor::extract(std::size_t pos, BLOB&)
{
	throw NotImplementedException("BLOB extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, CLOB&)
{
	throw NotImplementedException("CLOB extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, DateTime&)
{
	throw NotImplementedException("DateTime extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, Date& val)
{
	val.assign(1992, 12, 5);
	return true;
}


bool Extractor::extract(std::size_t pos, Time&)
{
	throw NotImplementedException("Time extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, Any&)
{
	throw NotImplementedException("Any extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, Dynamic::Var&)
{
	throw NotImplementedException("Dynamic::Var extractor must be implemented.");
}


bool Extractor::isNull(std::size_t col, std::size_t row)
{
	return false;
}


void Extractor::reset()
{
	AbstractExtractor::reset();
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::Int8>&)
{
	throw NotImplementedException("std::vector<Poco::Int8> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Int8>&)
{
	throw NotImplementedException("std::deque<Poco::Int8> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Int8>&)
{
	throw NotImplementedException("std::list<Poco::Int8> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::UInt8>&)
{
	throw NotImplementedException("std::vector<Poco::UInt8> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::UInt8>&)
{
	throw NotImplementedException("std::deque<Poco::UInt8> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::list<Poco::UInt8>&)
{
	throw NotImplementedException("std::list<Poco::UInt8> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::Int16>&)
{
	throw NotImplementedException("std::vector<Poco::Int16> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Int16>&)
{
	throw NotImplementedException("std::deque<Poco::Int16> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Int16>&)
{
	throw NotImplementedException("std::list<Poco::Int16> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::UInt16>&)
{
	throw NotImplementedException("std::vector<Poco::UInt16> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::UInt16>&)
{
	throw NotImplementedException("std::deque<Poco::UInt16> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::list<Poco::UInt16>&)
{
	throw NotImplementedException("std::list<Poco::UInt16> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::Int32>&)
{
	throw NotImplementedException("std::vector<Poco::Int32> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Int32>&)
{
	throw NotImplementedException("std::deque<Poco::Int32> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Int32>&)
{
	throw NotImplementedException("std::list<Poco::Int32> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::UInt32>&)
{
	throw NotImplementedException("std::vector<Poco::UInt32> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::UInt32>&)
{
	throw NotImplementedException("std::deque<Poco::UInt32> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::list<Poco::UInt32>&)
{
	throw NotImplementedException("std::list<Poco::UInt32> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::Int64>&)
{
	throw NotImplementedException("std::vector<Poco::Int64> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Int64>&)
{
	throw NotImplementedException("std::deque<Poco::Int64> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Int64>&)
{
	throw NotImplementedException("std::list<Poco::Int64> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::UInt64>&)
{
	throw NotImplementedException("std::vector<Poco::UInt64> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::UInt64>&)
{
	throw NotImplementedException("std::deque<Poco::UInt64> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::list<Poco::UInt64>&)
{
	throw NotImplementedException("std::list<Poco::UInt64> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::vector<bool>&)
{
	throw NotImplementedException("std::vector<bool> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::deque<bool>&)
{
	throw NotImplementedException("std::deque<bool> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::list<bool>&)
{
	throw NotImplementedException("std::list<bool> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::vector<float>&)
{
	throw NotImplementedException("std::vector<float> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::deque<float>&)
{
	throw NotImplementedException("std::deque<float> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::list<float>&)
{
	throw NotImplementedException("std::list<float> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::vector<double>&)
{
	throw NotImplementedException("std::vector<double> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::deque<double>&)
{
	throw NotImplementedException("std::deque<double> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::list<double>&)
{
	throw NotImplementedException("std::list<double> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::vector<char>&)
{
	throw NotImplementedException("std::vector<char> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::deque<char>&)
{
	throw NotImplementedException("std::deque<char> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::list<char>&)
{
	throw NotImplementedException("std::list<char> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::vector<std::string>&)
{
	throw NotImplementedException("std::vector<std::string> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::deque<std::string>&)
{
	throw NotImplementedException("std::deque<std::string> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::list<std::string>&)
{
	throw NotImplementedException("std::list<std::string> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::vector<BLOB>&)
{
	throw NotImplementedException("std::vector<BLOB> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::deque<BLOB>&)
{
	throw NotImplementedException("std::deque<BLOB> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::list<BLOB>&)
{
	throw NotImplementedException("std::list<BLOB> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::vector<CLOB>&)
{
	throw NotImplementedException("std::vector<CLOB> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::deque<CLOB>&)
{
	throw NotImplementedException("std::deque<CLOB> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::list<CLOB>&)
{
	throw NotImplementedException("std::list<CLOB> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::vector<DateTime>&)
{
	throw NotImplementedException("std::vector<DateTime> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::deque<DateTime>&)
{
	throw NotImplementedException("std::deque<DateTime> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::list<DateTime>&)
{
	throw NotImplementedException("std::list<DateTime> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::vector<Date>&)
{
	throw NotImplementedException("std::vector<Date> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::deque<Date>&)
{
	throw NotImplementedException("std::deque<Date> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::list<Date>&)
{
	throw NotImplementedException("std::list<Date> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::vector<Time>&)
{
	throw NotImplementedException("std::vector<Time> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::deque<Time>&)
{
	throw NotImplementedException("std::deque<Time> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::list<Time>&)
{
	throw NotImplementedException("std::list<Time> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::vector<Any>&)
{
	throw NotImplementedException("std::vector<Any> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::deque<Any>&)
{
	throw NotImplementedException("std::deque<Any> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::list<Any>&)
{
	throw NotImplementedException("std::list<Any> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::vector<Dynamic::Var>&)
{
	throw NotImplementedException("std::vector<Dynamic::Var> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::deque<Dynamic::Var>&)
{
	throw NotImplementedException("std::deque<Dynamic::Var> extractor must be implemented.");
}


bool Extractor::extract(std::size_t pos, std::list<Dynamic::Var>&)
{
	throw NotImplementedException("std::list<Dynamic::Var> extractor must be implemented.");
}


} } } // namespace Poco::Data::Sybase
