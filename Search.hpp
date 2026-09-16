// Project Identifier: 40FB54C86566B9DDEAB902CC80E8CE85C1261AAD
#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <stack>
#include "Route.hpp"

using namespace std;

const char kStartMark = '@';

enum class Discovery {
  kNothing,
  kAdded,
  kLand,
  kTreasure
};

// Class that does the searching mechanism
class Search{
  public:
    Search(vector<char> m, int size, bool captain_stack, string captain_order, 
      bool mate_stack, string mate_order, int curr_row, int curr_col);

    void set_verbose(bool v);

    void set_stats(bool s);

    // Decides what to do with one of the neighboring directions
    Discovery discover(int row, int col, char direction, Route &route);

    // Investigates based on hunt order ()
    bool investigate(Coordinate current, Route &route);

    // Does the captain's hunt from the start
    // Eeturns whether the treasure was found
    // Builds the path if it finds the treasure
    bool water_hunt();

    // Does first_mate land hunt, returns whether the party found treasure
    bool land_hunt(Coordinate start);

    // Does backtracking to find most efficient path
    void backtrack();

    // Output. Call order is verbose (during the hunt), stats, path, results.
    void print_stats();
    void print_path_map();
    void print_path_list();
    void print_results();

  private:
    size_t index(int row, int col);
    Route captain;
    Route first_mate;
    vector<char> map;
    int n;
    Coordinate treasure;
    bool treasure_found;

    // 0 means undiscovered. Otherwise it holds the direction traveled to get there
    // (backtrack and discovered tracker)
    vector<char> came_from;
    // Built by backtrack(): the start-to-treasure path, in order.
    vector<Coordinate> path;

    int water_investigated;
    int land_investigated;
    int went_ashore;
    bool verbose;
    bool stats;
    Coordinate start;

};

#endif
