#include "WebServer.h"

int main(void)
{
    WebServer ws(8080);

    ws.get("/", [](Client& c) {
        c.serveFile("../index.html");
    });
    
    ws.run();
    return 0;
}
