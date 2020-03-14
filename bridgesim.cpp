#include "bridge.h"
#include <iostream>
#include <string>

#include <iterator>
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
    // cout << n_bridges << endl;

    bridges.resize(n_bridges + 1);
    config_has_b1.resize(n_bridges + 1, false);
    config_has_b1[0] = true;
    config_has_b1[1] = true;
    string inp;
    cin.ignore();
    int count = 0;

    for (int i = 1; i <= n_bridges; i++)
    {
        // cout << "I entered this loop body!" << endl;
        getline(cin, inp);
        // cout << inp << endl;
        int count_map = 0;
        int bn;
        bool name_bridge = false;
        for (int j = 0; j < inp.size(); j++)
        {
            // cout << inp[j] << endl;
            if (!name_bridge)
            {
                if (j == 0)
                    continue;
                else
                {
                    if (j == 1)
                    {
                        bn = stoi(inp.substr(1, inp.find(':') - 1));
                        bridges[i].pos = bn;
                        bridges[i].set_init_config_message();
                        name_bridge = true;

                        continue;
                    }
                    else
                    {
                        cout << "Huge error!";
                        break;
                    }
                }
            }
            if (inp[j] == ' ' || inp[j] == ':')
                continue;

            bridges[i].lan_status[inp[j]] = 0;
            lan_def[inp[j]].push_back(bn);

            // cout << inp[j] << " ";
        }
        // cout << endl;
    }

    // for (map<char, vector<int>>::iterator i = lan_def.begin(); i != lan_def.end(); i++)
    // {
    //     cout << i->first << ":"
    //          << " ";
    //     for (vector<int>::iterator it = i->second.begin(); it != i->second.end(); it++)
    //     {
    //         cout << *it << " ";
    //     }
    //     cout << endl;
    // }
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
            // cout << "Entered the body which adjusts received messages" << endl;

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
        // cout << "New config messages have been received by the machines" << endl;
        // cout << endl
        //      << endl
        //      << endl;
        // output_bridges();
        // cout << endl
        //      << endl
        //      << endl;
        // cout << "Exited the body for good." << endl;
        //Over here, essentially add the new messages received from
        for (int i = 1; i <= n_bridges; i++)
        {
            bridges[i].update_state();
        }
        // cout << "New config messages have been used to update their states" << endl;
        // cout << endl
        //      << endl
        //      << endl;
        // output_bridges();
        // cout << endl
        //      << endl
        //      << endl;
        //Process for sending messages
        message_on_lan = empty_lan;
        for (int i = 1; i <= n_bridges; i++)
        {
            if (config_has_b1[bridges[i].pos] == false && bridges[i].out_config_mes().rp == 1)
            {
                count_b1++;
                config_has_b1[i] = true;
            }
            li inp = bridges[i].send_config_message();

            for (li::iterator it = inp.begin(); it != inp.end(); ++it)
            {

                message_on_lan[*it].push_back(bridges[i].out_config_mes());
            }
        }

        // cout << "New config messages have been sent by the machines" << endl;
        // cout << endl
        //      << endl
        //      << endl;
        // output_bridges();
        // cout << endl
        //      << endl
        //      << endl;
        // cout << count_b1 << endl;

        if (!(count_b1 < n_bridges))
            count_to_all_update++;
    }
}