from collections import defaultdict
import os


source_file_dir = os.path.dirname(os.path.abspath(__file__))


with open(source_file_dir+"/"+"final.txt", "r", encoding="utf8") as f:

    points = 0
    cards = []
    for line in f:
        numbers = line.split(":")[1]
        winningNumbers = [int(s)
                          for s in numbers.split("|")[0].strip().split()]

        ourNumbers = [int(s) for s in numbers.split("|")[1].strip().split()]
        cards.append({"winningNumbers": winningNumbers,
                     "ourNumbers": ourNumbers, "count": 1})
        card_points = 0

        for number in ourNumbers:
            if number in winningNumbers:
                card_points += 1
        if card_points > 0:
            print("Winning numbers: ", winningNumbers, "Our numbers: ",
                  ourNumbers, "Card points: ", card_points, 2**(card_points-1))
            points += 2**(card_points-1)
    print(points)

    for i in range(len(cards)):
        card_points = 0
        for number in cards[i]["ourNumbers"]:
            if number in cards[i]["winningNumbers"]:
                card_points += 1
        # print("card", i, "add cards: ", i+1, "to", i+1+card_points,
        #       "with count", cards[i]["count"])

        for j in range(i+1, min(len(cards), i+1+card_points)):
            cards[j]["count"] += cards[i]["count"]

    # for i in range(len(cards)):
    #     print("card", i+1, "count", cards[i]["count"])

    result = sum(map(lambda card: card["count"], cards))
    print(result)
