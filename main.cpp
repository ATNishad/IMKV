#ifndef WIN32__LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

//supported only in clang or MSVC
#pragma comment(lib,"ws2_32")

#include<winsock2.h>
#include<ws2tcpip.h>
#include<iphlpapi.h>
#include<iostream>



int main(){
    
    WSADATA wsadata;  
    int iresult;

    iresult = WSAStartup(MAKEWORD(2,2),&wsadata);
    if( iresult != 0){
        std::cout<<"WSASTARTUP FAILED";
        return -1;
    }
    return 0;
}