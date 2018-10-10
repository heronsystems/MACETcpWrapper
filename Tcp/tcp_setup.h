#ifndef TCP_SETUP_H
#define TCP_SETUP_H

#include <vector>
#include <map>
#include <functional>
#include <mutex>

#include <iostream>
#include <i_link_events.h>
#include <tcp_global.h>
#include <tcp_link.h>

class TCPSHARED_EXPORT tcp_setup: public ILinkEvents
{
public:
    tcp_setup(const std::string& listenAddress, const int &listenPort, const std::string& senderAddress, const int &senderPort);

    ~tcp_setup();

    /**
     * @brief SetOnNewVehicleCallback
     * Set lambda to be called when a new vehicle is discovered by DigiMesh
     * @param func lambda to call.
     */
    void SetOnNewVehicleCallback(std::function<void(const int)> func);


    /**
     * @brief SetNewDataCallback
     * Set callback to be notified when new data has been transmitted to this node
     * @param func Function to call upon new data
     */
    void SetNewDataCallback(std::function<void(const std::vector<uint8_t> &)> func);

    virtual void ReceiveData(TcpLink *link_ptr, const std::vector<uint8_t> &buffer);

    virtual void CommunicationError(const TcpLink* link_ptr, const std::string &type, const std::string &msg);

    virtual void CommunicationUpdate(const TcpLink *link_ptr, const std::string &name, const std::string &msg);

    virtual void Connected(const TcpLink* link_ptr);

    virtual void ConnectionRemoved(const TcpLink *link_ptr);


private:

     TcpLink *m_Link;

     std::vector<int> m_OwnVehicles;
     std::map<int, int> m_RemoteVehiclesToAddress;

     std::function<void(const int)> m_NewVehicleCallback;
     std::function<void(const std::vector<uint8_t> &)> m_NewDataCallback;

     std::vector<uint8_t> m_CurrBuf;
     std::mutex m_CurrBuffMutex;

};

#endif // TCP_SETUP_H
