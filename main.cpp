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
    //prints the first 100 movies
    for (int i = 0; i < 100; i++)
        list.printMovie(i);

    return 0;
}

