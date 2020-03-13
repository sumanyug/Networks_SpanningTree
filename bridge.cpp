#include "bridge.h"
#include <iterator>
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

Message Message::increment_distance()
{
	Message new_Mess(rp, d + 1, sp);
	return new_Mess;
}

Bridge ::Bridge()
{
	pos = -1;
}

Bridge ::Bridge(int pos, li &vals)
{
	this->pos = pos;
	fields = vals;
	st.resize((int)vals.size(), 0); //Initially all are DPs
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
	int pos = 0, it_pos = 0;
	for (it = rm.begin(); it != rm.end(); it++)
	{
		st[it_pos] = 0;
		if (*it < Config_mes)
		{
			Config_mes = *it; //update the Message configuring the message.

			pos = it_pos;
		}
		it_pos++;
	}

	st[pos] = 2;
	Distri_mes = Config_mes.increment_distance(); //Create new configuration Message.

	//If new configuration message > any of the received configuration messages, the given port is not the designation port.
	pos = 0, it_pos = 0;
	for (it = rm.begin(); it != rm.end(); it++)
	{

		if (*it < Distri_mes && st[it_pos] != 2)
		{
			st[it_pos] = 1;
		}
		it_pos++;
	}
}

li Bridge::send_config_message()
{

	li new_messages;

	for (size_t i = 0; i < st.size(); i++)
	{
		if (st[i] == 0)
		{
			new_messages.push_back(fields[i]);
		}
	}

	return new_messages;
}

Message Bridge::out_config_mes()
{
	return Distri_mes;
}
