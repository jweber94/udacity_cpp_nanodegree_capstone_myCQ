#include "custom_net_lib.hpp"
#include "myCQ_client.hpp"

#include <boost/program_options.hpp>
#include <chrono>
#include <future>
#include <iostream>
#include <set>
#include <stdio.h>
#include <thread>
#include "payload_definitions.hpp"


// Util functions - keyboard input
std::set<char> possable_inputs{'p', 'm', 'l',
                               'n', 'q', ''}; // The last element is ctrl+c

void get_keyboard_input(std::promise<char> *promObj) {
  char input = 'a';
  while (true) //(input != 'q') && (input != '^C'))
  {
    system("stty raw");
    input = getchar();
    system("stty cooked");

    if (possable_inputs.find(input) != possable_inputs.end()) {
      if (input == '') {
        promObj->set_value(input);
        return;
      } else {
        // all other valid inputs
        //std::cout << " was your input.\n";
        promObj->set_value(input);
        return;
      }
    } else {
      /*
      std::cout << " was your input which is not a valid. Please try again "
                   "with\n p \t "
                   "ping\nm \t message all clients with a message that you "
                   "will insert\nl \t list all connected clients by their "
                   "ID\nn \t notify one of the clients by ID\nto interact "
                   "with the server or quit the program by pressing q.\n\n";*/
    }
  }
}

template <typename R> bool is_ready(std::future<R> const &f) {
  return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

int main(int argc, const char *argv[]) {

  // command line parser
  uint32_t port_num = 0;
  std::string ip_addr = "127.0.0.1";
  boost::program_options::options_description desc{
      "Options for the chatbot client."};
  boost::program_options::variables_map vm;

  desc.add_options()("help,h",
                     "Help and overview of all possible command line options")(
      "port,p", boost::program_options::value<uint32_t>()->default_value(60000),
      "Port where the client should try to connect to the server.")(
      "addr,a",
      boost::program_options::value<std::string>()->default_value("127.0.0.1"),
      "IP-address where the client should try to connect to find the server.");

  boost::program_options::store(
      boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);

  ip_addr = vm["addr"].as<std::string>();
  port_num = vm["port"].as<uint32_t>();

  if (vm.count("help")) {
    std::cout << desc << "\n";
    exit(0);
  }

  MyCQClient client;
  client.Connect(ip_addr, port_num);

  char input_character =
      'y'; // default value to not get randomly blocked since we are not under
           // contol whats in the memory before we allocate it
  std::promise<char> prmsChar;
  std::thread input_listening_thr;

  input_listening_thr = std::thread(get_keyboard_input, &prmsChar);
  std::future<char> ftrChar = prmsChar.get_future(); // start future

  while (true) {
    if (is_ready(ftrChar)) { // Check if the user wants the program something to
                             // send to the server
      // there is a new character given
      input_character = ftrChar.get(); // blocks the main thread until the user
                                       // gives the program some input

      if (input_character == 'q') {
        std::cout << "quit commend accepted!\n";
        input_listening_thr.join();
        return 0;
      }

      switch (input_character) {
      case 'l':
        client.RequestConnectedClientsList(); 
        break;
      case 'n':
        client.NotifyOne(); 
        std::cout << "-- Sending a private message --\n";
        break;
      case 'p':
        client.PingServer();
        break;
      case 'm':
        std::cout << "-- Message all connected clients --\n";
        client.MessageAll();
        break;
      case '':
        input_listening_thr.join();
        return 0;
        break;
      default:
        std::cout << input_character
                  << " is not a valid input. Please try again with\n p \t "
                     "ping\nm \t message all clients with a message that you "
                     "will insert\nl \t list all connected clients by their "
                     "ID\nn \t notify one of the clients by ID\nto interact "
                     "with the server or quit the program by pressing q.\n";
      }

      input_listening_thr.join();

      // setting up a new listening thread for the user input
      prmsChar = std::promise<char>();
      ftrChar = prmsChar.get_future();
      input_listening_thr = std::thread(get_keyboard_input, &prmsChar);
    } else {
      // if we do not want to request something from the server, we want to
      // check if he sends us some messages (maybe back after we requested
      // something before)
      if (client.isConnected()) {
        if (!client.Incoming().isEmpty()) {
          // The incoming message queue is not empty, so we want to read some
          // messages

          auto msg_from_server = client.Incoming().popFront().msg;
          switch (msg_from_server.header.id) {

          case MyCQMessages::ServerPing: {
            std::cout << "-- Server ping --\n";
            std::chrono::system_clock::time_point response_time =
                std::chrono::system_clock::now();
            std::chrono::system_clock::time_point request_time;
            msg_from_server >> request_time;
            std::cout << "Round trip time: "
                      << std::chrono::duration<double>(response_time -
                                                       request_time)
                             .count()
                      << "\n" << std::endl; 
            break;
          }

          case MyCQMessages::MessageAll: {
            uint32_t clientID;
            MessageAllDescription input_msg_all; 

            msg_from_server >> input_msg_all;
            std::cout << "-- Message to all from [" << input_msg_all.sender_id << "] --\n";

            // Extract and display message
            std::string printout_str(input_msg_all.message); 
            std::cout << printout_str << "\n" << std::endl; 

            break;
          }

          case MyCQMessages::ListAllClients: {
            std::cout << "-- List of all clients --" << std::endl;
            
            // receive payload
            ClientsListDescription received_payload; 
            msg_from_server >> received_payload; 

            std::cout << "I am: " << received_payload.own_id  << " and the others are: "<< std::endl;  
            for (int i = 0; i < received_payload.num_active_ids; i++){
              if (received_payload.client_ids[i] == received_payload.own_id){
                continue; 
              }
              std::cout << received_payload.client_ids[i] << "\n" << std::endl; 
            }

            break;
          }

          case MyCQMessages::NotifyOneClient: {
            // Extract data before displaying it, since we want to display the ID of the sending client

            NotifyOneDescription received_msg_from_client;
            msg_from_server >> received_msg_from_client; 
          
            std::cout << "-- Received private message from : " << received_msg_from_client.sender_id << "--\n"; 
            std::cout << received_msg_from_client.message << "\n" << std::endl; 
            break; 
          }

          default: {
            break;
          }
          }

        } else {
          std::this_thread::sleep_for(
              std::chrono::milliseconds(200)); // to not burn down the processor
        }
      }
    }
  }

  return 0;
}