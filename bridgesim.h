#include <vector>
#include <map>
#include <utility>
#include <set>
#include "bridge.h"
class Network
{
    int t;
    int n_bridges;
    int no_simulations;
    // string inp;

    std::vector<Bridge> bridges, bridges_2; //this will store all the bridges.
    std::vector<bool> config_has_b1;
    std::vector<std::pair<int, int>> host_to_host;
    std::vector<char> host_to_lan;
    std::map<char, std::vector<int>> lan_def;
    std::map<char, std::vector<int>> final_lan_def;
    std::map<char, std::vector<int>> lan_to_host;

public:
    Network();
    void input_t();
    void input_bridges();
    void process_bridges();
    void output_bridges();
    void setup_final_lan();
    void process_lan();
};