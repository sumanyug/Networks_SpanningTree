#include "bridge.h"
Bridge :: Bridge(int pos,  li &vals){

	root_port = pos;
	fields = vals;
	state.resize(vals.size(),0);



}

