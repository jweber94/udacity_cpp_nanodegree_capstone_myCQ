#include "myCQ_client.hpp"

#include <chrono>

void MyCQClient::PingServer() {
  custom_netlib::message<MyCQMessages> msg;
  msg.header.id = MyCQMessages::ServerPing;

  std::chrono::system_clock::time_point now_pc_clock =
      std::chrono::system_clock::
          now(); // CAUTION: Only works properly if the server host and the
                 // client host has the same system clock sent this to the
                 // server and he will it back to you so we can calculate the
                 // round trip time in the upcoming lines of code
  msg << now_pc_clock;

  Send(msg);
}

void MyCQClient::MessageAll() {
  custom_netlib::message<MyCQMessages> msg;
  msg.header.id = MyCQMessages::MessageAll;
  Send(msg);
}

void MyCQClient::RequestConnectedClientsList(){
  custom_netlib::message<MyCQMessages> msg;
  msg.header.id = MyCQMessages::ListAllClients; 
  Send(msg); 
}

void MyCQClient::printOtherClients(){
  std::cout << "These are the currently connected clients:\n"; 
  for (auto it : other_clients_){
    std::cout << it << "\n"; 
  }
  std::cout << "\n\n"; 
}

uint32_t MyCQClient::getMyID(){
  if (!myIDSet_){
    std::cerr << "ERROR: Asking for the client ID before it was requested from the server! Setting the ID to Zero.\n";
    return 0;
    
  }
  return my_id_; 
} 

void MyCQClient::setMyID(uint32_t inID){
  my_id_ = inID; 
  myIDSet_ = true; 
}

void MyCQClient::setOtherClients(std::vector<uint32_t> clients_vec){
  other_clients_ = clients_vec;
}