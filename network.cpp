#include "network.h"

using boost::asio::ip::tcp;
using boost::asio::ip::udp;


std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

std::string getPublicIpAddress() {
    return exec("curl -s ifconfig.me");
}

void hitStun(std::string& public_ip, int public_port, udp::socket& socket, boost::asio::io_context& io_context) {
    std::string stun_server = "stun.l.google.com"; // Public STUN server
    unsigned short stun_port = 19302;             // STUN port

    try {
        // Resolve the STUN server address
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

                    std::cout << std::endl;
                    std::cout << centerText("Your IP is: " + public_ip, getTerminalWidth()) << std::endl;
                    std::cout << centerText("Your Port is: ", getTerminalWidth()) << public_port << std::endl;
                    std::cout << centerText("Bound to local port:", getTerminalWidth()) << socket.local_endpoint().port() << std::endl;
                    std::cout << std::endl;
                    std::cout << centerText("Press enter to continue", getTerminalWidth());
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    return;

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

void punchHole(std::string& ip, int port, udp::socket& socket, boost::asio::io_context& io_context) {
    try {
        udp::endpoint peer_endpoint(boost::asio::ip::make_address(ip), port);
        // test message
        std::string message = "Hello, Peer!";
        socket.send_to(boost::asio::buffer(message), peer_endpoint);
        std::cout << "Sent message to peer: " << ip << ":" << port << std::endl;

        // wait for response
        char buffer[1024];
        udp::endpoint remote_endpoint;
        size_t len = socket.receive_from(boost::asio::buffer(buffer), remote_endpoint);

        std::cout << "Received response: " << std::string(buffer, len)
                  << " from " << remote_endpoint.address().to_string()
                  << ":" << remote_endpoint.port() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void receiveMessages(udp::socket& socket) {
    try {
        while (true) {
            char buffer[1024];
            udp::endpoint remote_endpoint;
            size_t len = socket.receive_from(boost::asio::buffer(buffer), remote_endpoint);

            std::string message(buffer, len);
            std::cout << "\n[Peer]: " << message << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "\nError receiving message: " << e.what() << std::endl;
    }
}


void sendMessages(udp::socket& socket, const udp::endpoint& peer_endpoint) {
    try {
        while (true) {
            std::cout << "[You]: ";
            std::string message;
            std::getline(std::cin, message);

            socket.send_to(boost::asio::buffer(message), peer_endpoint);
        }
    } catch (const std::exception& e) {
        std::cerr << "\nError sending message: " << e.what() << std::endl;
    }
}

//@@@@@@@@@@@@@@@@@@@@@@  LAN  @@@@@@@@@@@@@@@@@@@@@@@@@@
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
        // udp::socket socket(io_context, udp::endpoint(udp::v4(), port));
        socket.set_option(udp::socket::reuse_address(true));
        socket.set_option(udp::socket::broadcast(true));

        udp::endpoint broadcast_endpoint(boost::asio::ip::address_v4::broadcast(), port);

    std::cout << "Broadcasting..." << std::endl;
    while (keepBroadcasting) {
        std::string message = "Permission to LAN";
        socket.send_to(boost::asio::buffer(message), broadcast_endpoint);
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void listenForLan(udp::socket& socket, boost::asio::io_context& io_context, int port, std::string& local_ip, std::string& peer_ip) {
    try {
        // udp::socket socket(io_context, udp::endpoint(udp::v4(), port));
        // socket.set_option(udp::socket::reuse_address(true));
        // socket.set_option(udp::socket::broadcast(true));

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
                socket.send_to(boost::asio::buffer(response), remote_endpoint);
                keepBroadcasting = false;
                peer_ip = remote_endpoint.address().to_string();
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

//@@@@@@@@@@@@@@@@@@@@@@  SERVER  @@@@@@@@@@@@@@@@@@@@@@@@@@
std::queue<std::string> messageQueue;
std::mutex queueMutex;
std::condition_variable queueCondVar;

void startServer(int port) {
    try {
        boost::asio::io_context io_context;

        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));
        std::cout << "server listening on port " << port << std::endl;

        tcp::socket socket(io_context);
        acceptor.accept(socket);
        sleep(1);
        std::cout << "connection established" << std::endl;

        char buffer[1024];
        boost::system::error_code error;
        while (true) {
            size_t len = socket.read_some(boost::asio::buffer(buffer), error);
            if (error == boost::asio::error::eof) {
                std::cout << "client disconnected." << std::endl;
                sleep(2);
                break;
            }
            std::string message(buffer, len);
            std::cout << "message: " << message << std::endl;
        }

    } catch (std::exception& e) {
        sleep(5);
        std::cerr << "server error: " << e.what() << std::endl;
    }

}


//@@@@@@@@@@@@@@@@@@@@@@  CLIENT  @@@@@@@@@@@@@@@@@@@@@@@@@@
void startClient(std::string& host, int port) {
    try {
        boost::asio::io_context io_context;

        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(host, std::to_string(port));

        // create socket and connect to server
        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);
        std::cout << "\nconnected to the server" << std::endl;
        sleep(2);


        while (true) {
            std::unique_lock<std::mutex> lock(queueMutex);
            queueCondVar.wait(lock, [] { return !messageQueue.empty(); });
        // LISTEN



        //WRITE

            while (!messageQueue.empty()) {
                std::string message = messageQueue.front();
                messageQueue.pop();
                lock.unlock();
                boost::asio::write(socket, boost::asio::buffer(message));
                lock.lock();
            }
        }

    } catch (std::exception& e) {
        std::cerr << "Client error: " << e.what() << std::endl;
        sleep(5);
    }
}

void addMessageToQueue(const std::string& message) {
    std::lock_guard<std::mutex> lock(queueMutex);
    messageQueue.push(message);
    queueCondVar.notify_one();
}

std::string getMessageFromQueue() {
    std::unique_lock<std::mutex> lock(queueMutex);
    queueCondVar.wait(lock, [] { return !messageQueue.empty(); });
    std::string message = messageQueue.front();
    messageQueue.pop();
    return message;
}
