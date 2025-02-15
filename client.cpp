    #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #endif

    #pragma comment(lib,"ws2_32") //for clang or mvsc

    #define CLIENT_PORT_NUM 5000
    #define CLIENT_IP_ADDR "127.0.0.1"

    #include<winsock2.h>
    #include<windows.h>
    #include<iostream>
    #include<cassert>
    #include<cstdint>

    namespace full_SR{

        int receive_full(SOCKET handle,char* receive_buffer,size_t r_buffer_size){
            while( r_buffer_size > 0 ){ 
            ssize_t recv_size = recv(handle,receive_buffer,r_buffer_size,0);
            if(recv_size <= 0){
                return -1;  //receive error
            }
            assert(size_t(recv_size) <= r_buffer_size); //check size 
            r_buffer_size -= size_t(recv_size);
            receive_buffer += recv_size;
            }
            return 0;
        }
        
        int send_full(SOCKET handle,char* send_buffer,size_t s_buffer_size){
            while( s_buffer_size > 0){
            ssize_t send_size = send(handle,send_buffer,s_buffer_size,0);
            if(send_size <= 0){
                return -1;
            }
            assert((size_t)send_size <= s_buffer_size);
            s_buffer_size -= size_t(send_size);
            send_buffer += send_size;
            }
            return 0;
        }
        
    }


int32_t client_protocol(SOCKET socket, char* msg){

    const size_t MAX_MSG_SIZE= 2096;

    uint32_t s_len = (uint32_t)strlen(msg);
    if(s_len > MAX_MSG_SIZE){
        return -1;
    }

    char send_buffer[4+MAX_MSG_SIZE] = {};
    memcpy(send_buffer,&s_len,4);
    memcpy(&send_buffer[4],msg,s_len);
    int32_t s_err = full_SR::send_full(socket,send_buffer,4+s_len);
    if(s_err != 0){
        std::cerr<<"FULL SEND FAILED\n";
        return -1;
    }

    char receive_buffer[4+MAX_MSG_SIZE] = {};
    size_t r_buffer_size = sizeof(receive_buffer);

    uint32_t r_err = full_SR::receive_full(socket,receive_buffer,4);
    if(r_err != 0){
        std::cerr<<"FULL RECEIVE FAILED (byte)\n";
        return -1;        
    }

    uint32_t r_len = 0;
    memcpy(&r_len,receive_buffer,4);
    if(r_len > MAX_MSG_SIZE){
        std::cerr<<"RECEIVED MSG EXCEEDS BUFFER SIZE\n";
        return -1;
    }

    r_err = full_SR::receive_full(socket,&receive_buffer[4],r_len);
    if(r_err != 0){
        std::cerr<<"FULL RECEIVE FAILED (msg)\n";
        return -1;
    }

    std::cout<<"RECEIVED FROM SERVER:"<<&receive_buffer[4]<<"\n";
        return 0;
    }



    int main(){
        WSADATA wsadata;
        WSAStartup(MAKEWORD(2,2), &wsadata);

        int optval = 1;
        SOCKET client_socket_handle = socket(AF_INET,SOCK_STREAM,0);
        setsockopt(client_socket_handle,SOL_SOCKET,SO_REUSEADDR,(const char*)&optval,sizeof(optval));
        
        sockaddr_in clientSocketAddressBind = {};
        clientSocketAddressBind.sin_family = AF_INET;
        clientSocketAddressBind.sin_port = htons(CLIENT_PORT_NUM);
        clientSocketAddressBind.sin_addr.s_addr = inet_addr(CLIENT_IP_ADDR);


        connect(client_socket_handle,(const sockaddr*)&clientSocketAddressBind,sizeof(clientSocketAddressBind));

        char msg[] = "HELLO FROM CLIENT";
        int32_t err = client_protocol(client_socket_handle,msg);
        if(err != 0){
            std::cerr<<"CLIENT PROTOCOL ERROR: server offline\n";
            goto c_close;
        }

        std::cin.get();
    
        c_close:
        closesocket(client_socket_handle);
        WSACleanup();
    }