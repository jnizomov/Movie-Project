#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

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

public:
    MovieList(string movieFileName, string ratingFileName){
        ifstream file(movieFileName);
        if (!file.is_open()) {
            cout << "Error: Unable to open movie file " << movieFileName << endl;
        }
        string line;
        getline(file,line);

        while (getline(file, line)){
            stringstream ss(line);
            string token;

            Movie movie;
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
            movies.push_back(movie);
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
            int i = 0;
            getline(ss, token, ',');
            float currRating = stof(token);
            for (auto& movie : movies) {
                if (movieid == movie.id) {
                    movie.rating = (movie.rating * movie.numratings + currRating) / (movie.numratings + 1.0);
                    movie.numratings++;
                    break;
                }
            }
            getline(ss, token, ',');
        }

    }

    void printMovie(int index){
        cout << "Title: " << movies[index].title << endl;
        cout << "Genres: " ;
        for (string genre : movies[index].genres){
            cout << genre << ' ';
        }
        cout << "\nRating: " << to_string(movies[index].rating) << endl;
    }

};