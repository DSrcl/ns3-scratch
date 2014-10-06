#include "ns3/log.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/socket-factory.h"
#include "ns3/socket.h"
#include "ns3/packet.h"
#include "ns3/ipv4-packet-info-tag.h"
#include "ns3/ipv4-header.h"
#include "ns3/type-id.h"
#include "base-switch.h"
#include "utils.h"


using namespace ns3;

NS_LOG_COMPONENT_DEFINE("BaseSwitch");
NS_OBJECT_ENSURE_REGISTERED(BaseSwitch);


BaseSwitch::BaseSwitch()
{
	NS_LOG_FUNCTION(this);
}

BaseSwitch::~BaseSwitch()
{
	NS_LOG_FUNCTION(this);
	for (int i = 0; i < m_messageSockets.size(); i++) {
		Ptr<Socket> socket = m_messageSockets[i];
		socket->SetRecvCallback(MakeNullCallback<void, Ptr<Socket> >());
	}
	m_listenSocket->SetRecvCallback(MakeNullCallback<void, Ptr<Socket> >());
}


void BaseSwitch::StartApplication()
{
	NS_LOG_FUNCTION(this);

	std::cout <<"base switch start" << std::endl;
	for (int i = 0; i < m_peerAddresses.size(); i++) {
		Ipv4Address peerAddress = m_peerAddresses[i];
		Ptr<Socket> socket = Socket::CreateSocket(GetNode(), TypeId::LookupByName("ns3::UdpSocketFactory"));
		socket->Bind();
		socket->Connect(InetSocketAddress(peerAddress, 9));
		socket->SetRecvCallback(MakeCallback(&BaseSwitch::HandleRead, this));
		m_messageSockets.push_back(socket);
	}
	m_listenSocket = Socket::CreateSocket(GetNode(), TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_listenSocket->Bind(InetSocketAddress(Ipv4Address::GetAny(), 9));
	m_listenSocket->Listen();
	m_listenSocket->SetRecvCallback(MakeCallback(&BaseSwitch::HandleRead, this));
}

void BaseSwitch::StopApplication()
{
	NS_LOG_FUNCTION(this);
	
	for (int i = 0; i < m_messageSockets.size(); i++) {
		Ptr<Socket> socket = m_messageSockets[i];
		if (socket != 0) {
			socket->Close();
			socket->SetRecvCallback(MakeNullCallback<void, Ptr<Socket> >());
		}
	}
	if (m_listenSocket != 0) {
		m_listenSocket->Close();
		m_listenSocket->SetRecvCallback(MakeNullCallback<void, Ptr<Socket> >());
	}
}



void BaseSwitch::DoDispose()
{
	NS_LOG_FUNCTION(this);
	Application::DoDispose();
}



void BaseSwitch::HandleAccept(Ptr<Socket> s, const Address& from)
{
	NS_LOG_FUNCTION(this);
	s->SetRecvCallback(MakeCallback(&BaseSwitch::HandleRead, this));
}




void BaseSwitch::HandleRead(Ptr<Socket> socket)
{
	NS_LOG_FUNCTION(this << socket);
	Ptr<Packet> packet;
	Ipv4Address from;
	Ipv4Address to;
	while ((packet = socket->Recv())) {
		from = getPacketSource(packet);
		to = getPacketDest(packet);

		NS_LOG_INFO("At time" << Simulator::Now().GetSeconds() << "s switch received " << packet->GetSize()
			        << " bytes from host " << from);

		packet->RemoveAllByteTags();
		packet->RemoveAllPacketTags();

		for (int i = 0; i < m_peerAddresses.size(); i++) {
			Ipv4Address peerAddress = m_peerAddresses[i];
			if (peerAddress == to) {
				std::cout << "switch: from " << from << " to " << to <<std::endl;
				NS_LOG_INFO("At time" << Simulator::Now().GetSeconds() << "s switch sent " << packet->GetSize()
					<< "bytes to host " << to);
				Ptr<Socket> socket = m_messageSockets[i];
				socket->Send(packet);
				break;
			}
		}
	}
}





void BaseSwitch::ConnectPeer(Ipv4Address peerAddress)
{
	m_peerAddresses.push_back(peerAddress);
}



















