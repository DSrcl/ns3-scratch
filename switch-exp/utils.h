#ifndef __utils_h__
#define __utils_h__

#include "ns3/core-module.h"
#include "ns3/ptr.h"
#include "ns3/network-module.h"
#include "base-echo-client.h"
#include "base-echo-server.h"
#include "base-switch.h"


Ipv4Address getPacketDest(Ptr<Packet> packet);
Ipv4Address getPacketSource(Ptr<Packet> packet);
class EchoClientHelper {
public:
	EchoClientHelper();
	void SetAttribute(std::string name,
					const AttributeValue &value);
	ApplicationContainer Install(Ptr<Node> node);
private:
	ObjectFactory m_factory;
	Ptr<Application> InstallPriv(Ptr<Node> node);
};


class EchoServerHelper {
public:
	EchoServerHelper();
	void SetAttribute(std::string name,
					const AttributeValue &value);
	ApplicationContainer Install(Ptr<Node> node);
private:
	ObjectFactory m_factory;
	Ptr<Application> InstallPriv(Ptr<Node> node);
};





#endif