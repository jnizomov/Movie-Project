#include "movieList.h"
#include <chrono> // Include chrono library for timing

// Function to get user inputs for sorting algorithm, sorted by, and order
void getAlgoSortOrder(char& sortAlgo, char& sortedBy, char& order) {
    cout<< "What sorting algorithm would you like to use? (q for quick, h for heap, m for merge)" << endl;
    cin >> sortAlgo;
    cout << "What would you like the movies to be sorted by? (t for title or r for rating)" << endl;
    cin >> sortedBy;
    cout << "Sorted by descending or ascending order? (a for ascending or d for descending)" << endl;
    cin >> order;
}

// Function to get minimum ratings and genre from user
void getMinRatingsGenre(int& minRate, string& genre){
    cout << "What are the minimum number of user-submitted ratings you would like for the movies displayed to have? (1-100000)" << endl;
    cin >> minRate;
    cout << "What genre would you like to like to be listed? (Action, Adventure, Animation, Children, Comedy, Crime, Documentary, Drama, Fantasy, Film-Noir, Horror, Romance, Sci-Fi, Thriller, War, Western, or any other text for all genres)" << endl;
    cin >> genre;
}

int main()
{
    // Initialize MovieList object with movie and rating files
    MovieList list = MovieList("movies.csv", "ratings.csv");

    bool cont = true;

    string sep = "__________________________________________________";

    // Display welcome message
    cout << sep << endl;
    cout << "\n          Welcome to Movie Sorter!" << endl << endl;
    cout << "This program allows users to sort movies based on" << endl;
    cout << "either rating or title, providing you with a tailored" << endl;
    cout << "movie discovery experience." << endl;
    cout << sep << endl << endl;

    while (cont) {

        // Get user inputs on sorting algorithm, sort order, and ascending/descending
        char sortAlgo;
        char sortedBy;
        char order;
        getAlgoSortOrder(sortAlgo,sortedBy,order);

        // Determine if sorting should be in ascending order
        bool ascending;
        if (order == 'a'){
            ascending = true;
        } else {
            ascending = false;
        }

        sortByRating sorterRating;
        sortByTitle sorterTitle;
        string sortName;

        // Start clock to measure efficiency of sort, call sort on the list of movies
        auto start = chrono::steady_clock::now();

        if (sortAlgo == 'q'){
            // Call quicksort
            sortName = "quicksort";
            if (sortedBy == 'r'){
                list.quickSortHelper(sorterRating, ascending);
            } else {
                list.quickSortHelper(sorterTitle, ascending);
            }

        } else if (sortAlgo == 'h') {
            // Call heapsort
            sortName = "heapsort";
            if (sortedBy == 'r'){
                list.heapSort(sorterRating, ascending);
            } else {
                list.heapSort(sorterTitle, ascending);
            }

        } else if (sortAlgo == 'm') {
            // Call mergesort
            sortName = "mergesort";
            if(sortedBy == 'r'){
                list.mergeSortHelper(sorterRating, ascending);
            } else {
                list.mergeSortHelper(sorterTitle,ascending);
            }

        } else {
            cout << "Invalid Input" << endl;
            continue;
        }

        auto end = chrono::steady_clock::now();

        // Print the top 100 highest reviewed films with a minimum number of ratings for the user input.

        // Retrieve user input on minimum number of ratings and genre
        int minRate;
        string genre;
        getMinRatingsGenre(minRate,genre);

        bool genreExists = false;

        // Check if the provided genre exists
        if (genre == "Action" || genre == "Adventure" || genre == "Animation" || genre == "Children" || genre == "Comedy" || genre == "Crime" || genre == "Documentary" || genre == "Drama" || genre == "Fantasy" || genre == "Film-Noir" || genre == "Horror" || genre == "Romance" || genre == "Sci-Fi" || genre == "Thriller"|| genre == "War" || genre == "Western"){
            genreExists = true;
        }

        cout << endl << sep << endl << endl;

        int count = 0;
        int i = 0;
        int page = 1;

        // Iterate through movies and display them with given constraints
        while (true) {
            while (count < (page * 100) && list.movieExist(i)) {
                if (list.getNumRatings(i) >= minRate) {
                    if (genreExists) {
                        vector<string> genres = list.retriveGenre(i);

                        for (auto item : genres) {
                            if (item == genre) {
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

            if (!list.movieExist(i)) {
                break;
            }

            cout << "Would you like to view page " << page + 1 << "?"  << " (y for yes, any other character for no)" << std::endl;

            char keepPaging;
            cin >> keepPaging;

            if (keepPaging == 'y') {
                page++;
            } else {
                break;
            }
        }

        // Inform user if there were no results
        if (count == 0) {
            cout << "No movies found. Please try again." << endl << endl;
        } else {
            // Output time taken to run the algorithm at the end;
            cout << "Time taken for " << sortName << ": " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;
            cout << sep << endl << endl;
        }

        // Ask user if they want to try a different sorting algorithm
        char keepGoing;
        cout << "Would you like to try a different sorting algorithm? (y for yes, any other character for no)" << endl;
        cin >> keepGoing;

        if (keepGoing == 'y'){
            list.resetList();
            cout << sep << endl << endl;
        } else {
            cont = false;
        }
    }
    
    return 0;
}
