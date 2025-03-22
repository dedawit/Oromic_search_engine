#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

using namespace std;

int main() {
    // Read the inverted index file and store the data in a map
    unordered_map<string, unordered_map<string, int>> inverted_index;
    ifstream infile("inverted_index.txt");
    string line;
    while (getline(infile, line)) {
        istringstream iss(line);
        string term;
        iss >> term;
        string document;
        int frequency;
        while (iss >> document >> frequency) {
            inverted_index[term][document] = frequency;
        }
    }
    // Read the query from the user and tokenize it
    string query;
    cout << "Enter a query: ";
    getline(cin, query);
    istringstream iss(query);
    // Calculate the frequency of each term in the query
    unordered_map<string, int> term_frequencies;
    string term;
    while (iss >> term) {
        for (auto entry : inverted_index[term]) {
            string document = entry.first;
            int frequency = entry.second;
            term_frequencies[document] += frequency;
        }
    }
    // Display the frequency of each term to the user
    for (auto entry : term_frequencies) {
        string document = entry.first;
        int frequency = entry.second;
        cout << document << ": " << frequency << endl;
    }
    return 0;
}
