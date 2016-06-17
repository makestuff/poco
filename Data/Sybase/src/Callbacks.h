#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "ctpublic.h"

extern "C" {
	CS_RETCODE CS_PUBLIC serverMsgCallback(const CS_CONTEXT*, const CS_CONNECTION*, const CS_SERVERMSG*);
	CS_RETCODE CS_PUBLIC ctLibMsgCallback(const CS_CONTEXT*, const CS_CONNECTION*, const CS_CLIENTMSG*);
	CS_RETCODE CS_PUBLIC csLibMsgCallback(const CS_CONTEXT*, const CS_CLIENTMSG*);
}

#endif
