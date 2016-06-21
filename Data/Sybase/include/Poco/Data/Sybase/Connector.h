#ifndef Data_Sybase_Connector_INCLUDED
#define Data_Sybase_Connector_INCLUDED

#include "Poco/Data/Sybase/Sybase.h"
#include "Poco/Data/SessionImpl.h"
#include "Poco/Data/Connector.h"
#include "Poco/AutoPtr.h"
#include <string>

// ctpublic.h defines CS_CONTEXT as a typedef to "struct _cscontext". Unfortunately,
// because they're definitions, typedefs cannot be forward-declared, so to avoid
// including ctpublic.h here, I need to forward-declare "struct _cscontext", and use
// that instead of CS_CONTEXT.
//
struct _cscontext;

// Note: to avoid static (de)initialization problems,
// during connector automatic (un)registration, it is 
// best to have this as a macro.

#define POCO_DATA_SYBASE_CONNECTOR_NAME "sybase"

namespace Poco {
namespace Data {
namespace Sybase {

class Sybase_API Connector: public Poco::Data::Connector
	/// Connector instantiates Sybase SessionImpl objects.
{
public:
	static const std::string KEY;

	Connector();
		/// Creates the Connector.

	virtual ~Connector();
		/// Destroys the Connector.

	virtual const std::string& name() const;
		/// Returns the name associated with this connector.

	virtual Poco::AutoPtr<Poco::Data::SessionImpl> createSession(
		const std::string& connStr,
		std::size_t timeout = Poco::Data::SessionImpl::LOGIN_TIMEOUT_DEFAULT);
		/// Creates a Sybase SessionImpl object and initializes it with the given connectionString.

	static void registerConnector();
		/// Registers the Connector under the Keyword Connector::KEY at the Poco::Data::SessionFactory

	static void unregisterConnector();
		/// Unregisters the Connector under the Keyword Connector::KEY at the Poco::Data::SessionFactory

private:
	struct _cscontext* _ctlibContext;
};


} } } // namespace Poco::Data::Sybase


// 
// Automatic Connector registration
// 

struct Sybase_API SybaseConnectorRegistrator
	/// Connector registering class.
	/// A global instance of this class is instantiated
	/// with sole purpose to automatically register the 
	/// Sybase connector with central Poco Data registry.
{
	SybaseConnectorRegistrator()
		/// Calls Poco::Data::Sybase::registerConnector();
	{
		Poco::Data::Sybase::Connector::registerConnector();
	}

	~SybaseConnectorRegistrator()
		/// Calls Poco::Data::Sybase::unregisterConnector();
	{
		Poco::Data::Sybase::Connector::unregisterConnector();
	}
};


#if !defined(POCO_NO_AUTOMATIC_LIB_INIT)
	#if defined(POCO_OS_FAMILY_WINDOWS)
		extern "C" const struct Sybase_API SybaseConnectorRegistrator pocoSybaseConnectorRegistrator;
		#if defined(Sybase_EXPORTS)
			#if defined(_WIN64)
				#define POCO_DATA_SYBASE_FORCE_SYMBOL(s) __pragma(comment (linker, "/export:"#s))
			#elif defined(_WIN32)
				#define POCO_DATA_SYBASE_FORCE_SYMBOL(s) __pragma(comment (linker, "/export:_"#s))
			#endif
		#else  // !Sybase_EXPORTS
			#if defined(_WIN64)
				#define POCO_DATA_SYBASE_FORCE_SYMBOL(s) __pragma(comment (linker, "/include:"#s))
			#elif defined(_WIN32)
				#define POCO_DATA_SYBASE_FORCE_SYMBOL(s) __pragma(comment (linker, "/include:_"#s))
			#endif
		#endif // Sybase_EXPORTS
	#else // !POCO_OS_FAMILY_WINDOWS
			#define POCO_DATA_SYBASE_FORCE_SYMBOL(s) extern "C" const struct SybaseConnectorRegistrator s;
	#endif // POCO_OS_FAMILY_WINDOWS
	POCO_DATA_SYBASE_FORCE_SYMBOL(pocoSybaseConnectorRegistrator)
#endif // POCO_NO_AUTOMATIC_LIB_INIT

// 
// End automatic Connector registration
// 


#endif // Data_Sybase_Connector_INCLUDED
