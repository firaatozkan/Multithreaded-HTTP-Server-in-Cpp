#include <unistd.h>
#include <cstring>
#include "WebServer.h"

WebServer::WebServer(const int& port)
{
        serverFD = socket(AF_INET, SOCK_STREAM, 0);
        if(serverFD < 0)
                std::__throw_runtime_error("Couldn't create socket!\n");

        int optval = 1;
        if(setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) != 0)
                std::__throw_runtime_error("Couldn't create socket!\n");
        
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);

        if(bind(serverFD, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) != 0)
                std::__throw_runtime_error("Couldn't bind socket!\n");
}

WebServer::~WebServer()
{
        for (int i = 0; i < THREAD_POOL_SIZE; i++)
                pthread_cancel(threadPool[i]);
}

void* WebServer::serveClients__(void* context)
{
        return reinterpret_cast<WebServer*>(context)->serveClients();
}

void WebServer::run()
{
        for (int i = 0; i < THREAD_POOL_SIZE; i++)
                if(pthread_create(&threadPool[i], nullptr, &serveClients__, this) != 0)
                        std::__throw_runtime_error("Couldn't create threads!\n");
        
        if(listen(serverFD, THREAD_POOL_SIZE) != 0)
                std::__throw_runtime_error("Server couldn't start listening!\n");

        int clientFD_;
        sockaddr_in clientAddr_;
        socklen_t clientSize = sizeof(clientAddr_);
        while(true)
        {
                clientFD_ = accept(serverFD, reinterpret_cast<sockaddr*>(&clientAddr_), &clientSize);
                if(clientFD_ >= 0)
                        clientQueue.push(Client{clientFD_, clientAddr_});
        }
}

void* WebServer::serveClients()
{
        while(true)
        {
                Client output = clientQueue.pop();
                {
                        char buffer[2048]{0};
                        if(recv(output.clientFD, buffer, 2048, 0) > 0)
                        {
                                parseRequests(output, buffer);
                        }
                }
                operateCallbacks(output);
                close(output.clientFD);
        }
}

void WebServer::parseRequests(Client& c, char* buffer_)
{
        char* p = strtok_r(buffer_, " ", &buffer_);
        if(strcmp(p, "GET") == 0)
                c.clientRequestType = GET;
        else if(strcmp(p, "POST") == 0)
                c.clientRequestType = POST;
        
        c.clientRequestIndex = std::string(strtok_r(buffer_, " ", &buffer_));
}

void WebServer::operateCallbacks(Client& c)
{
        if(c.clientRequestType != NONE)
        {
                if(callbackTable[c.clientRequestType].count(c.clientRequestIndex) > 0)
                        (callbackTable[c.clientRequestType][c.clientRequestIndex])(c);
        }
}

void WebServer::get(const std::string& index, std::function<void (Client&)> callbackFunc)
{
        callbackTable[GET][index] = callbackFunc;
}

void WebServer::post(const std::string& index, std::function<void (Client&)> callbackFunc)
{
        callbackTable[POST][index] = callbackFunc;
}