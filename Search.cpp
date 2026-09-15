#include <iostream>
#include <stack>
#include <deque>
#include "Search.hpp"

using namespace std;

Search::Search(vector<char> m, int size,bool captain_stack, string captain_order, 
      bool mate_stack, string mate_order, int curr_row, int curr_col){
  map = m;
  n = size;
  came_from.assign(static_cast<size_t>(n) * static_cast<size_t>(n), '\0');

  captain.set_on_land(false);
  captain.set_order(captain_order);
  captain.set_container_type(captain_stack);

  first_mate.set_on_land(true);
  first_mate.set_order(mate_order);
  first_mate.set_container_type(mate_stack);

  location.row = curr_row;
  location.col = curr_col;
}

size_t Search::index(int row, int col){
  return static_cast<size_t>(row) * static_cast<size_t>(n) + static_cast<size_t>(col);
}

Discovery Search::discover(int row, int col, char direction, Route &route){
  // Reject anything out of bounds
  if(row < 0 || row >= n || col < 0 || col >= n){
    return Discovery::kNothing;
  }

  size_t i = index(row, col);
  char terrain = map[i];

  // Check if impassable
  if(terrain == '#'){
    return Discovery::kNothing;
  }

  bool land = (terrain == 'o' || terrain == '$');

  // If first mate, then ignore if water
  if(route.is_on_land() && !land){
    return Discovery::kNothing;
  }

  // Don't discover the same location twice
  if(came_from[i] != '\0'){
    return Discovery::kNothing;
  }

  came_from[i] = direction;

  // Check if the captain finds land
  if(!route.is_on_land() && land){
    return Discovery::kLand;
  }

  route.push({row, col});

  // Check if found treasure
  if(terrain == '$'){
    return Discovery::kTreasure;
  }

  return Discovery::kAdded;
}

bool Search::investigate(){
  char c = map[index(location.row, location.col)];
  Route &route = (c == '.' || c == '@') ? captain : first_mate;
  const string &order = route.get_order();

  for(size_t i = 0; i < 4; i++){
    char direction = order.at(i);

    // TODO: use return values
    Discovery d;
    if(direction == 'N'){
      d = discover(location.row - 1, location.col, 'N', route);
    }
    else if(direction == 'S'){
      d = discover(location.row + 1, location.col, 'S', route);
    }
    else if(direction == 'E'){
      d = discover(location.row, location.col + 1, 'E', route);
    }
    else if(direction == 'W'){
      d = discover(location.row, location.col - 1, 'W', route);
    }

    // TODO
    if(d == Discovery::kLand){
      // Increment went-ashore counter (where is that?)
      // If verbose, print went ashore at: row, col
      // Call the land hunt loop with that coordinate as the start
      // If verbose, print search-party result (depending on what step 3 returned)
      // If treasure was found, stop everything, if not keep looping remaining directions
    }
    // TODO
    else if (d == Discovery::kTreasure){
      // TODO: somehow return/have the treasure coordinates
      return true;
    }
  }
} //investigate()

// Does first_mate land hunt
Discovery land_hunt();

// Does backtracking to find most efficient path
// Use stack to backtrack?
void backtrack(){

}
