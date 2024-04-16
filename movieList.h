#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;
struct Movie{
    int id;
    string title;
    vector<string> genres;
    float rating = 0.0;
    int numratings = 0;

};

class MovieList{
    private:
        vector<Movie> movies;
        unordered_map<int, Movie> moviesMap;

        void heapify(vector<Movie>& arr, int n, int i);

    public:
        MovieList(string movieFileName, string ratingFileName);
        void printMovie(int index);
        void heapSort();
        int getNumRatings(int index);
        bool movieExist(int index);

        template<typename Compare>
        void quickSort(vector<Movie>& vec, int left, int right, Compare comp, bool ascending);

        template<typename Compare>
        void quickSortHelper(Compare comp, bool ascending);
};

MovieList::MovieList(string movieFileName, string ratingFileName){
    ifstream file(movieFileName);
    if (!file.is_open()) {
        cout << "Error: Unable to open movie file " << movieFileName << endl;
    }
    string line;
    getline(file,line);

    while (getline(file, line)){
        Movie movie;

        stringstream ss(line);
        string token;

        getline(ss, token, ',');
        movie.id = stoi(token);

        getline(ss, token, ',');

        if (token.front() == '"') {
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

        getline(ss, token, ',');
        stringstream genreStream(token);
        string genre;

        while (getline(genreStream, genre, '|')) {
            movie.genres.push_back(genre);
        }

        moviesMap.insert({movie.id, movie});
    }

    ifstream file2(ratingFileName);

    if (!file2.is_open()) {
        cout << "Error: Unable to open movie file " << ratingFileName << endl;
    }

    string line2;
    getline(file2, line2);

    while(getline(file2, line2)) {
        stringstream ss(line2);
        string token;

        getline(ss, token, ',');
        getline(ss, token, ',');

        int movieid = stoi(token);

        getline(ss, token, ',');

        float currRating = stof(token);
        Movie temp = moviesMap[movieid];

        temp.rating = (temp.rating * temp.numratings + currRating) / (temp.numratings + 1.0);
        temp.numratings++;

        moviesMap[movieid] = temp;

        getline(ss, token, ',');
    }

    //Put items from map into vector so items can be sorted
    for (const auto& pair: moviesMap){
        movies.push_back(pair.second);
    }

}

void MovieList::printMovie(int index){
    cout << "Title: " << movies[index].title << endl;
    cout << "Genre(s): " ;
    for (string genre : movies[index].genres){
        cout << genre << ' ';
    }

    cout << "\nRating: " << to_string(movies[index].rating) << endl;
    cout << "Number of Ratings: " << to_string(movies[index].numratings) << endl;
    cout << endl;
}

int MovieList::getNumRatings(int index){
    return movies[index].numratings;
}

bool MovieList::movieExist(int index){
    if (index >= movies.size()){
        return false;
    }
    return true;
}

// ------------------- //
// Sorting Algorithms  //
// ------------------- //

struct sortByRating {
    bool operator()(const Movie& a, const Movie& b, bool greaterThan) const {
        return greaterThan ? a.rating > b.rating : a.rating < b.rating;
    }
};

struct sortByTitle {
    bool operator()(const Movie& a, const Movie& b, bool greaterThan) const {
        return greaterThan ? a.title > b.title : a.title < b.title;
    }
};

void MovieList::heapify(vector<Movie>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left].rating < arr[largest].rating) {
        largest = left;
    }

    if (right < n && arr[right].rating < arr[largest].rating) {
        largest = right;
    }

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}


void MovieList::heapSort() {
    int n = movies.size();

    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(movies, n, i);
    }

    // One by one extract an element from heap
    for (int i = n - 1; i > 0; i--) {
        swap(movies[0], movies[i]);
        heapify(movies, i, 0);
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