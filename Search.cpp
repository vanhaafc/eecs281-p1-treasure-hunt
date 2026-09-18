// Project Identifier: 40FB54C86566B9DDEAB902CC80E8CE85C1261AAD
#include <algorithm>
#include <iostream>
#include <stack>
#include <deque>
#include "Search.hpp"

using namespace std;

Search::Search(vector<char> &m, int size, bool captain_stack, string captain_order, 
      bool mate_stack, string mate_order, int curr_row, int curr_col){
  // TODO: pass map by reference?
  map = m;
  n = size;
  came_from.assign(static_cast<size_t>(n) * static_cast<size_t>(n), '\0');

  captain.set_on_land(false);
  captain.set_order(captain_order);
  captain.set_container_type(captain_stack);

  first_mate.set_on_land(true);
  first_mate.set_order(mate_order);
  first_mate.set_container_type(mate_stack);

  start.row = curr_row;
  start.col = curr_col;
  start.row = curr_row;
  start.col = curr_col;

  treasure = {0, 0};
  treasure_found = false;
  water_investigated = 0;
  land_investigated = 0;
  went_ashore = 0;
  verbose = false;
  stats = false;
}

void Search::set_verbose(bool v){
  verbose = v;
}

void Search::set_stats(bool s){
  stats = s;
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

// Runs the Captain's hunt from the starting location
// Returns whether the treasure was found.
bool Search::water_hunt(){
  captain.clear();

  if(verbose){
    cout << "Treasure hunt started at: " << start.row << "," << start.col << "\n";
  }

  came_from[index(start.row, start.col)] = kStartMark;
  captain.push(start);

  while(!captain.empty()){
    Coordinate current = captain.next();
    water_investigated++;

    if(investigate(current, captain)){
      backtrack();
      return true;
    }
  }

  if(verbose){
    cout << "Treasure hunt failed\n";
  }

  return false;
}

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
void Search::backtrack(){
  path.clear();

  if(!treasure_found){
    return;
  }

  Coordinate current = treasure;

  while(true){
    path.push_back(current);

    char arrived = came_from[index(current.row, current.col)];

    if(arrived == kStartMark){
      break;
    }

    if(arrived == 'N'){
      current.row++;
    }
    else if(arrived == 'S'){
      current.row--;
    }
    else if(arrived == 'E'){
      current.col--;
    }
    else if(arrived == 'W'){
      current.col++;
    }
    else{
      break;
    }
  }

  // Collected treasure-first so flip to read start to finish
  reverse(path.begin(), path.end());
}



void Search::print_stats(){
  // Gated here, the same way verbose output is.
  if(!stats){
    return;
  }

  cout << "--- STATS ---\n";
  cout << "Starting location: " << start.row << "," << start.col << "\n";
  cout << "Water locations investigated: " << water_investigated << "\n";
  cout << "Land locations investigated: " << land_investigated << "\n";
  cout << "Went ashore: " << went_ashore << "\n";

  // These two lines are omitted entirely when no path was found.
  if(treasure_found){
    cout << "Path length: " << path.size() - 1 << "\n";
    cout << "Treasure location: " << treasure.row << "," << treasure.col << "\n";
  }

  cout << "--- STATS ---\n";
}

void Search::print_path_map(){
  // Nothing is printed when there is no path.
  if(!treasure_found){
    return;
  }

  vector<char> drawn = map;

  // Every cell between the start and the treasure gets a glyph chosen from the
  // direction it was entered and the direction it was left.
  for(size_t i = 1; i + 1 < path.size(); i++){
    char arrived = came_from[index(path[i].row, path[i].col)];
    char departed = came_from[index(path[i + 1].row, path[i + 1].col)];

    bool arrived_vertical = (arrived == 'N' || arrived == 'S');
    bool departed_vertical = (departed == 'N' || departed == 'S');

    char glyph = '+';
    if(arrived_vertical && departed_vertical){
      glyph = '|';
    }
    else if(!arrived_vertical && !departed_vertical){
      glyph = '-';
    }

    drawn[index(path[i].row, path[i].col)] = glyph;
  }

  // The start keeps its '@'; the treasure is overwritten.
  drawn[index(treasure.row, treasure.col)] = 'X';

  for(int row = 0; row < n; row++){
    for(int col = 0; col < n; col++){
      cout << drawn[index(row, col)];
    }
    cout << "\n";
  }
}

void Search::print_path_list(){
  if(!treasure_found){
    return;
  }

  cout << "Sail:\n";
  bool ashore = false;

  // The path crosses from water to land exactly once, so one flag is enough.
  for(size_t i = 0; i < path.size(); i++){
    char terrain = map[index(path[i].row, path[i].col)];

    if(!ashore && (terrain == 'o' || terrain == '$')){
      cout << "Search:\n";
      ashore = true;
    }

    cout << path[i].row << "," << path[i].col << "\n";
  }
}

void Search::print_results(){
  if(treasure_found){
    cout << "Treasure found at " << treasure.row << "," << treasure.col
         << " with path length " << path.size() - 1 << ".\n";
  }
  else{
    cout << "No treasure found after investigating "
         << water_investigated + land_investigated << " locations.\n";
  }
}
