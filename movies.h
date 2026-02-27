#ifndef MOVIES_H
#define MOVIES_H

#include <string>

class Movies {
    public:
        Movies(){}
        Movies(const std::string& n, double s) {
            name = n;
            score = s;
        }

        std::string getName() const {return name;}
        double getScore() const {return score;}
        friend bool operator >(const Movies& a, const Movies& b);

    private:
        std::string name;
        double score;
};

#endif
