#include "bridge.h"
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

Bridge ::Bridge(int pos, li &vals)
{
	fields = vals;
	st.resize((int)vals.size(), 0); //Initially all are DPs
}
void Bridge::update_state()
{
	//go through received messages.
	//update the Message configuring the message.
	//Create new configuration Message.
	//If new configuration message > any of the received configuration messages, the given port is not the designation port.
}