#include "main.h"


void startLocalGame() {
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
    int selected = 0;
    std::vector<std::string> options = {"Host Game", "Join Game", "Local", "Quit"};

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
                int ip = getIP();
                std::cout << "IP: " << ip << std::endl;
            } else if (options[selected] == "Join Game") {
                int ip = getIP();
                std::cout << "IP: " << ip << std::endl;
            } else if (options[selected] == "Local") {
                system("clear");
                setRawMode(false);
                startLocalGame();
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