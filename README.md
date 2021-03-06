# CPPND: Capstone myCQ

This is the Capstone Project of the udacity C++ nanodegree that I have done from june 2021 until september 2021. I decided to to my own, custom project and just rely on the given starter repository that can be found in [0]. I used this project in order to finish the nanodegree by doing this project as my capstone programming assignment by showing off my skills that I have learned during the programm and learn about networking in C++.
Therefore, I developed a client-server chat application, called myCQ. The aim of the project is to use a networking framework that is based on the video lecture series that can be found in [1], in order to implement a server application that is able to receive connections and requests from multiple client applications. The implemented client applications can ping the server for a check of the connection to the server, they can message all clients that are currently connected to the server, they can request a list of all connected clients from the server and can send a message to a specific client. The user of the chat-client application can interact with application purely over the terminal in which the client-application is running. Therefore, a plain C++ asynchronous keyboard interface is implemented. The server application is used to organize the connections and exchange the messages between the connected clients.

Ressources that I used to familiarize myself with networking in C++ and from which I gained inspiration for the implementation can be found in [1], [2] and [3]. Even if the header based networking library is mainly based on [1], I reimplemented it on my own in order to learn about networking and asynchronous programming with C++ and about networking in general.     

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
5. After starting the server application as well as some (two or more) clients, you can use one of the client applications to ping the server by pressing ***p*** within the terminal, message all connected clients by pressing ***m*** and then entering the message that you want to send or pressing ***l*** to get a list of all connected clients. If you want to write a message to a specific client, you can press ***n*** in order to notify a specific client. If you want to quit the program, you can eiter press ***q*** or you can use ```ctrl+c```.

## File and Class Structure
The project is structured into two main parts. One of them is the networking library that is written completly header based and can be found within the folder ```/include/custom_net_lib/``` and the other part is the myCQ chat application within the ```/src/``` folder.

In case of the application, the files are structured into ```*.cpp``` and ```*.hpp``` files with the same name before the dot. You can find the declaration within the ```*.hpp``` files and the definition within the ```*.cpp```. Aside from the corresponding files, you can find the ```main_client.cpp``` and ```main_server.cpp``` files, which contain the executables/entry points of the client and the server executables. Also, a file called ```payload_definitions.hpp``` can be found within the ```/src/``` folder. Within this file, the payload datastructures are defined that are used to exchange data between the client applications and the server.

In case of the networking library within the ```/include/custom_net_lib/``` folder, everything is implemented as a header file, since I am working with templates in order to create an interface for custom message types and payloads on the application level where the library is included and used. In the following table, you can see which ```.hpp``` file are used to implement which functionallity and which classes.

| Filename   | Used for          | Classes  |
| ------------- |:-------------:| -----:|
| common_net_includes.hpp         | Defines common includes, that are used in all networking applications, like the ```#include <boost/asio.hpp>``` header  | No classed defined in here |
| custom_net_lib.hpp | Contains all files of the created library to let the user include the complete library with just one include | No classed defined here |
| net_client.hpp | Defines a base class that can (but does not need to be) extented by inheritance for an individual client application. The class contains all functioning methods that are needed in order to communicate with the server. | ClientBaseInterface |
| net_connection_interface.hpp | Defines a connection interface class that can be used by the client as well as the server. The different use cases are distinguished from each other by the private member variable ```owner_```. This is mainly needed by the handshake process, that secures a valid connection and is different between the client and the server side. | ConnectionInterface |
| net_message.hpp | Defines the message types, used within the created networking library. The message itself contains a payload and a message header and is used to exchange data between the client and the server. The OwnedMessage class is only used by the server, since he needs to know from which of the (multiple) clients was a received message, since they are all stored in the same input message queue. | message_header, message, OwnedMessage |
| net_server.hpp | Defines an abstract base class for the server logic. Besides from the networking methods that are needed and implemented within the header file, some virtual methods are declared for the derived server logic call. This is especially what the server should do if he receives a message  (```virtual void onMessage(std::shared_ptr<ConnectionInterface<T>> client, message<T> &msg_input)```) and if a client connects to him (```virtual bool onClientConnect(std::shared_ptr<ConnectionInterface<T>> client)```). | ServerInterfaceClass |
| net_ts_queue.hpp | Defines a thread safe queue, where the messages (or owned messages on the server side) can be stored in. The queue needs to be thread safe, since inserting and extracting elements from the queue happen in a concurrent/asynchronous manner which can lead to memory management problems, if the queue is not thread safe. (i.e. New messages are inserted to the queue asynchronously from the calculation of the server logic.) | TsNetQueue |

The server base class (```ServerInterfaceClass```) as well as the client base class (```ClientBaseInterface```) contain a ```ConnectionInterface``` as their private/protected member in order to abstract away the networking part of the code from the client/server logic. Also they has a  ```TsNetQueue``` element as their private/protoced member for storing incoming messages.
Most of the templates typenames are used to abstract the custom ```message``` type or the ```OwnedMessage``` type, defined within the ```net_message.hpp```.

By implementing the client, the files ```myCQ_client.cpp``` and ```myCQ_client.hpp``` are used to program the requests to the server by inheriting from the base class ```ClientBaseInterface``` from ```net_client.hpp```. To let the user interact with the myCQ client application, a concurrent user input is implemented within ```main_client.cpp``` by using a promise-future concept. Also, a switch-case statement defines, how the client application should handle incoming messages from the server and is just used, if new messages are received by the derived ```MyCQClient``` instance.

