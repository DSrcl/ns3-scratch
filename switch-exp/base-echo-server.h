#ifndef __base_echo_server_h__
#define __base_echo_server_h__

#include "ns3/log.h"
#include "ns3/ptr.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"


using namespace ns3;

class BaseEchoServer : public Application {
public:
	static TypeId GetTypeId();
	BaseEchoServer();
	virtual ~BaseEchoServer();
protected:
	virtual void DoDispose();
private:
	virtual void StartApplication();
	virtual void StopApplication();
	virtual void HandleRead(Ptr<Socket> socket);
	virtual void HandleAccept(Ptr<Socket> socket, const Address& from);
	Address m_switchAddress;
	Address m_selfAddress;
	Address m_local;
	Ptr<Socket> m_socket;
};







#endif
