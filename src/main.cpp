#include "main.h"
//general
    //TODO when user picks color in online game, send it to opponent and assign colors that way
    //TODO fix bug where draw variables are not reset after user declines draw offer
    //TODO make it so when shaking hands, it attempts to both broadcast and listen for 20 seconds before timing out rather then sending a burst over 1 second
    //TODO add a way to cycle through previous moves with arrow keys

//startOnlineGame()
    //TODO return a move error from movePiece to give more descriptive reason why move is invalid

//startLocalGame()
    //TODO add an animation before board flips so its less abrupt
    //TODO fix bug where checkmate isn't being detected/printed

//menu stuff
    //TODO dynamically see how many newlines to add to center text

std::queue<std::string> moveQueue;
std::queue<std::string> chatQueue;
std::mutex moveQueueMutex;
std::mutex chatQueueMutex;
std::condition_variable moveQueueCondVar;
std::condition_variable chatQueueCondVar;

//config
Config config = parseConfig("/usr/share/terminalChess/settings.ini");
int localPort = config.local_port;
int peerPort = config.peer_port;
std::string whitePieces = config.white_pieces;
std::string blackPieces = config.black_pieces;
std::string boardColor = config.board_color;

void startOnlineGame(bool& turnRef, bool localColor, bool& drawOffered, bool& drawAccepted, bool& running, udp::socket& socket, udp::endpoint& peer_endpoint) {
    GameBoard board;
    //0 = white to play, 1 = black to play
    int turn = 1;
    std::string move = "";
    char gameResult = 'C';

        // Print the game board
        if (localColor == 0) {
            board.printBoardWhite(turnRef, turn, whitePieces, blackPieces, boardColor);
        } else {
            board.printBoardBlack(turnRef, turn, whitePieces, blackPieces, boardColor);
        }

    while (running) {

        if (drawAccepted) {
            gameResult = 'D';
        }
        // Process chat messages 
       std::string chatMessage;
       while (true) {
           if (chatQueue.empty()) break;
           dequeueString(chatQueue, chatMessage, chatQueueMutex, chatQueueCondVar);
           if ((chatMessage.rfind("[WC]", 0) == 0)) {
               if (localColor == 0) {
                  std::cout << whitePieces << "[You]: " << "\x1B[1;92m"  << chatMessage.substr(4) << "\033[0m"  << std::endl;
               } else {
                  std::cout << whitePieces << "[Opponent]: " << "\x1B[1;92m"  << chatMessage.substr(4) << "\033[0m"  << std::endl;
               }
           } else if (chatMessage.rfind("[BC]", 0) == 0) {
               if (localColor == 1) {
                  std::cout << blackPieces << "[You]: " << "\x1B[1;92m"  << chatMessage.substr(4) << "\033[0m"  << std::endl;
               } else {
                  std::cout << blackPieces << "[Opponent]: " << "\x1B[1;92m"  << chatMessage.substr(4) << "\033[0m"  << std::endl;
               }
           }
       }



        // proces moves
        if (turnRef == localColor) {
            std::unique_lock<std::mutex> moveLock(moveQueueMutex);

            // Wait for a move in the queue
            // ADD TIMEOUT EVERY 50 MS 
            if (moveQueueCondVar.wait_for(moveLock, std::chrono::milliseconds(100), [] { return !moveQueue.empty(); })) {
                std::string move = moveQueue.front();
                moveQueue.pop();
                moveLock.unlock();


            //process resignation and quitting
                if (move == "[WR]") {
                    gameResult = 'b';
                } else if(move == "[BR]") {
                    gameResult = 'w';
                } else if (move == "/quit" || move == "q") {
                    gameResult = 'q';
                }

                // Process the move
                if (move.rfind("[WM]", 0) == 0 || move.rfind("[BM]", 0) == 0) {
                    if (board.movePiece(move.substr(4), turnRef)) {
                        socket.send_to(boost::asio::buffer(move), peer_endpoint);
                        gameResult = board.checkForMateOrDraw(turnRef);
                        if(turnRef) turn++;
                        turnRef = !turnRef;
                    } else {
                        std::cout << "Invalid move: " << move.substr(4) << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(2));
                    }
    
                    system("clear");
                    // Print the game board
                    if (localColor == 0) {
                        board.printBoardWhite(turnRef, turn, whitePieces, blackPieces, boardColor);
                    } else {
                        board.printBoardBlack(turnRef, turn, whitePieces, blackPieces, boardColor);
                    }

                }
            }

        } else {
            std::unique_lock<std::mutex> moveLock(moveQueueMutex);
            if (moveQueueCondVar.wait_for(moveLock, std::chrono::milliseconds(100), [] { return !moveQueue.empty(); })) {

                std::string opponentMove = moveQueue.front();
                moveQueue.pop();
                moveLock.unlock();

                //process draw offers, resignation, and quitting
                if (opponentMove == "[WR]") {
                    gameResult = 'b';
                } else if(opponentMove == "[BR]") {
                    gameResult = 'w';
                } else if (opponentMove == "/quit" || move == "q") {
                    gameResult = 'q';
                }

                // Process the opponent's move
                if (opponentMove.rfind("[WM]", 0) == 0 || opponentMove.rfind("[BM]", 0) == 0) {
                    if (board.movePiece(opponentMove.substr(4), turnRef)) {
                        gameResult = board.checkForMateOrDraw(turnRef);
                        if(turnRef) turn++;
                        turnRef = !turnRef;
                    } else {
                        std::cout << "Opponent made an invalid move: " << opponentMove << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(2));
                    }

                    system("clear");
                    // Print the game board
                    if (localColor == 0) {
                        board.printBoardWhite(turnRef, turn, whitePieces, blackPieces, boardColor);
                    } else {
                        board.printBoardBlack(turnRef, turn, whitePieces, blackPieces, boardColor);
                    }

                }

            }
        }
        if (gameResult != 'C') {
            for (int i = 0; i < 5; i++) {
                socket.send_to(boost::asio::buffer("TERMINATE"), peer_endpoint);
                // std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            running = false;
        }

        std::unique_lock<std::mutex> boardLock(moveQueueMutex);
            if (reprint) {
                system("clear");
                // Print the game board
                if (localColor == 0) {
                    board.printBoardWhite(turnRef, turn, whitePieces, blackPieces, boardColor);
                } else {
                    board.printBoardBlack(turnRef, turn, whitePieces, blackPieces, boardColor);
                }
                reprint = false;
        }

    }

    announceGameResult(gameResult);

}

