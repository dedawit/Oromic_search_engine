#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <set>
#include <string>

using namespace std;

// Function to tokenize a string into a list of words
vector<string> tokenize(string str) {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

void read_inverted_index_query(string filename, vector<string>& inverted_index) {
    ifstream infile(filename);
    string line;
    while (getline(infile, line)) {
        vector<string> parts = tokenize(line);
        string term = parts[0];
        inverted_index.push_back(term);}}

// Function to read an inverted index file and store it in an unordered map
void read_inverted_index(string filename, map<string, map<string, double>>& inverted_index) {
    ifstream infile(filename);
    string line;
    while (getline(infile, line)) {
        vector<string> parts = tokenize(line);
        string term = parts[0];
        //cout<<term<<endl;
        double weight;
        string document_name;
        int c1=0,c2=0;
        for (int i = 2; i < parts.size(); i++) {
                if(parts[i]=="Doc1"){
                    i++;
                     string document_weight = parts[i];
            int open_paren_index = document_weight.find("(");
            int close_paren_index = document_weight.find(")");
                    document_name = "Doc1";
             weight = stod(document_weight.substr(open_paren_index + 1, close_paren_index - open_paren_index - 1));
             c1++;
                    }
                       if(parts[i]=="Doc2"){
                    i++;
                     string document_weight = parts[i];
            int open_paren_index = document_weight.find("(");
            int close_paren_index = document_weight.find(")");
                     document_name = "Doc2";
             weight = stod(document_weight.substr(open_paren_index + 1, close_paren_index - open_paren_index - 1));
             c2++;
                    }




             inverted_index[term][document_name] = weight;
             //cout<<document_name<<" "<<term<<" "<<weight<<endl;
        }
        if(c1==0){
            inverted_index[term]["Doc1"] = 0;
        }
        if(c2==0){
            inverted_index[term]["Doc2"] = 0;
        }
    }
    infile.close();
}

// Function to compute the dot product of two document vectors
double dot_product(map<string, double>& vector1, map<string, double>& vector2) {
    double result = 0;
    vector<double>query;
    vector<double>doc;

    for(auto entry:vector1){
            query.push_back(entry.second);


    }
    for(auto entry:vector2){
            doc.push_back(entry.second);


    }
    int k=0,l=0;
    for(;k<query.size();k++,l++){
             double weight1 = query.at(k);
        double weight2=doc.at(l);
        result += weight1 * weight2;

    }


    cout<<result<<endl;
    return result;
}

// Function to compute the magnitude of a document vector
double magnitude(map<string, double>& vector) {
    double result = 0;
    for (auto entry : vector) {
        double weight = entry.second;
        result += pow(weight, 2);
    }

    return sqrt(result);
}

// Function to compute the cosine similarity between two document vectors
double cosine_similarity(map<string, double>& vector1, map<string, double>& vector2) {
    double dot_product_value = dot_product(vector1, vector2);
    double magnitude1 = magnitude(vector1);
    double magnitude2 = magnitude(vector2);
    return dot_product_value / (magnitude1 * magnitude2);
}

// Define a utility function to compare pairs by value in descending order
bool sortByValue(const pair<string, int>& a, const pair<string, int>& b) {
    return a.second < b.second;
}

int main() {
    // Read the inverted index from a file
    // Read the inverted index file and store the data in a vector
    vector<string> inverted_index_vector;
    read_inverted_index_query("inverted_index.txt",inverted_index_vector);
    map<string, map<string, double>> inverted_index;
    read_inverted_index("inverted_index.txt", inverted_index);
    // Read the names of the two documents to compare
    map<string, double> term_weights1;
    map<string, double> term_weights2;
     map<string, double> query_weights;
      for(int i=0;i<inverted_index_vector.size();i++){
        query_weights[inverted_index_vector.at(i)]=0;
    }

      string query;
    cout << "Enter a query: ";
    getline(cin, query);
    stringstream iss(query);
    // Calculate the frequency of each term in the query
 map<string, int> term_frequencies;
    string term;
    while (iss >> term) {

            term_frequencies[term] ++;
        }
         for(auto entry:query_weights){
            string term=entry.first;
            auto it=term_frequencies.find(term);
        if(it!=term_frequencies.end()){
            query_weights[it->first]=it->second;

        }}

      int num=1;

          for (auto outer_entry : inverted_index) {
        string outer_key = outer_entry.first;
        map<string, double> inner_map = outer_entry.second;
        for (auto inner_entry : inner_map) {
            string inner_key = inner_entry.first;
            double inner_value = inner_entry.second;
            //cout<<outer_key<<":"<<inner_key<<":"<<inner_value<<endl;
            string first=outer_key + "_" + "Doc";
            if(num%2==1){
                  term_weights1.insert(make_pair(first, inner_value));
            }
            else
            {
               term_weights2.insert(make_pair(first, inner_value));
            }
            num++;


        }
    }

  /*  for(auto entry:term_weights2){
        string key=entry.first;
        double num=entry.second;
        cout<<key<<"  :   "<<num<<endl;
    }*/

    // Compute the cosine similarity

     map<string,double>retrieval;

            string name="document1";

            double similarity=cosine_similarity(query_weights, term_weights1);
            retrieval[name]=similarity;
             name="document2";

            similarity=cosine_similarity(query_weights, term_weights2);
            retrieval[name]=similarity;

    // Copy the contents of the map to a vector of pairs
    vector<pair<string, double>> my_vector(retrieval.begin(), retrieval.end());
    // Sort the vector by key
    //sort(my_vector.begin(), my_vector.end(),sortByValue);
    sort(my_vector.begin(), my_vector.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return a.first > b.first;
    });


    int counter=1;
    for(auto entry:my_vector){
        if(entry.second!=0&&counter<=2){
              ifstream file(entry.first + ".txt");
                string line;

                while (getline(file, line)) {

                        cout << endl<< line << endl;


                }
                 cout <<endl<<endl<< "###########################################################"<<endl<<endl;

            }

        }







    return 0;
}
