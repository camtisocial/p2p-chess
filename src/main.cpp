#include "main.h"

//BUG: review crashes on checkmate i think
//BUG: stockfish eval numbers seem random
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
bool lastMoved = config.last_moved; // bool for turning on and off highlighting last moved piece
// stockfish
int stockfishDepth = config.eval_depth;
std::string stockfishPath = config.stockfish_path;


//non config vars
std::queue<std::string> moveQueue;
std::queue<std::string> chatQueue;
std::mutex moveQueueMutex;
std::mutex chatQueueMutex;
std::condition_variable moveQueueCondVar;
std::condition_variable chatQueueCondVar;
std::shared_ptr<ChessPiece> lastMovedPiece = nullptr; 
std::vector<std::string> moveHistory;
std::vector<std::string> evalHistory;
int halfMoveClock{};
std::string currentCentipawnEval{};


void startOnlineGame(bool& turnRef, bool localColor, bool& drawOffered, bool& drawAccepted, bool& running, udp::socket& socket, udp::endpoint& peer_endpoint, float& turn, bool& opponentReady, GameBoard& board, char& gameResult, std::string& opening) {
    //0 = white to play, 1 = black to play
    std::string move = "";
    int evalSetting = 0;

        // Print the game board
        if (localColor == 0) {
            printBoardWhite(board.getBoard(), turnRef, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult, evalSetting, currentCentipawnEval, evalHistory);
        } else {
            printBoardBlack(board.getBoard(), turnRef, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult, evalSetting, currentCentipawnEval, evalHistory);
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


        // process moves
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

                    if (board.movePiece(move.substr(4), turnRef, halfMoveClock, turn, lastMovedPiece,
                                        moveHistory, evalHistory, opening, stockfishPath, stockfishDepth, currentCentipawnEval)) {
                        socket.send_to(boost::asio::buffer(move), peer_endpoint);
                        gameResult = board.checkForMateOrDraw(turnRef);
                        turn = turn + 0.5;
                        turnRef = !turnRef;

                    } 
    
                    system("clear");
                    // Print the game board
                    if (localColor == 0) {
                        printBoardWhite(board.getBoard(), turnRef, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult, evalSetting, currentCentipawnEval, evalHistory);
                    } else {
                        printBoardBlack(board.getBoard(), turnRef, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult, evalSetting, currentCentipawnEval, evalHistory);
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
                    if (board.movePiece(opponentMove.substr(4), turnRef, halfMoveClock, turn,
                                        lastMovedPiece, moveHistory, evalHistory, opening, stockfishPath, stockfishDepth, currentCentipawnEval)) {
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
                        printBoardWhite(board.getBoard(), turnRef, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult, evalSetting, currentCentipawnEval, evalHistory);
                    } else {
                        printBoardBlack(board.getBoard(), turnRef, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult, evalSetting, currentCentipawnEval, evalHistory);
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
                    printBoardWhite(board.getBoard(), turnRef, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult, evalSetting, currentCentipawnEval, evalHistory);
                } else {
                    printBoardBlack(board.getBoard(), turnRef, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult, evalSetting, currentCentipawnEval, evalHistory);
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
    int evalSetting = 0;
    bool running = true;
    bool to_play = 0;
    bool moved = false;
    float turn = 1;
    std::string move = "";

    if(!to_play) {
        printBoardWhite(board.getBoard(), to_play, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult, evalSetting, currentCentipawnEval, evalHistory);
    } else {
        printBoardBlack(board.getBoard(), to_play, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult, evalSetting, currentCentipawnEval, evalHistory);
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
            std::cout << "/labels:    turn on labels for ranks/files" << std::endl;
            std::cout << "/moved:    turn on highlighting for last moved piece" << std::endl;
            std::cout << "/quit:    return to main menu" << std::endl;
            std::cout << std::endl;
            std::cout << "check out the README for more information on \nsetting the color pallete or networking options";
            continue;
        } else if (move == "/resign" || move == "resign") {
            gameResult = to_play ? 'w' : 'b';
        } else if (move == "/draw" || move == "draw") {
            gameResult = 'D';

        } else if (move == "/labels") { 
            //cycle back down to zero if labelsOn is 2
            if (labelsOn > 1) {
                labelsOn = 0;
            } else {
                labelsOn = labelsOn+1;
            }
            system("clear");
            if (!to_play) {
                printBoardWhite(board.getBoard(), to_play, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult, evalSetting, currentCentipawnEval, evalHistory);
            } else {
                printBoardBlack(board.getBoard(), to_play, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult, evalSetting, currentCentipawnEval, evalHistory);
            }
            continue;
        } else if (move == "/moved") {
            lastMoved = !lastMoved;
            system("clear");
            if (!to_play) {
                printBoardWhite(board.getBoard(), to_play, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult, evalSetting, currentCentipawnEval, evalHistory);
            } else {
                printBoardBlack(board.getBoard(), to_play, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult, evalSetting, currentCentipawnEval, evalHistory);
            }
            continue;

        } else if (board.movePiece(move, to_play, halfMoveClock, turn, lastMovedPiece, moveHistory,
                                   evalHistory, opening, stockfishPath, stockfishDepth, currentCentipawnEval)) {
            gameResult = board.checkForMateOrDraw(to_play);
            if(to_play) {turn++;}
            to_play = !to_play;
            moved = true;
        } else {
            moved = false;
            system("clear");
            if (!to_play) {
                printBoardWhite(board.getBoard(), to_play, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult, evalSetting, currentCentipawnEval, evalHistory);
            } else {
                printBoardBlack(board.getBoard(), to_play, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult, evalSetting, currentCentipawnEval, evalHistory);
            }
            continue;
        }

        system("clear");
        if(!to_play && gameResult == 'C' && moved) {
            printBoardBlack(board.getBoard(), to_play, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult, evalSetting, currentCentipawnEval, evalHistory);
            sleep(1.5);
            system("clear");
            printBoardWhite(board.getBoard(), to_play, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult, evalSetting, currentCentipawnEval, evalHistory);
        } else if (gameResult == 'C' && moved) {
            printBoardWhite(board.getBoard(), to_play, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult, evalSetting, currentCentipawnEval, evalHistory);
            sleep(1.5);
            system("clear");
            printBoardBlack(board.getBoard(), to_play, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult, evalSetting, currentCentipawnEval, evalHistory);
        }

        if (gameResult != 'C') {
            running = false;
        }
    }
    if (to_play) {
        printBoardBlack(board.getBoard(), to_play, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult, evalSetting, currentCentipawnEval, evalHistory);
    } else {
        printBoardWhite(board.getBoard(), to_play, turn, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, lastMovedPiece, lastMoved, 0, opening, gameResult, evalSetting, currentCentipawnEval, evalHistory);
    }
    announceGameResult(gameResult);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (moveHistory.size() > 0) {
        reviewOrReturn(moveHistory, board.getBoard(), whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, gameResult, opening, 1, currentCentipawnEval, evalHistory, 0);
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

//get evaluation of initial position
  std::string initialFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -";
  std::string initialStockfishCentipawnEval{};

  std::thread evalThread(getStockFishEval, initialFen, stockfishPath, stockfishDepth, std::ref(evalHistory), std::ref(initialStockfishCentipawnEval));
  evalThread.detach();

    std::string externalIP;  
    int boundPort;
    int selected = 0;
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
                    std::string stockfishCentipawnEval{};
                    GameBoard board;
                    float turnNumber{1};
                    udp::socket socket(io_context, udp::endpoint(udp::v4(), localPort));

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
                        reviewOrReturn(moveHistory, board.getBoard(), whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, gameResult, opening, 1, currentCentipawnEval, evalHistory, localColor);
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
                    stockfishCentipawnEval = "";
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
                    std::string stockfishCentipawnEval{};

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
                    std::thread localInput(ingestLocalData, std::ref(currentColor), std::ref(localColor), std::ref(drawOffered), std::ref(drawAccepted), std::ref(drawOfferReceived),
                                           std::ref(socket), std::ref(peer_endpoint), std::ref(moveQueue), std::ref(chatQueue), std::ref(moveQueueMutex), std::ref(chatQueueMutex),
                                           std::ref(moveQueueCondVar), std::ref (running), std::ref(turnNumber), std::ref(labelsOn), std::ref(lastMoved));
                    std::thread externalInput(ingestExternalData, std::ref(localColor), std::ref(drawOffered), std::ref(drawAccepted), std::ref(drawOfferReceived), std::ref(socket),
                                              std::ref(peer_endpoint), std::ref(moveQueue), std::ref(chatQueue), std::ref(moveQueueMutex), std::ref(chatQueueMutex), std::ref(moveQueueCondVar),
                                              std::ref (running), std::ref(turnNumber), std::ref(opponentReady));
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
                        reviewOrReturn(moveHistory, board.getBoard(), whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, gameResult, opening, 1, currentCentipawnEval, evalHistory, localColor);
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
                    stockfishCentipawnEval = "";

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
            } 
        }
    }
    setRawMode(false);
	return 0;
}
