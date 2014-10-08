#include <set>
#include <map>
#include <algorithm>
#include "ns3/log.h"
#include "ns3/network-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/bridge-module.h"
#include "lan-helper.h"

using namespace ns3;





LanHelper::LanHelper(SimpleNetDeviceHelper deviceHelper)
{
    m_deviceHelper = deviceHelper;
}



LanHelper::~LanHelper(){}



void LanHelper::addConnection(const Ptr<Node> switchNode, const NodeContainer& nodes)
{
    uint32_t switchId = switchNode->GetId();
    std::set<uint32_t> nodeIds;

    if (m_connections.count(switchId) > 0) {
        nodeIds = m_connections[switchId];
    }

    for (int i = 0; i < nodes.GetN(); i++) {
        nodeIds.insert(nodes.Get(i)->GetId());
    }
    
    m_connections[switchId] = nodeIds;
}


std::set<uint32_t> LanHelper::getTerminalIds()
{
    std::set<uint32_t> terminalIds;
    std::set<uint32_t> connectedIds;
    std::set<uint32_t> switchIds;
    
    for (std::map<uint32_t, std::set<uint32_t> >::iterator connIt = m_connections.begin();
            connIt != m_connections.end();
            connIt++) {
        switchIds.insert(connIt->first);
        connectedIds.insert(connIt->second.begin(), 
                            connIt->second.end());
    }

    // remove switches from possible terminals and get a set of terminal ids
    std::set_difference(connectedIds.begin(), connectedIds.end(),
                        switchIds.begin(), switchIds.end(),
                        std::inserter(terminalIds, terminalIds.end()));

    return terminalIds;
}




void LanHelper::buildTopology()
{
    std::set<uint32_t> terminalIds = getTerminalIds();

    for (std::map<uint32_t, std::set<uint32_t> >::iterator connIt = m_connections.begin();
            connIt != m_connections.end();
            connIt++) {
        uint32_t switchId = connIt->first;
        Ptr<Node> switchNode = NodeList::GetNode(switchId);
        std::set<uint32_t> connectedIds = connIt->second;
       
        // iterate over the nodes that are supposed to link with the switch 
        // and establish connections
        for (std::set<uint32_t>::iterator nodeIdIt = connectedIds.begin();
                nodeIdIt != connectedIds.end();
                nodeIdIt++) {
            Ptr<Node> node = NodeList::GetNode(*nodeIdIt);
            NetDeviceContainer link = m_deviceHelper.Install(NodeContainer(switchNode, node));
            // put switch device into container
            putSwitchDevice(switchId, link.Get(0));
            if (terminalIds.find(*nodeIdIt) != terminalIds.end()) {
                // connected node is a terminal node
                m_terminalDevices.Add(link.Get(1));
            } else {
                // connected node is also a switch
                putSwitchDevice(*nodeIdIt, link.Get(1));
            }
        }
    }

    m_terminalNodes = getNodesFromIds(terminalIds);
}


void LanHelper::installSwitches()
{
    BridgeHelper bridgeHelper;

    for (std::map<uint32_t, NetDeviceContainer>::iterator switchIt = m_switchDevices.begin();
            switchIt != m_switchDevices.end();
            switchIt++) {
        Ptr<Node> switchNode = NodeList::GetNode(switchIt->first);
        bridgeHelper.Install(switchNode, switchIt->second);
    }
}



void LanHelper::installInternetStack()
{
    InternetStackHelper internetStack;
    internetStack.Install(m_terminalNodes);
}


NetDeviceContainer LanHelper::install()
{
    buildTopology();
    installSwitches();
    installInternetStack();
    return m_terminalDevices;
}



void LanHelper::putSwitchDevice(uint32_t switchId, Ptr<NetDevice> switchDevice)
{
    NetDeviceContainer cSwitchDevices;
    if (m_switchDevices.count(switchId) > 0) {
        cSwitchDevices = m_switchDevices[switchId];
    }
    cSwitchDevices.Add(switchDevice);
    m_switchDevices[switchId] = cSwitchDevices;  
}



NodeContainer LanHelper::getNodesFromIds(std::set<uint32_t> nodeIds)
{
    NodeContainer nodes;
    for (std::set<uint32_t>::iterator nodeIdIt = nodeIds.begin();
            nodeIdIt != nodeIds.end();
            nodeIdIt++) {
        nodes.Add(NodeList::GetNode(*nodeIdIt));
    }

    return nodes;
}

uint32_t LanHelper::findTerminal(Ptr<Node> node)
{
    for (int i = 0; i < m_terminalNodes.GetN(); i++) {
        if (m_terminalNodes.Get(i)->GetId() == node->GetId()) {
            return i;
        }
    }

    return -1;
}













