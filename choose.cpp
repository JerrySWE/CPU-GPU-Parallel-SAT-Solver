// separate + and -
int chooseLiteral() {
    int maxLiteral = 0;
    int maxFrequency = -1;

    for (int i = 1; i < (int)frequencies.size(); i++) {
        pair<int, int> frequency = frequencies[i];
        if (literalStates[i].assignment == 0) {
            if (frequency.first > maxFrequency) {
                maxLiteral = i;
                maxFrequency = frequency.first;
            }
            if (frequency.second > maxFrequency) {
                maxLiteral = -i;
                maxFrequency = frequency.second;
            }
        }
    }

    return maxLiteral;
}

// separate + and -, choose opposite
int chooseLiteral() {
    int maxLiteral = 0;
    int maxFrequency = -1;

    for (int i = 1; i < (int)frequencies.size(); i++) {
        pair<int, int> frequency = frequencies[i];
        if (literalStates[i].assignment == 0) {
            if (frequency.first > maxFrequency) {
                maxLiteral = -i;
                maxFrequency = frequency.first;
            }
            if (frequency.second > maxFrequency) {
                maxLiteral = i;
                maxFrequency = frequency.second;
            }
        }
    }

    return maxLiteral;
}

// combine + and -, choose largest frequency
int chooseLiteral() {
    int maxLiteral = 0;
    int maxFrequency = -1;

    for (int i = 1; i < (int)frequencies.size(); i++) {
        pair<int, int> frequency = frequencies[i];
        if (literalStates[i].assignment == 0) {
            if (frequency.first + frequency.second > maxFrequency) {
                if (frequency.first > frequency.second) {
                    maxLiteral = i;
                    maxFrequency = frequency.first;
                }
                else {
                    maxLiteral = -i;
                    maxFrequency = frequency.second;
                }
            }
        }
    }

    return maxLiteral;
}

// combine + and -, choose opposite
int chooseLiteral() {
    int maxLiteral = 0;
    int maxFrequency = -1;

    for (int i = 1; i < (int)frequencies.size(); i++) {
        pair<int, int> frequency = frequencies[i];
        if (literalStates[i].assignment == 0) {
            if (frequency.first + frequency.second > maxFrequency) {
                if (frequency.first > frequency.second) {
                    maxLiteral = -i;
                    maxFrequency = frequency.first;
                }
                else {
                    maxLiteral = i;
                    maxFrequency = frequency.second;
                }
            }
        }
    }

    return maxLiteral;
}

// random abs, choose largest frequency
int chooseLiteral() {
    int randomLiteral = rand() % literalStates.size() + 1;
    while (literalStates[randomLiteral].assignment != 0) {
        randomLiteral = rand() % literalStates.size() + 1;
    }
    if (frequencies[randomLiteral].first > frequencies[randomLiteral].second) {
        return randomLiteral;
    }
    else {
        return -randomLiteral;
    }
}

// in order abs, choose largest frequency
int chooseLiteral() {
    for (int i = 1; i < literalStates.size(); i++) {
        if (literalStates[i].assignment == 0) {
            if (frequencies[i].first > frequencies[i].second) {
                return i;
            }
            else {
                return -i;
            }
        }
    }
}