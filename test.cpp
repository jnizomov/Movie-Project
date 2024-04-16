#include "CLI11.hpp"
#include <iostream>
#include <string>

int main(int argc, char **argv) {
    CLI::App app{"Movie Recommendation System"};

    std::string director, actor;
    std::vector<std::string> genres;
    bool isFamilyFriendly = false;

    app.add_option("-d,--director", director, "Filter by director's name");
    app.add_option("-a,--actor", actor, "Filter by actor's name");
    app.add_option("-g,--genre", genres, "Filter by genre")->expected(-1);
    app.add_flag("--family-friendly", isFamilyFriendly, "Include only family-friendly movies");

    CLI11_PARSE(app, argc, argv);

    std::cout << "Searching movies...\n";
    // Your logic to filter and display movies based on the options provided

    return 0;
}
