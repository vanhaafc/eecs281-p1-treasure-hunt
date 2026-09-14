// Project Identifier: 40FB54C86566B9DDEAB902CC80E8CE85C1261AAD

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <getopt.h>
#include "Route.hpp"
using namespace std;


// Which data structure a hunter uses to hold discovered locations.
enum class Container {
    kStack,
    kQueue,
};  // Container{}

// Whether --show-path was requested, and in which format.
enum class PathMode {
    kNone = 0,  // option not given: print nothing
    kMap,       // -p M
    kList,      // -p L
};  // PathMode{}


// Everything the command line can change about a run. The defaults here are
// the spec's defaults, so an option that is never given just keeps its value.
struct Options {
    Container captain = Container::kStack;    // default per spec
    Container firstMate = Container::kQueue;  // default per spec
    string huntOrder = "NESW";                // default per spec
    bool verbose = false;
    bool stats = false;
    PathMode showPath = PathMode::kNone;
};  // Options{}


// Print help for the user when requested.
// argv[0] is the name of the currently executing program
void printHelp(char *command) {
    cout << "Usage: " << command << " [options] < inputfile\n";
    cout << "Options:\n";
    cout << "  -h, --help                     Print this message and exit\n";
    cout << "  -c, --captain <QUEUE|STACK>    Container used while sailing (default STACK)\n";
    cout << "  -f, --first-mate <QUEUE|STACK> Container used on land (default QUEUE)\n";
    cout << "  -o, --hunt-order <ORDER>       Permutation of NESW (default NESW)\n";
    cout << "  -v, --verbose                  Print output while hunting\n";
    cout << "  -s, --stats                    Print statistics after the hunt\n";
    cout << "  -p, --show-path <M|L>          Print the path as a map or a list\n";
}  // printHelp()


// Turn "QUEUE"/"STACK" into a Container, or exit(1) with the approved error
// message. `which` is the option name that goes in that message.
Container readContainer(const string &arg, const string &which) {
    if (arg == "QUEUE") {
        return Container::kQueue;
    }  // if ..QUEUE
    if (arg == "STACK") {
        return Container::kStack;
    }  // if ..STACK

    // The first line of error output has to be a 'fixed' message for the
    // autograder to show it to you.
    cerr << "Invalid argument to --" << which << '\n' << flush;
    // The second line can provide more information, but you won't see it on the AG.
    cerr << "  Invalid argument is: " << arg << '\n' << flush;
    exit(1);
}  // readContainer()


// A hunt order is legal only if it is exactly four characters long and uses
// each of N, E, S, and W exactly once.
bool validHuntOrder(const string &arg) {
    if (arg.length() != 4) {
        return false;
    }  // if ..length

    for (char dir : string("NESW")) {
        if (count(arg.begin(), arg.end(), dir) != 1) {
            return false;
        }  // if ..count
    }  // for ..dir
    return true;
}  // validHuntOrder()


// Process the command line; there is no return value, but the Options
// struct is passed by reference and is modified by this function to send
// information back to the calling function.
void getOptions(int argc, char **argv, Options &options) {
    // These are used with getopt_long()
    opterr = static_cast<int>(false);  // Let us handle all error output for command line options
    int choice = 0;
    int index = 0;

    // NOLINTBEGIN: getopt predates C++ style, this usage is from `man getopt`
    option longOptions[] = {
        {"help",       no_argument,       nullptr, 'h'},
        {"captain",    required_argument, nullptr, 'c'},
        {"first-mate", required_argument, nullptr, 'f'},
        {"hunt-order", required_argument, nullptr, 'o'},
        {"verbose",    no_argument,       nullptr, 'v'},
        {"stats",      no_argument,       nullptr, 's'},
        {"show-path",  required_argument, nullptr, 'p'},
        {nullptr,      0,                 nullptr, '\0'},
    };  // longOptions[]
    // NOLINTEND

    while ((choice = getopt_long(argc, argv, "hc:f:o:vsp:", static_cast<option *>(longOptions), &index)) != -1) {
        switch (choice) {
        case 'h':
            printHelp(*argv);
            exit(0);

        case 'c':
            options.captain = readContainer(string(optarg), "captain");
            break;

        case 'f':
            options.firstMate = readContainer(string(optarg), "first-mate");
            break;

        case 'o': {  // Need a block here to declare a variable inside a case
            string arg { optarg };
            if (!validHuntOrder(arg)) {
                cerr << "Invalid argument to --hunt-order\n" << flush;
                cerr << "  Invalid argument is: " << arg << '\n' << flush;
                exit(1);
            }  // if ..valid
            options.huntOrder = arg;
            break;
        }  // case 'o'

        case 'v':
            options.verbose = true;
            break;

        case 's':
            options.stats = true;
            break;

        case 'p': {
            if (options.showPath != PathMode::kNone) {
                cerr << "Specify --show-path only once\n" << flush;
                exit(1);
            }  // if ..already specified

            string arg { optarg };
            if (arg == "M") {
                options.showPath = PathMode::kMap;
            } else if (arg == "L") {
                options.showPath = PathMode::kList;
            } else {
                cerr << "Invalid argument to --show-path\n" << flush;
                cerr << "  Invalid argument is: " << arg << '\n' << flush;
                exit(1);
            }  // if ..arg
            break;
        }  // case 'p'

        default:
            cerr << "Unknown option\n" << flush;
            exit(1);

        }  // switch ..choice
    }  // while
}  // getOptions()

// ADT (TODO: needs to relate with getOpt?)
class Search{
  public:
    //TODO - Implement
    //Pass 2D vector as an argument?
    Search();

    int getN(){
      return n;
    }

    void mapInput(vector<vector<char>> &m){
      string line;
      int row = 0;
      int col = 0;
      while(getline(cin, line)){
        col = 0;
        for(char c : line){
          if(c == '@'){
            start_row = row;
            start_col = col;
          }
          m[row][col] = c;
          col++;
        }
        row++;
      }
    }

    void listInput(vector<vector<char>> &m){

      string line;
      while(getline(cin, line)){
        int row = static_cast<int>(line.at(0));
        int col = static_cast<int>(line.at(2));
        char t = line.at(4);

        if(row >= n || row < 0 || col >= n || col < 0){
          cout << "Invalid coordinates in list mode input";
          exit(1);
        }

        if(t == '@'){
          start_row = row;
          start_col = col;
        }
        
        m[row][col] = t;
      }
    }

    void readInfo(){
      string junk;
      string size;

      while (cin.peek() == '#') { 
        getline(cin, junk); 
      }
      
      getline(cin, inputType);
      getline(cin, size);
      n = stoi(size);
    }

    void readMap(vector<vector<char>> &m){
      if(inputType == "M"){
        mapInput(m);
      }
      else if(inputType == "L"){
        listInput(m);
      }
      else{
        cout << "Unkown option";
      }

      // TODO: check if no treasure or start location
    }

    int getStartRow(){
      return start_row;
    }

    int getStartColumn(){
      return start_col;
    }


  private:
    string inputType;
    int n;
    //TODO: is this right or should these go in main?
    int start_row;
    int start_col;

};


int main(int argc, char *argv[]) {
    // This should be in all of your projects, speeds up I/O
    ios_base::sync_with_stdio(false);

    // Get the options from the command line
    Options options;
    getOptions(argc, argv, options);

    //TODO: Finish
    Search s;
    s.readInfo();
    int n = s.getN();
    vector<vector<char>> map(n, vector<char>(n, '.'));
    s.readMap(map);

    return 0;
}  // main()
