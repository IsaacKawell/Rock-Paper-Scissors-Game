#include "rps_game.h"
#include <iostream>
#include <algorithm>
#include <limits>
#include <cctype>

using namespace std;

// =============================
// Utility functions
// =============================
string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(),
              [](unsigned char c) { return static_cast<char>(tolower(c)); });
    return s;
}

string removeSpaces(string s) {
    s.erase(remove_if(s.begin(), s.end(),
                      [](unsigned char c) { return isspace(c); }),
            s.end());
    return s;
}

string moveToString(Move m) {
    switch (m) {
        case Move::Rock:     return "Rock";
        case Move::Paper:    return "Paper";
        case Move::Scissors: return "Scissors";
        default:             return "Invalid";
    }
}

Move parseMove(const string& inputRaw) {
    string s = removeSpaces(toLower(inputRaw));

    if (s == "r" || s == "rock") return Move::Rock;
    if (s == "p" || s == "paper") return Move::Paper;
    if (s == "s" || s == "scissors" || s == "scissor") return Move::Scissors;

    return Move::Invalid;
}

// =============================
// HumanPlayer
// =============================
Move HumanPlayer::getMove() {
    while (true) {
        cout << "Enter your move (R/P/S or Rock/Paper/Scissors). Type Q to quit: ";
        string input;
        getline(cin, input);

        string norm = removeSpaces(toLower(input));
        if (norm == "q" || norm == "quit" || norm == "exit") {
            return Move::Invalid; // signal to quit series/mode
        }

        Move m = parseMove(input);
        if (m != Move::Invalid) return m;

        cout << "Invalid input. Please try again.\n";
    }
}

// =============================
// ComputerPlayer
// =============================
ComputerPlayer::ComputerPlayer()
    : rng(random_device{}()), dist(0, 2) {}

Move ComputerPlayer::getMove() {
    int roll = dist(rng);
    if (roll == 0) return Move::Rock;
    if (roll == 1) return Move::Paper;
    return Move::Scissors;
}

// =============================
// Game: Menu + general helpers
// =============================
void Game::printMenu() const {
    cout << "\n=============================\n";
    cout << "   Rock - Paper - Scissors\n";
    cout << "=============================\n";
    cout << "1) Play best-of-N series (N odd: 3, 5, 7...)\n";
    cout << "2) Endless mode (play until you quit)\n";
    cout << "3) View help / rules\n";
    cout << "4) Exit\n";
    cout << "Choose an option: ";
}

int Game::readMenuChoice() {
    while (true) {
        int choice;
        if (cin >> choice) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (choice >= 1 && choice <= 4) return choice;
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "Invalid choice. Enter 1, 2, 3, or 4: ";
    }
}

void Game::printHelp() const {
    cout << "\n========== HELP / RULES ==========\n";
    cout << "Rules:\n";
    cout << "Rock vs Scissors -> Rock wins\n";
    cout << "Rock vs Paper    -> Paper wins\n";
    cout << "Paper vs Scissors-> Scissors wins\n";
    cout << "Same move        -> Tie\n\n";
    cout << "Input:\n";
    cout << "- Letters: R, P, S\n";
    cout << "- Words: rock, paper, scissors\n";
    cout << "Case-insensitive.\n";
    cout << "Type Q to quit a series/mode.\n";
    cout << "==================================\n";
}

bool Game::askPlayAgain(const string& prompt) {
    while (true) {
        cout << prompt;
        string ans;
        getline(cin, ans);
        ans = removeSpaces(toLower(ans));

        if (ans == "y" || ans == "yes") return true;
        if (ans == "n" || ans == "no") return false;

        cout << "Please enter Y or N.\n";
    }
}

void Game::printCumulativeStats() const {
    cout << "\n=== Cumulative Stats (overall) ===\n";
    cout << "Round wins - You: " << totalHumanRoundWins
         << " | Computer: " << totalComputerRoundWins
         << " | Ties: " << totalRoundTies << "\n";
    cout << "Series wins- You: " << totalSeriesHuman
         << " | Computer: " << totalSeriesComputer << "\n";
    cout << "=================================\n";
}

