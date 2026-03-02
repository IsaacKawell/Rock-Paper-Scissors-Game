#ifndef RPS_GAME_H
#define RPS_GAME_H

#include <string>
#include <random>

// -----------------------------
// Enums
// -----------------------------
enum class Move { Rock, Paper, Scissors, Invalid };
enum class Outcome { HumanWin, ComputerWin, Tie };

// -----------------------------
// Utility functions
// -----------------------------
std::string toLower(std::string s);
std::string removeSpaces(std::string s);
std::string moveToString(Move m);
Move parseMove(const std::string& inputRaw);

// -----------------------------
// OOP Players
// -----------------------------
class Player {
public:
    virtual ~Player() = default;
    virtual Move getMove() = 0;
};

class HumanPlayer : public Player {
public:
    Move getMove() override;
};

class ComputerPlayer : public Player {
private:
    std::mt19937 rng;
    std::uniform_int_distribution<int> dist;

public:
    ComputerPlayer();
    Move getMove() override;
};

// -----------------------------
// Game
// -----------------------------
class Game {
private:
    HumanPlayer human;
    ComputerPlayer computer;

    // Current-series stats
    int humanWins = 0;
    int computerWins = 0;
    int ties = 0;

    // Cumulative stats (across series)
    int totalHumanRoundWins = 0;
    int totalComputerRoundWins = 0;
    int totalRoundTies = 0;
    int totalSeriesHuman = 0;
    int totalSeriesComputer = 0;

    // Menu + helpers
    void printMenu() const;
    int readMenuChoice();
    void printHelp() const;

    // Best-of
    int readOddBestOf();
    int winsNeeded(int bestOfN) const;
    void resetSeriesStats();
    void printSeriesScore(int needed) const;

    // Round logic
    Outcome determineOutcome(Move h, Move c) const;
    bool playRound(); // returns false if user quits round/series

    // Modes
    void playBestOfSeries();
    void playEndless();

    // Post-game
    void printCumulativeStats() const;
    bool askPlayAgain(const std::string& prompt);

public:
    void run();
};

#endif