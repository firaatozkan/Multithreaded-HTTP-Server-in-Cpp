#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <arpa/inet.h>
#include <string>
#include <unordered_map>
#include <functional>
#include "Client.h"
#include "ThreadsafeQueue.h"

#define THREAD_POOL_SIZE        12

class WebServer
{
private:
        int serverFD;
        sockaddr_in serverAddr;

        std::unordered_map<std::string, std::function<void (Client&)>> callbackTable[NUM_OF_HTTP_REQUEST_TYPES - 1]{};

        ThreadsafeQueue<Client> clientQueue{};

        pthread_t threadPool[THREAD_POOL_SIZE];

        static void* serveClients__(void*);

        static void parseRequests(Client& c, char* buffer_);

        void* serveClients();

        void operateCallbacks(Client& c);

public:
        explicit WebServer(const int& port);
        ~WebServer();
        void run();
        void get(const std::string& index, std::function<void (Client&)> callbackFunc);
        void post(const std::string& index, std::function<void (Client&)> callbackFunc);
};


#endif