import sys
import getopt
import random

clauseSize = 3

def getBoolVal(assignment, var):
    if (var < 0):
        var = abs(var)
        return ((assignment >> var) & 1) == 0
    else:
        return ((assignment >> var) & 1) == 1

def verifyUnsatisfiable(testFilePath):

    f = open(testFilePath, "r")

    lines = f.readlines()

    vCount = int(lines[0].split(" ")[0])
    cCount = int(lines[0].split(" ")[1])

    CNF = list()

    for i in range(1, len(lines)):
        line = lines[i]
        clause = line.split(" ")
        CNF.append(int(clause[0]))
        CNF.append(int(clause[1]))
        CNF.append(int(clause[2]))
    for assign in range(1<<vCount):
        satisfiable = True

        for i in range(0, len(CNF), 3):
            c1 = CNF[i]
            c2 = CNF[i+1]
            c3 = CNF[i+2]

            

            

def verifySatisfiable():
    pass

def main(argv):
    testFilePath = ""
    assignmentFile = ""

    try:
        opts, args = getopt.getopt(argv, "hi:a:")
    except getopt.GetoptError:
        print("verifier.py -i <testFilePath> -a <assignmentFile>")
        sys.exit(2)
    
    for opt, arg in opts:
        if opt == "-h":
            print("verifier.py -i <testFilePath> -a <assignmentFile>")
            sys.exit()
        elif opt == "-i":
            testFilePath = arg
        elif opt == "-a":
            assignmentFile = arg

    if (len(testFilePath) == 0):
        print("verifier.py -i <testFilePath> -a <assignmentFile>")
        sys.exit()

    if (len(assignmentFile) == 0):
        # No assignment given, verify the test case is unsatisfiable
        print("Verifying the unsatisfiability of the equation!")
        verifyUnsatisfiable(testFilePath)
    else:
        # Verify the authenticity of the solution
        print("Verifying Solution!")


if __name__ == "__main__":
    main(sys.argv[1:])