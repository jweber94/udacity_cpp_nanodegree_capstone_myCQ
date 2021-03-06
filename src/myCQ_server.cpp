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
  std::cout << "[SERVER]: Removing client [" << client_ptr->getID() << "]\n";
}

void MyCQServer::onMessage(
    std::shared_ptr<custom_netlib::ConnectionInterface<MyCQMessages>> client,
    custom_netlib::message<MyCQMessages> &msg_input) {
  switch (msg_input.header.id) {

  case MyCQMessages::ServerPing: {
    //std::cout << "[" << client->getID() << "]: Server ping\n";
    client->SendData(msg_input);
    break;
  }

  case MyCQMessages::MessageAll: {
    //std::cout << "[" << client->getID() << "]: Message all\n";
    custom_netlib::message<MyCQMessages> msg;
    msg.header.id = MyCQMessages::MessageAll;
    // msg << client->getID();

    // get the message from the client:
    MessageAllDescription payload_from_client;
    msg_input >> payload_from_client;

    // Update message from client
    payload_from_client.sender_id = client->getID();

    // insert the updated payload and send it to all other clients
    msg << payload_from_client;

    sendMessageToAllClients(msg, client); // just return the header
    break;
  }

  case MyCQMessages::ListAllClients: {
    //std::cout << "list all clients to \n";

    // create payload
    ClientsListDescription payload_msg;
    payload_msg.num_active_ids = connectionsQueue_.size();
    payload_msg.own_id = client->getID();
    size_t i = 0;
    for (auto it : connectionsQueue_) {
      payload_msg.client_ids[i] = connectionsQueue_[i]->getID();
      i++;
    }

    // create message
    custom_netlib::message<MyCQMessages> msgLC;
    msgLC.header.id = MyCQMessages::ListAllClients;
    msgLC << payload_msg;

    // send message to the requesting client
    client->SendData(msgLC);

    break;
  }

  case MyCQMessages::NotifyOneClient: {
    //std::cout << "notify one client\n"; // FIXME
    custom_netlib::message<MyCQMessages> msg;

    msg.header.id = MyCQMessages::NotifyOneClient;

    // create paylaod
    NotifyOneDescription received_msg_from_client;
    msg_input >> received_msg_from_client;
    received_msg_from_client.sender_id = client->getID();

    msg << received_msg_from_client;

    for (auto it : connectionsQueue_) {
      if (it->getID() == received_msg_from_client.receiver_id) {
        //std::cout << "Sending to " << it->getID() << std::endl;
        it->SendData(msg);
      }
    }
    break;
  }

  default: { break; }
  }
}
