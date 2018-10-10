#include "tcp_setup.h"

tcp_setup::tcp_setup(const std::string& listenAddress, const int &listenPort, const std::string& senderAddress, const int &senderPort) :
    m_Link(NULL)
{

    TcpConfiguration config(listenAddress, listenPort, senderAddress, senderPort);

    m_Link = new TcpLink(config);

    m_Link->Connect();

    m_Link->AddListener(this);
}

tcp_setup::~tcp_setup() {

    if(m_Link != NULL) {
        delete m_Link;
    }
}

/**
 * @brief SetOnNewVehicleCallback
 * Set lambda to be called when a new vehicle is discovered by DigiMesh
 * @param func lambda to call.
 */
void tcp_setup::SetOnNewVehicleCallback(std::function<void(const int)> func)
{
    this->m_NewVehicleCallback = func;
}


/**
 * @brief SetNewDataCallback
 * Set callback to be notified when new data has been transmitted to this node
 * @param func Function to call upon new data
 */
void tcp_setup::SetNewDataCallback(std::function<void(const std::vector<uint8_t> &)> func)
{
    this->m_NewDataCallback = func;
}


void tcp_setup::ReceiveData(TcpLink *link_ptr, const std::vector<uint8_t> &buffer)
{

    //do tcp receive here


    //add what we received to the current buffer.

    m_CurrBuffMutex.lock();
    for(size_t i = 0 ; i < buffer.size() ; i++) {
        m_CurrBuf.push_back(buffer.at(i));
    }
    m_CurrBuffMutex.unlock();


}

void tcp_setup::CommunicationError(const TcpLink* link_ptr, const std::string &type, const std::string &msg)
{

}

void tcp_setup::CommunicationUpdate(const TcpLink *link_ptr, const std::string &name, const std::string &msg)
{

}

void tcp_setup::Connected(const TcpLink* link_ptr)
{

}

void tcp_setup::ConnectionRemoved(const TcpLink *link_ptr)
{

}