void startLocalGame() {
    GameBoard board;
    //0 = white to play, 1 = black to play
    char gameResult{'C'};
    bool running = true;
    bool to_play = 0;
    int turn = 1;
    
    std::string move = "";
    while(running) {
        if(!to_play) {
            board.printBoardWhite(to_play, turn, whitePieces, blackPieces, boardColor);
        } else {
            board.printBoardBlack(to_play, turn, whitePieces, blackPieces, boardColor);
        }
        std::cout << "\n\n";
        std::cout << "   Enter move: ";
        std::cout.flush();
        std::getline(std::cin,  move);

        if (move == "/quit" || move == "q") {
            running = false;
         
        } else if (board.movePiece(move, to_play)) {
            char gameResult = board.checkForMateOrDraw(to_play);
            if(to_play) {turn++;}
            to_play = !to_play;
        }

        if (gameResult != 'C') {
            running = false;
        }
        system("clear");
    }
    announceGameResult(gameResult);

}


int main(int argc, char** argv) {
    std::string externalIP;  
    int boundPort;
    int selected = 0;
    std::vector<std::string> options = {"Online", "Local", "LAN", "Colors", "Quit"};

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
                    //TODO dynamically see how many newlines to add to center text
                    boost::asio::io_context io_context;
                    bool running{true};
                    bool localColor{};
                    bool drawOffered{};
                    bool drawAccepted{};
                    bool drawOfferReceived{};
                    udp::socket socket(io_context, udp::endpoint(udp::v4(), 12345));
                    system("clear");
                    displayMenu({}, selected);

                    //set up connection
                    hitStun(externalIP, boundPort, socket, io_context);
                    std::string peer_ip = setPeerIP();
                    punchHole(peer_ip, peerPort, socket, io_context);
                    udp::endpoint peer_endpoint(boost::asio::ip::make_address(peer_ip), peerPort);

                    // setting up game
                    //TODO implement ready check sent to other player before starting game, triggered by picking color
                    bool currentColor = 0;
                    localColor = setLocalColor();
                    std::cout << centerText("press enter to continue", getTerminalWidth());
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    system("clear");
                    setRawMode(false);
                    clearSocketBuffer(socket);
                    std::thread localInput(ingestLocalData, std::ref(currentColor), std::ref(localColor), std::ref(drawOffered), std::ref(drawAccepted), std::ref(drawOfferReceived),  std::ref(socket), std::ref(peer_endpoint), std::ref(moveQueue), std::ref(chatQueue), std::ref(moveQueueMutex), std::ref(chatQueueMutex), std::ref(moveQueueCondVar), std::ref (running));
                    std::thread externalInput(ingestExternalData, std::ref(localColor), std::ref(drawOffered), std::ref(drawAccepted), std::ref(drawOfferReceived), std::ref(socket), std::ref(peer_endpoint), std::ref(moveQueue), std::ref(chatQueue), std::ref(moveQueueMutex), std::ref(chatQueueMutex), std::ref(moveQueueCondVar), std::ref (running));
                    startOnlineGame(currentColor, localColor, drawOffered, drawAccepted, running, socket, peer_endpoint);
                    io_context.stop();
                    socket.close();

                    //clean up
                    bool joined_1{};
                    bool joined_2{};
                    while (!joined_1 || !joined_2) {
                        if (localInput.joinable()) {
                            localInput.join();
                            joined_1 = true;
                        }
                        if (externalInput.joinable()) {
                            externalInput.join();
                            joined_2 = true;
                        }
                    }

                    std::queue<std::string>().swap(moveQueue);
                    std::queue<std::string>().swap(chatQueue);

                    setRawMode(true);
                    io_context.restart();
                    socket = udp::socket(io_context, udp::endpoint(udp::v4(), localPort));
                    running = false;
                    drawOffered = false;
                    drawAccepted = false;
                    drawOfferReceived = false;

                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ** LAN ** @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            } else if (options[selected] == "LAN") {
                setRawMode(false);
                bool running{true};
                bool localColor{};
                bool drawOffered{};
                bool drawAccepted{};
                bool drawOfferReceived{};
                int timeout{5000};
                std::string localIP{};
                std::string peerIP{};

                // setting up connection
                getIpForLan(localIP);
                boost::asio::io_context io_context;
                udp::socket socket(io_context, udp::endpoint(udp::v4(), localPort));
                std::thread broadcaster(broadcastIP, std::ref(socket), std::ref(io_context), localPort, localIP);
                std::thread listener(listenForLan, std::ref(socket), std::ref(io_context), localPort, std::ref(localIP), std::ref(peerIP));
                broadcaster.join();
                listener.join();
                udp::endpoint peer_endpoint(boost::asio::ip::make_address(peerIP), localPort);

    
                // setting up game
                //TODO implement ready check sent to other player before starting game, triggered by picking color
                bool currentColor = 0;
                localColor = setLocalColor();
                std::cout << centerText("press enter to continue", getTerminalWidth());
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                system("clear");
                setRawMode(false);
                clearSocketBuffer(socket);
                std::thread localInput(ingestLocalData, std::ref(currentColor), std::ref(localColor), std::ref(drawOffered), std::ref(drawAccepted), std::ref(drawOfferReceived),  std::ref(socket), std::ref(peer_endpoint), std::ref(moveQueue), std::ref(chatQueue), std::ref(moveQueueMutex), std::ref(chatQueueMutex), std::ref(moveQueueCondVar), std::ref (running));
                std::thread externalInput(ingestExternalData, std::ref(localColor), std::ref(drawOffered), std::ref(drawAccepted), std::ref(drawOfferReceived), std::ref(socket), std::ref(peer_endpoint), std::ref(moveQueue), std::ref(chatQueue), std::ref(moveQueueMutex), std::ref(chatQueueMutex), std::ref(moveQueueCondVar), std::ref (running));
                startOnlineGame(currentColor, localColor, drawOffered, drawAccepted, running, socket, peer_endpoint);
                io_context.stop();
                socket.close();

                //clean up
                bool joined_1{};
                bool joined_2{};
                while (!joined_1 || !joined_2) {
                    if (localInput.joinable()) {
                        localInput.join();
                        joined_1 = true;
                    }
                    if (externalInput.joinable()) {
                        externalInput.join();
                        joined_2 = true;
                    }
                }

                std::queue<std::string>().swap(moveQueue);
                std::queue<std::string>().swap(chatQueue);

                setRawMode(true);
                io_context.restart();
                socket = udp::socket(io_context, udp::endpoint(udp::v4(), localPort));
                running = false;
                drawOffered = false;
                drawAccepted = false;
                drawOfferReceived = false;
                keepBroadcasting = true;

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ** LOCAL ** @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            } else if (options[selected] == "Local") {
                system("clear");
                setRawMode(false);
                startLocalGame();
                setRawMode(true);

            } else if (options[selected] == "Colors") {
                system("clear");
                seeColorOptions();
            } 
        }
    }
    setRawMode(false);
	return 0;
}