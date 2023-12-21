import os
source_file_dir = os.path.dirname(os.path.abspath(__file__))
maxAllowed = {'red': 12, 'blue': 14, 'green': 13}
with open(source_file_dir+"/"+"a_final.txt", "r", encoding="utf8") as f:
    result = 0
    resultPower = 0
    for line in f:
        parts = line.split(":")
        print(parts)
        gameID = parts[0].split(" ")[1]

        rounds = parts[1].split(";")
        allRoundsValid = True

        maxNeeded = {'red': 1, 'blue': 1, 'green': 1}
        for gameround in rounds:

            pieces = gameround.split(",")
            maxCubes = {}

            for piece in pieces:
                cube = piece.strip().split(" ")

                if cube[1] in maxCubes:
                    maxCubes[cube[1]] += int(cube[0])
                else:
                    maxCubes[cube[1]] = int(cube[0])

            for color, count in maxCubes.items():
                if count > maxAllowed[color]:
                    allRoundsValid = False
                maxNeeded[color] = max(maxNeeded[color], count)
        power = maxNeeded['red'] * maxNeeded['blue'] * maxNeeded['green']
        print(gameID, "Power", power)
        resultPower += power

        print(gameID, allRoundsValid)
        if allRoundsValid:
            result += int(gameID)

    print("Result", result)
    print("ResultPower", resultPower)
