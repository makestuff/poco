#ifndef Data_Sybase_SessionHandle_INCLUDED
#define Data_Sybase_SessionHandle_INCLUDED

#include <map>
#include "ctpublic.h"
#include "Poco/Data/Sybase/SybaseException.h"

namespace Poco {
namespace Data {
namespace Sybase {

class SessionHandle
	/// Sybase session handle
{
public:
	explicit SessionHandle(CS_CONTEXT* cslibContext);
		/// Creates session handle

	~SessionHandle();
		/// Destroy handle, close connection

	void connect(const std::map<std::string, std::string>& optionsMap);
		/// Connect to server

	bool isConnected() const;
		/// is a connection established?

	void disconnect();
		/// Close connection

	CS_CONNECTION* getConnection();
		/// TODO: this kinda sucks because it exposes internals
private:
	bool isConnectedNoLock() const;
	void use(const std::string& dbName) const;

	CS_CONTEXT* _cslibContext;
	CS_CONNECTION* _connection;
	bool _isConnected;
};

inline CS_CONNECTION* SessionHandle::getConnection()
{
	return _connection;
}


}}}

#endif // Data_Sybase_SessionHandle_INCLUDED
