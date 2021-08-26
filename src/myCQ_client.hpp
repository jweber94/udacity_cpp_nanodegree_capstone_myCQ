#ifndef MYCQCLIENT
#define MYCQCLIENT

#include "custom_net_lib.hpp"
#include <iostream>
#include <vector>
#include "payload_definitions.hpp"
#include <mutex>

enum class MyCQMessages : uint32_t {
  MessageAll,
  ListAllClients,
  ServerPing,
  NotifyOneClient,
  ServerAccept
};

class MyCQClient : public custom_netlib::ClientBaseInterface<MyCQMessages> {
public:
  void PingServer();
  void MessageAll();
  void RequestConnectedClientsList(); 
  void printOtherClients();
  void setMyID(uint32_t inID); 
  uint32_t getMyID(); 
  void setOtherClients(std::vector<uint32_t> clients_vec); 

private:
  std::vector<uint32_t> other_clients_; 
  uint32_t my_id_ = 0; // default value
  bool myIDSet_ = false; 
  std::mutex input_mtx_; 
};

#endif /* MYCQCLIENT */