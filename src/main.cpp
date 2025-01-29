#include "main.h"

    //bugs
    //TODO make highlighted option in display not move so far to the left
    //features
    //TODO add toggle stockfish evaluation
    //TODO add option to play stockfish

//config
Config config = parseConfig("/usr/share/terminalChess/settings.ini");
int localPort = config.local_port;
int peerPort = config.peer_port;
std::string whitePieces = config.white_pieces;
std::string blackPieces = config.black_pieces;
std::string boardColor = config.board_color;
std::string altTextColor = config.alt_text_color;
std::string lastMovedColor = config.last_moved_color;
int labelsOn = config.labels_on; // int for choosing which style of rank/file labels to print
bool lastMoved = config.last_moved; //bool for turning on and off highlighting last moved piece

//non config vars
std::queue<std::string> moveQueue;
std::queue<std::string> chatQueue;
std::mutex moveQueueMutex;
std::mutex chatQueueMutex;
std::condition_variable moveQueueCondVar;
std::condition_variable chatQueueCondVar;
std::shared_ptr<ChessPiece> lastMovedPiece = nullptr; 
std::vector<std::string> moveHistory;
int halfMoveClock{};


void startOnlineGame(bool& turnRef, bool localColor, bool& drawOffered, bool& drawAccepted, bool& running, udp::socket& socket, udp::endpoint& peer_endpoint, float& turn, bool& opponentReady, GameBoard& board, char& gameResult, std::string& opening) {
    //0 = white to play, 1 = black to play
    std::string move = "";

        // Print the game board
        if (localColor == 0) {
            printBoardWhite(board.getBoard(), turnRef, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult);
        } else {
            printBoardBlack(board.getBoard(), turnRef, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult);
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

           if (!opponentReady) {
            std::cout << "Opponent has not joined the game yet" << std::endl;
            continue;
           }

           if ((chatMessage.rfind("[WC]", 0) == 0)) {
               if (localColor == 0) {
                  std::cout << whitePieces << "[You]: " << "\x1B[1;37m"  << chatMessage.substr(4) << "\033[0m"  << std::endl;
               } else {
                  std::cout << whitePieces << "[Opponent]: " << "\x1B[1;37m"  << chatMessage.substr(4) << "\033[0m"  << std::endl;
               }
           } else if (chatMessage.rfind("[BC]", 0) == 0) {
               if (localColor == 1) {
                  std::cout << blackPieces << "[You]: " << "\x1B[1;37m"  << chatMessage.substr(4) << "\033[0m"  << std::endl;
               } else {
                  std::cout << blackPieces << "[Opponent]: " << "\x1B[1;37m"  << chatMessage.substr(4) << "\033[0m"  << std::endl;
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

                    if (!opponentReady) {
                     std::cout << "Opponent has not joined the game yet" << std::endl;
                     continue;
                    }

                    if (board.movePiece(move.substr(4), turnRef, halfMoveClock, turn, lastMovedPiece, moveHistory, opening)) {
                        socket.send_to(boost::asio::buffer(move), peer_endpoint);
                        gameResult = board.checkForMateOrDraw(turnRef);
                        turn = turn + 0.5;
                        turnRef = !turnRef;

                    } 
    
                    system("clear");
                    // Print the game board
                    if (localColor == 0) {
                        printBoardWhite(board.getBoard(), turnRef, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult);
                    } else {
                        printBoardBlack(board.getBoard(), turnRef, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult);
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
                    if (board.movePiece(opponentMove.substr(4), turnRef, halfMoveClock, turn, lastMovedPiece, moveHistory, opening)) {
                        gameResult = board.checkForMateOrDraw(turnRef);
                        turn = turn + 0.5;
                        turnRef = !turnRef;
                    } else {
                        std::cout << "Opponent made an invalid move: " << opponentMove << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(2));
                    }

                    system("clear");
                    // Print the game board
                    if (localColor == 0) {
                        printBoardWhite(board.getBoard(), turnRef, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult);
                    } else {
                        printBoardBlack(board.getBoard(), turnRef, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult);
                    }

                }

            }
        }
        if (gameResult != 'C') {
            for (int i = 0; i < 5; i++) {
                socket.send_to(boost::asio::buffer("TERMINATE"), peer_endpoint);
            }
            running = false;
        }

        std::unique_lock<std::mutex> boardLock(moveQueueMutex);
            if (reprint) {
                system("clear");
                // Print the game board
                if (localColor == 0) {
                    printBoardWhite(board.getBoard(), turnRef, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult);
                } else {
                    printBoardBlack(board.getBoard(), turnRef, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult);
                }
                reprint = false;
        }

    }

    announceGameResult(gameResult);

}

