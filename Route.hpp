#ifndef ROUTE_HPP
#define ROUTE_HPP

 #include <iostream>
 #include <stack>
 #include <queue>

 using namespace std;

 class Route {
    public:
      // Default Constructor
      Route();

      //TODO: other member funtions
      void investigate();

      //Constructor
    private:
      char valid_terrain;
      //TODO: variable for hunt order?
      //TODO: stack/queue?
      int curr_row;
      int curr_col;
      
      // Member variables
      
 };

 #endif