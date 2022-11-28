import sys
import getopt
import random

clauseSize = 3

def generateTestCase(vNum, cNum, outputPath):
    if (outputPath[-4:] != ".txt"):
        outputPath += ".txt"

    f = open(outputPath, "w")

    f.write(str(vNum) + " " + str(cNum) + "\n")

    for i in range(cNum):
        clause = ""
        for j in range(clauseSize-1):
            clause += str(random.randint(-vNum+1, vNum-1)) + " "
        clause += str(random.randint(-vNum+1, vNum-1)) + "\n"

        f.write(clause)
    
    f.close()

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
    
    generateTestCase(variableNum, clauseNum, outputFilePath)


if __name__ == "__main__":
    main(sys.argv[1:])