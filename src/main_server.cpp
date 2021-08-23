#include "custom_net_lib.hpp"
#include "myCQ_server.hpp"

#include <boost/program_options.hpp>
#include <iostream>

int main(int argc, const char *argv[]) {

  // command line parser
  uint32_t port_num = 0;
  boost::program_options::options_description desc{
      "Options for the chatbot server."};
  boost::program_options::variables_map vm;

  desc.add_options()("help,h",
                     "Help and overview of all possible command line options")(
      "port,p", boost::program_options::value<uint32_t>()->default_value(60000),
      "Port where the server should listen for new connections from clients.");

  boost::program_options::store(
      boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);

  port_num = vm["port"].as<uint32_t>();

  if (vm.count("help")) {
    std::cout << desc << "\n";
    exit(0);
  }

  // CustomServerLogic server_test(60000);
  CustomServerLogic server_test(port_num);
  server_test.Start();

  custom_netlib::message<CustomMsgTypes> msg_test;

  std::cout << "Server was successfully created and already started!\n";

  while (true) {
    server_test.update(-1, true);
  }

  return 0;
}