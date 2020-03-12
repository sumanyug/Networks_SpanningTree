#include<vector>
#include<pair>
typedef std::vector<char> li;
typedef std::vector<int> state;
struct Message{

	public:
		int rp,d,sp;

	bool operator < (const Message& rhs) const{
		if(rp<rhs.rp){
			return true;
		}
		else{
			if(rp == rhs.rp){
				if(d<rhs.d){
					return true;
				}
				else{
					if(d == rhs.d){
						if(sp<rhs.sp)
							return true;
					}
				}
			}
		}

		return false;
	}

	bool operator > ( const Message& rhs) const{
		return rhs < *this;

	}

	bool operator == (const Message& rhs) const{
		return rp == rhs.rp && d == rhs.d && sp == rhs.sp;
	}

	Message operator = (const Message& rhs){
		rp = rhs.rp;
		d = rhs.d;
		sp = rhs.sp;

		return *this;
	}
};
typedef vector<Message> received_messages;
class Bridge{
	public:
		li fields;
		state st;
		received_messages rm;

		//
		// s-> 0 --- Designated Port 
		// s-> 1 --- Null Port  //No traffic to be sent
		// s-> 2 --- Configuration Port // No traffic to be sent.
		//
		int root_port;//this will store the root port integer
		char root_port_lan;//this will store the LAN which connects it to the root port.
		int dist_root_port;


		Bridge(int pos,li& vals);



};
