#include <boost/asio.hpp>
#include "network.h"

using boost::asio::ip::tcp;

void startServer(int port) {
    try {
        boost::asio::io_context io_context;

        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));
        std::cout << "server listening on port " << port << std::endl;

        tcp::socket socket(io_context);
        acceptor.accept(socket);
        sleep(5);
        std::cout << "connection established" << std::endl;

        char buffer[1024];
        boost::system::error_code error;
        while (true) {
            size_t len = socket.read_some(boost::asio::buffer(buffer), error);
            if (error == boost::asio::error::eof) {
                std::cout << "client disconnected." << std::endl;
                sleep(5);
                break;
            }
            std::string message(buffer, len);
            std::cout << "message: " << message << std::endl;
            sleep(10);
        }

    } catch (std::exception& e) {
        sleep(5);
        std::cerr << "server error: " << e.what() << std::endl;
    }

}

void startClient(std::string& host, int port) {
    try {
        boost::asio::io_context io_context;

        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(host, std::to_string(port));

        // create socket and connect to server
        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);
        std::cout << "connected to the server" << std::endl;
        sleep(5);

        // manage connection
        std::string message = "hello server";
        sleep(5);
        boost::asio::write(socket, boost::asio::buffer(message));
    } catch (std::exception& e) {
        std::cerr << "Client error: " << e.what() << std::endl;
        sleep(5);
    }
}