// =============================
// Game: Best-of helpers
// =============================
int Game::readOddBestOf() {
    while (true) {
        cout << "Enter series length (odd number like 3, 5, 7...): ";
        int n;
        if (cin >> n) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (n >= 1 && (n % 2 == 1)) return n;
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "Invalid number. Must be an odd integer (>=1).\n";
    }
}

int Game::winsNeeded(int bestOfN) const {
    return (bestOfN / 2) + 1; // ceil(N/2) for odd N
}

void Game::resetSeriesStats() {
    humanWins = 0;
    computerWins = 0;
    ties = 0;
}

void Game::printSeriesScore(int needed) const {
    cout << "\n--- Scoreboard (first to " << needed << " wins) ---\n";
    cout << "You: " << humanWins
         << " | Computer: " << computerWins
         << " | Ties: " << ties << "\n";
    cout << "-------------------------------------------\n";
}

// =============================
// Game: Round logic
// =============================
Outcome Game::determineOutcome(Move h, Move c) const {
    if (h == c) return Outcome::Tie;

    if ((h == Move::Rock     && c == Move::Scissors) ||
        (h == Move::Paper    && c == Move::Rock)     ||
        (h == Move::Scissors && c == Move::Paper))
        return Outcome::HumanWin;

    return Outcome::ComputerWin;
}

// Returns false if user quits (Q). Otherwise plays a normal round and returns true.
bool Game::playRound() {
    Move hMove = human.getMove();
    if (hMove == Move::Invalid) {
        cout << "\nYou quit.\n";
        return false;
    }

    Move cMove = computer.getMove();

    cout << "\nYou chose: " << moveToString(hMove) << "\n";
    cout << "Computer chose: " << moveToString(cMove) << "\n";

    Outcome result = determineOutcome(hMove, cMove);

    if (result == Outcome::Tie) {
        cout << "Result: Tie!\n";
        ties++;
        totalRoundTies++;
    } else if (result == Outcome::HumanWin) {
        cout << "Result: You win this round!\n";
        humanWins++;
        totalHumanRoundWins++;
    } else {
        cout << "Result: Computer wins this round.\n";
        computerWins++;
        totalComputerRoundWins++;
    }

    return true;
}

// =============================
// Game Modes
// =============================
void Game::playBestOfSeries() {
    int bestOfN = readOddBestOf();
    int needed = winsNeeded(bestOfN);

    resetSeriesStats();

    cout << "\nStarting best-of-" << bestOfN
         << " series (first to " << needed << " wins).\n";
    cout << "Note: Cumulative stats are preserved across series.\n";

    // Play until someone reaches needed wins OR user quits
    while (humanWins < needed && computerWins < needed) {
        printSeriesScore(needed);
        if (!playRound()) {
            cout << "Series ended early (no winner).\n";
            printCumulativeStats();
            return;
        }
    }

    printSeriesScore(needed);

    if (humanWins > computerWins) {
        cout << "Series Winner: YOU!\n";
        totalSeriesHuman++;
    } else {
        cout << "Series Winner: COMPUTER\n";
        totalSeriesComputer++;
    }

    printCumulativeStats();
}

void Game::playEndless() {
    resetSeriesStats();
    cout << "\nEndless mode started (type Q to quit).\n";

    while (true) {
        cout << "\n--- Endless Score ---\n";
        cout << "You: " << humanWins << " | Computer: " << computerWins << " | Ties: " << ties << "\n";
        cout << "---------------------\n";

        if (!playRound()) break;
    }

    cout << "Endless mode ended.\n";
    printCumulativeStats();
}

// =============================
// Game::run (main loop)
// =============================
void Game::run() {
    cout << "Welcome to Rock - Paper - Scissors!\n";

    while (true) {
        printMenu();
        int choice = readMenuChoice();

        if (choice == 1) {
            playBestOfSeries();
            if (!askPlayAgain("\nPlay another best-of series? (Y/N): ")) {
                cout << "Returning to menu...\n";
            }
        } else if (choice == 2) {
            playEndless();
            if (!askPlayAgain("\nPlay endless mode again? (Y/N): ")) {
                cout << "Returning to menu...\n";
            }
        } else if (choice == 3) {
            printHelp();
        } else { // 4
            cout << "Goodbye!\n";
            break;
        }
    }
}