#ifndef I_LINK_EVENTS_H
#define I_LINK_EVENTS_H

#include <cstdlib>
#include <vector>
#include <string>

class TcpLink;

class ILinkEvents
{
public:

    virtual void ReceiveData(TcpLink *link_ptr, const std::vector<uint8_t> &buffer) = 0;

    virtual void CommunicationError(const TcpLink* link_ptr, const std::string &type, const std::string &msg) = 0;

    virtual void CommunicationUpdate(const TcpLink *link_ptr, const std::string &name, const std::string &msg) = 0;

    virtual void Connected(const TcpLink* link_ptr) = 0;

    virtual void ConnectionRemoved(const TcpLink *link_ptr) = 0;
};


#endif // I_LINK_EVENTS_H
