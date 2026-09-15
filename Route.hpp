#ifndef ROUTE_HPP
#define ROUTE_HPP

 #include <iostream>
 #include <string>
 #include <deque>

 using namespace std;

 struct Coordinate {
  int row;
  int col;
 };

 class Route {
    public:
      Route();

      bool empty();

      void push(Coordinate c);

      void pop();

      Coordinate next();

      void clear();

      void set_order(string s);

      void set_valid_terrain(char c);

      void set_container_type(bool a);

    private:
      deque<Coordinate> discovered;
      string hunt_order;
      char valid_terrain;
      bool stack;
      
 };

 #endif