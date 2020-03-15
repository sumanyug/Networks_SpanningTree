#include <vector>
#include <map>
#include <utility>
#include <set>
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
struct Data
{
	int sender, receiver, bridge;
};
class Bridge
{
public:
	int pos;
	received_messages rm;
	std::map<char, int> lan_status;
	std::set<char> lan_send_to;
	std::map<int, char> learn_table;
	char received_from;
	//
	// s-> 0 --- Designated Port
	// s-> 1 --- Null Port  //No traffic to be sent
	// s-> 2 --- Configuration Port // No traffic to be sent.
	//
	Message Config_mes, Distri_mes;

	Data forwarding_data;
	//-----------------------------------------
	Bridge();
	Bridge(int pos, li &vals);

	void set_init_config_message();
	void update_state(int t, int tr_count); // this will update the output configuration messages that are to be sent.
	li send_config_message();
	Message out_config_mes();
	void output();
	void lan_send_to_setup();
	std::set<char> sending_table_setup();
	std::set<char> lan_forward_to(char input_lan, Data input_data);
	void print_learning_table();

}; //A bridge with basic functionalities defined.
