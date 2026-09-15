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

  treasure = {0, 0};
  treasure_found = false;
  water_investigated = 0;
  land_investigated = 0;
  went_ashore = 0;
  verbose = false;
}

void Search::set_verbose(bool v){
  verbose = v;
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
    treasure = {row, col};
    treasure_found = true;
    return Discovery::kTreasure;
  }

  return Discovery::kAdded;
}

bool Search::investigate(Coordinate current, Route &route){
  const string &order = route.get_order();

  for(size_t i = 0; i < 4; i++){
    char direction = order.at(i);

    // Step one square in this direction. The coordinate is needed after the
    // call too, so it is computed up front rather than inside each branch.
    Coordinate next = current;
    if(direction == 'N'){
      next.row--;
    }
    else if(direction == 'S'){
      next.row++;
    }
    else if(direction == 'E'){
      next.col++;
    }
    else{
      next.col--;
    }

    Discovery d = discover(next.row, next.col, direction, route);

    if(d == Discovery::kLand){
      went_ashore++;

      if(verbose){
        cout << "Went ashore at: " << next.row << "," << next.col << "\n";
      }

      bool found = land_hunt(next);

      if(verbose){
        if(found){
          cout << "Searching island... party found treasure at "
               << treasure.row << "," << treasure.col << ".\n";
        }
        else{
          cout << "Searching island... party returned with no treasure.\n";
        }
      }

      if(found){
        return true;
      }
    }
    else if(d == Discovery::kTreasure){
      // discover() recorded the coordinates in `treasure`.
      land_investigated++;
      return true;
    }
  }

  return false;
} //investigate()

// Runs one search party from the location where the Captain put it ashore.
// Returns whether this party found the treasure.
bool Search::land_hunt(Coordinate start){
  first_mate.clear();

  if(map[index(start.row, start.col)] == '$'){
    treasure = start;
    treasure_found = true;
    land_investigated++;
    return true;
  }

  first_mate.push(start);

  while(!first_mate.empty()){
    Coordinate current = first_mate.next();
    land_investigated++;

    if(investigate(current, first_mate)){
      return true;
    }
  }

  return false;
}




// Does backtracking to find most efficient path
// Use stack to backtrack? 
void Search::backtrack(){

}
