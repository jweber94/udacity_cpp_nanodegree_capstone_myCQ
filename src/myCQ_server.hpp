#ifndef MYCQSERVER
#define MYCQSERVER

#include "custom_net_lib.hpp"
#include <memory>

enum class CustomMsgTypes : uint32_t {
  MessageAll,
  ListAllClients,
  ServerPing,
  NotifyOneClient,
  ServerAccept
};

class CustomServerLogic
    : public custom_netlib::ServerInterfaceClass<CustomMsgTypes> {
public:
  CustomServerLogic(uint16_t n_port)
      : custom_netlib::ServerInterfaceClass<CustomMsgTypes>(n_port){};

protected:
  // Implementations for the virtual methods of the server interface base class
  virtual bool onClientConnect(
      std::shared_ptr<custom_netlib::ConnectionInterface<CustomMsgTypes>>
          client_ptr);

  virtual void onClientDisconnect(
      std::shared_ptr<custom_netlib::ConnectionInterface<CustomMsgTypes>>
          client_ptr);

  virtual void
  onMessage(std::shared_ptr<custom_netlib::ConnectionInterface<CustomMsgTypes>>
                client,
            custom_netlib::message<CustomMsgTypes> &msg_input);
};

#endif /* MYCQSERVER */