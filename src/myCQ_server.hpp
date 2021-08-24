#ifndef MYCQSERVER
#define MYCQSERVER

#include "custom_net_lib.hpp"
#include <memory>

enum class MyCQMessages : uint32_t {
  MessageAll,
  ListAllClients,
  ServerPing,
  NotifyOneClient,
  ServerAccept
};

class MyCQServer
    : public custom_netlib::ServerInterfaceClass<MyCQMessages> {
public:
  MyCQServer(uint16_t n_port)
      : custom_netlib::ServerInterfaceClass<MyCQMessages>(n_port){};

  std::vector<u_int32_t> getAllClientIDs(); 

protected:
  // Implementations for the virtual methods of the server interface base class
  virtual bool onClientConnect(
      std::shared_ptr<custom_netlib::ConnectionInterface<MyCQMessages>>
          client_ptr);

  virtual void onClientDisconnect(
      std::shared_ptr<custom_netlib::ConnectionInterface<MyCQMessages>>
          client_ptr);

  virtual void
  onMessage(std::shared_ptr<custom_netlib::ConnectionInterface<MyCQMessages>>
                client,
            custom_netlib::message<MyCQMessages> &msg_input);
};

#endif /* MYCQSERVER */