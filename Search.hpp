#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <stack>
#include "Route.hpp"

using namespace std;


enum class Discovery {
  kNothing,
  kAdded,
  kLand,
  kTreasure
};

// Class that does the searching mechanism
class Search{
  public:
    Search(vector<char> m, int size,bool captain_stack, string captain_order, 
      bool mate_stack, string mate_order, int curr_row, int curr_col);

    void set_verbose(bool v);

    // Decides what to do with one of the neighboring directions
    Discovery discover(int row, int col, char direction, Route &route);

    // Investigates based on hunt order ()
    bool investigate(Coordinate current, Route &route);

    // Does first_mate land hunt, returns whether the party found treasure
    bool land_hunt(Coordinate start);

    // Does backtracking to find most efficient path
    void backtrack();

  private:
    // Flat index into map and came_from.
    size_t index(int row, int col);

    Route captain;
    Route first_mate;
    // 0 means undiscovered. Otherwise it holds the direction traveled to
    // arrive here, so it is both the discovered mark and the backtrack link.
    vector<char> came_from;
    Coordinate location;
    vector<char> map;
    int n;

    // Set once the treasure is discovered, used by verbose and stats output.
    Coordinate treasure;
    bool treasure_found;

    int water_investigated;
    int land_investigated;
    int went_ashore;
    bool verbose;

};

#endif
