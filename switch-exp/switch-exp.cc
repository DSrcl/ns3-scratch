#include <vector>
#include "ns3/log.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/packet-socket-helper.h"
#include "ns3/point-to-point-module.h"
#include "ns3/internet-module.h"
#include "base-echo-client.h"
#include "base-echo-server.h"
#include "base-switch.h"
#include "utils.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("SwitchExperiment");

int main(int args, char *argv[])
{
	Time::SetResolution(Time::NS);

	/*
	// 1 to 1
	NodeContainer nodes;
	nodes.Create(3);


	// install devices on client and switch
	PointToPointHelper clientChannel;
	clientChannel.SetDeviceAttribute("DataRate", StringValue("0.5Mbps"));
	clientChannel.SetChannelAttribute("Delay", StringValue("2ms"));
	NetDeviceContainer clientDevices;
	clientDevices = clientChannel.Install(nodes.Get(0), nodes.Get(1));

	// install devices on server and switch
	PointToPointHelper serverChannel;
	serverChannel.SetDeviceAttribute("DataRate", StringValue("0.5Mbps"));
	serverChannel.SetChannelAttribute("Delay", StringValue("2ms"));
	serverChannel.SetQueue("ns3::DropTailQueue",
						"MaxPackets", UintegerValue(5));
	NetDeviceContainer serverDevices;
	serverDevices = serverChannel.Install(nodes.Get(1), nodes.Get(2));


	InternetStackHelper stack;
	stack.Install(nodes);


	Ipv4AddressHelper addressHelper;
	addressHelper.SetBase("10.1.1.0", "255.255.255.0");
	Ipv4InterfaceContainer clientInterfaces = addressHelper.Assign(clientDevices);
	addressHelper.SetBase("10.1.2.0", "255.255.255.0");
	Ipv4InterfaceContainer serverInterfaces = addressHelper.Assign(serverDevices);


	// install client application
	EchoClientHelper clientHelper;
	clientHelper.SetAttribute("SwitchAddress", 
							AddressValue(Address(clientInterfaces.GetAddress(1))));
	clientHelper.SetAttribute("PeerAddress", 
							AddressValue(Address(serverInterfaces.GetAddress(1))));
	clientHelper.SetAttribute("LocalAddress", 
							AddressValue(Address(clientInterfaces.GetAddress(0))));
	clientHelper.SetAttribute("Interval",
							TimeValue(Seconds(0.001)));
	clientHelper.SetAttribute("PacketSize",
							UintegerValue(5000));
	ApplicationContainer clientContainer = clientHelper.Install(nodes.Get(0));
	clientContainer.Start(Seconds(2.0));
	clientContainer.Stop(Seconds(10.0));

	// install switch application
	Ptr<BaseSwitch> switchApp =  CreateObject<BaseSwitch>();
	switchApp->ConnectPeer(clientInterfaces.GetAddress(0));
	switchApp->ConnectPeer(serverInterfaces.GetAddress(1));
	nodes.Get(1)->AddApplication(switchApp);
	switchApp->SetStartTime(Seconds(0.5));
	switchApp->SetStopTime(Seconds(10.0));

	// install server application
	EchoServerHelper serverHelper;
	serverHelper.SetAttribute("SwitchAddress", 
							AddressValue(Address(serverInterfaces.GetAddress(0))));
	serverHelper.SetAttribute("LocalAddress", 
							AddressValue(Address(serverInterfaces.GetAddress(1))));
	ApplicationContainer serverContainer = serverHelper.Install(nodes.Get(2));
	serverContainer.Start(Seconds(1.5));
	serverContainer.Stop(Seconds(10.0)); 

	clientChannel.EnableAscii("client", clientDevices.Get(0), false); 
	
	*/
	// 2 to 1
	NodeContainer nodes;
	nodes.Create(4);


	// install devices on client and switch
	PointToPointHelper clientChannel;
	clientChannel.SetDeviceAttribute("DataRate", StringValue("0.5Mbps"));
	clientChannel.SetChannelAttribute("Delay", StringValue("2ms"));
	NetDeviceContainer clientDevices;
	NetDeviceContainer clientDevices2;
	clientDevices = clientChannel.Install(nodes.Get(0), nodes.Get(2));
	clientDevices2 = clientChannel.Install(nodes.Get(1), nodes.Get(2));

	// install devices on server and switch
	PointToPointHelper serverChannel;
	serverChannel.SetDeviceAttribute("DataRate", StringValue("0.5Mbps"));
	serverChannel.SetChannelAttribute("Delay", StringValue("2ms"));
	serverChannel.SetQueue("ns3::DropTailQueue",
							"MaxPackets", UintegerValue(5));
	NetDeviceContainer serverDevices;
	serverDevices = serverChannel.Install(nodes.Get(2), nodes.Get(3));


	InternetStackHelper stack;
	stack.Install(nodes);


	Ipv4AddressHelper addressHelper;
	addressHelper.SetBase("10.1.1.0", "255.255.255.0");
	Ipv4InterfaceContainer clientInterfaces = addressHelper.Assign(clientDevices);
	addressHelper.SetBase("10.1.2.0", "255.255.255.0");
	Ipv4InterfaceContainer clientInterfaces2 = addressHelper.Assign(clientDevices2);
	addressHelper.SetBase("10.1.3.0", "255.255.255.0");
	Ipv4InterfaceContainer serverInterfaces = addressHelper.Assign(serverDevices);


	// install client application
	EchoClientHelper clientHelper;
	clientHelper.SetAttribute("SwitchAddress", 
							AddressValue(Address(clientInterfaces.GetAddress(1))));
	clientHelper.SetAttribute("PeerAddress", 
							AddressValue(Address(serverInterfaces.GetAddress(1))));
	clientHelper.SetAttribute("LocalAddress", 
							AddressValue(Address(clientInterfaces.GetAddress(0))));
	clientHelper.SetAttribute("Interval",
							TimeValue(Seconds(0.001)));
	clientHelper.SetAttribute("PacketSize",
							UintegerValue(5000));
	ApplicationContainer clientContainer = clientHelper.Install(nodes.Get(0));
	clientHelper.SetAttribute("LocalAddress",
							AddressValue(Address(clientInterfaces2.GetAddress(0))));
	clientContainer.Add(clientHelper.Install(nodes.Get(1)));
	clientContainer.Start(Seconds(2.0));
	clientContainer.Stop(Seconds(10.0));

	// install switch application
	Ptr<BaseSwitch> switchApp =  CreateObject<BaseSwitch>();
	switchApp->ConnectPeer(clientInterfaces.GetAddress(0));
	switchApp->ConnectPeer(clientInterfaces2.GetAddress(0));
	switchApp->ConnectPeer(serverInterfaces.GetAddress(1));
	nodes.Get(2)->AddApplication(switchApp);
	switchApp->SetStartTime(Seconds(0.5));
	switchApp->SetStopTime(Seconds(10.5));

	// install server application
	EchoServerHelper serverHelper;
	serverHelper.SetAttribute("SwitchAddress", 
							AddressValue(Address(serverInterfaces.GetAddress(0))));
	serverHelper.SetAttribute("LocalAddress", 
							AddressValue(Address(serverInterfaces.GetAddress(1))));
	ApplicationContainer serverContainer = serverHelper.Install(nodes.Get(3));
	serverContainer.Start(Seconds(1.5));
	serverContainer.Stop(Seconds(10.0));

	clientChannel.EnableAscii("client", clientDevices2.Get(0), false); 


	Simulator::Run();
	Simulator::Destroy();
	return 0;
}