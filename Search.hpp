#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <iostream>
#include <string>
#include <deque>
#include "Route.hpp"

using namespace std;

// Class that does the searching mechanism
class Search{
  public:
    Search(bool captain_stack, string captain_order, bool mate_stack, string mate_order);

    // Maybe not void return type?
    void investigate();

    void discover();

  private:
    Route captain;
    Route first_mate;
};

#endif