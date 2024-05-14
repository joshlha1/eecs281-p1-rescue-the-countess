//Project ID: B99292359FFD910ED13A7E6C7F9705B8742F0D79

#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>
using namespace std;

class Marco {
    private:

    //Routing Schemes
    bool stackMode = false;
    bool queueMode = false;

    //Output Formats
    bool mapMode = true;
    bool clMode = false; //coordinate list

    bool found = false;
    int discovered = 0;



    public: 


    struct tile {
        char symbol = '.';
        bool discovered = false;
        char move = 'X'; //Where we moved from i.e north/south/east/west or floor#
    };

    vector<vector<vector<tile>>> map;
    

    char inputMode; 
    uint32_t numRooms = 0;
    uint32_t length = 0;


    struct coordinate {
        uint32_t room;
        uint32_t row;
        uint32_t col; 

        coordinate() {}

        coordinate(uint32_t room_in, uint32_t  row_in, uint32_t col_in) :
            room(room_in), row(row_in), col(col_in) {}
    };

    coordinate start;
    coordinate countess;

    deque<coordinate> sc; 
    deque<coordinate> path;
    
    void getOptions(int argc, char** argv);
    void readMap();
    void search();
    void backTrace();
    void output();

    bool isValidChar(char symb); 
};

int main(int argc, char **argv) {
    ios_base::sync_with_stdio(false);

    Marco myMarco; 

    myMarco.getOptions(argc, argv);

    myMarco.readMap();

    myMarco.search();

    myMarco.output();

 
    return 0;
}


void Marco::getOptions(int argc, char** argv) {
    int option_index = 0, option = 0;

    // Don't display getopt error messages about options
    opterr = false;

    struct option longOpts[] = {{ "help", no_argument, nullptr, 'h' },
                                {"queue", no_argument, nullptr, 'q'},
                                {"stack", no_argument, nullptr, 's'},
                                {"output", required_argument, nullptr, 'o'}, //Legal args: (M|L)
                                { nullptr, 0, nullptr, '\0' }};
    
    while ((option = getopt_long(argc, argv, "hqso:", longOpts, &option_index)) != -1) {
        switch (option) {

            case 'h':
                std::cout << "<Insert helpful message here>" << endl;
                exit(0);

            case 'q':
                queueMode = true;
                break;
            
            case 's':
                stackMode = true;
                break;

            case 'o':    
                if (*optarg == 'M') {
                     mapMode = true;
                } else if (*optarg == 'L'){
                    clMode = true; 
                    mapMode = false;
                } 
            break;
        }
    } 
    if (stackMode && queueMode) {
        cerr << "Stack or queue can only be specified once" << endl; 
        exit(1);
    }
    if (!stackMode && !queueMode) {
        cerr << "Stack or queue must be specified" << endl;
        exit(1);
    }
}

void Marco::readMap() {

    cin >> inputMode >> numRooms >> length;
    map.resize(numRooms, vector<vector<tile>>(length, vector<tile>(length)));

    char symbol;
    string junk;

    if(inputMode == 'M') {
        for (uint32_t room = 0; room < numRooms; room++) {
            for (uint32_t row = 0; row < length; row++) {
                for (uint32_t col = 0; col < length; col++) {
                    cin >> symbol;
                    while (symbol == '/') {
                        getline(cin, junk);
                        cin >> symbol;
                    }
                        map[room][row][col].symbol = symbol;

                        if (symbol == 'S') {
                            start = coordinate(room, row, col);
                        }
                        else if (symbol == 'C') {
                            countess = coordinate(room, row, col);
                        }
                   //TODO: ERROR CHECK HERE: INVALID SYMBOLS//
                }
            }
        }
    }

    else {
        //Reading List Mode
        uint32_t room = 0;
        uint32_t row = 0;
        uint32_t col = 0;
        char trash = 'e'; 

        while (cin >> symbol) {
            if (symbol == '/') {
                getline(cin, junk);
            }
            else {

                cin >> room >> trash >> row >> trash >> col >> trash >> symbol >> trash;

                map[room][row][col].symbol = symbol;

                if (symbol == 'S') {
                    start = coordinate(room, row, col);
                }
                if (symbol == 'C') {
                    countess = coordinate(room, row, col);
                }
            }
        }
    }
}

bool Marco::isValidChar(char symb) {
    if ((symb >= '0') && (symb <= '9')) {
        return true;
    }
    else if (symb == '.') {
        return true;
    }
    else if (symb == 'C') {
        found = true;
        return true;
    }
    else if (symb == '#' || symb == '!') {
        return false;
    }
    else {
        //TODO: REPORT ERROR !!!!!!!!// maybe even report and exit here
        return false;
    }
}

