#include "ns3/log.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ethernet-header.h"
#include "ns3/socket-factory.h"
#include "ns3/socket.h"
#include "ns3/packet.h"
#include "ns3/ipv4-packet-info-tag.h"
#include "ns3/ipv4-header.h"
#include "base-echo-server.h"
#include "utils.h"


using namespace ns3;

NS_LOG_COMPONENT_DEFINE("BaseEchoServer");
NS_OBJECT_ENSURE_REGISTERED(BaseEchoServer);

TypeId BaseEchoServer::GetTypeId()
{
	static TypeId tid = TypeId("BaseEchoServer")
	.SetParent<Application>()
	.AddConstructor<BaseEchoServer>()
	.AddAttribute("SwitchAddress",
				"The address of switch connected",
				AddressValue(),
				MakeAddressAccessor(&BaseEchoServer::m_switchAddress),
				MakeAddressChecker())
	.AddAttribute("LocalAddress",
				"Local address",
				AddressValue(),
				MakeAddressAccessor(&BaseEchoServer::m_local),
				MakeAddressChecker());
	return tid;
}



BaseEchoServer::BaseEchoServer()
{
	NS_LOG_FUNCTION(this);
	m_socket = 0;
}

BaseEchoServer::~BaseEchoServer()
{
	NS_LOG_FUNCTION(this);
	m_socket = 0;
}

void BaseEchoServer::StartApplication()
{
	NS_LOG_FUNCTION(this);
	std::cout << "base echo server start" << std::endl;
	if (m_socket == 0) {
		TypeId tid = TypeId::LookupByName("ns3::UdpSocketFactory");
		m_socket = Socket::CreateSocket(GetNode(), tid);
		m_socket->Bind(InetSocketAddress(Ipv4Address::GetAny(), 9));
		m_socket->Listen();
	}

	m_socket->SetRecvCallback(MakeCallback(&BaseEchoServer::HandleRead, this));
	m_socket->SetAcceptCallback(
			MakeNullCallback<bool, Ptr<Socket>, const Address&>(),
			MakeCallback(&BaseEchoServer::HandleAccept, this));
}

void BaseEchoServer::StopApplication()
{
	NS_LOG_FUNCTION(this);

	if (m_socket != 0) {
		m_socket->Close();
		m_socket->SetRecvCallback(MakeNullCallback<void, Ptr<Socket> >());
	}
}


void BaseEchoServer::DoDispose()
{
	NS_LOG_FUNCTION(this);
	Application::DoDispose();
}


void BaseEchoServer::HandleAccept(Ptr<Socket> s, const Address& from)
{
	NS_LOG_FUNCTION(this);
	s->SetRecvCallback(MakeCallback(&BaseEchoServer::HandleRead, this));
}


void BaseEchoServer::HandleRead(Ptr<Socket> socket)
{
	Ptr<Packet> packet;
	Ipv4Address from;
	Address switchAddress;
	while ((packet = socket->RecvFrom(switchAddress))) {
		from = getPacketSource(packet);
		NS_LOG_INFO("At time" << Simulator::Now().GetSeconds() << "s server received " << packet->GetSize()
			        << " bytes from " << from);

		packet->RemoveAllPacketTags();
		packet->RemoveAllByteTags();

		Ipv4Header header;
		packet->RemoveHeader(header);
		header.SetSource(Ipv4Address::ConvertFrom(m_local));
		header.SetDestination(from);
		packet->AddHeader(header);
		socket->SendTo(packet, 0, switchAddress);

		NS_LOG_INFO("At time" << Simulator::Now().GetSeconds() << "s server sent " << packet->GetSize()
			        << " bytes to " << from);
	}
}



















