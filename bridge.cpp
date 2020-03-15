#include "bridge.h"
#include <iostream>
#include <iterator>
using namespace std;
bool Message::operator<(const Message &rhs) const
{
	if (rp < rhs.rp)
	{
		return true;
	}
	else
	{
		if (rp == rhs.rp)
		{
			if (d < rhs.d)
			{
				return true;
			}
			else
			{
				if (d == rhs.d)
				{
					if (sp < rhs.sp)
						return true;
				}
			}
		}
	}

	return false;
}

bool Message::operator>(const Message &rhs) const
{
	return rhs < *this;
}

bool Message ::operator==(const Message &rhs) const
{
	return rp == rhs.rp && d == rhs.d && sp == rhs.sp;
}

Message Message::operator=(const Message &rhs)
{
	rp = rhs.rp;
	d = rhs.d;
	sp = rhs.sp;

	return *this;
}
Message::Message()
{
	rp = -1;
	d = -1;
	sp = -1;
}
Message::Message(int rp_in, int d_in, int sp_in)
{
	rp = rp_in;
	d = d_in;
	sp = sp_in;
}

Message Message::increment_distance(int newpos)
{
	Message new_Mess(rp, d + 1, newpos);
	return new_Mess;
}
//----------------------------------------------------------

Bridge ::Bridge()
{
	pos = -1;
}

Bridge ::Bridge(int pos, li &vals)
{
	this->pos = pos;
}

void Bridge::set_init_config_message()
{
	Message conf(pos, -1, -1), dist(pos, 0, pos);
	Config_mes = conf;
	Distri_mes = dist;
}

void Bridge::update_state()
{

	//go through received messages.

	received_messages ::iterator it;
	char new_pos = '0';
	bool new_pos_obtained = false;
	for (it = rm.begin(); it != rm.end(); it++)
	{
		char lan_from = it->first;

		if (it->second < Config_mes)
		{
			new_pos_obtained = true;

			Config_mes = it->second; //update the Message configuring the message.

			new_pos = lan_from;
		}
	}

	if (new_pos_obtained)
	{
		// cout << "New position has been found for bridge " << pos << endl;

		for (it = rm.begin(); it != rm.end(); it++)
		{
			char lan_from = it->first;
			lan_status[lan_from] = 0;
		}

		lan_status[new_pos] = 2;
		Distri_mes = Config_mes.increment_distance(pos); //Create new configuration Message.
	}
	//If new configuration message > any of the received configuration messages, the given port is not the designation port.
	for (it = rm.begin(); it != rm.end(); it++)
	{
		char lan_from = it->first;
		if (it->second < Distri_mes && lan_status[lan_from] != 2)
		{
			lan_status[lan_from] = 1;
		}
	}

	rm.resize(0);
}

li Bridge::send_config_message()
{

	li new_messages;
	map<char, int>::iterator it;

	for (it = lan_status.begin(); it != lan_status.end(); it++)
	{
		if (it->second == 0)
		{
			new_messages.push_back(it->first);
			// cout << it->first << " ";
		}
	}
	// cout << "(" << Distri_mes.rp << "," << Distri_mes.d << "," << Distri_mes.sp << ")" << endl;

	return new_messages;
}

Message Bridge::out_config_mes()
{
	return Distri_mes;
}

void Bridge::output()
{
	cout << "B" << pos << ": ";

	map<char, int>::iterator it;

	for (it = lan_status.begin(); it != lan_status.end(); it++)
	{
		cout << it->first << "-";
		int val = it->second;
		if (val == 0)
			cout << "DP";
		if (val == 1)
			cout << "NP";
		if (val == 2)
			cout << "RP";
		if (it != --lan_status.end())
			cout << " ";
	}
	cout << endl;
	// cout << " Config:"
	// 	 << "(" << Config_mes.rp << "," << Config_mes.d << "," << Config_mes.sp << ")"
	// 	 << "|||";

	// cout << "Distri:"
	// 	 << "(" << Distri_mes.rp << "," << Distri_mes.d << "," << Distri_mes.sp << ")" << endl;
}

void Bridge::lan_send_to_setup()
{
	map<char, int>::iterator it;

	for (it = lan_status.begin(); it != lan_status.end(); ++it)
	{
		if (it->second == 0 || it->second == 2)
			lan_send_to.insert(it->first);
	}
}

set<char> Bridge::lan_forward_to(char input_lan, Data input_data)
{
	set<char> output_lans;
	if (learn_table.find(input_data.receiver) != learn_table.end())
	{
		char c = (learn_table.find(input_data.receiver))->second;
		if (c != input_lan)
			output_lans.insert((learn_table.find(input_data.receiver))->second);
	}
	else
	{

		output_lans = lan_send_to;
		output_lans.erase(output_lans.find(input_lan));
	}

	learn_table[input_data.sender] = input_lan;
	forwarding_data = input_data;
	forwarding_data.bridge = pos;
	return output_lans;
}

set<char> Bridge::sending_table_setup()
{
	return lan_send_to;
}

void Bridge::print_learning_table()
{
	cout << "B" << pos << ":" << endl
		 << "HOST ID | FORWARDING PORT" << endl;
	for (map<int, char>::iterator i = learn_table.begin(); i != learn_table.end(); i++)
	{
		cout << "H" << i->first << " | " << i->second << endl;
	}
}