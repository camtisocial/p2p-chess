#include "main.h"


void startLocalGame() {
    //ADD ANIMATION BEFORE BOARD FLIPS SO ITS LESS ABRUPT
    GameBoard board;
    //0 = white to play, 1 = black to play
    bool to_play = 0;
    int turn = 1;
    
    std::string q = "";
    while(q != "q") {
        std::cout << "\n\n\n";
        if(!to_play) {
            board.printBoardWhite(to_play, turn);
        } else {
            board.printBoardBlack(to_play, turn);
        }
        std::cout << "\n\n";
        std::cout << "   Enter move: ";
        std::cout.flush();
        std::getline(std::cin,  q);
        if (q != "q") {
            if (board.movePiece(q, to_play)) {
                if(to_play) {turn++;}
                to_play = !to_play;
            };
        }
        system("clear");
    }
}


int main(int argc, char** argv) {
    std::string externalIP;  
    int ephemeralPort;
    int localPort;
    int selected = 0;
    std::vector<std::string> options = {"test", "Host Game", "Join Game", "Local", "Quit"};

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


            } else if (options[selected] == "Host Game") {
                try {
                    localPort = setLocalPort();
                    boost::asio::io_context io_context;
                    udp::socket socket(io_context, udp::endpoint(udp::v4(), localPort));
                    hitStun(externalIP, ephemeralPort, socket, io_context);
                    int port = setPeerPort();
                    // std::string ip = setPeerIP();
                    std::string ip = "127.0.0.1";
                    // punchHole(ip, port, socket, io_context);

                    udp::endpoint peer_endpoint(boost::asio::ip::make_address(ip), port);
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



            } else if (options[selected] == "Local") {
                system("clear");
                setRawMode(false);
                startLocalGame();
            } else if (options[selected] == "test") {
                boost::asio::io_context io_context;
                udp::socket socket(io_context, udp::endpoint(udp::v4(), localPort));
                hitStun(externalIP, ephemeralPort, socket, io_context);

            }
        }
    }
    
//         std::cout << centerText(R"(
//          _                   
//      ___| |__   ___  ___ ___  
//     / __| '_ \ / _ \/ __/ __|
//    | (__| | | |  __/\__ \__ \
//     \___|_| |_|\___||___/___/)", terminalWidth);



    //     board.printBoard();
    //     std::cout << "\n\n";
    //     std::cout << "   Enter move: ";
    //     std::cout.flush();
    //     std::getline(std::cin,  q);
    //     if (q != "q") {
    //         if (board.movePiece(q, to_play)) {
    //             turn++;
    //             to_play = !to_play;
    //             board.saveBoardState(turn, to_play, boardState);
    //             std::cout << boardState.dump(4) << std::endl;
    //         };
            
    //     }
    //     std::cout << "\n";
    //     system("cls");

    setRawMode(false);
	return 0;
}

/*

int main(int argc, char** argv) {

    GameBoard board;
    nlohmann::json boardState;
    //0 = white to play, 1 = black to play
    bool to_play = 0;
    int turn = 0;
    
    std::cout << std::endl;
    std::string q = "";
    while(q != "q") {
        std::cout << R"(
         _                   
     ___| |__   ___  ___ ___  
    / __| '_ \ / _ \/ __/ __|
   | (__| | | |  __/\__ \__ \
    \___|_| |_|\___||___/___/)";


        std::cout << "\n\n\n";
        board.printBoard();
        std::cout << "\n\n";
        std::cout << "   Enter move: ";
        std::cout.flush();
        std::getline(std::cin,  q);
        if (q != "q") {
            if (board.movePiece(q, to_play)) {
                turn++;
                to_play = !to_play;
                board.saveBoardState(turn, to_play, boardState);
                std::cout << boardState.dump(4) << std::endl;
            };
            
        }
        std::cout << "\n";
        system("cls");
    }

	return 0;
}


*/