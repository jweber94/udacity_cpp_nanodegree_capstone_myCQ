# CPPND: Capstone myCQ

This is the Capstone Project of the udacity C++ nanodegree that I have done from june 2021 until september 2021. I decided to to my own, custom project and just rely on the given starter repository that can be found in [0]. I used this project in order to finish the nanodegree by doing this project as my capstone programming assignment by showing off my skills that I have learned during the programm and learn about networking in C++.
Therefore, I developed a client-server chat application, called myCQ. The aim of the project is to use a self-developed networking framework that is based on the video lecture series that can be found in [1], in order to implement a server application that is able to receive connections and requests from multiple client applications. The implemented client applications can ping the server for a check of the connection to the server, they can message all clients that are currently connected to the server, they can request a list of all connected clients from the server and can send a message to a specific client. The user of the chat-client application can interact with application purely over the terminal in which the client-application is running. Therefore, a plain C++ asynchronous keyboard interface is implemented. The server application is used to organize the connections and exchange the messages between the connected clients. 

Other ressources, that I used to familiarize myself with networking in C++ and from which I gained inspiration for the implementation can be found in [1], [2] and [3]. Even if the header based networking library is mainly based on [1], I programmed and refactored it completly on my own, in order to make it suitable for the myCQ application.     

[0]: https://github.com/udacity/CppND-Capstone-Hello-World
[1]: https://www.youtube.com/watch?v=2hNdkYInj4g, https://www.youtube.com/watch?v=UbjxGvrDrbw and https://www.youtube.com/watch?v=hHowZ3bWsio 
[2]: https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa
[3]: https://theboostcpplibraries.com/boost.asio


## Dependencies for Running Locally
* cmake >= 3.11.4
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* Boost >= 1.72.0 (This is how to install it properly: https://stackoverflow.com/questions/12578499/how-to-install-boost-on-ubuntu) 
* GTest >= 1.11.0

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: 
  - Server `./myCQ_server`
  - Client `./myCQ_client`
  - ***CAUTION:*** You need to start the `myCQ_server` application _BEFORE_ the `myCQ_client` application(s)!
  - You can change the default port (60000) to a user defined port where the server should listen for new connections and where the client should try to connect to the server (as well as the IP-address of the machine where the server is running in case of the client application) by using the ```--port <YourPortNumber>``` flag. All settings that you can hand over to the applications can be seen if you run ```./myCQ_client --help```. The implementation of the terminal parser is done with ```boost::program_options``` and can be seen within the ```/src/main_client.cpp``` and ```/src/main_server.cpp```.
5. After starting the server application as well as some (two or more) clients, you can use one of the client applications to ping the server by pressing ***p*** within the terminal, message all connected clients by pressing ***m*** and then entering the message that you want to send or pressing ***l*** to get a list of all connected clients. If you want to write a message to a specific client, you can press ***n*** in order to notify a specific client 

## File and Class Structure

## Expected behaviour of the application
This screencast shows the expected behaviour

## Code - Udacity Rubric referencing 
In order to complete my capstone project, I have to reference at least five of the not required rubric points that can be found in https://review.udacity.com/#!/rubrics/2533/view. In the following table, the mapping between the rubric point and my code is shown. 

| Rubric point  | File          | Line  |
| ------------- |:-------------:| -----:|
| Fixme         | Fixme         | Fixme |


## Main takeaways
+ I learned a lot about asynchronous and concurrent programming. This is a key element whenever we want to interact with the network, since we are not under control of the program flow, after we send out a request or a message to the server/client
+ One of the most interesting things that I have learned, were the handshake process between the client and the server to ensure, that no invalid/mallicious communication can take place. The implementation can be found within the file ```net_connection_interface.hpp``` 
+ I learned, that templates are mainly places in header files, since the explicit code is written at compile time by the compiler and also learned how to abstract functionallities to a common form. 
+ I learned the key concepts of asynchronous programming with the ```boost::asio``` framework. 
+ Also I familliarized myself with GTest and how to include unit tests with GTest in a CMake project, by creating a unit test for the net_message.hpp implementation to make sure, that the serialization and deserialization is done properly.
+ Header of a networking message can be a very bad security issue, especially if they contain a size information from which a memory allocation is derived within the server/client process. Therefore, a handshake between the client and the server before any payload data is exchanged is a useful feature that can make sure, that no unintented mallicious communication takes place. 
+ I learned how to use condition variables to block a thread that is waiting for an asynchronous event to happen (i.e. waiting for an incoming message), so that the processor is not occupied the complete waiting time.