#include <vector>
#include <map>
#include <utility>
typedef std::vector<char> li;
typedef std::vector<int> state;
struct Message
{

public:
	int rp, d, sp;
	bool operator<(const Message &rhs) const;
	bool operator>(const Message &rhs) const;
	bool operator==(const Message &rhs) const;
	Message operator=(const Message &rhs);
	Message();
	Message(int rp_in, int d_in, int sp_in);
	Message increment_distance(int newpos);

}; // A message has been defined.
typedef std::vector<std::pair<char, Message>> received_messages;

class Bridge
{
public:
	int pos;
	received_messages rm;
	std::map<char, int> lan_status;

	//
	// s-> 0 --- Designated Port
	// s-> 1 --- Null Port  //No traffic to be sent
	// s-> 2 --- Configuration Port // No traffic to be sent.
	//
	Message Config_mes, Distri_mes;
	Bridge();
	Bridge(int pos, li &vals);
	void set_init_config_message();
	void update_state(); // this will update the output configuration messages that are to be sent.
	li send_config_message();
	Message out_config_mes();
	void output();
}; //A bridge with basic functionalities defined.

class Network
{
	int t;
	int n_bridges;
	int no_simulations;
	// string inp;

	std::vector<Bridge> bridges, bridges_2; //this will store all the bridges.
	std::vector<bool> config_has_b1;
	li int_to_lan;
	std::map<char, std::vector<int>> lan_def, lan_to_host;

public:
	Network();
	void input_t();
	void input_bridges();
	void process_bridges();
	void output_bridges();
	// void input_lan();
	// void process_lan();
};
