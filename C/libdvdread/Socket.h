#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <string>

class Socket {
protected:
    int handle;

public:
    void close();
};


class ClientSocket : public Socket {
   char buffer[1024];
   int nread;
   char* bufPtr;

public:
    ClientSocket();
    ClientSocket(int handle);
    //ClientSocket(const ClientSocket& other);

    /**
     * Reads the next line of text from the socket.
     * The result does not include the terminating character 
     * (similar to Java's BufferedReader.readLine())
     * TODO: can not distinguish between empty line and end of stream!
     */
    std::string readLine();

    std::string checkLine();

    int writeData(const uint8_t* data, int len);

    void writeLine(const char* data);

    void writeHeader(const std::string& header);

};


class ServerSocket : public Socket {
public:
    ServerSocket(int port);

    ClientSocket accept();
};

#endif
