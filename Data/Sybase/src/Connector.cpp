#include "Poco/Data/Sybase/Connector.h"
#include "Poco/Data/Sybase/SessionImpl.h"
#include "Poco/Data/SessionFactory.h"
#include "Callbacks.h"

const SybaseConnectorRegistrator pocoSybaseConnectorRegistrator;

namespace Poco {
namespace Data {
namespace Sybase {


const std::string Connector::KEY = POCO_DATA_SYBASE_CONNECTOR_NAME;


Connector::Connector():
	_ctlibContext(0)
{
	// TODO: how to deal with failures? Perhaps delay until it's safe to throw an exception?
	//
	CS_RETCODE ret = cs_ctx_alloc(CS_CURRENT_VERSION, &_ctlibContext);
	if ( ret != CS_SUCCEED )
	{
		_ctlibContext = 0;
		return;
	}
	ret = ct_init(_ctlibContext, CS_CURRENT_VERSION);

	// Install message callbacks. TODO: replace with inline messages!
	#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wpedantic"

		// Install cs-library message callback
		ret = cs_config(_ctlibContext, CS_SET, CS_MESSAGE_CB, (CS_VOID*)csLibMsgCallback, CS_UNUSED, NULL);

		// Install ct-library message callback
		ret = ct_callback(_ctlibContext, NULL, CS_SET, CS_CLIENTMSG_CB, (CS_VOID*)ctLibMsgCallback);

		// Install server message callback
		ret = ct_callback(_ctlibContext, NULL, CS_SET, CS_SERVERMSG_CB, (CS_VOID*)serverMsgCallback);
	#pragma GCC diagnostic pop
	(void)ret;
}


Connector::~Connector()
{
	if ( _ctlibContext )
	{
		ct_exit(_ctlibContext, CS_UNUSED);
		cs_ctx_drop(_ctlibContext);
	}
}

const std::string& Connector::name() const
{
	static const std::string n(POCO_DATA_SYBASE_CONNECTOR_NAME);
	return n;
}

Poco::AutoPtr<Poco::Data::SessionImpl> Connector::createSession(
	const std::string& connStr, std::size_t timeout)
{
	return Poco::AutoPtr<Poco::Data::SessionImpl>(new SessionImpl(connStr, _ctlibContext, timeout));
}

void Connector::registerConnector()
{
	Poco::Data::SessionFactory::instance().add(new Connector());
}

void Connector::unregisterConnector()
{
	Poco::Data::SessionFactory::instance().remove(POCO_DATA_SYBASE_CONNECTOR_NAME);
}


} } } // namespace Poco::Data::Sybase

