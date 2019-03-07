/* Copyright (c) 2013-2018 the Civetweb developers
 * Copyright (c) 2013 No Face Press, LLC
 * License http://opensource.org/licenses/mit-license.php MIT License
 */

// Simple example program on how to use Embedded C++ interface.

#include "CivetServer.h"
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define DOCUMENT_ROOT "."
#define PORT "8082"
#define EXIT_URI "/exit"
bool exitNow = false;


class ExitHandler : public CivetHandler
{
  public:
	bool
	handleGet(CivetServer *server, struct mg_connection *conn)
	{
		mg_printf(conn,
		          "HTTP/1.1 200 OK\r\nContent-Type: "
		          "text/plain\r\nConnection: close\r\n\r\n");
		mg_printf(conn, "Bye!\n");
		exitNow = true;
		return true;
	}
};


#include <fstream>
#include <iostream>
#ifdef USE_WEBSOCKET
class WebSocketHandler : public CivetWebSocketHandler {

	virtual bool handleConnection(CivetServer *server,
	                              const struct mg_connection *conn) {
		printf("WS connected\n");
		return true;
	}
	
	virtual void handleReadyState(CivetServer *server,
	                              struct mg_connection *conn) {
		printf("WS ready\n");

		//const char *text = "Hello from the websocket ready handler";
		//mg_websocket_write(conn, MG_WEBSOCKET_OPCODE_TEXT, text, strlen(text));
		std::ifstream testInput("frag_bunny.mp4", std::ifstream::binary);
		if (testInput) {
			testInput.seekg(0, testInput.end);
			int length = static_cast<int>(testInput.tellg());
			testInput.seekg(0, testInput.beg);
			char *buffer = new char[length];

			// read data as a block:
			testInput.read(buffer, length);

			if (testInput)
				std::cout << "all characters read successfully.";
			else
				std::cout << "error: only " << testInput.gcount()
				          << " could be read";
			testInput.close();

			// ...buffer contains the entire file...
			mg_websocket_write(conn,
			                   MG_WEBSOCKET_OPCODE_BINARY,
			                   buffer,
			                   length);
			delete[] buffer;
		}
	}

	virtual bool handleData(CivetServer *server,
	                        struct mg_connection *conn,
	                        int bits,
	                        char *data,
	                        size_t data_len) {
		printf("WS got %lu bytes: ", (long unsigned)data_len);
		fwrite(data, 1, data_len, stdout);
		printf("\n");

		//mg_websocket_write(conn, MG_WEBSOCKET_OPCODE_TEXT, data, data_len);
		return (data_len<4);
	}

	virtual void handleClose(CivetServer *server,
	                         const struct mg_connection *conn) {
		printf("WS closed\n");
	}
};
#endif

int
main(int argc, char *argv[])
{
	std::vector<std::string> cpp_options;
	cpp_options.push_back("document_root");
	cpp_options.push_back(DOCUMENT_ROOT);
	cpp_options.push_back("listening_ports");
	cpp_options.push_back(PORT);

	CivetServer server(cpp_options); // <-- C++ style start

	ExitHandler h_exit;
	server.addHandler(EXIT_URI, h_exit);

	WebSocketHandler h_websocket;
	server.addWebSocketHandler("/h5sws", h_websocket);
	printf("Run websocket example at http://localhost:%s/h5sws\n", PORT);

	printf("Exit at http://localhost:%s%s\n", PORT, EXIT_URI);

	while (!exitNow) {
#ifdef _WIN32
		Sleep(1000);
#else
		sleep(1);
#endif
	}

	printf("Bye!\n");

	return 0;
}
