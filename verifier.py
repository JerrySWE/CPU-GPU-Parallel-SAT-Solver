import sys
import getopt
import random

clauseSize = 3

def getBoolVal(assignment, var):
    pos = abs(var) - 1

    if (var < 0):
        return ((assignment >> pos) & 1) == 0
    else:
        return ((assignment >> pos) & 1) == 1

def readCNF(testFilePath):
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
    
    f.close()
    return vCount, CNF

def readSol(solutionPath):
    f = open(solutionPath, "r")

    lines = f.readlines()
    solution = list()

    for line in lines:
        if (line.split(" ")[-1].strip() == "true"):
            solution.append(True)
        else:
            solution.append(False)

    return solution

def verifyAssignment(assignment, CNF):
    for i in range(0, len(CNF), 3):
        c1 = CNF[i]
        c2 = CNF[i+1]
        c3 = CNF[i+2]

        if (getBoolVal(assignment, c1) or 
            getBoolVal(assignment, c2) or
            getBoolVal(assignment, c3)):
            continue
        else:
            return False

    return True

def verifyUnsatisfiable(testFilePath):

    vCount, CNF = readCNF(testFilePath)

    for assignment in range(80000000, 1<<vCount):
        if (assignment % 10000000 == 0):
            print(assignment)

        if (verifyAssignment(assignment, CNF)):
            print(f"CNF satisfiable with {assignment}")
            return
    
    print("CNF unsatisfiable")

def verifySatisfiable(testFilePath, solutionFilePath):
    vCount, CNF = readCNF(testFilePath)
    sol = readSol(solutionFilePath)

    for i in range(0, len(CNF), 3):
        c1 = CNF[i]
        c2 = CNF[i+1]
        c3 = CNF[i+2]

        c1Pos = abs(c1) - 1
        c2Pos = abs(c2) - 1
        c3Pos = abs(c3) - 1

        if ((c1 > 0 and sol[c1Pos]) or (c2 > 0 and sol[c2Pos]) or (c3 > 0 and sol[c3Pos]) or 
            (c1 < 0 and not sol[c1Pos]) or (c2 < 0 and not sol[c2Pos]) or (c3 < 0 and not sol[c3Pos])):
            continue
        print("CNF clause false under assignment!")
        return
    
    print("Solution Legal!")


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
        verifySatisfiable(testFilePath, assignmentFile)


if __name__ == "__main__":
    main(sys.argv[1:])