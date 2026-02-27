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
#include <map>
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
    map<string, double> movieTable;
    string line, movieName;
    double movieRating;
    // Read each file and store the name and rating
    while (getline (movieFile, line) && parseLine(line, movieName, movieRating)){
            // Use std::string movieName and double movieRating
            // to construct your Movie objects
            // cout << movieName << " has rating " << movieRating << endl;
            // insert elements into your data structure
            movieTable[movieName] = movieRating;
    }

    movieFile.close();

    if (argc == 2){
            //print all the movies in ascending alphabetical order of movie names
            priority_queue<Movies, vector<Movies>, greater<Movies>> pq;
            for (auto &pair : movieTable) {
                pq.push(Movies(pair.first, pair.second));
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
        auto it = movieTable.lower_bound(prefix);
        vector<Movies> matched;
        Movies bestMovie;
        bool found = false;
        while (it != movieTable.end() && it -> first.substr(0, prefix.size()) == prefix) {
            Movies m(it->first, it->second);
            matched.push_back(m);

            if (!found || m.getScore() > bestMovie.getScore() ||
                (m.getScore() == bestMovie.getScore() && m.getName() < bestMovie.getName())) {
                bestMovie = m;
                found = true;
            }
            ++it;
        }

        if (matched.empty()) {
            cout << "No movies found with prefix " << prefix << endl;

            continue;
        }

        sort(matched.begin(), matched.end(), [](const Movies &a, const Movies &b){
            if (a.getScore() != b.getScore()) 
                return a.getScore() > b.getScore();
            return a.getName() < b.getName();
        });

        for (const auto &p : matched) {
            cout << p.getName() << ", " << fixed << setprecision(1) << p.getScore() << endl;
        }
        cout << endl;
        bestMovies.push_back({prefix, bestMovie});
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
   1. Constructing the vector of prefixes takes O(m) since each prefix is read once from the file.

   2. Reading the movie file and inserting into movieTable (a std::map) takes O(n*log(n)) 
      because each insertion into a map is O(log n).

   3. For each prefix (O(m)), the program uses lower_bound to find the first movie with that prefix. 
      Iterating over all matching movies requires scanning k movies (number of matches). 
      - Constructing Movies objects and storing them in a vector is O(k).
      - Sorting the matched vector by rating and name is O(k*log k).
      - Printing all matched movies is O(k).

   Therefore, for each prefix, the time is O(k*log k + k) ≈ O(k*log k). 
   Scanning the map with lower_bound adds O(log n), negligible compared to sorting k matches.

   Across all m prefixes, the total time is:
      O(n*log n) [building map] + sum_over_prefixes(O(k*log k)) ≈ O(n*log n + m*k*log k)

   In the worst case where every prefix matches every movie (k = n), this becomes:
      O(n*log n + m*n*log n) ≈ O(m*n*log n)

   for input_20_random: real 0m0.141s user0m0.011s sys0m0.019s
   for input_100_random: real 0m0.134s user 0m0.045s sys 0m0.012s
   for input_1000_random: real 0m0.488s user 0m0.474s sys 0m0.012s
   for input_76920_random: real 0m57.26s user 0m56.799s sys 0m0.117s  
*/

/* Part 3b Space Complexity Analysis:
   - The vector of prefixes stores m prefixes, each up to length l: O(m*l)
   - movieTable stores n movies, each name length l: O(n*l)
   - For each prefix, the vector matched stores up to k movies: O(k*l)
     (this vector is reused per prefix, so max extra space at a time is O(k*l))
   - bestMovies vector stores one movie per prefix: O(m*l)

   Total space usage: O(n*l + m*l + k*l)
*/

/* Part 3c Tradeoffs:
   - The algorithm was primarily designed for low **time complexity**. 
     Using a std::map with lower_bound allows iterating over relevant movies efficiently 
     rather than scanning all n movies for every prefix.

   - To reduce runtime when printing all matches, the algorithm stores all matches in a vector 
     and sorts them, which increases space usage to O(k*l) per prefix. This is a tradeoff: 
     faster sorting and printing vs. extra space for storing matches.

   - Low space complexity was considered, but the need to store matched movies for sorting 
     prevents using purely O(1) extra space. So we used O(k*l) temporary storage.

   - Achieving low **time complexity** was harder than minimizing space. 
     Prefix search with sorting required careful data structure choice and iteration strategies. 
     Space optimization was easier because temporary vectors are reused for each prefix.
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