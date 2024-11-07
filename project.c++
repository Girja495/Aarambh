#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>

using namespace std;

const vector<string> stopWords = {"a", "and", "an", "of", "in", "the"};

void normalizeWord(string &word) {
    for (char &c : word) {
        c = toupper(c);
    }
}

bool isStopWord(const string &word) {
    return find(stopWords.begin(), stopWords.end(), word) != stopWords.end();
}

map<string, double> getWordFrequencies(const string &filename, int &totalWords) {
    ifstream file(filename);
    map<string, int> wordCount;
    totalWords = 0;
    string word;

    while (file >> word) {
        normalizeWord(word);
        if (!isStopWord(word)) {
            wordCount[word]++;
            totalWords++;
        }
 }

    map<string, double> normalizedFreq;
    for (const auto &pair : wordCount) {
        normalizedFreq[pair.first] = static_cast<double>(pair.second) / totalWords;
    }
    
    return normalizedFreq;
}

double calculateSimilarity(const map<string, double> &freq1, const map<string, double> &freq2) {
    double similarity = 0.0;
    for (const auto &pair : freq1) {
        if (freq2.find(pair.first) != freq2.end()) {
            similarity += pair.second + freq2.at(pair.first);
        }
    }
    return similarity;
}

int main() {
    const int numBooks = 64;
    vector<string> filenames(numBooks);
    vector<map<string, double>> frequencies(numBooks);
    double similarityMatrix[numBooks][numBooks] = {0};

    // Load filenames
    for (int i = 0; i < numBooks; ++i) {
        filenames[i] = "book" + to_string(i + 1) + ".txt"; // Assuming files are named book1.txt, book2.txt, ...
        int totalWords;
        frequencies[i] = getWordFrequencies(filenames[i], totalWords);
    }
// Calculate similarity matrix
    for (int i = 0; i < numBooks; ++i) {
        for (int j = i + 1; j < numBooks; ++j) {
            similarityMatrix[i][j] = calculateSimilarity(frequencies[i], frequencies[j]);
            similarityMatrix[j][i] = similarityMatrix[i][j]; // Symmetric matrix
        }
    }

    // Find top ten similar pairs
    vector<pair<pair<int, int>, double>> similarPairs;

    for (int i = 0; i < numBooks; ++i) {
        for (int j = i + 1; j < numBooks; ++j) {
            similarPairs.emplace_back(make_pair(i, j), similarityMatrix[i][j]);
        }
    }

    sort(similarPairs.begin(), similarPairs.end(), [](const auto &a, const auto &b) {
        return a.second > b.second; // Sort by descending similarity
    });

    cout << "Top 10 similar pairs of textbooks:\n";
    
    for (int i = 0; i < 10 && i < similarPairs.size(); ++i) {
        cout << "Pair: (" << similarPairs[i].first.first + 1 << ", " << similarPairs[i].first.second + 1 << ") - Similarity: " 
             << similarPairs[i].second << "\n";
    }
return 0;
}