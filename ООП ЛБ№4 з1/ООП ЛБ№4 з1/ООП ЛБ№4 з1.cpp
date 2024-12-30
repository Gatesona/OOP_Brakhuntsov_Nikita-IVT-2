#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <ctime>
#include <stdexcept>

class Card {
public:
    enum Suit { SPADES, HEARTS, DIAMONDS, CLUBS };
    static const std::vector<std::string> SUIT_SYMBOLS;
    static const std::vector<std::string> RANKS;

    Card(const std::string& rank, Suit suit) : rank(rank), suit(suit) {}

    int value() const {
        if (rank == "J" || rank == "Q" || rank == "K") {
            return 10;
        }
        else if (rank == "A") {
            return 11;
        }
        else {
            return std::stoi(rank);
        }
    }

    std::string toString() const {
        return rank + SUIT_SYMBOLS[suit];
    }

    friend std::ostream& operator<<(std::ostream& os, const Card& card) {
        os << card.toString();
        return os;
    }

private:
    std::string rank;
    Suit suit;
};

const std::vector<std::string> Card::SUIT_SYMBOLS = { "\u2660", "\u2665", "\u2666", "\u2663" };
const std::vector<std::string> Card::RANKS = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };

class Deck {
public:
    Deck(int numDecks = 4, bool isShort = false) {
        for (int i = 0; i < numDecks; ++i) {
            for (const auto& suit : { Card::SPADES, Card::HEARTS, Card::DIAMONDS, Card::CLUBS }) {
                for (size_t j = isShort ? 4 : 0; j < Card::RANKS.size(); ++j) {
                    cards.emplace_back(Card::RANKS[j], suit);
                }
            }
        }
        shuffle();
    }

    Card deal() {
        if (cards.empty()) {
            throw std::runtime_error("Deck is empty!");
        }
        Card dealtCard = cards.back();
        cards.pop_back();
        return dealtCard;
    }

    void shuffle() {
        std::shuffle(cards.begin(), cards.end(), std::mt19937(std::random_device{}()));
    }

    size_t size() const {
        return cards.size();
    }

private:
    std::vector<Card> cards;
};

class Hand {
public:
    void addCard(const Card& card) {
        cards.push_back(card);
    }

    Card removeCard(size_t index) {
        if (index >= cards.size()) {
            throw std::out_of_range("Invalid card index!");
        }
        Card card = cards[index];
        cards.erase(cards.begin() + index);
        return card;
    }

    int value() const {
        int total = 0;
        int aces = 0;
        for (const auto& card : cards) {
            total += card.value();
            if (card.toString()[0] == 'A') {
                ++aces;
            }
        }
        while (total > 21 && aces > 0) {
            total -= 10;
            --aces;
        }
        return total;
    }

    bool isBlackjack() const {
        return cards.size() == 2 && value() == 21;
    }

    bool isBust() const {
        return value() > 21;
    }

    std::string toString() const {
        std::string result;
        for (const auto& card : cards) {
            result += card.toString() + " ";
        }
        return result;
    }

    void clear() {
        cards.clear();
    }

private:
    std::vector<Card> cards;
};

class Dealer {
public:
    void play(Deck& deck) {
        while (hand.value() < 17) {
            hand.addCard(deck.deal());
        }
    }

    Hand hand;
};

class Player {
public:
    Hand hand;
    Hand splitHand;
    int balance = 10000;
};

class Game {
public:
    Game(int numDecks = 4, bool isShort = false) : deck(numDecks, isShort), player(), dealer() {}

    void play() {
        std::cout << "Welcome to BlackJack with Split Rule!\n";

        while (true) {
            int bet;
            std::cout << "Your balance: " << player.balance << ". Your bet? ";
            std::cin >> bet;

            if (bet <= 0 || bet > player.balance) {
                std::cout << "Invalid bet.\n";
                continue;
            }

            player.hand.clear();
            player.splitHand.clear();
            dealer.hand.clear();

            player.hand.addCard(deck.deal());
            player.hand.addCard(deck.deal());
            dealer.hand.addCard(deck.deal());
            dealer.hand.addCard(deck.deal());

            std::cout << "Dealer: " << dealer.hand.toString().substr(0, 3) << " ??\n";
            std::cout << "You: " << player.hand.toString() << "\n";

            bool splitPossible = player.hand.toString()[0] == player.hand.toString()[3];
            bool splitUsed = false;

            while (true) {
                if (player.hand.isBlackjack()) {
                    std::cout << "Congratulations, you have Blackjack!\n";
                    break;
                }

                std::cout << "1. Stand\n2. Hit\n";
                if (splitPossible) {
                    std::cout << "3. Split\n";
                }
                std::cout << "Your choice: ";
                int choice;
                std::cin >> choice;

                if (choice == 1) {
                    break;
                }
                else if (choice == 2) {
                    player.hand.addCard(deck.deal());
                    std::cout << "You: " << player.hand.toString() << "\n";
                    if (player.hand.isBust()) {
                        std::cout << "Bust! You lose.\n";
                        player.balance -= bet;
                        break;
                    }
                }
                else if (choice == 3 && splitPossible) {
                    splitUsed = true;
                    player.splitHand.addCard(player.hand.removeCard(0)); // Перенос первой карты в сплит-руку
                    player.hand.addCard(deck.deal());
                    player.splitHand.addCard(deck.deal());
                    splitPossible = false;

                    std::cout << "Split hands:\n";
                    std::cout << "Hand 1: " << player.hand.toString() << "\n";
                    std::cout << "Hand 2: " << player.splitHand.toString() << "\n";
                }
                else {
                    std::cout << "Invalid choice.\n";
                }
            }

            if (!player.hand.isBust()) {
                dealer.play(deck);
                std::cout << "Dealer: " << dealer.hand.toString() << "\n";

                int winCount = 0;
                int loseCount = 0;

                auto evaluateHand = [&](const Hand& hand) {
                    if (dealer.hand.isBust() || hand.value() > dealer.hand.value()) {
                        ++winCount;
                    }
                    else if (hand.value() < dealer.hand.value()) {
                        ++loseCount;
                    }
                    };

                evaluateHand(player.hand);
                if (splitUsed) {
                    evaluateHand(player.splitHand);
                }

                if (winCount == 2) {
                    std::cout << "Both hands win!\n";
                    player.balance += bet * 2;
                }
                else if (winCount == 1 && loseCount == 1) {
                    std::cout << "One hand wins, one hand loses.\n";
                }
                else {
                    std::cout << "Both hands lose.\n";
                    player.balance -= bet;
                }
            }

            std::cout << "Your balance: " << player.balance << "\n";
            if (player.balance <= 0) {
                std::cout << "You are out of money. Game over.\n";
                break;
            }

            std::cout << "Play again? (yes/no): ";
            std::string playAgain;
            std::cin >> playAgain;
            if (playAgain != "yes") {
                break;
            }
        }
    }

private:
    Deck deck;
    Player player;
    Dealer dealer;
};

int main() {
    Game game;
    game.play();
    return 0;
}