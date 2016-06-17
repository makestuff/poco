// TODO: Switch to inline message handling, and remove this file!
//
#include <iostream>
#include <string>
#include <algorithm>
#include "Callbacks.h"

// Handler for server messages. Client-Library will call this routine when it
// receives a message from the server.
//
CS_RETCODE CS_PUBLIC serverMsgCallback(const CS_CONTEXT*, const CS_CONNECTION*, const CS_SERVERMSG* msg)
{
	std::string s = msg->text;
	s.erase(find_if(s.rbegin(), s.rend(), not1(std::ptr_fun<int, int>(isspace))).base(), s.end());
	std::cout << "Server message: \"" << s << "\"\n";
	return CS_SUCCEED;
}

// Client-Library error handler. This function will be invoked when a Client-Library
// has detected an error. Before Client-Library routines return CS_FAIL, this
// handler will be called with additional error information.
//
CS_RETCODE CS_PUBLIC ctLibMsgCallback(const CS_CONTEXT*, const CS_CONNECTION*, const CS_CLIENTMSG* msg)
{
	// Error number: Print the error's severity, number, origin, and layer. These
	// four numbers uniquely identify the error.
	//
	std::cout << "CT-Library error: severity(" << CS_SEVERITY(msg->msgnumber) << "); layer(" << CS_LAYER(msg->msgnumber) << "); origin(" << CS_ORIGIN(msg->msgnumber) << "); number(" << CS_NUMBER(msg->msgnumber) << ")\n";

	// Error text: Print the error text.
	//
	std::cout << "  " << msg->msgstring << "\n";

	// Operating system error information: Some errors, such as  network errors,
	// may have an operating system error associated with them. If there was an
	// operating system error, this code prints the error message text.
	//
	if ( msg->osnumber )
		std::cout << "  Operating system error number(" << msg->osnumber << "): \"" << msg->osstring << "\"\n";

	// If we return CS_FAIL, Client-Library marks the connection as dead. This
	// means that it cannot be used anymore. If we return CS_SUCCEED, the
	// connection remains alive if it was not already dead.
	//
	return CS_SUCCEED;
}

// CS-Library error handler. This function will be invoked when CS-Library has
// detected an error.
//
CS_RETCODE CS_PUBLIC csLibMsgCallback(const CS_CONTEXT*, const CS_CLIENTMSG* msg)
{
	// Print the error number and message.
	//
	std::cout << "CS-Library error: severity(" << CS_SEVERITY(msg->msgnumber) << "); layer(" << CS_LAYER(msg->msgnumber) << "); origin(" << CS_ORIGIN(msg->msgnumber) << "); number(" << CS_NUMBER(msg->msgnumber) << ")\n";
	
	// Print any operating system error information.
	//
	if ( msg->osnumber )
		std::cout << "  Operating system error number(" << msg->osnumber << "):\n  \"" << msg->osstring << "\"\n";

	return CS_SUCCEED;
}