void startLocalGame(std::string& opening) {
    GameBoard board;
    //0 = white to play, 1 = black to play
    char gameResult{'C'};
    bool running = true;
    bool to_play = 0;
    bool moved = false;
    float turn = 1;
    std::string move = "";

    if(!to_play) {
        printBoardWhite(board.getBoard(), to_play, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult);
    } else {
        printBoardBlack(board.getBoard(), to_play, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult);
    }

    while(running) {
        std::cout << "\n\n";
        std::cout << "   Enter move: ";
        std::cout.flush();
        std::getline(std::cin,  move);

        if (move == "/quit" || move == "q" || move == "/q") {
            gameResult = 'q';
        }else if (move == "/help" || move == "/h") {
            std::cout << std::endl;
            std::cout << "/resign:  resign the game " << std::endl;
            std::cout << "/draw:    draw the game" << std::endl;
            std::cout << "/quit:    return to main menu" << std::endl;
            std::cout << std::endl;
            std::cout << "check out the README for more information on \nsetting the color pallete or networking options";
            continue;
        } else if (move == "/resign" || move == "resign") {
            gameResult = to_play ? 'w' : 'b';
        } else if (move == "/draw" || move == "draw") {
            gameResult = 'D';

        } else if (move == "/labels") { 

        } else if (move == "/moved") {

        } else if (board.movePiece(move, to_play, halfMoveClock, turn, lastMovedPiece, moveHistory, opening)) {
            gameResult = board.checkForMateOrDraw(to_play);
            if(to_play) {turn++;}
            to_play = !to_play;
            moved = true;
        } else {
            moved = false;
            system("clear");
            if (!to_play) {
                printBoardWhite(board.getBoard(), to_play, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult);
            } else {
                printBoardBlack(board.getBoard(), to_play, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult);
            }
            continue;
        }

        system("clear");
        if(!to_play && gameResult == 'C' && moved) {
            printBoardBlack(board.getBoard(), to_play, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult);
            sleep(1.5);
            system("clear");
            printBoardWhite(board.getBoard(), to_play, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult);
        } else if (gameResult == 'C' && moved) {
            printBoardWhite(board.getBoard(), to_play, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult);
            sleep(1.5);
            system("clear");
            printBoardBlack(board.getBoard(), to_play, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult);
        }

        if (gameResult != 'C') {
            running = false;
        }
    }
    if (to_play) {
        printBoardBlack(board.getBoard(), to_play, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult);
    } else {
        printBoardWhite(board.getBoard(), to_play, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult);
    }
    announceGameResult(gameResult);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (moveHistory.size() > 0) {
        reviewOrReturn(moveHistory, board.getBoard(), whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, gameResult , opening);
    }

}


