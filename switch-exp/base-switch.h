#ifndef __base_switch_h__
#define __base_switch_h__

#include <vector>
#include "ns3/log.h"
#include "ns3/ptr.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"


using namespace ns3;


class BaseSwitch : public Application {
public:
	BaseSwitch();
	virtual ~BaseSwitch();
	void ConnectPeer(Ipv4Address peerAddress);
protected:
	virtual void DoDispose();
private:
	virtual void StartApplication();
	virtual void StopApplication();
	virtual void HandleRead(Ptr<Socket> socket);
	virtual void HandleAccept(Ptr<Socket> s, const Address& from);
	Address m_local;
	std::vector<Ipv4Address> m_peerAddresses;
	Ptr<Socket> m_listenSocket;
	std::vector<Ptr<Socket> > m_messageSockets;
};


#endif