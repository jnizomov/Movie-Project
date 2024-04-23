#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

// Structure to represent a Movie
struct Movie {
    int id;
    string title;
    vector<string> genres;
    float rating = 0.0; // Average rating of the movie
    int numratings = 0; // Number of ratings received for the movie
};

// Class to manage a list of movies
class MovieList {
private:
    vector<Movie> movies; // Vector to store movies
    unordered_map<int, Movie> moviesMap; // Map to quickly access movies by id

public:
    // Constructor to initialize MovieList from files
    MovieList(string movieFileName, string ratingFileName);

    // Method to print details of a movie
    void printMovie(int index);

    // Method to get the number of ratings for a movie
    int getNumRatings(int index);

    // Method to check if a movie exists at given index
    bool movieExist(int index);

    // Method to reset the movie list
    void resetList();

    // Method to retrieve genres of a movie
    vector<string> retriveGenre(int index);

    // Sorting algorithms

    // Heap sort
    template<typename Compare>
    void heapSort(Compare comp, bool ascending);

    // Helper function for heap sort
    template<typename Compare>
    void heapify(vector<Movie>& arr, int n, int i, Compare comp, bool ascending);

    // Quick sort
    template<typename Compare>
    void quickSort(vector<Movie>& vec, int left, int right, Compare comp, bool ascending);

    // Helper function for quick sort
    template<typename Compare>
    void quickSortHelper(Compare comp, bool ascending);

    // Merge sort
    template<typename Compare>
    void mergeSort(vector<Movie>& moviesVec, int left, int right, Compare comp, bool ascending);

    // Helper function for merge sort
    template<typename Compare>
    void merge(vector<Movie>& moviesVec, int left, int mid, int right, Compare comp, bool ascending);

    // Helper function for merge sort
    template<typename Compare>
    void mergeSortHelper(Compare comp, bool ascending);
};

// Constructor implementation
MovieList::MovieList(string movieFileName, string ratingFileName) {
    // Read movie file
    ifstream file(movieFileName);
    if (!file.is_open()) {
        cout << "Error: Unable to open movie file " << movieFileName << endl;
    }
    string line;
    getline(file, line);

    // Parse movie file and populate movies and moviesMap
    while (getline(file, line)) {
        Movie movie;
        stringstream ss(line);
        string token;

        // Parse movie details from line
        // Extract movie id
        getline(ss, token, ',');
        movie.id = stoi(token);

        // Extract movie title
        getline(ss, token, ',');
        if (token.front() == '"') {
            // Handle title with commas
            size_t startPos = line.find_first_of('"');
            size_t endPos = line.find_last_of('"');
            movie.title = line.substr(startPos + 1, endPos - startPos - 1);
            for (char ch : movie.title) {
                if (ch == ',') {
                    getline(ss, token, ',');
                }
            }
        } else {
            movie.title = token;
        }

        // Extract movie genres
        getline(ss, token, ',');
        stringstream genreStream(token);
        string genre;
        while (getline(genreStream, genre, '|')) {
            movie.genres.push_back(genre);
        }

        // Insert movie into moviesMap
        moviesMap.insert({movie.id, movie});
    }

    // Read rating file
    ifstream file2(ratingFileName);
    if (!file2.is_open()) {
        cout << "Error: Unable to open movie file " << ratingFileName << endl;
    }
    string line2;
    getline(file2, line2);

    // Parse rating file and update movie ratings
    while (getline(file2, line2)) {
        stringstream ss(line2);
        string token;

        // Extract movie id
        getline(ss, token, ',');
        getline(ss, token, ',');
        int movieid = stoi(token);

        // Extract rating
        getline(ss, token, ',');
        float currRating = stof(token);

        // Update movie rating and number of ratings
        Movie temp = moviesMap[movieid];
        temp.rating = (temp.rating * temp.numratings + currRating) / (temp.numratings + 1.0);
        temp.numratings++;
        moviesMap[movieid] = temp;

        getline(ss, token, ',');
    }

    // Put items from map into vector for sorting
    for (const auto& pair: moviesMap) {
        movies.push_back(pair.second);
    }
}

// Method to print details of a movie
void MovieList::printMovie(int index) {
    cout << "Title: " << movies[index].title << endl;
    if (!movies[index].genres.empty()) {
        cout << "Genre(s): " << movies[index].genres.front();
        for (size_t i = 1; i < movies[index].genres.size(); ++i) {
            cout << ", " << movies[index].genres[i];
        }
    }
    
    cout << "\nRating: " << to_string(movies[index].rating) << endl;
    cout << "Number of Ratings: " << to_string(movies[index].numratings) << endl;
    cout << endl;
}

// Method to get the number of ratings for a movie
int MovieList::getNumRatings(int index) {
    return movies[index].numratings;
}

