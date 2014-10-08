#include <map>
#include <set>
#include "ns3/core-module.h"
#include "ns3/network-module.h"


/*
    Class for setting up a LAN network where hosts are connected by switches.
*/

using namespace ns3;

class LanHelper {
private:
    // links of the topology, keys are the ids of the switchs
    // values are ids are the ids of the terminal nodes
    std::map<uint32_t, std::set<uint32_t> > m_connections;
    // helper for installing net device
    SimpleNetDeviceHelper m_deviceHelper;
    // container for switch devices; keys are ids of switch nodes
    std::map<uint32_t, NetDeviceContainer> m_switchDevices;
    // terminal nodes
    NodeContainer m_terminalNodes;
    // terminal devices
    NetDeviceContainer m_terminalDevices;
    // function for pushing switch device into m_switchDevices
    void putSwitchDevice(uint32_t switchId, Ptr<NetDevice> switchDevice);
    // function for installing switch given switch devices and their nodes
    void installSwitches();
    // function for building the topology
    void buildTopology();
    // function for giving terminal nodes ipv4/6, tcp, udp capability, etc
    void installInternetStack();
    // function for geting ids of terminal nodes based on the connections
    std::set<uint32_t> getTerminalIds();
    // helper function for getting nodes given their ids
    static NodeContainer getNodesFromIds(std::set<uint32_t> nodeIds);
public:
    LanHelper(SimpleNetDeviceHelper deviceHelper);
    ~LanHelper();
    // find index of a terminal in m_terminalNodes and m_terminalDevices given the node
    // return -1 if not found
    uint32_t findTerminal(const Ptr<Node> node);
    // function to specify topology
    void addConnection(const Ptr<Node> switchNode, const NodeContainer& nodes);
    // function to install the specified topology return terminal devices
    NetDeviceContainer install();
};







