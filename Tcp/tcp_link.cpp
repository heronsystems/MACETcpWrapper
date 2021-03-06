#include "tcp_link.h"
#include <QHostAddress>
#include <QCoreApplication>


 //!
//! \brief This class defines a thread such that a QObject can run in peace.
//!
class ClientThread : public QThread
{
public:
    ClientThread(const std::function<void(void)> &func):
        m_func(func),
        m_Shutdown(false)
    {
        if(QCoreApplication::instance() == NULL)
        {
            int argc = 0;
            char * argv[] = {(char *)"sharedlib.app"};
            pApp = new QCoreApplication(argc, argv);
        }
    }

    virtual void run()
    {
        while(true)
        {
            if(m_Shutdown == true)
            {
                break;
            }

            QCoreApplication::processEvents();
            m_func();
        }
    }

    virtual void shutdown()
    {
        m_Shutdown = true;
        this->wait();
    }

private:

    std::function<void(void)> m_func;
    QCoreApplication *pApp;
    bool m_Shutdown;
};

class ServerThread : public QThread
{
public:
    ServerThread(const std::function<void(void)> &func):
        m_func(func),
        m_Shutdown(false)
    {
        if(QCoreApplication::instance() == NULL)
        {
            int argc = 0;
            char * argv[] = {(char *)"sharedlib.app"};
            pApp = new QCoreApplication(argc, argv);
        }
    }

    virtual void run()
    {
        while(true)
        {
            if(m_Shutdown == true)
            {
                break;
            }
            QCoreApplication::processEvents();
            m_func();
        }
    }

    virtual void shutdown()
    {
        m_Shutdown = true;
        this->wait();
    }

private:

    std::function<void(void)> m_func;
    QCoreApplication *pApp;
    bool m_Shutdown;
};



TcpLink::TcpLink(const TcpConfiguration &config) :
    _config(config)
{
    tcpClient    = NULL;
    m_bytesRead = 0;
    m_stopp    = false;
    m_reqReset = false;

    std::cout << "Create TcpLink: " << config.getServerAddress() << ":" << config.getServerPortNumber() << std::endl;
}

TcpLink::~TcpLink()
{
    Disconnect();
    if(tcpClient) delete tcpClient;
    tcpClient = NULL;
}

void TcpLink::RequestReset()
{
    m_stoppMutex.lock();
    m_reqReset = true;
    m_stoppMutex.unlock();
}


bool TcpLink::Connect(void)
{
    Disconnect();

    QTcpSocket::SocketError error;
    QString errorString;

    // Initialize the connection
    if (!_hardwareConnect(error, errorString)) {
        /*if (_config.isAutoConnect()) {
            // Be careful with spitting out open error related to trying to open a busy port using autoconnect
            if (error == QTcpSocket::AddressInUseError) {
                // Device already open, ignore and fail connect
                return false;
            }

        }*/

        _emitLinkError("Error connecting: Could not create port. " + errorString.toStdString());
        return false;
    }
    return true;
}

void TcpLink::Disconnect(void)
{
    if (tcpClient) {

        ((ClientThread*)m_ListenThread)->shutdown();
        delete m_ListenThread;

        tcpClient->close();

        delete tcpClient;
        tcpClient = NULL;
    }
}