void Marco::search() {
    coordinate current;
    current = start;

    sc.push_back(current);
    ++discovered;

    map[current.room][current.row][current.col].discovered = true;
    
    while (!sc.empty() && !found) {
        if (stackMode) {
            current = sc.back();
            sc.pop_back();
        }
        else {
            current = sc.front();
            sc.pop_front();
        }

        char currSym = map[current.room][current.row][current.col].symbol;
     
        if ((currSym >= '0') && (currSym <= '9')) {
            if (static_cast<uint32_t>(currSym - '0') < numRooms) {
                if (isValidChar(map[static_cast<uint32_t>(currSym - '0')][current.row][current.col].symbol)) {

                    coordinate newCor(static_cast<uint32_t>(currSym - '0'), current.row, current.col);
                    sc.push_back(newCor);
                    ++discovered;

                    map[static_cast<uint32_t>(currSym - '0')][current.row][current.col].discovered = true;
                    map[static_cast<uint32_t>(currSym - '0')][current.row][current.col].move = static_cast<char>(current.room + '0');
                }
            }
        }//Investigate Pipe

        else {
                if (current.row > 0 && !found) {
                    if (!map[current.room][current.row - 1][current.col].discovered && 
                        isValidChar(map[current.room][current.row - 1][current.col].symbol)) {
                        coordinate newCor(current.room, current.row - 1, current.col);
                        sc.push_back(newCor);
                        ++discovered;

                        map[current.room][current.row - 1][current.col].discovered = true;
                        map[current.room][current.row - 1][current.col].move = 'n';
                    }
                } //Investigate North

                if (current.col < length - 1 && !found) {
                    if (!map[current.room][current.row][current.col + 1].discovered && 
                            isValidChar(map[current.room][current.row][current.col + 1].symbol)) {
                        coordinate newCor(current.room, current.row, current.col + 1);
                        sc.push_back(newCor);
                        ++discovered;

                        map[current.room][current.row][current.col + 1].discovered = true;
                        map[current.room][current.row][current.col + 1].move = 'e';
                    }
                } //Investigate East

                if (current.row < length - 1 && !found) {
                    if (!map[current.room][current.row + 1][current.col].discovered && 
                            isValidChar(map[current.room][current.row + 1][current.col].symbol)) {
                        coordinate newCor(current.room, current.row + 1, current.col);
                        sc.push_back(newCor);
                        ++discovered;

                        map[current.room][current.row + 1][current.col].discovered = true;
                        map[current.room][current.row + 1][current.col].move = 's';
                    }
                } //Investigate South

                if (current.col > 0 && !found) {
                    if (!map[current.room][current.row][current.col - 1].discovered && 
                            isValidChar(map[current.room][current.row][current.col - 1].symbol)) {
                        coordinate newCor(current.room, current.row, current.col - 1);
                        sc.push_back(newCor);
                        ++discovered;

                        map[current.room][current.row][current.col - 1].discovered = true;
                        map[current.room][current.row][current.col - 1].move = 'w';
                    }
                }//Investigate West
        }
    }
}

void Marco::backTrace() {
    coordinate current;
    current = countess;
    path.push_back(current);

    char currMove = map[current.room][current.row][current.col].move;
    while (currMove != 'X') {
        if ((currMove >= '0') && (currMove <= '9')) {
            coordinate newCor(static_cast<uint32_t>(currMove - '0'), current.row, current.col);
            path.push_back(newCor);
        }
        else if (currMove == 'n') {
            coordinate newCor(current.room, current.row + 1, current.col);
            path.push_back(newCor);
        }
        else if (currMove == 'e') {
            coordinate newCor(current.room, current.row, current.col - 1);
            path.push_back(newCor);
        }
        else if (currMove == 's') {
            coordinate newCor(current.room, current.row - 1, current.col);
            path.push_back(newCor);
        }
        else if (currMove == 'w') {
            coordinate newCor(current.room, current.row, current.col + 1);
            path.push_back(newCor);
        }

        current = path.back();
        currMove = map[current.room][current.row][current.col].move;
    }
}

void Marco::output() {
    if (!found) {
        cout << "No solution, " << discovered << " tiles discovered." << endl;
        return; 
    }
    else {
        backTrace(); 

        if (clMode) {
            cout << "Path taken:\n";
            while (path.size() > 1) {

                cout << "(" << path.back().room << "," << path.back().row << "," << path.back().col << ",";

                path.pop_back();
                char pathSymbol = map[path.back().room][path.back().row][path.back().col].move;

                if ((pathSymbol >= '0') && (pathSymbol <= '9')) {
                    cout << "p";
                }
                else {
                    cout << pathSymbol; 
                }

                cout << ")\n";
            }
        }
        else {
            while (path.size() > 1) {
                coordinate current(path.back().room, path.back().row, path.back().col);
                path.pop_back();
                char symbol = map[path.back().room][path.back().row][path.back().col].move;
                if ((symbol >= '0') && (symbol <= '9')) {
                    map[current.room][current.row][current.col].symbol = 'p';
                }
                else {
                    map[current.room][current.row][current.col].symbol = symbol;
                }
            }
            cout << "Start in room " << start.room << ", row " << start.row << ", column " << start.col << "\n";

            for (uint32_t room = 0; room < numRooms; room++) {
                cout << "//castle room " << room << "\n";
                for (uint32_t row = 0; row < length; row++) {
                    for (uint32_t col = 0; col < length; col++) {
                        cout << map[room][row][col].symbol;
                    } 
                    cout << "\n";  
                }
            }
        }
    }
}