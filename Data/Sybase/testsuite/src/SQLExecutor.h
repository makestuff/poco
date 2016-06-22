#ifndef SQLExecutor_Sybase_INCLUDED
#define SQLExecutor_Sybase_INCLUDED


#include "Poco/Data/Sybase/Sybase.h"
#include "Poco/Data/Session.h"


class SQLExecutor: public CppUnit::TestCase {
public:
	SQLExecutor(const std::string& name, Poco::Data::Session* session);
	~SQLExecutor();

	void simpleAccess();

private:
	Poco::Data::Session* _session;
};


#endif // SQLExecutor_Sybase_INCLUDED
