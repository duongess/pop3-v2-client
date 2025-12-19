#include "tcpclient.h"
#include <iostream>
#include <fstream>

TcpClient::TcpClient()
{
    connected = false;

}

TcpClient::~TcpClient()
{

}


bool TcpClient::open(const std::string& serverHost, unsigned short port)
{
    try
    {
        this->localsocket.connect(serverHost,port);
        this->connected = true;
        return true;

    }
    catch(SocketException& e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

bool TcpClient::open(const std::string& serverHost, const std::string& port)
{
    try
    {
        this->localsocket.connect(serverHost,port);
        this->connected = true;
        return true;

    }
    catch(SocketException& e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
}


int TcpClient::sendStringRequest(const std::string& request)
{
    try
    {
        return this->localsocket.send(request);
    }
    catch(SocketException& e)
    {
        std::cerr << e.what();
        return -1;
    }
}
int TcpClient::sendDataBuffer(const char* buffer, unsigned int bufLen)
{
    try
    {
       return this->localsocket.send(buffer, bufLen);
    }
    catch(SocketException& e)
    {
        std::cerr << e.what();
        return -1;
    }
}
int TcpClient::recvGetLine(char* buffer, unsigned int maxLen)
{
    int bytes;
    try
    {
        bytes = localsocket.recvLine(buffer, maxLen-1);
        if(bytes>2)
        {
            buffer[bytes-2] = 0;
        }
        else
        {
            *buffer =0;
        }
        return bytes;
    }
    catch(SocketException& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}

int TcpClient::recvDataBuffer(char* buffer, unsigned int bufLen)
{
    int bytes;
    try
    {
        bytes = localsocket.recv(buffer, bufLen-1);
        return bytes;
    }
    catch(SocketException& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}

int TcpClient::recvStringBuffer(char* buffer, unsigned int bufLen)
{
    int bytes;
    try
    {
        bytes = localsocket.recv(buffer, bufLen-1);
        if(bytes>0)
        {
            buffer[bytes]=0;
        }
        else
        {
            *buffer = 0;
        }
        return bytes;
    }
    catch(SocketException& e)
    {
        std::cerr << e.what() << std::endl;
        *buffer = 0;
        return -1;
    }
}


void TcpClient::close()
{
    try
    {
        localsocket.shutdown(SHUTDOWN_BOTH);
        localsocket.close();
    }
    catch(SocketException& e)
    {
        std::cerr << e.what() << std::endl;
    }
    connected = false;
}

void TcpClient::printLn(const std::string& msg)
{
    std::cout << msg << std::endl << std::endl;

}

void TcpClient::print(const std::string& msg)
{
    std::cout << msg << std::endl;

}

