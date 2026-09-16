// Project Identifier: 40FB54C86566B9DDEAB902CC80E8CE85C1261AAD
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

void Route::clear(){
  container.clear();
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
