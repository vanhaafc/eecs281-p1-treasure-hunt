#include <iostream>
#include <stack>
#include <deque>
#include "Search.hpp"

using namespace std;

Search::Search(bool captain_stack, string captain_order, bool mate_stack, string mate_order){
  captain.set_valid_terrain('.');
  captain.set_order(captain_order);
  captain.set_container_type(captain_stack);

  first_mate.set_valid_terrain('o');
  first_mate.set_order(mate_order);
  first_mate.set_container_type(mate_stack);
}