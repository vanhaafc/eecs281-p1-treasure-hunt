#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <iostream>
#include <string>
#include <vector>
#include <deque>
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

    // Decides what to do with one of the neighboring directions
    Discovery discover(int row, int col, char direction, Route &route);

    // Maybe not void return type?
    // Investigates based on hunt order ()
    bool investigate();

    // Does first_mate land hunt
    Discovery land_hunt();

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

};

#endif
