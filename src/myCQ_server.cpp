#include "myCQ_server.hpp"

bool MyCQServer::onClientConnect(
    std::shared_ptr<custom_netlib::ConnectionInterface<MyCQMessages>>
        client_ptr) {
  custom_netlib::message<MyCQMessages> msg;
  msg.header.id = MyCQMessages::ServerAccept;
  client_ptr->SendData(msg);
  return true; // accept all requested client connections
}

void MyCQServer::onClientDisconnect(
    std::shared_ptr<custom_netlib::ConnectionInterface<MyCQMessages>>
        client_ptr) {
  std::cout << "Removing client [" << client_ptr->getID() << "]\n";
}

void MyCQServer::onMessage(
    std::shared_ptr<custom_netlib::ConnectionInterface<MyCQMessages>> client,
    custom_netlib::message<MyCQMessages> &msg_input) {
  switch (msg_input.header.id) {

  case MyCQMessages::ServerPing: {
    std::cout << "[" << client->getID() << "]: Server ping\n";
    client->SendData(msg_input);
    break;
  }

  case MyCQMessages::MessageAll: {
    std::cout << "[" << client->getID() << "]: Message all\n";
    custom_netlib::message<MyCQMessages> msg;
    msg.header.id = MyCQMessages::MessageAll;
    msg << client->getID();
    sendMessageToAllClients(msg, client); // just return the header
    break;
  }

  case MyCQMessages::ListAllClients: {
    std::cout << "list all clients to \n";      

    // create payload
    ClientsListDescription payload_msg; 
    payload_msg.num_active_ids = connectionsQueue_.size(); 
    payload_msg.own_id = client->getID(); 
    size_t i = 0; 
    for (auto it : connectionsQueue_){
      payload_msg.client_ids[i] = connectionsQueue_[i]->getID(); 
      i++; 
    }

    // create message
    custom_netlib::message<MyCQMessages> msgLC;
    msgLC.header.id = MyCQMessages::ListAllClients;

    msgLC << payload_msg; 

    // send message to the requesting client
    //sendMessageToClient(client, msgLC);
    client->SendData(msgLC);  

    break; 
  }

  case MyCQMessages::NotifyOneClient:{
    std::cout << "notify one client\n"; // FIXME
    break; 
  }

  default: { break; }
  }
}

