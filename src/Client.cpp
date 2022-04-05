#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include "Client.h"

void Client::serveFile(const std::string& filePath)
{
        std::ifstream fileOpener;
        fileOpener.open(filePath, std::ios::in | std::ios::binary);
        if(fileOpener.is_open() == false)
                std::__throw_runtime_error("Invalid path to file!");

        std::string fileType = filePath.substr(filePath.find_last_of('.') + 1);
        std::stringstream stringStream;
        stringStream << fileOpener.rdbuf();

        std::string buffer{"HTTP/1.1 200 OK\r\nContent-Type: text/" + fileType + "\r\nContent-Length : "};
        buffer += std::to_string(stringStream.str().length()) + "\r\n\r\n";
        buffer += stringStream.str();


        send(clientFD, buffer.data(), buffer.length(), 0);
}