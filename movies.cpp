#include "movies.h"
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

bool operator >(const Movies& a, const Movies& b) {
    return a.getName() > b.getName();
}