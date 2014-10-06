#ifndef __base_echo_client_h__
#define __base_echo_client_h__

#include "ns3/log.h"
#include "ns3/ptr.h"
#include "ns3/event-id.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"

using namespace ns3;


class BaseEchoClient : public Application {
public:
	static TypeId GetTypeId();	
	BaseEchoClient();
	virtual ~BaseEchoClient();
protected:
	virtual void DoDispose();
private:
	virtual void StartApplication();
	virtual void StopApplication();
	virtual void ScheduleTransmit(Time dt);
	virtual void Send();
	virtual void HandleRead(Ptr<Socket> socket);
	virtual Address GetAddress();
	Ptr<Socket> m_socket;
	Ptr<Socket> m_socket2;
	Address m_switchAddress;
	Address m_peerAddress;
	Address m_local;
	uint32_t m_packetSize;
	uint8_t *m_data;
	EventId m_sendEvent;
	Time m_interval;
};


#endif