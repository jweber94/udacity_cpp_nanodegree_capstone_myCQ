#include "myCQ_server.hpp"

bool CustomServerLogic::onClientConnect(
    std::shared_ptr<custom_netlib::ConnectionInterface<CustomMsgTypes>>
        client_ptr) {
  custom_netlib::message<CustomMsgTypes> msg;
  msg.header.id = CustomMsgTypes::ServerAccept;
  client_ptr->SendData(msg);
  return true; // accept all requested client connections
}

void CustomServerLogic::onClientDisconnect(
    std::shared_ptr<custom_netlib::ConnectionInterface<CustomMsgTypes>>
        client_ptr) {
  std::cout << "Removing client [" << client_ptr->getID() << "]\n";
}

void CustomServerLogic::onMessage(
    std::shared_ptr<custom_netlib::ConnectionInterface<CustomMsgTypes>> client,
    custom_netlib::message<CustomMsgTypes> &msg_input) {
  switch (msg_input.header.id) {

  case CustomMsgTypes::ServerPing: {
    std::cout << "[" << client->getID() << "]: Server ping\n";
    client->SendData(msg_input);
    break;
  }

  case CustomMsgTypes::MessageAll: {
    std::cout << "[" << client->getID() << "]: Message all\n";
    custom_netlib::message<CustomMsgTypes> msg;
    msg.header.id = CustomMsgTypes::MessageAll;
    msg << client->getID();
    sendMessageToAllClients(msg, client); // just return the header
    break;
  }

  case CustomMsgTypes::ListAllClients: {
    std::cout << "list all clients to \n"; // FIXME
    break; 
  }

  case CustomMsgTypes::NotifyOneClient:{
    std::cout << "notify one client\n"; // FIXME
    break; 
  }

  default: { break; }
  }
}