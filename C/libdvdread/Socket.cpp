#include <string>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <poll.h>

#include <stdio.h>
#include <unistd.h>
#include <strings.h>

#include <iostream>
#include "Socket.h"

using namespace std;

void Socket::close() {
    ::close(handle);
}

//ClientSocket::ClientSocket(const ClientSocket& other) {
//   handle = other.handle;
//
//   nread = 0;
//   bufPtr = buffer;
//}

ClientSocket::ClientSocket() {
   handle = 0;
   nread = 0;
   bufPtr = buffer;
}

ClientSocket::ClientSocket(int sHandle) {
   handle = sHandle;

   nread = 0;
   bufPtr = buffer;
}

const int MAXLEN = 128;  // maximum line size

string ClientSocket::readLine() {
   char result[MAXLEN + 1];
   char* dstPtr = result;

   // scan through buffer and find \n
   for (int i = 0;  i < MAXLEN;  i++) {

      // read next bunch of characters
      if (nread <= 0) {
         nread = ::read(handle, buffer, sizeof(buffer));
         if (nread < 0) ::perror("read");
         if (nread == 0) break;	// EOF
         bufPtr = buffer;
      }

      // bufPtr: next character in buffer
      // dstPtr: next character in destination
      char c = *bufPtr++;
      nread--;

      if (c == '\n') {
         *dstPtr = 0;
         return result;
      }

      *dstPtr = c;
      dstPtr++;
   }

   *dstPtr = 0;
   return result;
}


string ClientSocket::checkLine() {
   char result[MAXLEN + 1];
   char* dstPtr = result;

   // scan through buffer and find \n
   for (int i = 0;  i < MAXLEN;  i++) {

      // read next bunch of characters
      if (nread <= 0) {
         pollfd toPoll = {handle, POLLRDNORM, 0};
         int ready = poll (&toPoll, 1, 0);
         if (ready == -1) ::perror("poll");
         if (ready == 0) { return ""; }

         nread = ::read(handle, buffer, sizeof(buffer));
         if (nread < 0) ::perror("read");
         if (nread == 0) break;	// EOF
         bufPtr = buffer;
      }

      // bufPtr: next character in buffer
      // dstPtr: next character in destination
      char c = *bufPtr++;
      nread--;

      if (c == '\n') {
         *dstPtr = 0;
         return result;
      }

      *dstPtr = c;
      dstPtr++;
   }

   *dstPtr = 0;
   return result;
}


int ClientSocket::writeData(const uint8_t* data, int len) {
     size_t nleft = len;
     const uint8_t *ptr = data;

     while (nleft > 0) {
//         ssize_t nwritten = write(handle, ptr, nleft);
         ssize_t nwritten = send(handle, ptr, nleft, MSG_MORE);
         if (nwritten <= 0) {
             if (nwritten < 0 && errno == EINTR) {
                 nwritten = 0;   /* and call write() again */
             } else {
                 if (errno == ECONNRESET) { return 1; }
                 //return (-1);    /* error */
                 perror("write");
             }
          }

          nleft -= nwritten;
          ptr += nwritten;
     }
    return 0;
}

void ClientSocket::writeLine(const char* data) {
    writeData((uint8_t*) data, strlen(data));
    writeData((uint8_t*)"\n", 1);
}

void ClientSocket::writeHeader(const std::string& data) {
    writeData((uint8_t*) data.c_str(), data.length());
    writeData((uint8_t*)"\r\n", 2);
}


ServerSocket::ServerSocket(int port) {
   handle = ::socket(AF_INET, SOCK_STREAM, 0);
   if (handle == -1) ::perror("socket");

   const int enabled = 1;
   int error = ::setsockopt(handle, SOL_SOCKET, SO_REUSEADDR, &enabled, sizeof(int));
   if (error != 0) perror("setsockopt");

   struct sockaddr_in servaddr;
   ::bzero(&servaddr, sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr = ::htonl (INADDR_ANY);
   servaddr.sin_port = ::htons(port);

   error = ::bind(handle, (struct sockaddr*) &servaddr, sizeof(servaddr));
   if (error != 0) ::perror("bind");

   error = ::listen(handle, 1);
   if (error != 0) ::perror("listen");
}


ClientSocket ServerSocket::accept() {
   cerr << "Waiting for connection ..." << endl;

   struct sockaddr_in client;
   socklen_t clientSize = sizeof(client);
   int clientSocket = ::accept(handle, (struct sockaddr*) &client, &clientSize);
   if (clientSocket == -1) perror("accept");

   char addrBuf[INET_ADDRSTRLEN];
   const char* res = ::inet_ntop(AF_INET, &client.sin_addr, addrBuf, INET_ADDRSTRLEN);
   if (res != addrBuf) {
      perror("inet_ntop");
   }

   cerr << "Connection from " << addrBuf << endl;

   const int enabled = 0;
   int error = ::setsockopt(handle, IPPROTO_TCP, TCP_CORK, &enabled, sizeof(int));
   if (error != 0) perror("setsockopt");

   return ClientSocket(clientSocket);
}

