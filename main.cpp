// Winter'24
// Instructor: Diba Mirza
// Student name: Peter Li
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <cstring>
#include <algorithm>
#include <limits.h>
#include <iomanip>
#include <set>
#include <queue>
#include <sstream>
#include <unordered_map>
using namespace std;

#include "utilities.h"
#include "movies.h"

bool parseLine(string &line, string &movieName, double &movieRating);

int main(int argc, char** argv){
    if (argc < 2){
        cerr << "Not enough arguments provided (need at least 1 argument)." << endl;
        cerr << "Usage: " << argv[ 0 ] << " moviesFilename prefixFilename " << endl;
        exit(1);
    }

    ifstream movieFile (argv[1]);
 
    if (movieFile.fail()){
        cerr << "Could not open file " << argv[1];
        exit(1);
    }
  
    // Create an object of a STL data-structure to store all the movies
    unordered_map<string, Movies> movieTable;
    string line, movieName;
    double movieRating;
    // Read each file and store the name and rating
    while (getline (movieFile, line) && parseLine(line, movieName, movieRating)){
            // Use std::string movieName and double movieRating
            // to construct your Movie objects
            // cout << movieName << " has rating " << movieRating << endl;
            // insert elements into your data structure
            movieTable[movieName] = Movies(movieName, movieRating);
    }

    movieFile.close();

    if (argc == 2){
            //print all the movies in ascending alphabetical order of movie names
            priority_queue<Movies, vector<Movies>, greater<Movies>> pq;
            for (auto &pair : movieTable) {
                pq.push(pair.second);
            }

            while (!pq.empty()) {
                Movies m = pq.top();
                pq.pop();
                cout << m.getName() << ", " << m.getScore() << endl;
            }

            return 0;
    }

    ifstream prefixFile (argv[2]);

    if (prefixFile.fail()) {
        cerr << "Could not open file " << argv[2];
        exit(1);
    }

    vector<string> prefixes;
    while (getline (prefixFile, line)) {
        if (!line.empty()) {
            prefixes.push_back(line);
        }
    }
    prefixFile.close();

    vector<pair<string, Movies>> bestMovies;

    for (const string& prefix : prefixes) {

        vector<Movies> matched;

        for (auto &pair : movieTable) {
            if (pair.first.substr(0, prefix.size()) == prefix) {
                matched.push_back(pair.second);
            }
        }

        if (matched.empty()) {
            cout << "No movies found with prefix " << prefix << endl;
            continue;
        }

        sort(matched.begin(), matched.end(),
            [](const Movies& a, const Movies& b) {
                if (a.getScore() != b.getScore())
                    return a.getScore() > b.getScore();
                return a.getName() < b.getName();
            });

        for (const Movies& m : matched) {
            cout << m.getName() << ", "
                << fixed << setprecision(1)
                << m.getScore() << endl;
        }

        cout << endl;

        bestMovies.push_back({prefix, matched.front()});
    }

    for (auto &entry : bestMovies) {
        cout << "Best movie with prefix "
            << entry.first << " is: "
            << entry.second.getName()
            << " with rating "
            << fixed << setprecision(1)
            << entry.second.getScore() << endl;
    }
    return 0;
}

/* Add your run time analysis for part 3 of the assignment here as commented block*/
/* Part 3a Time Complexity Analysis: 
   Constructing the vector of prefixes takes O(m) since each prefix is read once from 
   the file.

   Next, the program reads the movie file and inserts each movie into movieTable (an 
   unordered_map). The loop runs O(n) times, and ech insertion into the unordered_map 
   takes O(1) on average. Therefore, storing all movies takes O(n).

   After that, for each prefix (O(m)), the program scans the entire movieTable to find 
   matching movies. Since the table contains n movies, it takes O(n) for each prefix.
   For each prefix, if there are k matching movies, they are sorted by rating and name.
   The sorting takes O(k * log(k)). Then, printing the matched movies takes O(k). Thus, 
   for each prefix, the total running time is O(n + k * log(k)).

   Since this happens for m prefixes, the total running time is O(m * (n + k * log(k))).

   In the worst case, every movie matches every prefix (k = n), this becomes 
   O(m * (n + n * log(n))), which is O(m * n * log(n)).

*/

/* Part 3b Space Complexity Analysis:
   The vector of prefixes stores m prefixes. Each prefix may be up to length l, so this 
   takes O(m * l). 
*/

/* Part 3c Designed tradeoffs:
- Optimized for reasonable time efficiency while using additional space
- Could optimize prefix search further with trie or ordered map, but for this assignment,
  unordered_map + linear scan is acceptable for clarity and correctness.
*/

bool parseLine(string &line, string &movieName, double &movieRating) {
    int commaIndex = line.find_last_of(",");
    movieName = line.substr(0, commaIndex);
    movieRating = stod(line.substr(commaIndex+1));
    if (movieName[0] == '\"') {
        movieName = movieName.substr(1, movieName.length() - 2);
    }
    return true;
}