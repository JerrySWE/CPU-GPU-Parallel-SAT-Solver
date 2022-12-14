import sys
import getopt
import random

clauseSize = 3

def verifyTestCaseIntegrity(vNum, cNum, testCasePath):
    f = open(testCasePath, "r")

    lines = f.readlines()

    vCount = int(lines[0].split()[0])
    cCount = int(lines[0].split()[1])

    if (vCount != vNum):
        print("variable count does not match!")
        return
    if (cCount != cNum):
        print("clause count does not match!")
        return
    
    for i in range(1, len(lines)):
        clause = lines[i].split(" ")
        c1 = abs(int(clause[0]))
        c2 = abs(int(clause[1]))
        c3 = abs(int(clause[2]))
        if (c1 == 0 or c2 == 0 or c3 == 0):
            print("Variable 0 detected! Not legal!!!")
        if (c1 > vCount or c2 > vCount or c3 > vCount):
            print("Variable out of range!!!")
    print("Generated Test Case is Legal!")

def generateTestCase(vNum, cNum, outputPath):
    if (outputPath[-4:] != ".txt"):
        outputPath += ".txt"

    f = open(outputPath, "w")

    f.write(str(vNum) + " " + str(cNum) + "\n")

    for i in range(cNum):
        clause = ""

        for j in range(clauseSize-1):
            sign = random.choice([-1, 1])
            randVar = random.randint(1, vNum)
            clause += str(randVar * sign) + " "

        sign = random.choice([-1, 1])
        randVar = random.randint(1, vNum)
        clause += str(randVar * sign) + "\n"

        f.write(clause)
    
    f.close()

    verifyTestCaseIntegrity(vNum, cNum, outputPath)


def main(argv):
    variableNum = 0
    clauseNum = 0
    outputFilePath = ""

    try:
        opts, args = getopt.getopt(argv, "hv:c:o:")
    except getopt.GetoptError:
        print("generator.py -v <variableNumber> -c <clauseNumber> -o <outputFilePath>")
        sys.exit(2)
    
    for opt, arg in opts:
        if opt == "-h":
            print("generator.py -v <variableNumber> -c <clauseNumber> -o <outputFilePath>")
            sys.exit()
        elif opt == "-v":
            variableNum = int(arg)
        elif opt == "-c":
            clauseNum = int(arg)
        elif opt == "-o":
            outputFilePath = arg
    
    if (len(outputFilePath) == 0):
        print("Must provide an output path!")
        sys.exit()
    if (variableNum <= 0):
        print("Variable number must be a positive number!")
        sys.exit()
    if (clauseNum <= 0):
        print("Clause number must be a positive number!")
        sys.exit()
    
    generateTestCase(variableNum, clauseNum, outputFilePath)


if __name__ == "__main__":
    main(sys.argv[1:])