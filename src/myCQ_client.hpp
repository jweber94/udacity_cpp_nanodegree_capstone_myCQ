#ifndef MYCQCLIENT
#define MYCQCLIENT

#include "custom_net_lib.hpp"
#include <iostream>

enum class CustomMsgTypes : uint32_t {
  MessageAll,
  ListAllClients,
  ServerPing,
  NotifyOneClient,
  ServerAccept
};

class CustomClient : public custom_netlib::ClientBaseInterface<CustomMsgTypes> {
public:
  void PingServer();
  void MessageAll();

private:
};

#endif /* MYCQCLIENT */