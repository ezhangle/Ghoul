/*****************************************************************************************
 *                                                                                       *
 * GHOUL                                                                                 *
 * General Helpful Open Utility Library                                                  *
 *                                                                                       *
 * Copyright (c) 2012-2017                                                               *
 *                                                                                       *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this  *
 * software and associated documentation files (the "Software"), to deal in the Software *
 * without restriction, including without limitation the rights to use, copy, modify,    *
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to    *
 * permit persons to whom the Software is furnished to do so, subject to the following   *
 * conditions:                                                                           *
 *                                                                                       *
 * The above copyright notice and this permission notice shall be included in all copies *
 * or substantial portions of the Software.                                              *
 *                                                                                       *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,   *
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A         *
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT    *
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF  *
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE  *
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                         *
 ****************************************************************************************/

#ifndef __GHOUL___TCPSOCKET___H__
#define __GHOUL___TCPSOCKET___H__

#include <ghoul/misc/exception.h>

#include <thread>
#include <deque>
#include <array>
#include <mutex>
#include <condition_variable>
#include <atomic>

 // OS specific socket implementation normalization.

#ifdef WIN32
typedef size_t _SOCKET;
typedef int _SOCKLEN;
#else //linux & macOS
typedef int _SOCKET;
typedef socklen_t _SOCKLEN;
#endif

#ifdef WIN32
#define NOMINMAX
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#ifndef _ERRNO
#define _ERRNO WSAGetLastError()
#endif

#pragma warning(push)
#pragma warning (disable : 4996)
#else //Use BSD sockets
#ifdef _XCODE
#include <unistd.h>
#endif
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#ifndef SOCKET_ERROR
#define SOCKET_ERROR (-1)
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET (_SOCKET)(~0)
#endif

#ifndef NO_ERROR
#define NO_ERROR 0L
#endif

#ifndef _ERRNO
#define _ERRNO errno
#endif
#endif

namespace ghoul {
namespace io {

class TcpSocketServer;

class TcpSocket {
public:
    struct TcpSocketError : public ghoul::RuntimeError {
        explicit TcpSocketError(std::string message, std::string component = "");
    };

    TcpSocket(std::string address, int port);
    TcpSocket(std::string address, int port, _SOCKET socket);
    ~TcpSocket();
    void connect();
    void disconnect();
    bool isConnected();
    bool isConnecting();
    int socketId() const;

    /*
    * Read size bytes from the socket, store them in buffer and dequeue them from input.
    * Block until size bytes have been read.
    * Return false if this fails. Use error() to get the socket error code.
    */
    template <typename T = char>
    bool get(T* buffer, size_t nItems = 1);

    /*
    * Read size bytes from the socket, store them in buffer.
    * Do NOT dequeue them from input.
    * Block until size bytes have been read.
    * Return false if this fails. Use error() to get the socket error code.
    */
    template <typename T = char>
    bool peek(T* buffer, size_t nItems);

    /*
    * Skip size bytes from the socket.
    * Block until size bytes have been read.
    * Return false if this fails. Use error() to get the socket error code.
    */
    template <typename T = char>
    bool skip(size_t nItems);

    /**
    * Write size bytes from buffer into the socket.
    * Return false if this fails. Use error() to get the socket error code.
    */
    template <typename T = char>
    bool put(const T* buffer, size_t size = 1);


    static void initializeNetworkApi();
    static bool initializedNetworkApi();
private:
    void establishConnection(addrinfo *info);
    void streamInput();
    void streamOutput();
    bool waitForInput(size_t nBytes);
    bool waitForOutput(size_t nBytes);

    const std::string _address;
    const int _port;
    std::atomic<bool> _shouldDisconnect;
    std::atomic<bool> _isConnecting;
    std::atomic<bool> _isConnected;
    std::atomic<bool> _error;
    const int _socketId;

    _SOCKET _socket;
    TcpSocketServer* _server;
    std::unique_ptr<std::thread> _inputThread;
    std::unique_ptr<std::thread> _outputThread;

    std::mutex _inputBufferMutex;
    std::mutex _inputQueueMutex;
    std::condition_variable _inputNotifier;
    std::deque<char> _inputQueue;
    std::array<char, 4096> _inputBuffer;

    std::mutex _outputBufferMutex;
    std::mutex _outputQueueMutex;
    std::condition_variable _outputNotifier;
    std::deque<char> _outputQueue;
    std::array<char, 4096> _outputBuffer;

    static std::atomic<int> _nextSocketId;
    static std::atomic<bool> _initializedNetworkApi;
};

} // namespace io
} // namespace ghoul

#include "tcpsocket.inl"

#endif // __GHOUL___TCPSOCKET___H__
