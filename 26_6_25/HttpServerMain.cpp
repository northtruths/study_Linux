#include "HttpServer.hpp"

int main(){
    HttpServer hs(8888);
    hs.start();
    return 0;
}