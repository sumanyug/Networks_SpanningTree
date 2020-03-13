#include <vector>
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
	Message increment_distance();

}; // A message has been defined.
typedef std::vector<Message> received_messages;

class Bridge
{
public:
	int pos;
	li fields;
	state st;
	received_messages rm;

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
}; //A bridge with basic functionalities defined.
