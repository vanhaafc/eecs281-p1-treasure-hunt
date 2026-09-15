#include <iostream>
#include <stack>
#include <deque>
#include "Route.hpp"

using namespace std;

Route::Route(){
  hunt_order = "NESW";
  on_land = false;
  stack = true;
}

bool Route::empty(){
  if(container.empty()){
    return true;
  }
  return false;
}

void Route::push(Coordinate c){
  container.push_front(c);
}

Coordinate Route::next(){
  Coordinate c;
  if(stack){
    c = container.front();
    container.pop_front();
  }
  else{
    c = container.back();
    container.pop_back();
  }

  return c;
}

// TODO: Implement
void Route::clear(){
  for(size_t i = 0; i < container.size(); i++){
    pop();
  }
}

void Route::set_order(string s){
  hunt_order = s;
}

const string &Route::get_order(){
  return hunt_order;
}

void Route::set_on_land(bool l){
  on_land = l;
}

bool Route::is_on_land(){
  return on_land;
}

void Route::set_container_type(bool a){
  stack = a;
}
