#include "myCQ_client.hpp"

#include <chrono>

void CustomClient::PingServer() {
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

void CustomClient::MessageAll() {
  custom_netlib::message<CustomMsgTypes> msg;
  msg.header.id = CustomMsgTypes::MessageAll;
  Send(msg);
}