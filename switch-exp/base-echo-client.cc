#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/nstime.h"
#include "ns3/core-module.h"
#include "ns3/simulator.h"
#include "ns3/network-module.h"
#include "ns3/ethernet-header.h"
#include "ns3/socket-factory.h"
#include "ns3/socket.h"
#include "ns3/ipv4-packet-info-tag.h"
#include "ns3/ipv4-header.h"
#include "ns3/trace-source-accessor.h"
#include "base-echo-client.h"
#include "utils.h"


using namespace ns3;

NS_LOG_COMPONENT_DEFINE("BaseEchoClient");
NS_OBJECT_ENSURE_REGISTERED(BaseEchoClient);


TypeId BaseEchoClient::GetTypeId()
{
	static TypeId tid = TypeId("BaseEchoClient")
	.SetParent<Application>()
	.AddConstructor<BaseEchoClient>()
	.AddAttribute("Interval",
				"The time to wait between packets",
				TimeValue(Seconds(1.0)),
				MakeTimeAccessor(&BaseEchoClient::m_interval),
				MakeTimeChecker())
	.AddAttribute("PacketSize",
				"Size of the packets to be sent",
				UintegerValue(0),
				MakeUintegerAccessor(&BaseEchoClient::m_packetSize),
				MakeUintegerChecker<uint32_t>())
	.AddAttribute("SwitchAddress",
				"The address of switch connected",
				AddressValue(),
				MakeAddressAccessor(&BaseEchoClient::m_switchAddress),
				MakeAddressChecker())
	.AddAttribute("PeerAddress",
				"The address of switch connected",
				AddressValue(),
				MakeAddressAccessor(&BaseEchoClient::m_peerAddress),
				MakeAddressChecker())
	.AddAttribute("LocalAddress",
				"Local address",
				AddressValue(),
				MakeAddressAccessor(&BaseEchoClient::m_local),
				MakeAddressChecker());
	return tid;
}


BaseEchoClient::BaseEchoClient() {
	NS_LOG_FUNCTION(this);
	m_packetSize = 0;
	m_socket = 0;
	m_socket2 = 0;
	m_sendEvent = EventId();
}



BaseEchoClient::~BaseEchoClient()
{
	NS_LOG_FUNCTION(this);
}



void BaseEchoClient::StartApplication()
{
	NS_LOG_FUNCTION(this);

	std::cout << "base echo client start" << std::endl;

	TypeId tid = TypeId::LookupByName("ns3::UdpSocketFactory");
	m_socket = Socket::CreateSocket(GetNode(), tid);
	m_socket->Bind();
	m_socket->Connect(InetSocketAddress(Ipv4Address::ConvertFrom(m_switchAddress), 9));
	
	
	m_socket2 = Socket::CreateSocket(GetNode(), TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_socket2->Bind(InetSocketAddress(Ipv4Address::GetAny(), 9));
	m_socket2->Listen();
	m_socket2->SetRecvCallback(MakeCallback(&BaseEchoClient::HandleRead, this));

	ScheduleTransmit(Seconds(0.0));
}



void BaseEchoClient::StopApplication()
{
	NS_LOG_FUNCTION(this);
	Simulator::Cancel(m_sendEvent);
	m_socket2->SetRecvCallback(MakeNullCallback<void, Ptr<Socket> >());
}


void BaseEchoClient::DoDispose()
{
	NS_LOG_FUNCTION(this);
	Application::DoDispose();
}

void BaseEchoClient::ScheduleTransmit(Time dt)
{
	NS_LOG_FUNCTION(this);
	m_sendEvent = Simulator::Schedule(dt, &BaseEchoClient::Send, this);
}



void BaseEchoClient::Send()
{
	NS_LOG_FUNCTION(this);
	NS_ASSERT(m_socket != 0);
	m_data = new uint8_t[m_packetSize];
	Ptr<Packet> packet = Create<Packet>(m_data, m_packetSize);	
	Ipv4Header header;
	header.SetSource(Ipv4Address::ConvertFrom(m_local));
	header.SetDestination(Ipv4Address::ConvertFrom(m_peerAddress));
	packet->AddHeader(header);
	m_socket->Send(packet);
	ScheduleTransmit(m_interval);
}


void BaseEchoClient::HandleRead(Ptr<Socket> socket)
{
	NS_LOG_FUNCTION(this << socket);

	Ptr<Packet> packet;
	Ipv4Address from;
	while ((packet = socket->Recv())) {
		from = getPacketSource(packet);
		NS_LOG_INFO("At time" << Simulator::Now().GetSeconds() << "s client received " <<  packet->GetSize()
			        << " bytes from " << from);
	}
}

Address BaseEchoClient::GetAddress()
{
	// each device has one device, which is used to connect its switch
	return GetNode()->GetDevice(0)->GetAddress();
}










