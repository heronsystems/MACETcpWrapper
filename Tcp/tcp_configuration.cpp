#include "tcp_configuration.h"

#include <iostream>


TcpConfiguration::~TcpConfiguration()
{
    //mtb - configuration only mean to hold parmaters, not tehe object itself
    // Delete socket and break out of this loop:
    /*
    if (m_socket) {
        m_socket->close();
        delete m_socket;
        m_socket = NULL;
    }
    */
}

TcpConfiguration::TcpConfiguration(const std::string& listenAddress, const int &listenPort, const std::string& senderAddress, const int &senderPort)
{
    _listenAddress       = listenAddress;
    _listenPortNumber    = listenPort;
    _senderAddress       = senderAddress;
    _senderPortNumber    = senderPort;
}

TcpConfiguration::TcpConfiguration(TcpConfiguration* copy)
{
    _listenAddress       = copy->listenAddress();
    _listenPortNumber    = copy->listenPortNumber();
    _senderAddress       = copy->senderAddress();
    _senderPortNumber    = copy->senderPortNumber();
}


void TcpConfiguration::setListenAddress(std::string address)
{
    _listenAddress = address;
}

void TcpConfiguration::setListenPortNumber(int portNumber)
{
    _listenPortNumber = portNumber;
}

void TcpConfiguration::setSenderAddress(std::string address)
{
    _senderAddress = address;
}

void TcpConfiguration::setSenderPortNumber(int portNumber)
{
    _senderPortNumber = portNumber;
}



