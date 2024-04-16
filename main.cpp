#include "movieList.h"
#include <chrono> // Include chrono library for timing

int main()
{
    MovieList list = MovieList("movies.csv", "ratings.csv");

    bool cont = true;
    char sortAlgo;
    char sortedBy;
    char order;
    bool ascending;

    cout << "Welcome to Movie Sorting Algorithms: The program that allows users to sort movies based on rating or title." << endl;

    while (cont) {
        cout<< "What sorting algorithm would you like to use? (q for quick, h for heap, m for merge)" << endl;
        cin >> sortAlgo;
        cout << "What would you like the movies to be sorted by? (t for title or r for rating)" << endl;
        cin >> sortedBy;
        cout << "Sorted by descending or ascending order? (a for ascending or d for descending)" << endl;
        cin >> order;
        if (order == 'a'){
            ascending = true;
        } else {
            ascending = false;
        }
        sortByRating sorterRating;
        sortByTitle sorterTitle;

        if (sortAlgo == 'q'){
            auto start = chrono::steady_clock::now();
            if (sortedBy == 'r'){
                list.quickSortHelper(sorterRating, ascending);
            } else {
                list.quickSortHelper(sorterTitle, ascending);
            }
            auto end = chrono::steady_clock::now();
            cout << "Time taken for quicksort: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;
        } else if (sortAlgo == 'h') {
            auto start = chrono::steady_clock::now();
            if (sortedBy == 'r'){
                list.heapSort(sorterRating, ascending);
            } else {
                list.heapSort(sorterTitle, ascending);
            }
            auto end = chrono::steady_clock::now();
            cout << "Time taken for heapsort: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;
        } else if (sortAlgo == 'm') {
            auto start = chrono::steady_clock::now();
            // Call merge sort algorithm here
            auto end = chrono::steady_clock::now();
            cout << "Time taken for mergesort: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;
        } else {
            cout << "Invalid Input" << endl;
            continue;
        }
        //prints the top 100 highest reviewed films with a minimum number of ratings for the user input.
        int minRate;
        string genre;
        cout << "What are the minimum number of ratings you would like for the movies displayed to have? (1-100000)" << endl;
        cin >> minRate;
        cout << "What genre would you like to like to be listed? (Action, Adventure, Animation, Children, Comedy, Crime, Documentary, Drama, Fantasy, Film-Noir, Horror, Romance, Sci-Fi, Thriller, War, Western, or any other text for all genres)" << endl;
        cin >> genre;
        bool genreExists = false;
        if (genre == "Action" || genre == "Adventure" || genre == "Animation" || genre == "Children" || genre == "Comedy" || genre == "Crime" || genre == "Documentary" || genre == "Drama" || genre == "Fantasy" || genre == "Film-Noir" || genre == "Horror" || genre == "Romance" || genre == "Sci-Fi" || genre == "Thriller"|| genre == "War" || genre == "Western"){
            genreExists = true;
        }
        int count = 0;
        int i = 0;
        while (count < 100 && list.movieExist(i)) {
            if (list.getNumRatings(i) >= minRate) {
                if (genreExists){
                    vector<string> genres = list.retriveGenre(i);
                    for (auto item: genres){
                        if (genre == item){
                            list.printMovie(i);
                            count++;
                            break;
                        }
                    }
                } else {
                    list.printMovie(i);
                    count++;
                }
            }
            i++;
        }

        char keepGoing;
        cout << "Would you like to try a different sorting algorithm? (y for yes, any other character for no)" << endl;
        cin >> keepGoing;
        if (keepGoing == 'y'){
            list.resetList();
        } else {
            cont = false;
        }
    }
    return 0;
}
