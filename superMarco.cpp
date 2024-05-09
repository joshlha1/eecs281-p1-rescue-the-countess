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
    bool mapMode = false;
    bool clMode = false; //coordinate list

    /*
    -Need to store start/countess location -- best way to store location??
    -Tile constructor?
    */

    public: 

    struct tile {
        char symbol;
        bool discovered = false;
        char prev = 'X'; //Where we moved from i.e north/south/east/west or floor#
    };

    vector<vector<vector<tile>>> map;

    char inputMode; 
    uint32_t numRooms = 0;
    uint32_t length = 0;

    void getOptions(int argc, char** argv);
    void readMap();
    void search(); 
};

int main(int argc, char **argv) {
    ios_base::sync_with_stdio(false);

    Marco myMarco; 

    myMarco.getOptions(argc, argv);

    myMarco.readMap();

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
                std::cout << "<Insert helpful message here>\n";
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
                    while (symbol == '/') {
                        getline(cin, junk);
                        cin >> symbol;
                    }
                        map[room][row][col].symbol;
                }
            }
        }
    }

    else {
        //Reading List Mode
        uint32_t room = 0;
        uint32_t row = 0;
        uint32_t col = 0;
        char trash;

        while (cin >> symbol) {
            if (symbol == '/') {
                getline(cin, junk);
                cin >> symbol;
            }
            else {
                cin >> trash >> room >> trash >> row 
                >> trash >> col >> trash >> symbol >> trash;
            }
        }
    }
}

