#include "custom_net_lib.hpp"
#include <chrono>
#include <future>
#include <iostream>
#include <set>
#include <stdio.h>
#include <thread>
#include <boost/program_options.hpp>

std::set<char> possable_inputs{'a', 'd', 'p', 'm',
                               's', 'q', ''}; // The last element is ctrl+c

enum class CustomMsgTypes : uint32_t {
  ServerAccept,
  ServerDeny,
  ServerPing,
  MessageAll,
  ServerMessage
};

class CustomClient : public custom_netlib::ClientBaseInterface<CustomMsgTypes> {
public:
  void PingServer() {
    custom_netlib::message<CustomMsgTypes> msg;
    msg.header.id = CustomMsgTypes::ServerPing;

    std::chrono::system_clock::time_point now_pc_clock =
        std::chrono::system_clock::
            now(); // CAUTION: Only works properly if the server host and the
                   // client host has the same system clock sent this to the
                   // server and he will it back to you so we can calculate the
                   // round trip time in the upcoming lines of code
    msg << now_pc_clock;

    Send(msg);
  }

  void MessageAll() {
    custom_netlib::message<CustomMsgTypes> msg;
    msg.header.id = CustomMsgTypes::MessageAll;
    Send(msg);
  }

private:
};

// Util functions
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
        std::cout << " was your input\n";
        promObj->set_value(input);
        return;
      }

    } else {
      std::cout << " was your input - no action triggered.\n";
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

  desc.add_options()(
        "help,h", "Help and overview of all possible command line options")(
        "port,p",
        boost::program_options::value<uint32_t>()->default_value(60000),
        "Port where the client should try to connect to the server.")
        ("addr,a", boost::program_options::value<std::string>()->default_value("127.0.0.1"), "IP-address where the client should try to connect to find the server.");

    boost::program_options::store(
        boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);

    ip_addr = vm["addr"].as<std::string>();
    port_num = vm["port"].as<uint32_t>(); 

    if (vm.count("help")) {
      std::cout << desc << "\n";
      exit(0);
    }

  CustomClient client;
  //client.Connect("127.0.0.1", 60000);
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
      case 'a':
        std::cout << "server accept\n";
        break;
      case 'd':
        std::cout << "server deny\n";
        break;
      case 'p':
        std::cout << "server ping\n";
        client.PingServer();
        break;
      case 'm':
        std::cout << "message all\n";
        client.MessageAll();
        break;
      case 's':
        std::cout << "server message\n";
        break;
      case '':
        input_listening_thr.join();
        return 0;
        break;
      default:
        std::cout << input_character
                  << " is not a valid input. Please try again with a, d, p, m "
                     "or s to interact with the server or quit the program by "
                     "pressing q.\n";
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

          case CustomMsgTypes::ServerPing: {
            std::chrono::system_clock::time_point response_time =
                std::chrono::system_clock::now();
            std::chrono::system_clock::time_point request_time;
            msg_from_server >> request_time;
            std::cout << "Ping: "
                      << std::chrono::duration<double>(response_time -
                                                       request_time)
                             .count()
                      << "\n";
            break;
          }

          case CustomMsgTypes::MessageAll: {
            uint32_t clientID;
            msg_from_server >> clientID;
            std::cout << "Hello from [" << clientID << "]\n";
            break;
          }

          case CustomMsgTypes::ServerAccept: {
            std::cout << "Server accepted connection.\n";
            break;
          }

          default: {
            std::cout << "Default case!\n";
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