// Method to check if a movie exists at given index
bool MovieList::movieExist(int index) {
    return (index < movies.size());
}

// Method to reset the movie list
void MovieList::resetList() {
    vector<Movie> temp;
    for (const auto& pair: moviesMap) {
        temp.push_back(pair.second);
    }
    movies = temp;
}

// Method to retrieve genres of a movie
vector<string> MovieList::retriveGenre(int index) {
    return movies[index].genres;
}

// ------------------- //
// Sorting Algorithms  //
// ------------------- //

// Comparator for sorting by rating
struct sortByRating {
    bool operator()(const Movie& a, const Movie& b, bool greaterThan) const {
        return greaterThan ? a.rating > b.rating : a.rating < b.rating;
    }
};

// Comparator for sorting by title
struct sortByTitle {
    bool operator()(const Movie& a, const Movie& b, bool greaterThan) const {
        return greaterThan ? a.title > b.title : a.title < b.title;
    }
};

template<typename Compare>
void MovieList::heapify(vector<Movie>& arr, int n, int i, Compare comp, bool ascending) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && comp(arr[left], arr[largest], ascending)) {
        largest = left;
    }

    if (right < n && comp(arr[right], arr[largest], ascending)) {
        largest = right;
    }

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest, comp, ascending);
    }
}

template<typename Compare>
void MovieList::heapSort(Compare comp, bool ascending) {
    int n = movies.size();

    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(movies, n, i, comp, ascending);
    }

    // One by one extract an element from heap
    for (int i = n - 1; i > 0; i--) {
        swap(movies[0], movies[i]);
        heapify(movies, i, 0, comp, ascending);
    }
}

template<typename Compare>
int partition(vector<Movie>& vec, int low, int high, Compare comp, bool ascending) {
    Movie& pivot = vec.at(low); // pivot is lowest element;

    int up = low;
    int down = high;

    while (up < down) {
        for (int i = up; i < high; i++) {
            if (comp(vec.at(up), pivot, ascending)) {
                break;
            }

            up++;
        }

        for (int i = high; i > low; i--) {
            if (comp(vec.at(down), pivot, !ascending)) {
                break;
            }

            down--;
        }

        if (up < down) {
            std::swap(vec.at(up), vec.at(down));
        }
    }

    std::swap(vec.at(low), vec.at(down));

    return down;
}

template<typename Compare>
void MovieList::quickSort(vector<Movie>& vec, int left, int right, Compare comp, bool ascending) {
    if (left < right) {
        int pivot = partition(vec, left, right, comp, ascending);

        quickSort(vec, left, pivot - 1, comp, ascending);
        quickSort(vec, pivot + 1, right, comp, ascending);
    }

    return;
}

template<typename Compare>
void MovieList::quickSortHelper(Compare comp, bool ascending) {
    //std::cout << (ascending ? "Ascending " : "Descending") << " sort starting." << std::endl;

    quickSort(movies, 0, movies.size() - 1, comp, ascending);

    //std::cout << "Sorting ending." << std::endl;
}


template<typename Compare>
void MovieList::mergeSort(vector<Movie>& moviesVec, int left, int right, Compare comp, bool ascending){
    if(left < right){

        //mid divides vector into 2
        int mid = left + (right - left) / 2;

        //recurisvely divide left side into half until its 1 unit
        mergeSort(moviesVec,left,mid,comp,ascending);
        mergeSort(moviesVec,mid+1,right,comp,ascending);

        //merge sorted subarrays
        merge(moviesVec,left,mid,right,comp,ascending);
    }
}

template<typename Compare>
void MovieList::merge(vector<Movie>& moviesVec, int left, int mid, int right,Compare comp, bool ascending){

    //size of left and right
    int n1 = mid - left +1;
    int n2 = right - mid;

    //init left and right temp movies
    Movie* X = new Movie[n1];
    Movie* Y = new Movie[n2];

    //fill temps with information
    for (int i = 0; i < n1; i++)
        X[i] = moviesVec[left + i];
    for (int j = 0; j < n2; j++)
        Y[j] = moviesVec[mid + 1 + j];

    //merging temp arrays into moviesVec
    int i = 0;
    int j = 0;
    int k = left;

    while(i < n1 && j <n2) {
        if (comp(X[i], Y[j], ascending)) {
            moviesVec[k] = X[i];
            i++;
        } else {
            moviesVec[k] = Y[j];
            j++;
        }
        k++;
    }

    //once out of elements in one, just add the rest from the other
    while(i < n1){
        moviesVec[k] = X[i];
        i++;
        k++;
    }
    while(j < n2){
        moviesVec[k] = Y[j];
        j++;
        k++;
    }

    delete[] X;
    delete[] Y;
}

template<typename Compare>
void MovieList::mergeSortHelper(Compare comp, bool ascending){
    mergeSort(movies, 0, movies.size() -1, comp, !ascending);
}

