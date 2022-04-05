#ifndef CLIENT_H
#define CLIENT_H

struct sockaddr_in;

typedef enum
{
        NONE,
        GET,
        POST,

        NUM_OF_HTTP_REQUEST_TYPES
} HttpRequestType;

struct Client
{
        int clientFD;
        sockaddr_in clientAddr;
        std::string clientRequestIndex{""};
        HttpRequestType clientRequestType{NONE};

        void serveFile(const std::string& filePath);
};


#endif