int main(int argc, char** argv) {

//command line argument to see available colors
    if (argc > 1) {
        if (std::string(argv[1]) == "--colors" || std::string(argv[1]) == "-c") {
            system("clear");
            seeColorOptions();
            return 0;
        }
    }


    std::string externalIP;  
    int boundPort;
    int selected = 0;
    // std::vector<std::string> options = {"Online", "Local", "LAN", "Quit", "test"};
    std::vector<std::string> options = {"Online", "Local", "LAN", "Quit"};

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
                    boost::asio::io_context io_context;
                    bool running{true};
                    bool localColor{};
                    bool opponentReady{};
                    bool drawOffered{};
                    bool drawAccepted{};
                    bool drawOfferReceived{};
                    char gameResult{'C'};
                    std::string opening = "";
                    GameBoard board;
                    float turnNumber{1};
                    udp::socket socket(io_context, udp::endpoint(udp::v4(), 12345));

                    system("clear");
                    displayMenu({}, selected);

                    //set up connection
                    hitStun(externalIP, boundPort, socket, io_context);
                    std::string peer_ip = setPeerIP();
                    punchHole(peer_ip, peerPort, socket, io_context);
                    udp::endpoint peer_endpoint(boost::asio::ip::make_address(peer_ip), peerPort);

                    // setting up game
                    bool currentColor = 0;
                    std::thread colorListener(listenForColor, std::ref(socket), std::ref(peer_endpoint), std::ref(localColor), std::ref(io_context));
                    setLocalColor(socket, peer_endpoint, localColor);
                    if (colorListener.joinable()) {
                        colorListener.join();
                    } else {
                        std::cout << "colorListener not joinable" << std::endl;
                    }
                    std::cout << std::endl;

                    if (playerPickedColor && !localPickedColor) {
                        std::cout << centerText("Opponent chose ", getTerminalWidth()-4) << (localColor ? "white" : "black") << std::endl;
                        std::cout << std::endl;
                    }
                    std::cout << centerText("You play ", getTerminalWidth()-4) << (localColor ? "black" : "white") << std::endl;
                    std::cout << centerText("type /h for help", getTerminalWidth()) << std::endl;
                    std::cout << centerText("press enter to continue", getTerminalWidth());
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    for (int i = 0; i < 5; ++i) {
                        socket.send_to(boost::asio::buffer("READY"), peer_endpoint);
                    }

                    system("clear");
                    setRawMode(false);
                    clearSocketBuffer(socket);
                    std::thread localInput(ingestLocalData, std::ref(currentColor), std::ref(localColor), std::ref(drawOffered), std::ref(drawAccepted), std::ref(drawOfferReceived),  std::ref(socket), std::ref(peer_endpoint), std::ref(moveQueue), std::ref(chatQueue), std::ref(moveQueueMutex), std::ref(chatQueueMutex), std::ref(moveQueueCondVar), std::ref (running), std::ref(turnNumber), std::ref(labelsOn), std::ref(lastMoved));
                    std::thread externalInput(ingestExternalData, std::ref(localColor), std::ref(drawOffered), std::ref(drawAccepted), std::ref(drawOfferReceived), std::ref(socket), std::ref(peer_endpoint), std::ref(moveQueue), std::ref(chatQueue), std::ref(moveQueueMutex), std::ref(chatQueueMutex), std::ref(moveQueueCondVar), std::ref (running), std::ref(turnNumber), std::ref(opponentReady));
                    startOnlineGame(currentColor, localColor, drawOffered, drawAccepted, running, socket, peer_endpoint, turnNumber, opponentReady, board, gameResult, opening);
                    io_context.stop();
                    socket.close();

                    //clean up threads
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

                    //review game this has to go after the threads are joined
                    if (moveHistory.size() > 0) {
                        reviewOrReturn(moveHistory, board.getBoard(), whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, gameResult, opening);
                    }   

                    //empty queues
                    std::queue<std::string>().swap(moveQueue);
                    std::queue<std::string>().swap(chatQueue);

                    //reset variables/sockets
                    setRawMode(true);
                    io_context.restart();
                    socket = udp::socket(io_context, udp::endpoint(udp::v4(), localPort));
                    
                    running = false;
                    drawOffered = false;
                    drawAccepted = false;
                    drawOfferReceived = false;
                    keepBroadcasting = true;
                    playerPickedColor = false;
                    localPickedColor = false;
                    turnNumber = 1;
                    opponentReady = false;
                    lastMovedPiece = nullptr;
                    halfMoveClock = 0;
                    gameResult = 'C';
                    opening = "";
                    board = GameBoard();
                    moveHistory.clear();

                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ** LAN ** @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            } else if (options[selected] == "LAN") {
                try {
                    setRawMode(false);
                    bool running{true};
                    bool localColor{};
                    bool opponentReady{};
                    bool drawOffered{};
                    bool drawAccepted{};
                    bool drawOfferReceived{};
                    char gameResult{'C'};
                    std::string opening = "";
                    GameBoard board;
                    float turnNumber{1};
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
                    bool currentColor = 0;
                    std::thread colorListener(listenForColor, std::ref(socket), std::ref(peer_endpoint), std::ref(localColor), std::ref(io_context));
                    setLocalColor(socket, peer_endpoint, localColor);
                    if (colorListener.joinable()) {
                        colorListener.join();
                    } else {
                        std::cout << "colorListener not joinable" << std::endl;
                    }
                    std::cout << std::endl;

                    if (playerPickedColor && !localPickedColor) {
                        std::cout << centerText("Opponent chose ", getTerminalWidth()-4) << (localColor ? "white" : "black") << std::endl;
                        std::cout << std::endl;
                    }
                    std::cout << centerText("You play ", getTerminalWidth()-4) << (localColor ? "black" : "white") << std::endl;
                    std::cout << centerText("  type /h for help", getTerminalWidth()) << std::endl;
                    std::cout << centerText("  press enter to continue", getTerminalWidth());
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    socket.send_to(boost::asio::buffer("READY"), peer_endpoint);

                    system("clear");
                    setRawMode(false);
                    clearSocketBuffer(socket);
                    std::thread localInput(ingestLocalData, std::ref(currentColor), std::ref(localColor), std::ref(drawOffered), std::ref(drawAccepted), std::ref(drawOfferReceived),  std::ref(socket), std::ref(peer_endpoint), std::ref(moveQueue), std::ref(chatQueue), std::ref(moveQueueMutex), std::ref(chatQueueMutex), std::ref(moveQueueCondVar), std::ref (running), std::ref(turnNumber), std::ref(labelsOn), std::ref(lastMoved));
                    std::thread externalInput(ingestExternalData, std::ref(localColor), std::ref(drawOffered), std::ref(drawAccepted), std::ref(drawOfferReceived), std::ref(socket), std::ref(peer_endpoint), std::ref(moveQueue), std::ref(chatQueue), std::ref(moveQueueMutex), std::ref(chatQueueMutex), std::ref(moveQueueCondVar), std::ref (running), std::ref(turnNumber), std::ref(opponentReady));
                    startOnlineGame(currentColor, localColor, drawOffered, drawAccepted, running, socket, peer_endpoint, turnNumber, opponentReady, board, gameResult, opening);
                    io_context.stop();
                    socket.close();


                    //clean up threads
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

                    //review game
                    if (moveHistory.size() > 0) {
                        reviewOrReturn(moveHistory, board.getBoard(), whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, gameResult, opening);
                    }

                    //empty queues
                    std::queue<std::string>().swap(moveQueue);
                    std::queue<std::string>().swap(chatQueue);

                    //reset variables/sockets
                    setRawMode(true);
                    io_context.restart();
                    socket = udp::socket(io_context, udp::endpoint(udp::v4(), localPort));
                    running = false;
                    drawOffered = false;
                    drawAccepted = false;
                    drawOfferReceived = false;
                    keepBroadcasting = true;
                    playerPickedColor = false;
                    localPickedColor = false;
                    turnNumber = 1;
                    opponentReady = false;
                    lastMovedPiece = nullptr;
                    halfMoveClock = 0;
                    gameResult = 'C';
                    opening = "";
                    board = GameBoard();
                    moveHistory.clear();

                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ** LOCAL ** @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            } else if (options[selected] == "Local") {
                system("clear");
                std::string opening{};
                setRawMode(false);
                startLocalGame(opening);
                setRawMode(true);
                halfMoveClock = 0;
                moveHistory.clear();
                opening = "";
            } else if (options[selected] == "test") {
                GameBoard board;
                std::string opening{};
                system("clear");
                std::cout << "Fen for hungarian opening:rnbqkbnr/pppppppp/8/8/8/6P1/PPPPPP1P/RNBQKBNR b KQkq -" << std::endl;
                std::cout << "output for openingsMap: " << board.identifyOpening("rnbqkbnr/pppppppp/8/8/8/6P1/PPPPPP1P/RNBQKBNR b KQkq -", opening) << std::endl;
                std::cout << std::endl;
                std::cout << std::endl;
                sleep(4);
            //     std::vector<std::string> tmpVec{
            //                                    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQqk - 0 1",
            //                                    "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQqk e3 0 1",
            //                                    "rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w KQqk e6 0 1",
            //                                    "rnbqkbnr/pppp1ppp/8/4p3/4P3/2N5/PPPP1PPP/R1BQKBNR b KQqk - 1 2",
            //                                    "rnbqkb1r/pppp1ppp/5n2/4p3/4P3/2N5/PPPP1PPP/R1BQKBNR w KQqk - 2 2",
            //                                    "rnbqkb1r/pppp1ppp/5n2/4p3/4P3/2N2N2/PPPP1PPP/R1BQKB1R b KQqk - 3 3",
            //                                    "r1bqkb1r/pppp1ppp/2n2n2/4p3/4P3/2N2N2/PPPP1PPP/R1BQKB1R w KQqk - 4 3",
            //                                    "r1bqkb1r/pppp1ppp/2n2n2/4p3/4P3/2NP1N2/PPP2PPP/R1BQKB1R b KQqk - 0 4",
            //                                    "r1bqk2r/pppp1ppp/2n2n2/2b1p3/4P3/2NP1N2/PPP2PPP/R1BQKB1R w KQqk - 1 4",
            //                                    "r1bqk2r/pppp1ppp/2n2n2/2b1p1B1/4P3/2NP1N2/PPP2PPP/R2QKB1R b KQqk - 2 5",
            //                                    "r1bq1rk1/pppp1ppp/2n2n2/2b1p1B1/4P3/2NP1N2/PPP2PPP/R2QKB1R w KQ - 3 5",
            //                                    "r1bq1rk1/pppp1ppp/2n2n2/2b1p1B1/4P3/2NP1N2/PPP1BPPP/R2QK2R b KQ - 4 6",
            //                                    "r1bq1rk1/ppp2ppp/2np1n2/2b1p1B1/4P3/2NP1N2/PPP1BPPP/R2QK2R w KQ - 0 6",
            //                                    "r1bq1rk1/ppp2ppp/2np1n2/2b1p1B1/4P3/2NP1N2/PPP1BPPP/R2Q1RK1 b - - 1 7",
            //                                    "r2q1rk1/ppp2ppp/2npbn2/2b1p1B1/4P3/2NP1N2/PPP1BPPP/R2Q1RK1 w - - 2 7",
            //                                    "r2q1rk1/ppp2ppp/2npbn2/2b1p1B1/4P3/2NP1N2/PPP1BPPP/R2QR1K1 b - - 3 8",
            //                                    "r2q1rk1/1pp2ppp/p1npbn2/2b1p1B1/4P3/2NP1N2/PPP1BPPP/R2QR1K1 w - - 0 8",
            //                                    "r2q1rk1/1pp2ppp/p1npbn2/2b1p1B1/4P3/2NP1N2/PPPQBPPP/R3R1K1 b - - 1 9",
            //                                    "r2q1rk1/2p2ppp/p1npbn2/1pb1p1B1/4P3/2NP1N2/PPPQBPPP/R3R1K1 w - b6 0 9",
            //                                    "r2q1rk1/2p2ppp/p1npbn2/1pb1p1B1/4P3/P1NP1N2/1PPQBPPP/R3R1K1 b - - 0 10",
            //                                    "1r1q1rk1/2p2ppp/p1npbn2/1pb1p1B1/4P3/P1NP1N2/1PPQBPPP/R3R1K1 w - - 1 10",
            //                                    "1r1q1rk1/2p2ppp/p1npbn2/1pb1p1B1/1P2P3/P1NP1N2/2PQBPPP/R3R1K1 b - b3 0 11",
            //                                    "1r1q1rk1/2p2ppp/pbnpbn2/1p2p1B1/1P2P3/P1NP1N2/2PQBPPP/R3R1K1 w - - 1 11",
            //                                    "1r1q1rk1/2p2ppp/pbnpbn2/1p2p1B1/PP2P3/2NP1N2/2PQBPPP/R3R1K1 b - - 0 12",
            //                                    "1r1qr1k1/2p2ppp/pbnpbn2/1p2p1B1/PP2P3/2NP1N2/2PQBPPP/R3R1K1 w - - 1 12",
            //                                    "1r1qr1k1/2p2ppp/pbnpbn2/1P2p1B1/1P2P3/2NP1N2/2PQBPPP/R3R1K1 b - - 0 13",
            //                                    "1r1qr1k1/2p2ppp/1bnpbn2/1p2p1B1/1P2P3/2NP1N2/2PQBPPP/R3R1K1 w - - 0 13",
            //                                    "1r1qr1k1/2p2ppp/1bnpbn2/1N2p1B1/1P2P3/3P1N2/2PQBPPP/R3R1K1 b - - 0 14",
            //                                    "1r1qr1k1/2p2ppp/1b1pbn2/1N2p1B1/1P1nP3/3P1N2/2PQBPPP/R3R1K1 w - - 1 14",
            //                                    "1r1qr1k1/2p2ppp/1b1pbn2/1N2p3/1P1nPB2/3P1N2/2PQBPPP/R3R1K1 b - - 2 15",
            //                                    "3qr1k1/1rp2ppp/1b1pbn2/1N2p3/1P1nPB2/3P1N2/2PQBPPP/R3R1K1 w - - 3 15",
            //                                    "3qr1k1/1rp2ppp/1b1pbn2/1N2p1B1/1P1nP3/3P1N2/2PQBPPP/R3R1K1 b - - 4 16",
            //                                    "1r1qr1k1/2p2ppp/1b1pbn2/1N2p1B1/1P1nP3/3P1N2/2PQBPPP/R3R1K1 w - - 5 16",
            //                                    "1r1qr1k1/2p2ppp/1b1pbB2/1N2p3/1P1nP3/3P1N2/2PQBPPP/R3R1K1 b - - 0 17",
            //                                    "1r2r1k1/2p2ppp/1b1pbq2/1N2p3/1P1nP3/3P1N2/2PQBPPP/R3R1K1 w - - 0 17",
            //                                    "1r2r1k1/2p2ppp/1b1pbq2/4p3/1P1NP3/3P1N2/2PQBPPP/R3R1K1 b - - 0 18",
            //                                    "1r2r1k1/2p2ppp/1b1pbq2/8/1P1pP3/3P1N2/2PQBPPP/R3R1K1 w - d5 0 18",
            //                                    "1r2r1k1/2p2ppp/1b1pbq2/8/1P1pP3/3P1N2/2PQBPPP/R1R3K1 b - - 1 19",
            //                                    "1r2r1k1/5ppp/1b1pbq2/2p5/1P1pP3/3P1N2/2PQBPPP/R1R3K1 w - c6 0 19",
            //                                    "1r2r1k1/5ppp/1b1pbq2/2P5/3pP3/3P1N2/2PQBPPP/R1R3K1 b - c4 0 20",
            //                                    "1r2r1k1/5ppp/1b2bq2/2p5/3pP3/3P1N2/2PQBPPP/R1R3K1 w - - 0 20",
            //                                    "1r2r1k1/5ppp/1b2bq2/2p1P3/3p4/3P1N2/2PQBPPP/R1R3K1 b - e4 0 21",
            //                                    "1r2r1k1/5ppp/1b2b3/2p1Pq2/3p4/3P1N2/2PQBPPP/R1R3K1 w - - 1 21",
            //                                    "1r2r1k1/5ppp/1b2b3/2p1Pq2/3p4/2PP1N2/3QBPPP/R1R3K1 b - - 0 22",
            //                                    "1r2r1k1/5ppp/1b2b3/2p1Pq2/8/2pP1N2/3QBPPP/R1R3K1 w - c4 0 22",
            //                                    "1r2r1k1/5ppp/1b2b3/2p1Pq2/8/2QP1N2/4BPPP/R1R3K1 b - - 0 23",
            //                                    "1r2r1k1/5pp1/1b2b3/2p1Pq1p/8/2QP1N2/4BPPP/R1R3K1 w - h6 0 23",
            //                                    "1r2r1k1/5pp1/1b2b3/2p1Pq1p/3P4/2Q2N2/4BPPP/R1R3K1 b - - 0 24",
            //                                    "1r2r1k1/5pp1/1b2b3/4Pq1p/3p4/2Q2N2/4BPPP/R1R3K1 w - - 0 24",
            //                                    "1r2r1k1/5pp1/1b2b3/4Pq1p/3N4/2Q5/4BPPP/R1R3K1 b - - 0 25",
            //                                    "1r2r1k1/5pp1/4b3/4Pq1p/3b4/2Q5/4BPPP/R1R3K1 w - - 0 25",
            //                                    "1r2r1k1/5pp1/4b3/4Pq1p/3Q4/8/4BPPP/R1R3K1 b - - 0 26",
            //                                    "3rr1k1/5pp1/4b3/4Pq1p/3Q4/8/4BPPP/R1R3K1 w - - 1 26",
            //                                    "3rr1k1/5pp1/4b3/4Pq1p/8/4Q3/4BPPP/R1R3K1 b - - 2 27",
            //                                    "3rr1k1/5pp1/4b3/4Pq2/7p/4Q3/4BPPP/R1R3K1 w - h5 0 27",
            //                                    "3rr1k1/5pp1/4b3/4Pq2/7p/4Q2P/4BPP1/R1R3K1 b - - 0 28",
            //                                    "3rr1k1/5pp1/4b1q1/4P3/7p/4Q2P/4BPP1/R1R3K1 w - - 1 28",
            //                                    "3rr1k1/5pp1/4b1q1/4P3/7p/4QB1P/5PP1/R1R3K1 b - - 2 29",
            //                                    "3rr1k1/5pp1/6q1/4P3/7p/4QB1b/5PP1/R1R3K1 w - - 0 29",
            //                                    "3rr1k1/5pp1/2B3q1/4P3/7p/4Q2b/5PP1/R1R3K1 b - - 1 30",
            //                                    "4r1k1/5pp1/2B3q1/4P3/7p/3rQ2b/5PP1/R1R3K1 w - - 2 30",
            //                                    "4r1k1/5pp1/2B3q1/4P3/4Q2p/3r3b/5PP1/R1R3K1 b - - 3 31",
            //                                    "4r1k1/5pp1/2B5/4P3/4Q2p/3r3b/5Pq1/R1R3K1 w - - 0 31",
            //                                    "4r1k1/5pp1/2B5/4P3/7p/3r3b/5PQ1/R1R3K1 b - - 0 32",
            //                                    "4r1k1/5pp1/2B5/4P3/7p/3r4/5Pb1/R1R3K1 w - - 0 32",
            //                                    "4B1k1/5pp1/8/4P3/7p/3r4/5Pb1/R1R3K1 b - - 0 33",
            //                                     };
            // reviewGame(tmpVec, board.getBoard(), whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, opening);
            }
        }
    }
    setRawMode(false);
	return 0;
}