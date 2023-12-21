from collections import defaultdict
import os


source_file_dir = os.path.dirname(os.path.abspath(__file__))

cardStrength = {"2": 1, "3": 2, "4": 3, "5": 4, "6": 5,
                "7": 6, "8": 7, "9": 8, "T": 9, "J": 10,
                "Q": 11, "K": 12, "A": 13}


class Hand:
    def __init__(self, cards, bid):
        self.cards = cards
        self.bid = bid
        card_map = defaultdict(int)
        for card in cards:
            card_map[card] += 1

        self.unique_count = [value for _, value in card_map.items()]

        if 5 in self.unique_count:
            self.handStrenth = 7
            return
        if 4 in self.unique_count:
            self.handStrenth = 6
            return
        if 3 in self.unique_count and 2 in self.unique_count:
            self.handStrenth = 5
            return
        if 3 in self.unique_count:
            self.handStrenth = 4
            return
        if self.unique_count.count(2) == 2:
            self.handStrenth = 3
            return
        if self.unique_count.count(2) == 1:
            self.handStrenth = 2
            return
        self.handStrenth = 1

    def __lt__(self, other):
        if self.handStrenth != other.handStrenth:
            return self.handStrenth < other.handStrenth
        for i in range(4):
            if self.cards[i] != other.cards[i]:
                return cardStrength[self.cards[i]] < cardStrength[other.cards[i]]
        return cardStrength[self.cards[4]] < cardStrength[other.cards[4]]


with open(source_file_dir+"/"+"final.txt", "r", encoding="utf8") as f:

    hands = []
    for line in f:
        cards = line.split()[0]
        hands.append(Hand(line.split()[0], int(line.split()[1])))

    hands.sort()
    sumBids = 0
    for i, hand in enumerate(hands):
        print(i, hand.cards, hand.bid)
        sumBids += (i+1)*hand.bid
    print(sumBids)
