// Project Identifier: 40FB54C86566B9DDEAB902CC80E8CE85C1261AAD

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <getopt.h>
#include "Search.hpp"
#include "Route.hpp"
using namespace std;


// Which data structure a hunter uses to hold discovered locations.
enum class Container {
    kStack,
    kQueue,
};  // Container{}

// Whether --show-path was requested, and in which format.
enum class PathMode {
    kNone = 0,
    kMap,
    kList,
};  // PathMode{}


// Everything the command line can change about a run. The defaults here are
// the spec's defaults, so an option that is never given just keeps its value.
struct Options {
    Container captain = Container::kStack;
    Container firstMate = Container::kQueue;
    string huntOrder = "NESW";
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
Container readContainer(const string &arg, const string &error_type) {
    if (arg == "QUEUE") {
        return Container::kQueue;
    }
    if (arg == "STACK") {
        return Container::kStack;
    }

    cerr << "Invalid argument to --" << error_type << '\n';
    cerr << "  Invalid argument is: " << arg << '\n';
    exit(1);
}  // readContainer()


// A hunt order is legal only if it is exactly four characters long and uses
// each of N, E, S, and W exactly once.
bool validHuntOrder(const string &arg) {
    if (arg.length() != 4) {
        return false;
    }

    for (char dir : string("NESW")) {
        if (count(arg.begin(), arg.end(), dir) != 1) {
            return false;
        } 
    }
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

        case 'o': {
            string arg { optarg };
            if (!validHuntOrder(arg)) {
                cerr << "Invalid argument to --hunt-order\n";
                cerr << "  Invalid argument is: " << arg << '\n';
                exit(1);
            }
            options.huntOrder = arg;
            break;
        }

        case 'v':
            options.verbose = true;
            break;

        case 's':
            options.stats = true;
            break;

        case 'p': {
            if (options.showPath != PathMode::kNone) {
                cerr << "Specify --show-path only once\n";
                exit(1);
            }

            string arg { optarg };
            if (arg == "M") {
                options.showPath = PathMode::kMap;
            } else if (arg == "L") {
                options.showPath = PathMode::kList;
            } else {
                cerr << "Invalid argument to --show-path\n";
                cerr << "  Invalid argument is: " << arg << '\n';
                exit(1);
            }
            break;
        }

        default:
            cerr << "Unknown option\n";
            exit(1);

        }
    }
}  // getOptions()

// ADT
class Read{
  public:
    Read(){
      
    }

    int getN(){
      return n;
    }

    void mapInput(vector<char> &m){
      string line;
      int row = 0;
      int col = 0;
      while(getline(cin, line)){
        col = 0;
        for(char c : line){
          if(c != '.' && c != '@' && c != 'o' && c != '$' && c != '#'){
            cerr << "Invalid terrain type\n";
            exit(1);
          }

          if(c == '@'){
            start_row = row;
            start_col = col;
          }
          m[static_cast<size_t>(row * n + col)] = c;
          col++;
        }
        row++;
      }
    }

    void listInput(vector<char> &m){
      int row = 0;
      int col = 0;
      char t = '\0';

      // Extraction skips whitespace and blank lines by itself, and copes with
      // coordinates of more than one digit.
      while(cin >> row >> col >> t){
        if(row >= n || row < 0 || col >= n || col < 0){
          cerr << "Invalid coordinates in list mode input\n";
          exit(1);
        }

        if(t != '.' && t != '@' && t != 'o' && t != '$' && t != '#'){
          cerr << "Invalid terrain type\n";
          exit(1);
        }

        if(t == '@'){
          start_row = row;
          start_col = col;
        }

        m[static_cast<size_t>(row * n + col)] = t;
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

    void readMap(vector<char> &m){
      if(inputType == "M"){
        mapInput(m);
      }
      else if(inputType == "L"){
        listInput(m);
      }
      else{
        cerr << "Invalid input file type\n";
        exit(1);
      }

      // Check if no treasure
      vector<char>::iterator it1 = find(m.begin(), m.end(), '$');
      if(it1 == m.end()){
        cerr << "Map does not have treasure\n";
        exit(1);
      }

      // Check if no start location 
      vector<char>::iterator it2 = find(m.begin(), m.end(), '@');
      if(it2 == m.end()){
        cerr << "Map does not have a start location\n";
        exit(1);
      }
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
    int start_row;
    int start_col;

};


int main(int argc, char *argv[]) {
    // This should be in all of your projects, speeds up I/O
    ios_base::sync_with_stdio(false);

    // Get the options from the command line
    Options options;
    getOptions(argc, argv, options);

    Read r;
    r.readInfo();
    int n = r.getN();

    // TODO: 2D or 1D map?
    //vector<vector<char>> map;
    vector<char> map(static_cast<size_t>(n * n), '.');
    r.readMap(map);

    int start_row = r.getStartRow();
    int start_col = r.getStartColumn();

    // One --hunt-order applies to both hunters.
    Search s(map, n,
             options.captain == Container::kStack, options.huntOrder,
             options.firstMate == Container::kStack, options.huntOrder,
             start_row, start_col);

    s.set_verbose(options.verbose);
    s.set_stats(options.stats);

    s.water_hunt();

    // Verbose output already printed during the hunt. Remaining order is
    // stats, then path, then the results line.
    s.print_stats();

    if(options.showPath == PathMode::kMap){
        s.print_path_map();
    }
    else if(options.showPath == PathMode::kList){
        s.print_path_list();
    }

    s.print_results();

    return 0;
}  // main()
