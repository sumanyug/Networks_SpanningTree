#include "bridgesim.h"
#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>
using namespace std;

Network::Network()
{
    t = 0;
    bridges.resize(0);
}

void Network::input_t()
{
    cin >> t;
}

void Network::input_bridges()
{
    cin >> n_bridges;
    bridges.resize(n_bridges + 1);
    config_has_b1.resize(n_bridges + 1, false);

    //------------------------------------------
    config_has_b1[0] = true;
    config_has_b1[1] = true;
    string inp;
    int count = 0;
    int count_map = 0;
    int bn;
    bool name_bridge = false, all_bridges_done = false;
    vector<vector<char>> inp_bridge;
    if (n_bridges >= 1)
    {
        cin >> inp;
    }
    int i = 1;
    while (i <= n_bridges)
    {
        bn = stoi(inp.substr(1, inp.find(':') - 1));
        name_bridge = true;
        bridges[i].pos = bn;
        bridges[i].set_init_config_message();
        while (true)
        {
            cin >> inp;
            if (inp.size() > 1)
                break;
            bridges[i].lan_status[inp[0]] = 0;
            lan_def[inp[0]].push_back(bn);
        }
        i++;
    }
    i = 0;
    char c;
    int max = 0;
    while (i < lan_def.size())
    {
        c = inp[0];
        vector<int> hosts;
        while (true)
        {
            cin >> inp;
            if (inp[0] >= '0' && inp[0] <= '9')
                break;
            if (inp[1] == ':')
                break;
            int hn = stoi(inp.substr(1, inp.size() - 1));
            lan_to_host[c].push_back(hn);
            max = max > hn ? max : hn;
        }
        i++;
    }
    host_to_lan.resize(max + 1);

    for (map<char, vector<int>>::iterator it = lan_to_host.begin(); it != lan_to_host.end(); it++)
    {
        for (vector<int>::iterator jt = (it->second).begin(); jt != (it->second).end(); jt++)
        {
            host_to_lan[*jt] = it->first;
        }
    }

    no_simulations = stoi(inp);

    for (int i = 0; i < no_simulations; i++)
    {
        char c1, c2;
        int t1, t2;
        cin >> c1 >> t1 >> c2 >> t2;
        pair<int, int> p(t1, t2);
        host_to_host.push_back(p);
    }
}

void Network::output_bridges()
{
    for (int i = 1; i <= n_bridges; i++)
    {
        bridges[i].output();
    }
}

void Network::process_bridges()
{
    int tr_count = 0;
    int count_b1 = 1;
    map<char, vector<Message>> message_on_lan, empty_lan;
    int count_to_all_update = -1;
    while (count_to_all_update < 1)
    {

        //Process for received messages
        for (map<char, vector<Message>>::iterator i = message_on_lan.begin(); i != message_on_lan.end(); i++)
        {

            vector<Message> ms = i->second;
            vector<int> positions = lan_def[i->first];
            for (vector<int>::iterator j = positions.begin(); j != positions.end(); j++)
            {
                for (vector<Message>::iterator k = ms.begin(); k != ms.end(); k++)
                {
                    if (*j != k->sp)
                    {
                        pair<char, Message> value(i->first, *k);
                        bridges[*j].rm.push_back(value);
                    }
                }
            }
        }
        for (int i = 1; i <= n_bridges; i++)
        {
            bridges[i].update_state(t, tr_count);
        }
        message_on_lan = empty_lan;
        for (int i = 1; i <= n_bridges; i++)
        {
            if (config_has_b1[bridges[i].pos] == false && bridges[i].out_config_mes().rp == 1)
            {
                count_b1++;
                config_has_b1[i] = true;
            }
            std::vector<char> inp = bridges[i].send_config_message();
            if (t == 1)
            {
                for (size_t k = 0; k < inp.size(); k++)
                {
                    cout << tr_count << " s B" << bridges[i].pos << " "
                         << "(B" << bridges[i].out_config_mes().rp << ","
                         << " " << bridges[i].out_config_mes().d << ", B" << bridges[i].Distri_mes.sp << ")" << endl;
                }
            }
            for (std::vector<char>::iterator it = inp.begin(); it != inp.end(); ++it)
            {

                message_on_lan[*it].push_back(bridges[i].out_config_mes());
            }
        }
        if (!(count_b1 < n_bridges))
            count_to_all_update++;
        tr_count++;
    }
}

void Network::setup_final_lan()
{
    for (int i = 1; i <= n_bridges; i++)
    {
        bridges[i].lan_send_to_setup();
        set<char> lans = bridges[i].sending_table_setup();
        //------------------------------------------------
        for (set<char>::iterator it = lans.begin(); it != lans.end(); ++it)
        {
            final_lan_def[*it].push_back(i);
        }
    } //final lan set up
}

void Network::process_lan()
{

    for (int i = 0; i < no_simulations; i++)
    {
        int starting_port = host_to_host[i].first;
        int ending_port = host_to_host[i].second;
        int tcount = 1;

        Data init_ping;
        init_ping.sender = host_to_host[i].first;
        init_ping.receiver = host_to_host[i].second;
        init_ping.bridge = -1;

        map<char, Data> all_messages;

        char init_lan = host_to_lan[init_ping.sender];

        all_messages[init_lan] = init_ping;
        while (!all_messages.empty())
        {
            //step 1 -> Go through all the Data that has been passed through. Give it to bridge.
            map<char, Data> new_all_messages;
            vector<pair<int, char>> output_t1, output_t2;

            if (t == 1)
            {
                for (map<char, Data>::iterator it = all_messages.begin(); it != all_messages.end(); it++)
                {
                    if (t == 1)
                    {
                        for (vector<int>::iterator jt = final_lan_def[it->first].begin(); jt != final_lan_def[it->first].end(); jt++)
                        {
                            if ((it->second).bridge != *jt)
                            {
                                pair<char, int> val(bridges[i].pos, it->first);
                                output_t1.push_back(val);
                            }
                        }
                    }
                }

                sort(output_t1.begin(), output_t1.end());
                for (int i = 0; i < output_t1.size(); i++)
                {
                    cout << tcount << " r B" << output_t1[i].first << " " << output_t1[i].second << endl;
                }
            }
            for (map<char, Data>::iterator it = all_messages.begin(); it != all_messages.end(); it++)
            {
                for (vector<int>::iterator jt = final_lan_def[it->first].begin(); jt != final_lan_def[it->first].end(); jt++)
                {
                    if ((it->second).bridge != *jt)
                    {
                        //step 2 -> Let the bridge learn the new port and forward the messages as required.

                        set<char> new_set_messages = bridges[*jt].lan_forward_to(it->first, it->second);
                        //add_new_message

                        for (set<char>::iterator kt = new_set_messages.begin(); kt != new_set_messages.end(); ++kt)
                        {
                            pair<int, char> val2(*jt, *kt);
                            output_t2.push_back(val2);
                            new_all_messages[*kt] = bridges[*jt].forwarding_data;
                        }
                    }
                }
            }

            if (t == 1)
            {
                sort(output_t2.begin(), output_t2.end());
                for (size_t i = 0; i < output_t2.size(); i++)
                {
                    cout << tcount << " s B" << output_t1[i].first << " " << output_t1[i].second << endl;
                }
            }

            //step 3 -> Add all the messages to the new lan.
            all_messages = new_all_messages;
            tcount++;
        }

        for (int i = 1; i <= n_bridges; i++)
        {
            bridges[i].print_learning_table();
        }
        cout << endl;
    }
}