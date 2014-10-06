#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/csma-module.h"
#include "ns3/bridge-module.h"
#include "ns3/applications-module.h"
#include "ns3/log.h"




/*
	   echo clients
	 _	 _	 _	 _
 	|_|	|_| |_|	|_| ...
 	 |	 |	 |	 |	 
 	 |	 |	 |	 |	 
	|--------------|
	|              |
	|    switch    |
	|              |
	|--------------|
	       |
	       |
	     |---|
	     |___|
	   echo server
	
*/



using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Switch");



int main (int argc, char *argv[])
{
	Time::SetResolution(Time::MS);


	NodeContainer terminalNodes1;
	terminalNodes1.Create(2);
	NodeContainer terminalNodes2;
	terminalNodes2.Create(3);

	NodeContainer switchContainer1;
	//small siwtch
	switchContainer1.Create(1);

	NodeContainer switchContainer2;
	//big switch
	switchContainer2.Create(1);


	SimpleNetDeviceHelper simpleDevice;
	simpleDevice.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
	simpleDevice.SetChannelAttribute("Delay", StringValue("2ms"));

	// terminal for everything
	NetDeviceContainer terminalDevices;
	// small switch
	NetDeviceContainer switchDevices1;
	// big switch
	NetDeviceContainer switchDevices2;



	for (int i =0; i<2; i++) {
		NetDeviceContainer link = simpleDevice.Install(NodeContainer(terminalNodes1.Get(i), switchContainer1));
		terminalDevices.Add(link.Get(0));
		switchDevices1.Add(link.Get(1));
	}
	for (int i =0; i<2; i++) {
		NetDeviceContainer link = simpleDevice.Install(NodeContainer(terminalNodes2.Get(i), switchContainer2));
		terminalDevices.Add(link.Get(0));
		switchDevices2.Add(link.Get(1));
	}
	//connect switch to switch
	NetDeviceContainer link = simpleDevice.Install(NodeContainer(switchContainer1.Get(0), switchContainer2));
	switchDevices1.Add(link.Get(0));
	switchDevices2.Add(link.Get(1));
	// connect switch to server
	link = simpleDevice.Install(NodeContainer(terminalNodes2.Get(2), switchContainer2));
	terminalDevices.Add(link.Get(0));
	switchDevices2.Add(link.Get(1));
	

	BridgeHelper bridge;
	bridge.Install(switchContainer1.Get(0), switchDevices1);
	bridge.Install(switchContainer2.Get(0), switchDevices2);


	InternetStackHelper internet;
	internet.Install(terminalNodes1);
	internet.Install(terminalNodes2);


	Ipv4AddressHelper addressHelper;
	addressHelper.SetBase("10.1.1.0", "255.255.255.0");
	Ipv4InterfaceContainer interfaces = addressHelper.Assign(terminalDevices);

	UdpEchoServerHelper echoServer(9);
	ApplicationContainer serverApp = echoServer.Install(terminalNodes2.Get(2));
	serverApp.Start(Seconds(1.0));
	serverApp.Stop(Seconds(5.0));

	UdpEchoClientHelper echoClient(interfaces.GetAddress(4), 9);
	echoClient.SetAttribute("MaxPackets", UintegerValue(1000));
	echoClient.SetAttribute("Interval", TimeValue(Seconds(0.01)));
	echoClient.SetAttribute("PacketSize", UintegerValue(5000));
	ApplicationContainer clientApps;
	for (int i = 0; i < 2; i++) {
		Ptr<Node> client = terminalNodes1.Get(i);
		clientApps.Add(echoClient.Install(client));
	}
	for (int i = 0; i < 2; i++) {
		Ptr<Node> client = terminalNodes2.Get(i);
		clientApps.Add(echoClient.Install(client));
	}
	clientApps.Start(Seconds(1.5));
	clientApps.Stop(Seconds(4.5));


	Simulator::Run();
	Simulator::Destroy();
	return 0;
}













