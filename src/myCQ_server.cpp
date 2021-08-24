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
    

    // test client --> server
    //uint32_t test; 
    //uint32_t test_1;

    //msg_input >> test >> test_1; 
    //std::cout << "test: " << test << std::endl; 
    //std::cout << "test1: " << test_1 << std::endl;

    // create message
    custom_netlib::message<MyCQMessages> msgLC;
    msgLC.header.id = MyCQMessages::ListAllClients;
    //std::vector<uint32_t> ids_vec(getAllClientIDs()); 
    //uint32_t num_of_ids = connectionsQueue_.size();

    /*
    for (auto it : ids_vec){
      msgLC << it; // FIXME: Check if the iterator gets dereferenced automatically  
      std::cout << it << "\n"; 
    }
    msgLC << client->getID(); // The second last uint32_t element of the payload is the ID of the requesting client so that he can identify which ID is he
    msgLC << num_of_ids; // The last uint32_t element of the payload is the number of IDs that I can read out of the message (last element since we read out the last element first on the client side)
    */
    // DEBUG
    //std::cout << client->getID() << std::endl; 
    //std::cout << msgLC << std::endl; 
    
    uint32_t test0 = 66;
    uint32_t test1 = 42;
    uint32_t test2 = 1; 
      
    msgLC << test0 << test1 << test2; 

    /*
    uint32_t test00;
    uint32_t test11;
    uint32_t test22; 
    msgLC >> test00 >> test11 >> test22; 
    
    std::cout << test00 << ", " << test11 << ", " << test22 << "\n";
    */

    /*
    // DEBUG
    std::cout << "TEST" << std::endl; 
    uint32_t test_num; 
    uint32_t test_id; 

    msgLC >> test_num; 
    msgLC >> test_id; 

    std::cout << "test_num: " << test_num << "\ntest_id: " << test_id << std::endl; 

    std::vector<uint32_t> test_vec; 
    uint32_t tmp_element;
    for (int i = 0; i < test_num; i++){
      if (test_id == client->getID()){
        continue; 
      }
      msgLC >> tmp_element; 
      std::cout << msgLC << std::endl; 
    }
    std::cout << "END TEST" << std::endl; 
    */

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

std::vector<uint32_t> MyCQServer::getAllClientIDs(){
  std::vector<uint32_t> resultVec; 
  for (auto it : connectionsQueue_){
    resultVec.push_back(it->getID()); 
  }
  //return std::move(resultVec); 
  return resultVec; 
}

