#ifndef TCPCONFIGURATION_H
#define TCPCONFIGURATION_H

#include "tcp_global.h"
#include <QTcpSocket>
#include <QStringList>
#include <string>


class TCPSHARED_EXPORT TcpConfiguration
{

public:

    TcpConfiguration(const std::string& serverAddress, const int &serverPort, const std::string& clientAddress, const int &clientPort);
    TcpConfiguration(TcpConfiguration* copy);

    // Destructor
    ~TcpConfiguration();

    int getServerPortNumber() const { return serverPortNumber; }
    const std::string getServerAddress() const { return serverAddress; }
    int getClientPortNumber() const { return clientPortNumber; }
    const std::string getClientAddress() const { return clientAddress; }

    void setServerPortNumber(int portNumber);
    void setServerAddress(std::string address);
    void setClientPortNumber(int portNumber);
    void setClientAddress(std::string address);

private:

    int serverPortNumber;
    std::string serverAddress;
    int clientPortNumber;
    std::string clientAddress;

};

#endif // TCPCONFIGURATION_H
