#include "network.h"

using boost::asio::ip::tcp;



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