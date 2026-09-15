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

      const string &get_order();

      void set_on_land(bool l);

      bool is_on_land();

      void set_container_type(bool a);

    private:
      deque<Coordinate> container;
      string hunt_order;
      bool on_land;
      bool stack;
      
 };

 #endif