/// Performs the actual hardware port connection.
///     @param[out] error if failed
///     @param[out] error string if failed
/// @return success/fail
bool TcpLink::_hardwareConnect(QAbstractSocket::SocketError &error, QString& errorString)
{
    if (tcpClient) {
        std::cout << "TcpLink:" << QString::number((long)this, 16).toStdString() << "closing port" << std::endl;
        tcpClient->close();
        std::this_thread::sleep_for(std::chrono::microseconds(50000));
        delete tcpClient;
        tcpClient = NULL;
    }

    std::cout << "UdpLink: hardwareConnect to " << _config.getServerAddress() << ":" << _config.getServerPortNumber() << std::endl;

    tcpClient = new QTcpSocket();
    tcpClient->bind(QHostAddress(QString::fromStdString((_config.getServerAddress()))), _config.getServerPortNumber(), QTcpSocket::ShareAddress);
    //tcpClient->connectToHost(QHostAddress(QString::fromStdString((_config.address()))), _config.portNumber());
    //tcpClient->waitForConnected(1000);
    //tcpClient->bind(_config.portNumber(), QUdpSocket::ShareAddress);

    for (int openRetries = 0; openRetries < 4; openRetries++) {
        if (!tcpClient->open(QIODevice::ReadWrite)) {
            //std::cout << "Port open failed, retrying" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        } else {
            break;
        }
    }

    if (!tcpClient->isOpen() ) {
        //std::cerr << "open failed" << m_port->errorString().toStdString() << m_port->error() << getName() << _config.isAutoConnect() << std::endl;
        error = tcpClient->error();
        errorString = tcpClient->errorString();
        EmitEvent([&](ILinkEvents *ptr){ptr->CommunicationUpdate(this, _config.getServerAddress(), "Error opening port: " + errorString.toStdString());});
        tcpClient->close();
        delete tcpClient;
        tcpClient = NULL;
        return false; // couldn't open udp port
    }


    // TODO: Figure out the alternative to this:
    EmitEvent([this](ILinkEvents *ptr){ptr->CommunicationUpdate(this, getClientAddress(), "Opened port!");});
    EmitEvent([this](ILinkEvents *ptr){ptr->Connected(this);});

    std::cout << "Connection TCPLink: " << "with settings " << _config.getServerAddress() << ":" << _config.getServerPortNumber() << std::endl;


    m_ListenThread = new ClientThread([&](){
        if(tcpClient->waitForReadyRead(300))
            this->processData();

        if(tcpClient->errorString() != "")
        {
//            std::cout << "Socket error: " << tcpClient->errorString().toStdString() << std::endl;
        }

/*
        tcpServer = std::make_shared<QTcpServer>();
        m_ListenThread = new ServerThread([&](){
            if(tcpServer->hasPendingConnections())
                this->on_newConnection();
        });

        // For some reason, listening on any other specific address (i.e. not Any) fails.
        //      - As a workaround, I check the incoming connection below for equality with the guiHostAddress before parsing
        tcpServer->listen(QHostAddress::Any, m_listenPort);
    //    tcpServer->listen(m_guiHostAddress, m_listenPort);
    */
    });

    tcpServer.moveToThread(m_ListenThread);
    m_ListenThread->start();


    if(!tcpServer.isListening())
    {
        std::cout << "Server could not start..." << std::endl;
    }
    else
    {
        std::cout << "GUI TCP Server started" << std::endl;
    }

    return tcpServer.isListening();
}


void TcpLink::WriteBytes(const char *bytes, int length)
{
    QByteArray data(bytes, length);
    if(tcpClient && tcpClient->isOpen()) {
        // TODO: Listen for UDP messages, identify sender port, set _config.senderPort (or something), use that to send.
        //          --May want to have a senderAddress as well...
    //TODO: ETW: use correct write for TCP    tcpClient->writeDatagram(data, QHostAddress(QString::fromStdString(_config.senderAddress())), _config.senderPortNumber());
    } else {
        // Error occured
        _emitLinkError("Could not send data - link " + getClientAddress() + ":" + std::to_string(getClientPortNumber()) + " is disconnected!");
    }
}



//!
//! \brief Determine the connection status
//! \return True if the connection is established, false otherwise
//!
bool TcpLink::isConnected() const
{
    bool isConnected = false;

    if (tcpClient) {
        isConnected = tcpClient->isOpen();
    }

    return isConnected;
}


void TcpLink::_emitLinkError(const std::string& errorMsg)
{
    std::string msg = "Error on link " + getServerAddress() + ":" + std::to_string(getServerPortNumber()) + " - " + errorMsg;
    EmitEvent([&](ILinkEvents *ptr){ptr->CommunicationError(this, "Link Error", msg);});
}


void TcpLink::processData(void)
{

        QByteArray dataBuff(tcpClient->readAll());

        std::vector<uint8_t> vec_buffer = std::vector<uint8_t>(dataBuff.begin(), dataBuff.end());
        EmitEvent([this,&vec_buffer](ILinkEvents *ptr){ptr->ReceiveData(this, vec_buffer);});
  //  }

}

void TcpLink::linkError(QTcpSocket::SocketError error)
{
    switch (error) {
    case QTcpSocket::AddressInUseError:
        EmitEvent([this](ILinkEvents *ptr){ptr->ConnectionRemoved(this);});
        break;
    default:
        break;
    }
}



