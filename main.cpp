#include "movieList.h"
#include "CLI11.hpp"

#include <iostream>
#include <string>

//TODO: Is there a way to parse and load in the movies.csv and ratings.csv before the program is ran (i.e. store it?)

int main(int argc, char **argv) {
    MovieList list = MovieList("movies.csv", "ratings.csv");
    //prints the first 100 movies

    sortByRating sorter;

    list.quickSortHelper(sorter, false); // true = ascending, false = descending

    /*

    for (int i = 0; i < 100; i++)
        list.printMovie(i);

    */

    return 0;
}
