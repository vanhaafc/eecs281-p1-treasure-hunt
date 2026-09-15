#include <iostream>
#include <stack>
#include <deque>
#include "Route.hpp"

using namespace std;

Route::Route(){
  hunt_order = "NESW";
  valid_terrain = '.';
  stack = true;
}

bool Route::empty(){
  if(discovered.empty()){
    return true;
  }
  return false;
}

void Route::push(Coordinate c){
  if(stack){
    discovered.push_front(c);
  }
  else{
    discovered.push_back(c);
  }
}

Coordinate Route::next(){
  Coordinate c;
  if(stack){
    c = discovered.front();
    discovered.pop_front();
  }
  else{
    c = discovered.back();
    discovered.pop_back();
  }

  return c;
}

// TODO: Implement
void Route::clear(){
  for(size_t i = 0; i < discovered.size(); i++){
    pop();
  }
}

void Route::set_order(string s){
  hunt_order = s;
}

void Route::set_valid_terrain(char c){
  valid_terrain = c;
}

void Route::set_container_type(bool a){
  stack = a;
}
