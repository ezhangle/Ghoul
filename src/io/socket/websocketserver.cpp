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

#include <ghoul/io/socket/websocketserver.h>
#include <iostream>

namespace ghoul {
namespace io {

WebSocketServer::WebSocketServer() {
}

WebSocketServer::~WebSocketServer() {}

std::string WebSocketServer::address() const {
    return "";
}

int WebSocketServer::port() const {
    return 0;
}

void WebSocketServer::close() {
    // todo.
}

void WebSocketServer::listen(std::string address, int port)
{
    std::cout << "websocket server listening on" << address << ":" << port << std::endl;
}

bool WebSocketServer::isListening() const
{
    return false;
}

bool WebSocketServer::hasPendingSockets() const
{
    return false;
}

std::unique_ptr<WebSocket> WebSocketServer::nextPendingWebSocket()
{
    return std::unique_ptr<WebSocket>();
}

std::unique_ptr<Socket> WebSocketServer::nextPendingSocket()
{
    return std::unique_ptr<Socket>();
}

std::unique_ptr<WebSocket> WebSocketServer::awaitPendingWebSocket()
{
    return std::unique_ptr<WebSocket>();
}

std::unique_ptr<Socket> WebSocketServer::awaitPendingSocket()
{
    return std::unique_ptr<Socket>();
}


}
}