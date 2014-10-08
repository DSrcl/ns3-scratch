## My NS-3 experiments

# what's in there?

* LanHelper -- easily building LAN network with hosts connected by switches in arbitrary topology

## how to use LanHelper?
1. Create a helper with specified **SimpleNetDeviceHelper**. 
LanHelper currently uses **SimpleNetDevice**; Other NS-3 devices will be supported in the future.  
```
SimpleNetDeviceHelper deviceHelper;
// ... configuration of device helper
LanHelper lan(deviceHelper);
```

2. Specify your topology with `LanHelper::addConnection` function.  
While first argument is a smart pointer to the switch node, the second argument is a **NodeContainer** for arbitrary nodes that you want to connect to the switch (you can connect a switch to other switches by including switch node in the second argument).  
```
Ptr<Node> node;
NodeContainer hosts;
lan.addConnection(switchNode, hosts);
lan.addConnection(...other links...);
```

3. Install the specified topology with `LanHelper::install` function.
The install function returns a **NetDeviceContainer** containing net devices of the terminal nodes, which you can later used to build your network interfaces (e.g. using **Ipv4AddressHelper** to install **Ipv4Interfaces**). Node that this function already uses **InternetStackHelper** to enable a few of NS-3 builtin protocols.
```
NetDeviceContainer hostsDevices = lan.install();
```

## Example  
* An example of using LanHelper to build a basic UdpEcho application is [here](www.google.com).