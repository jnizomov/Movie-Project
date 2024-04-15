// #include <QApplication>
// #include <QPushButton>
#include "movieList.h"


int main(/*int argc, char **argv */)
{
//  QApplication app (argc, argv);

//  QPushButton button ("Hello world !");
//  button.show();

//  return app.exec();

    MovieList list = MovieList("movies.csv", "ratings.csv");
    //Uses a heap sort to modify the movie list.
    list.heapSort();
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
    return 0;
}
