#include "network.h"

using boost::asio::ip::tcp;
using boost::asio::ip::udp;
std::atomic<bool> running = true;


//@@@@@@@@@@@@@@@@@@@@@@  UTILITIES @@@@@@@@@@@@@@@@@@@@@@@@@@
void printLocalIP() {
    try {
        boost::asio::io_context io_context;

        boost::asio::ip::udp::socket socket(io_context);
        socket.open(boost::asio::ip::udp::v4());
        socket.connect(boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string("8.8.8.8"), 53));

        auto local_endpoint = socket.local_endpoint();
        std::cout << centerText("Local IP: ", getTerminalWidth()) << local_endpoint.address().to_string() << std::endl;
        std::cout << std::endl;
        std::cout << centerText("Press enter to continue", getTerminalWidth());
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void clearSocketBuffer(udp:: socket& socket) {
    socket.non_blocking(true);
    char buffer[1024];
    udp::endpoint remote_endpoint;
    boost::system::error_code error;
    while (socket.receive_from(boost::asio::buffer(buffer), remote_endpoint, 0, error) != 0) {
        if (error == boost::asio::error::would_block || error == boost::asio::error::try_again) {
            break;
        }
    }
    socket.non_blocking(false);
}


//@@@@@@@@@@@@@@@@@@@@@@  LAN  @@@@@@@@@@@@@@@@@@@@@@@@@@
void getIpForLan(std::string& ip) {
    try {
        boost::asio::io_context io_context;
        boost::asio::ip::udp::socket socket(io_context);
        socket.open(boost::asio::ip::udp::v4());
        socket.connect(boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string("8.8.8.8"), 53));
        auto local_endpoint = socket.local_endpoint();
        ip = local_endpoint.address().to_string();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void broadcastIP(udp::socket& socket, boost::asio::io_context& io_context, int port, std::string local_ip) {
    try {
        //Send broadcast message
        socket.set_option(udp::socket::reuse_address(true));
        socket.set_option(udp::socket::broadcast(true));

        udp::endpoint broadcast_endpoint(boost::asio::ip::address_v4::broadcast(), port);

    std::cout << "Broadcasting..." << std::endl;
    while (keepBroadcasting) {
        std::string message = "Permission to LAN";
        socket.send_to(boost::asio::buffer(message), broadcast_endpoint);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void listenForLan(udp::socket& socket, boost::asio::io_context& io_context, int port, std::string& local_ip, std::string& peer_ip) {
    try {

        while (!peer_ip.size()) {
            //listen for broadcast
            char buffer[1024];
            udp::endpoint remote_endpoint;
            size_t len = socket.receive_from(boost::asio::buffer(buffer), remote_endpoint);

            std::string message(buffer, len);
            if (message == "Permission to LAN" && remote_endpoint.address().to_string() != local_ip) {
                std::cout << "Received signal from: " << remote_endpoint.address().to_string() << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(2));
                std::string response = "Permission granted";


                for (int i = 0; i < 5; ++i) { 
                    socket.send_to(boost::asio::buffer(response), remote_endpoint);
                    std::this_thread::sleep_for(std::chrono::milliseconds(200));
                }

                keepBroadcasting = false;
                peer_ip = remote_endpoint.address().to_string();
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}


//@@@@@@@@@@@@@@@@@@@@@@  P2P  @@@@@@@@@@@@@@@@@@@@@@@@@@
void hitStun(std::string& public_ip, int& public_port, udp::socket& socket, boost::asio::io_context& io_context) {
    std::string stun_server = "stun.l.google.com"; // Public STUN server
    unsigned short stun_port = 19302;             // STUN port

    try {
        // Resolve the STUN server akddress
        udp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(udp::v4(), stun_server, std::to_string(stun_port));

        // stun request
        std::vector<uint8_t> request = {
            0x00, 0x01, // Binding Request
            0x00, 0x00, // Message Length: 0
            0x21, 0x12, 0xA4, 0x42, // Magic Cookie whatever that means
            0x63, 0xC7, 0x11, 0xAB, 0xA1, 0x1F, 0xAC, 0xA4, 0x71, 0x12, 0xD0, 0x13 // Transaction ID (randomized)
        };

        socket.send_to(boost::asio::buffer(request), *endpoints.begin());

        // response
        std::vector<uint8_t> response(1024);
        udp::endpoint sender_endpoint;
        size_t length = socket.receive_from(boost::asio::buffer(response), sender_endpoint);

        bool flag = true;
        // parse stun
        if (length >= 20 && response[0] == 0x01 && response[1] == 0x01) { // Check if successfult bind
            size_t index = 20; // Start after the STUN header (20 bytes)
            while (index + 4 < length && flag) {
                uint16_t attr_type = (response[index] << 8) | response[index + 1];
                uint16_t attr_length = (response[index + 2] << 8) | response[index + 3];
                if (attr_type == 0x0020 && attr_length >= 8) { // XOR-MAPPED-ADDRESS
                    public_port = ((response[index + 6] << 8) | response[index + 7]) ^ 0x2112;
                    uint32_t ip =
                        ((response[index + 8] ^ 0x21) << 24) |
                        ((response[index + 9] ^ 0x12) << 16) |
                        ((response[index + 10] ^ 0xA4) << 8) |
                        (response[index + 11] ^ 0x42);

                    public_ip = std::to_string((ip >> 24) & 0xFF) + "." +
                                std::to_string((ip >> 16) & 0xFF) + "." +
                                std::to_string((ip >> 8) & 0xFF) + "." +
                                std::to_string(ip & 0xFF);

                    std::string tmpIpString = "Your IP is: " + public_ip;
                    std::string tmpPortString = "Your Port is: " + std::to_string(public_port);
                    std::string tmpBoundPortString = "Bound to local port: " + std::to_string(socket.local_endpoint().port());
                    std::cout << centerText(tmpIpString, getTerminalWidth()) << std::endl;
                    std::cout << centerText(tmpPortString, getTerminalWidth()) << std::endl;
                    std::cout << centerText(tmpBoundPortString, getTerminalWidth()) << std::endl;
                    std::cout << std::endl;
                    return; //prevents an error from being thrown for god knows why

                    flag = false;
                }
                index += 4 + attr_length; 
            }
        }

        throw std::runtime_error("Failed to parse STUN response");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    } 
}

void punchHole(std::string& peer_ip, int peer_port, udp::socket& socket, boost::asio::io_context& io_context) {
    std::cout << std::endl;
    std::cout << centerText("Wait until your peer is ready, then hit enter:", getTerminalWidth());
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << std::endl;
    std::cout << centerText("Punching hole...", getTerminalWidth()) << std::endl;
    try {
        udp::endpoint peer_endpoint(boost::asio::ip::make_address(peer_ip), peer_port);
        // test message
         std::string message = "Hello, Peer!";
        for (int i = 0; i < 5; ++i) {
            socket.send_to(boost::asio::buffer(message), peer_endpoint);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }

        // wait for response
        char buffer[1024];
        udp::endpoint remote_endpoint;
        size_t len = socket.receive_from(boost::asio::buffer(buffer), remote_endpoint);

        std::cout << std::endl;
        std::cout << centerText("Received response: ", getTerminalWidth()) << std::string(buffer, len)
                  << " from " << remote_endpoint.address().to_string()
                  << ":" << remote_endpoint.port() << std::endl;

         for (int i = 0; i < 5; ++i) {
            socket.send_to(boost::asio::buffer(message), peer_endpoint);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        } 
                      sleep(10);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}




//@@@@@@@@@@@** queue functions  **@@@@@@@@@@@@@@
void ingestLocalData(bool& currentColor, bool& localColor, bool& drawOffered, bool& drawAccepted, bool& drawOfferReceived, udp::socket& socket, udp::endpoint& peer_endpoint, std::queue<std::string>& moveQueue,
                     std::queue<std::string>& chatQueue, std::mutex& moveMutex, std::mutex& chatMutex, std::condition_variable& queueCondVar) {

   std::string localInput;
   char colorChar = localColor ? 'B' : 'W';

   while (running) {
       std::getline(std::cin, localInput);
       clearLastLine();
       //append and queue chat 
       if (localInput.rfind("/t", 0) == 0) {
           localInput = "[" + std::string(1, colorChar) + "C]" + localInput.substr(2);
           socket.send_to(boost::asio::buffer(localInput), peer_endpoint);
           enqueueString(chatQueue, localInput, chatMutex, queueCondVar);
       //append and queue moves
       } else if(localInput == "/quit" || localInput == "q") {
           socket.send_to(boost::asio::buffer(localInput), peer_endpoint);
           enqueueString(moveQueue, localInput, moveMutex, queueCondVar);
       } else if(localInput == "/resign") {
           socket.send_to(boost::asio::buffer(localInput), peer_endpoint);
           enqueueString(moveQueue, localInput, moveMutex, queueCondVar);
       } else if(localInput == "/draw") {
           if(drawOfferReceived) {
               socket.send_to(boost::asio::buffer(localInput), peer_endpoint);
               drawAccepted = true;
           } else {
               socket.send_to(boost::asio::buffer(localInput), peer_endpoint);
               drawOffered = true;
               std::cout << "draw offered" << std::endl;
               std::this_thread::sleep_for(std::chrono::seconds(2));
           }
       } else {
            if(currentColor == localColor) {
               localInput = "[" + std::string(1, colorChar) + "M]" + localInput;
               enqueueString(moveQueue, localInput, moveMutex, queueCondVar);
            } else {
               std::cout << "It is not your turn" << std::endl;}
               std::this_thread::sleep_for(std::chrono::seconds(2));
               reprint = true;
       }
   }
}
    
void ingestExternalData(bool& localColor, bool& drawOffered, bool& drawAccepted, bool& drawOfferReceived, udp::socket& socket, udp::endpoint& peer_endpoint, std::queue<std::string>& moveQueue,
                   std::queue<std::string>& chatQueue, std::mutex& moveMutex, std::mutex& chatMutex, std::condition_variable& queueCondVar) {

    char colorChar = localColor ? 'W' : 'B';

    try {
        while (running) {
            char buffer[1024];
            udp::endpoint remote_endpoint;
            size_t len = socket.receive_from(boost::asio::buffer(buffer), remote_endpoint);
            std::string message(buffer, len);

            if (message.rfind("[WC]", 0) == 0) {
                enqueueString(chatQueue, message, chatMutex, queueCondVar);
            } else if (message.rfind("[BC]", 0) == 0) {
                enqueueString(chatQueue, message, chatMutex, queueCondVar);
            } else if (message == "/quit" || message == "q") {
                enqueueString(moveQueue, message, moveMutex, queueCondVar);
            } else if (message == "/resign") {
                enqueueString(moveQueue, message, moveMutex, queueCondVar);
            } else if (message == "/draw") {
                if (!drawOffered) {
                    drawOfferReceived = true;
                    std::cout << "Opponent offered draw, respond with /draw to accept" << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                  } else {
                    drawAccepted = true;
                  }
            } else {
                enqueueString(moveQueue, message, moveMutex, queueCondVar);
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
    
void enqueueString(std::queue<std::string>& queue, std::string item, std::mutex& mutex, std::condition_variable& condVar) {
    std::lock_guard<std::mutex> lock(mutex);
    queue.push(item);
    condVar.notify_one();
}

void dequeueString(std::queue<std::string>& queue, std::string& item, std::mutex& mutex, std::condition_variable& condVar) {
    std::unique_lock<std::mutex> lock(mutex);
    condVar.wait(lock, [&] { return !queue.empty(); });
    item = queue.front();
    queue.pop();
}

bool waitForDrawResponse(udp::socket& socket, udp::endpoint& peer_endpoint) {
    char buffer[1024];
    udp::endpoint remote_endpoint;
    size_t len = socket.receive_from(boost::asio::buffer(buffer), remote_endpoint);
    std::string message(buffer, len);
    if (message == "y") {
        return true;
    } else {
        return false;
    }

}