The MyCQ server is implemented in a similar way as the client. Also the ```myCQ_server.cpp``` and the ```myCQ_server.hpp``` are implementing the server logic by inheriting from the server base class ```ServerInterfaceClass``` from the file net_server.hpp of the library. The main focus of the implentation is on the ```virtual void onMessage(std::shared_ptr<custom_netlib::ConnectionInterface<MyCQMessages>> client, custom_netlib::message<MyCQMessages> &msg_input)``` virtual method of the base class, where the behaviour of the server in dependency of incoming messages is defined. Within ```main_server.cpp```, the ```MyCQServer``` instance is called in order to wait for new connections or requests from already connected clients.

In ```main_client.cpp``` as well as ```main_server.cpp```, an interface for handing over the IP-address as well as the port via the terminal call of the individual executables is implemented, using the ```boost::program_options``` library.

## Code - Udacity Rubric referencing
In order to complete my capstone project, I have to reference at least five of the not required rubric points that can be found in https://review.udacity.com/#!/rubrics/2533/view. In the following table, the mapping between the rubric point and my code is shown. Besides from the five described code sections in the table, I used most of the rubrics points within the project, but the listed ones are the most clear ones, in order to complete the capstone project.   

| Rubric point  | File          | Line  | Explaination |
| ------------- |:-------------:|:-------------:| -----:|
| ***Classes follow an appropriate inheritance hierarchy*** & ***Derived class functions override virtual base class functions*** | net_server.hpp, myCQ_server.hpp & myCQ_server.cpp | net_server.hpp-line 215 to 265, myCQ_server.cpp-complete file | The net_server.hpp implementation defines a base class that describes the behaviour that all servers need and set the virtual methods from line 215 to 265 as an interface for the implementation of the derived MyCQServer class. |
| ***The project uses smart pointers instead of raw pointers.*** | net_client.hpp | 119 | The ```ConnectionInterface``` is created as a unique_ptr, since it needs to be exclusivly owned by the client. Other smartpointers are also used within the net_client.hpp implementation for a similar purpose. No raw pointers are used within the project. |
| ***The project uses multithreading.*** | main_client.cpp | 87 to 89 | A separate thread is used to let the user interact with the client application concurrently to the running message processing. |
| ***A promise and future is used in the project.*** | main_client.cpp | 86 - 90 | A promise-future approach is used to make the user input available within the main thread of execution within the myCQ client application. |
| ***A mutex or lock is used in the project.*** | myCQ_client.cpp | 74 - 95 | The ```std::cin``` ressource is lock by a unique_lock in order to make the user input thread safe. |
| ***A condition variable is used in the project.*** | net_ts_queue.hpp | 104 - 105 & 38 to 59 | The condition variable is set in order to block the execution of the thread where the ```wait()``` method of the thread safe queue is called. If we do not block the thread, one complete processor core will be working near 100%, since it always checks for new messages within the queue. This is prevented by the condition variable that gets notifyed whenever a new element is inserted by ```pushBack()``` or ```pushFront()```. |

Other components that are used in many places within the project:
* The project demonstrates an understanding of C++ functions and control structures:
  - e.g. ```main_client.cpp``` with the switch-case and if statements as well as the little utility functions that are defined before the ```int main()```.
* The project accepts user input and processes the input:
  - This is especially done within the ```main_client.cpp``` by starting a thread for the user input as well as using the ```boost::program_options``` library in order to hand over terminal inputs on the program startup.
* The project uses Object Oriented Programming techniques.
  - The most parts of the code is programmed in a object oriented way. Only the utility functions within ```main_client.cpp``` and the entry points for the executables are not object oriented.
* The project makes use of references in function declarations:
  - This is especially done in the lambda functions for the asynchronous ```boost::asio``` handlers.
* Classes use appropriate access specifiers for class members:
  - All class members in all classes are explicitly defined as public, private or protected.
* Templates generalize functions in the project:
  - The complete custom_net_lib library is template based

## Expected behaviour of the application
This screencast shows the expected behaviour.
![Usage](/images/demo.gif)

## Main takeaways
+ I learned a lot about asynchronous and concurrent programming. This is a key element whenever we want to interact with the network, since we are not under control of the program flow, after we send out a request or a message to the server/client
+ One of the most interesting things that I have learned, were the handshake process between the client and the server to ensure, that no invalid/mallicious communication can take place. The implementation can be found within the file ```net_connection_interface.hpp```
+ I learned, that templates are mainly places in header files, since the explicit code is written at compile time by the compiler and also learned how to abstract functionallities to a common form.
+ I learned the key concepts of asynchronous programming with the ```boost::asio``` framework.
+ Also I familliarized myself with GTest and how to include unit tests with GTest in a CMake project, by creating a unit test for the net_message.hpp implementation to make sure, that the serialization and deserialization is done properly.
+ Header of a networking message can be a very bad security issue, especially if they contain a size information from which a memory allocation is derived within the server/client process. Therefore, a handshake between the client and the server before any payload data is exchanged is a useful feature that can make sure, that no unintented mallicious communication takes place.
+ I learned how to use condition variables to block a thread that is waiting for an asynchronous event to happen (i.e. waiting for an incoming message), so that the processor is not occupied the complete waiting time. (see: net_ts_queue.hpp)
+ It is the best way to standartize the payload with a defined payload datastructure that is serializable and just send one payload per message! (Example: If you are makeing a HTTP request, the maximum number of byts per payload is also restricted to a fixed number)
  - Top Tip: Sometimes it is useful to define a form that the client sends to the server with already established information and that let the server fill out the missing parts of the datastructure

## Possable extensions
- [ ] Find a way to avoid to remain within the raw terminal interface for the user input on the client application.
- [ ] Add a SQL Database to the Server in order to learn about databases and create a dataset for makeing statistics about the network connections
- [ ] Make the payload encrypted and learn about encrytion of internet traffic
