#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <ctime>

// Интерфейс IFormattable
class IFormattable {
public:
    virtual std::string format() const = 0;
    virtual ~IFormattable() = default;
};

// Функция prettyPrint
void prettyPrint(const IFormattable& object) {
    std::cout << "Formatted Output:\n" << object.format() << "\n";
}

// Класс Card
class Card {
public:
    enum Suit { SPADES, HEARTS, DIAMONDS, CLUBS };
    static const std::vector<std::string> SUIT_SYMBOLS;
    static const std::vector<std::string> RANKS;

    Card(const std::string& rank, Suit suit) : rank(rank), suit(suit) {}

    std::string toString() const {
        return rank + SUIT_SYMBOLS[suit];
    }

private:
    std::string rank;
    Suit suit;
};

const std::vector<std::string> Card::SUIT_SYMBOLS = { "\u2660", "\u2665", "\u2666", "\u2663" };
const std::vector<std::string> Card::RANKS = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };

// Класс Deck
class Deck {
public:
    Deck(int numDecks = 1) {
        for (int i = 0; i < numDecks; ++i) {
            for (const auto& suit : { Card::SPADES, Card::HEARTS, Card::DIAMONDS, Card::CLUBS }) {
                for (const auto& rank : Card::RANKS) {
                    cards.emplace_back(rank, suit);
                }
            }
        }
        shuffle();
    }

    void shuffle() {
        std::shuffle(cards.begin(), cards.end(), std::mt19937(std::random_device{}()));
    }

    const std::vector<Card>& getCards() const {
        return cards;
    }

private:
    std::vector<Card> cards;
};

// Адаптер класса
class DeckClassAdapter : public Deck, public IFormattable {
public:
    DeckClassAdapter(int numDecks = 1) : Deck(numDecks) {}

    std::string format() const override {
        const auto& cards = getCards();
        std::string result = "Deck contains:\n";
        for (const auto& card : cards) {
            result += card.toString() + " ";
        }
        return result;
    }
};

// Адаптер объекта
class DeckObjectAdapter : public IFormattable {
public:
    DeckObjectAdapter(const Deck& deck) : deck(deck) {}

    std::string format() const override {
        const auto& cards = deck.getCards();
        std::string result = "Deck contains:\n";
        for (const auto& card : cards) {
            result += card.toString() + " ";
        }
        return result;
    }

private:
    const Deck& deck;
};

// Пример работы
int main() {
    std::cout << "Adapter Pattern Example\n";

    // Адаптер класса
    DeckClassAdapter classAdapter(1);
    prettyPrint(classAdapter);

    // Адаптер объекта
    Deck deck(1);
    DeckObjectAdapter objectAdapter(deck);
    prettyPrint(objectAdapter);

    return 0;
}
