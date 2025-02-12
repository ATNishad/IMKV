#ifndef WIN32__LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#pragma comment(lib,"ws2_32")   //supported only in clang or MSVC

#define SERVER_PORT_NUM 42069


#include<winsock2.h>
#include<ws2tcpip.h>
#include<iostream>

    
int main(){
    
    //winsock initialization
    WSADATA wsadata;  
    int iresult;

    iresult = WSAStartup(MAKEWORD(2,2),&wsadata);
    if( iresult != 0){
        std::cout<<"WSASTARTUP FAILED:"<<WSAGetLastError<<"\n";
        return -1;
    }


    //server socket creation
    int optval = 1;
    SOCKET socket_handle = socket(AF_INET,SOCK_STREAM, 0);
    setsockopt(socket_handle,SOL_SOCKET,SO_REUSEADDR,(const char*)(&optval),sizeof(optval));

    
    //binding socket to port and ip
    sockaddr_in socketAddressBind;
    socketAddressBind.sin_family = AF_INET; 
    socketAddressBind.sin_port = htons(SERVER_PORT_NUM);
    socketAddressBind.sin_addr.s_addr = htonl(0); 

    int bindval = bind(socket_handle,(const sockaddr*)&socketAddressBind,sizeof(socketAddressBind));
    if( bindval != 0){
        std::cout<<"BIND FAILED:"<<WSAGetLastError<<"\n";
        return -1;
    }

    

    //close socket at end
    if(closesocket(socket_handle) != 0){
        std::cout<<"SOCKET FAILED TO CLOSE:"<< WSAGetLastError <<"\n";
        return -1;
    }

    

}