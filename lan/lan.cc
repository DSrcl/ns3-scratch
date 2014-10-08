#include "lan-helper.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/core-module.h"
#include "ns3/applications-module.h"




/*
*      echo clients
*    _     _    _   _
*   |_|   |_|  |_| |_| ...
*    |     |    |   |
*    |     |    -----
*    |     |    |   |  <--- another switch
*    |     |    -----
*    |     |      |     
*   |--------------|
*   |              |
*   |    switch    |
*   |              |
*   |--------------|
*          |
*          |
*        |---|
*        |___|
*      echo server
* 
* ================================
* 
*   experiment testing LanHelper, a class for simplifying lan network of hosts connected by switches
* 
*/



using namespace ns3;

int main(int argc, char* argv[])
{
    NodeContainer terminalNodes1;
    terminalNodes1.Create(2);
    NodeContainer terminalNodes2;
    terminalNodes2.Create(2);
    NodeContainer terminalNodes3;
    terminalNodes3.Create(1);
    NodeContainer switchNodes;
    switchNodes.Create(2);


    SimpleNetDeviceHelper deviceHelper;
    deviceHelper.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    deviceHelper.SetChannelAttribute("Delay", StringValue("2ms"));

    LanHelper lan(deviceHelper);
    lan.addConnection(switchNodes.Get(0), NodeContainer(switchNodes.Get(1), terminalNodes1));
    lan.addConnection(switchNodes.Get(1), NodeContainer(terminalNodes2, terminalNodes3));
    NetDeviceContainer terminalDevices = lan.install();

    // locate index of the server in terminalDevices
    int serverIndex = lan.findTerminal(terminalNodes3.Get(0));
    NS_ASSERT(serverIndex != -1);

    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces = address.Assign(terminalDevices);

    UdpEchoServerHelper server(9);
    ApplicationContainer serverApps = server.Install(terminalNodes3);
    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(5.5));


    UdpEchoClientHelper client(interfaces.GetAddress(serverIndex), 9);
    client.SetAttribute("MaxPackets", UintegerValue(100000));
    client.SetAttribute("Interval", TimeValue(Seconds(0.01)));
    client.SetAttribute("PacketSize", UintegerValue(5000));
    ApplicationContainer clientApps = client.Install(terminalNodes1);
    clientApps.Add(client.Install(terminalNodes2));
    clientApps.Start(Seconds(1.5));
    clientApps.Stop(Seconds(5.0));

    Simulator::Run();
    Simulator::Destroy();
    return 0;
}
