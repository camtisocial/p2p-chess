#include "main.h"
//TODO make all strings that print to center account for length added by concatenation

std::atomic<bool> keepBroadcasting{true};

void startLocalGame() {
    //TODO ADD ANIMATION BEFORE BOARD FLIPS SO ITS LESS ABRUPT
    GameBoard board;
    //0 = white to play, 1 = black to play
    bool to_play = 0;
    int turn = 1;
    
    std::string move = "";
    while(move != "q") {
        std::cout << "\n\n\n";
        if(!to_play) {
            board.printBoardWhite(to_play, turn);
        } else {
            board.printBoardBlack(to_play, turn);
        }
        std::cout << "\n\n";
        std::cout << "   Enter move: ";
        std::cout.flush();
        std::getline(std::cin,  move);

        if (board.movePiece(move, to_play)) {
            if(to_play) {turn++;}
            to_play = !to_play;
        };
            
        system("clear");
    }
}


void startOnlineGame(bool localColor, udp::socket& socket, udp::endpoint& peer_endpoint) {
    GameBoard board;
    //0 = white to play, 1 = black to play
    bool to_play = 0;
    int turn = 1;

    std::string move = "";
    while (move != "q") {
        std::cout << "\n\n\n";
        if (localColor == 0) {
                board.printBoardWhite(to_play, turn);
        } else {
                board.printBoardBlack(to_play, turn);
        }
        if (localColor == to_play) { 
            std::cout << "\n\n";
            std::cout << "   Enter move: ";
            std::cout.flush();
            std::getline(std::cin, move);
            //TODO return a move error from movePiece to give more descriptive reason why move is invalid
            if (board.movePiece(move, to_play)) {
                turn++;
                to_play = !to_play;
                socket.send_to(boost::asio::buffer(move), peer_endpoint);
            } else {
                std::cout << "Invalid move" << std::endl;
                sleep(2);
            }
        } else {
            // std::cout << "Opponent's turn" << std::endl;
            std::cout << std::endl;
            char buffer [1024];
            udp::endpoint remote_endpoint;
            std::string opponent_move;
            size_t len = socket.receive_from(boost::asio::buffer(buffer), remote_endpoint);
            opponent_move = std::string(buffer, len);
            // std::cout << "Opponent's move: " << move << std::endl;
            // sleep(5);
            if (board.movePiece(opponent_move, to_play)) {
                turn++;
                to_play = !to_play;
            }
        }
        system("clear");
    }

}

void startOnlineGameTest(bool localColor, udp::socket& socket, udp::endpoint& peer_endpoint) {

    std::string move = "";
    while (move != "q") {
        std::cout << "enter move: ";
        std::getline(std::cin, move);
        std::cout << "you entered: " << move << std::endl;
    }

}

int main(int argc, char** argv) {
    // startOnlineGameTest(0);
    std::string externalIP;  
    int ephemeralPort;
    int localPort;
    int selected = 0;
    std::vector<std::string> options = {"Online", "Local", "LAN", "Quit"};
    // std::vector<std::string> options = {"Online", "Join Game", "Local", "LAN", "Quit"};
    // std::vector<std::string> options = {"Host Game", "Join Game", "Local", "LAN", "Quit", "test hitStun"};

    setRawMode(true);

    while (true) {
        system("clear");
        displayMenu(options, selected);

        KeyPress key = getKeyPress();
        if (key == UP) {
            selected = (selected - 1 + options.size()) % options.size();
        } else if (key == DOWN) {
            selected = (selected + 1) % options.size();
        } else if (key == ENTER) {
            if (options[selected] == "Quit") {
                break;


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ** ONLINE ** @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            } else if (options[selected] == "Online") {
                try {
                     //TODO set port by dot file or command line
                    boost::asio::io_context io_context;
                    udp::socket socket(io_context, udp::endpoint(udp::v4(), 12345));

                    system("clear");
                    displayMenu(options, selected);

                    hitStun(externalIP, ephemeralPort, socket, io_context);
                    std::string peer_ip = setPeerIP();
                    int peer_port = setPeerPort();
                    punchHole(peer_ip, peer_port, socket, io_context);

                    udp::endpoint peer_endpoint(boost::asio::ip::make_address(peer_ip), peer_port);
                    std::thread receiver(receiveMessages, std::ref(socket));
                    setRawMode(false);
                    sendMessages(socket, peer_endpoint);
                    receiver.join();
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
                // startServer(port);
            } else if (options[selected] == "Join Game") {
                boost::asio::io_context io_context;
                udp::socket socket(io_context, udp::endpoint(udp::v4(), localPort));
                hitStun(externalIP, ephemeralPort, socket, io_context);
                int port = setPeerPort();
                std::string ip = setPeerIP();
                punchHole(ip, port, socket, io_context);
                // int ip = getIP();


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ** LAN ** @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            } else if (options[selected] == "LAN") {
                setRawMode(false);
                bool localColor{};
                std::string localIP{};
                std::string peerIP{};
                localPort = 12344;

                // // setting up connection
                getIpForLan(localIP);
                boost::asio::io_context io_context;
                udp::socket socket(io_context, udp::endpoint(udp::v4(), localPort));
                std::thread broadcaster(broadcastIP, std::ref(socket), std::ref(io_context), localPort, localIP);
                std::thread listener(listenForLan, std::ref(socket), std::ref(io_context), localPort, std::ref(localIP), std::ref(peerIP));
                broadcaster.join();
                listener.join();
                udp::endpoint peer_endpoint(boost::asio::ip::make_address(peerIP), localPort);

                // // setting up game
                localColor = setLocalColor();
                setRawMode(false);
                clearSocketBuffer(socket);
                // startOnlineGameTest(0, socket, peer_endpoint);
                startOnlineGame(localColor, socket, peer_endpoint);

                // std::cout << "press any key to continue" << std::endl;
                // std::cin.get();

                // udp::endpoint peer_endpoint(boost::asio::ip::make_address(peerIP), localPort);
                // std::thread receiver(receiveMessages, std::ref(socket));
                // sendMessages(socket, peer_endpoint);
                // receiver.join();


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ** LOCAL ** @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            } else if (options[selected] == "Local") {
                system("clear");
                setRawMode(false);
                startLocalGame();


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ** TEST ** @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            } else if (options[selected] == "test hitStun") {
                int testPort = 12345;
                boost::asio::io_context io_context;
                udp::socket socket(io_context, udp::endpoint(udp::v4(), testPort));
                hitStun(externalIP, ephemeralPort, socket, io_context);
                sleep(10);

            }
        }
    }
    setRawMode(false);
	return 0;
}