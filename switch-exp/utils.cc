#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ipv4-packet-info-tag.h"
#include "ns3/ipv4-header.h"
#include "base-echo-client.h"
#include "base-switch.h"
#include "base-echo-server.h"
#include "utils.h"




Ipv4Address getPacketDest(Ptr<Packet> packet)
{
	Ipv4Header header;
	packet->PeekHeader(header);
	return header.GetDestination();
}

Ipv4Address getPacketSource(Ptr<Packet> packet)
{
	Ipv4Header header;
	packet->PeekHeader(header);
	return header.GetSource();
}



// ---------------- Echo client --------------------
EchoClientHelper::EchoClientHelper()
{
	m_factory.SetTypeId(BaseEchoClient::GetTypeId());
}

void EchoClientHelper::SetAttribute(std::string name,
									const AttributeValue &value)
{
	m_factory.Set(name, value);
}

ApplicationContainer EchoClientHelper::Install(Ptr<Node> node)
{
	return ApplicationContainer(InstallPriv(node));
}

Ptr<Application> EchoClientHelper::InstallPriv(Ptr<Node> node)
{
	Ptr<Application> app = m_factory.Create<BaseEchoClient>();
	node->AddApplication(app);
	return app;
}

// ------------ Echo server client -----------

EchoServerHelper::EchoServerHelper()
{
	m_factory.SetTypeId(BaseEchoServer::GetTypeId());
}

void EchoServerHelper::SetAttribute(std::string name,
									const AttributeValue &value)
{
	m_factory.Set(name, value);
}

ApplicationContainer EchoServerHelper::Install(Ptr<Node> node)
{
	return ApplicationContainer(InstallPriv(node));
}

Ptr<Application> EchoServerHelper::InstallPriv(Ptr<Node> node)
{
	Ptr<Application> app = m_factory.Create<BaseEchoServer>();
	node->AddApplication(app);
	return app;
}



























