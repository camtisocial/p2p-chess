#include "main.h"
//TODO FOR LAN AND ONLINE, DETERMINE SYSTEM FOR PICKING BLACK AND WHITE
// // TODO make all strings that print to center account for length added by concatenation

std::atomic<bool> keepBroadcasting{true};
std::queue<std::string> moveQueue;
std::queue<std::string> chatQueue;
std::mutex moveQueueMutex;
std::mutex chatQueueMutex;
std::condition_variable moveQueueCondVar;
std::condition_variable chatQueueCondVar;


void startOnlineGame(bool localColor, udp::socket& socket, udp::endpoint& peer_endpoint) {
    //TODO return a move error from movePiece to give more descriptive reason why move is invalid
    //TODO add option to select color pallet to dot file
    GameBoard board;
    //0 = white to play, 1 = black to play
    bool to_play = 0;
    int turn = 1;
    std::string move = "";

        // Print the game board
        if (localColor == 0) {
            board.printBoardWhite(to_play, turn);
        } else {
            board.printBoardBlack(to_play, turn);
        }

    while (running) {
        //This system clear may have to be moved or only run on a certain condition, perhaps when a new move is played or a chat message is received. 
        //Otherwise, it will be refreshing every 100ms, so the user won't be able to see what they are typing
        // system("clear");


        // Process chat messages 
       std::string chatMessage;
       while (true) {
           //pop chat messages from queue
           if (chatQueue.empty()) break;
           dequeueString(chatQueue, chatMessage, chatQueueMutex, chatQueueCondVar);
           std::cout << "\n[CHAT]: " << chatMessage << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
       }



        // proces moves
        if (to_play == localColor) {
            std::unique_lock<std::mutex> moveLock(moveQueueMutex);

            // Wait for a move in the queue
            // ADD TIMEOUT EVERY 50 MS 
            // moveQueueCondVar.wait(lock, [] { return !moveQueue.empty(); });
            if (moveQueueCondVar.wait_for(moveLock, std::chrono::milliseconds(100), [] { return !moveQueue.empty(); })) {
                std::string move = moveQueue.front();
                moveQueue.pop();
                moveLock.unlock();
    
                // Process the move
                if (move.rfind("[WM]", 0) == 0 || move.rfind("[BM]", 0) == 0) {
                    if (board.movePiece(move.substr(4), to_play)) {
                        socket.send_to(boost::asio::buffer(move), peer_endpoint);
                        to_play = !to_play;
                        turn++;
                    } else {
                        std::cout << "Invalid move: " << move.substr(4) << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(2));
                    }
    
                system("clear");
        // Print the game board
        if (localColor == 0) {
            board.printBoardWhite(to_play, turn);
        } else {
            board.printBoardBlack(to_play, turn);
        }
                }
            }

        } else {
            std::unique_lock<std::mutex> moveLock(moveQueueMutex);
            if (moveQueueCondVar.wait_for(moveLock, std::chrono::milliseconds(100), [] { return !moveQueue.empty(); })) {

                std::string opponentMove = moveQueue.front();
                moveQueue.pop();
                moveLock.unlock();

                // Process the opponent's move
                if ( opponentMove.rfind("[WM]", 0) == 0 || opponentMove.rfind("[BM]", 0) == 0) {
                    if (board.movePiece(opponentMove.substr(4), to_play)) {
                        to_play = !to_play;
                        turn++;
                    } else {
                        std::cout << "Opponent made an invalid move: " << opponentMove << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(2));
                    }

                system("clear");
        // Print the game board
        if (localColor == 0) {
            board.printBoardWhite(to_play, turn);
        } else {
            board.printBoardBlack(to_play, turn);
        }
                }
            }
        }
    }

    std::cout << "game ended" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
}


void startLocalGame() {
    //TODO ADD ANIMATION BEFORE BOARD FLIPS SO ITS LESS ABRUPT
    //TODO fix bug where input is messed up after quitting
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


void startOnlineGameTest(bool localColor, udp::socket& socket, udp::endpoint& peer_endpoint) {

    std::string move = "";
    while (move != "q") {
        std::cout << "enter move: ";
        std::getline(std::cin, move);
        std::cout << "you entered: " << move << std::endl;
    }

}

int main(int argc, char** argv) {
    std::string externalIP;  
    int boundPort;
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
                    //TODO dynamically see how many newlines to add to center text
                    boost::asio::io_context io_context;
                    udp::socket socket(io_context, udp::endpoint(udp::v4(), 12345));
                    system("clear");
                    displayMenu({}, selected);

                    //set up connection
                    hitStun(externalIP, boundPort, socket, io_context);
                    std::string peer_ip = setPeerIP();
                    int peer_port = setPeerPort();
                    punchHole(peer_ip, peer_port, socket, io_context);
                    udp::endpoint peer_endpoint(boost::asio::ip::make_address(peer_ip), peer_port);

                    //set up game
                    bool localColor = setLocalColor();
                    std::cout << centerText("press enter to continue", getTerminalWidth());
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    system("clear");
                    setRawMode(false);
                    clearSocketBuffer(socket);
                    startOnlineGame(localColor, socket, peer_endpoint);

                    // std::thread receiver(receiveMessages, std::ref(socket));
                    // setRawMode(false);
                    // sendMessages(socket, peer_endpoint);
                    // receiver.join();
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
                // startServer(port);
            } else if (options[selected] == "Join Game") {
                boost::asio::io_context io_context;
                udp::socket socket(io_context, udp::endpoint(udp::v4(), localPort));
                hitStun(externalIP, boundPort, socket, io_context);
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
                localColor = setLocalColor();
                std::cout << centerText("press enter to continue", getTerminalWidth());
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                system("clear");
                setRawMode(false);
                clearSocketBuffer(socket);
                std::thread localInput(ingestLocalData, std::ref(localColor), std::ref(moveQueue), std::ref(chatQueue), std::ref(moveQueueMutex), std::ref(chatQueueMutex), std::ref(moveQueueCondVar));
                std::thread externalInput(ingestExternalData, std::ref(localColor), std::ref(socket), std::ref(peer_endpoint), std::ref(moveQueue), std::ref(chatQueue), std::ref(moveQueueMutex), std::ref(chatQueueMutex), std::ref(moveQueueCondVar));
                startOnlineGame(localColor, socket, peer_endpoint);
                localInput.join();
                externalInput.join();
    
    
                // std::thread receiver(receiveMessages, std::ref(socket));

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
                hitStun(externalIP, boundPort, socket, io_context);
                sleep(10);

            }
        }
    }
    setRawMode(false);
	return 0;
}