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
    serverAddress       = listenAddress;
    serverPortNumber    = listenPort;
    clientAddress       = senderAddress;
    clientPortNumber    = senderPort;
}

TcpConfiguration::TcpConfiguration(TcpConfiguration* copy)
{
    serverAddress        = copy->getServerAddress();
    serverPortNumber     = copy->getServerPortNumber();
    clientAddress        = copy->getClientAddress();
    clientPortNumber     = copy->getClientPortNumber();
}


void TcpConfiguration::setServerAddress(std::string address)
{
    serverAddress = address;
}

void TcpConfiguration::setServerPortNumber(int portNumber)
{
    serverPortNumber = portNumber;
}

void TcpConfiguration::setClientAddress(std::string address)
{
    clientAddress = address;
}

void TcpConfiguration::setClientPortNumber(int portNumber)
{
    clientPortNumber = portNumber;
}



