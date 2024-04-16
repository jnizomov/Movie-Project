#include "movieList.h"


int main()
{
    MovieList list = MovieList("movies.csv", "ratings.csv");

    bool cont = true;
    string sortAlgo;

    cout << "Welcome to Movie Sorting Algorithms: The program that allows users to sort movies based on ratings." << endl;

    while (cont) {
        cout<< "What sorting algorithm would you like to use? (quick, heap, merge)" << endl;
        cin >> sortAlgo;
        if (sortAlgo == "quick"){
            sortByRating sorter; // comparison functor for sort type, change depending on input

            list.quickSortHelper(sorter, true); // true = ascending, false = descending
        } else if (sortAlgo == "heap") {
            list.heapSort();
            //print time taken to complete sorting algo
        } else if (sortAlgo == "merge") {
            cout << "mergesort" << endl;  // replace this line with a call to merge sort
        } else {
            cout << "Invalid Input" << endl;
            continue;
        }
        //prints the top 100 highest reviewed films with a minimum number of ratings for the user input.
        int minRate;
        cout << "What are the minimum number of ratings you would like for the movies displayed to have?" << endl;
        cin >> minRate;
        int count = 0;
        int i = 0;
        while (count < 100 && list.movieExist(i)) {
            if (list.getNumRatings(i) >= minRate) {
                list.printMovie(i);
                count++;
            }
            i++;
        }

        char keepGoing;
        cout << "Would you like to try a different sorting algorithm? (y for yes, any other character for no)" << endl;
        cin >> keepGoing;
        if (keepGoing == 'y'){
            //list.resetList();
        } else {
            cont = false;
        }
    }
    return 0;
}
