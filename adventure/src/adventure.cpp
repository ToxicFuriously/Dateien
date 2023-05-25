#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "adventure.h"

using namespace std;

Adventure *Adventure::instance = nullptr;

Adventure *Adventure::get_instance()
{
    if (!instance)
    {
        instance = new Adventure();
    }
    return instance;
}

Adventure::Adventure()
{
    srand(time(NULL));
    // Load Messages
    //../data/messages.txt

    // Load Actions
    //../data/actions.txt

    // Load Loaction Headers
    //../data/headers.txt

    // Load Loaction Descriptions
    //../data/descriptions.txt

    // Load Travel
    //../data/travel.txt

    // Load Objects
    //../data/objects.txt

    // Load Object Locations
    //../data/locations.txt
}

Adventure::~Adventure()
{
    for (vector<Text *>::iterator it = messages.begin(); it != messages.end(); ++it)
    {
        delete *it;
    }
    for (map<int, Text *>::iterator it = actions.begin(); it != actions.end(); ++it)
    {
        delete it->second;
    }
    for (vector<Location *>::iterator it = locations.begin(); it != locations.end(); ++it)
    {
        delete *it;
    }
    for (vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it)
    {
        delete *it;
    }
}

void Adventure::start_adventuring()
{
    cout << "\033[2J";
    cout << messages[0]->get() << endl;
    cout << endl
         << "Press a key to start your adventure." << endl;
    cin.get();
    // cin.get();
    int current = 1;
    while (current != -1)
    {
        current = visit(current);
    }
}

int Adventure::visit(int location)
{
    Location *l = locations[location - 1];
    cout << "\033[2J";
    l->print();
    int dest_menu_offset = 1;
    int obj_menu_offset = l->print_destinations(dest_menu_offset);
    int menu_end = l->print_object_actions(obj_menu_offset);

    int target = -1;
        cout << "Select your action: ";
        cin >> target;
        if (target >= dest_menu_offset && target < obj_menu_offset)
        {
            target = target - dest_menu_offset;
        }
        else if (target >= obj_menu_offset && target < menu_end)
        {
            Object *obj = l->remove_movable_object(target - obj_menu_offset);
            inventory.push_back(obj);
            return location;
        }
        else
        {
            cout << "Please select a action from the list!" << endl;
            cout << endl
                 << "Press a key to continue your adventure." << endl;
            cin.get();
            return location;
        }

        Travel *dest = l->get_destination(target);
        //dest->print(-1);
        //cin.get();
        //cin.get();
        int m = dest->get_condition();
        int n = dest->get_target();
        bool condition_fulfilled = true;
        if (m != 0)
        {
            if (m > 0 && m < 100)
            {
                // IT IS DONE WITH M% PROBABILITY
                if (rand() % 100 > m)
                {
                    condition_fulfilled = false;
                }
            }
            else if (m == 100)
            {
                // UNCONDITIONAL, BUT FORBIDDEN TO DWARVES.
            }
            else if (m > 100 && m <= 200)
            {
                // HE MUST BE CARRYING OBJECT M-100.
                if (!is_carring(m - 100))
                {
                    condition_fulfilled = false;
                }
            }
            else if (m > 200 && m <= 300)
            {
                // MUST BE CARRYING OR IN SAME ROOM AS M-200
                if (!is_carring(m - 200) && !l->has_object(m - 200))
                {
                    condition_fulfilled = false;
                }
            }
            else
            {
                // PROP(M MOD 100) MUST *NOT* BE X
                // X = ceil(M / 100.0) - 4
                if(objects[m % 100]->get_state() == ceil(m / 100.0) - 4)
                {
                    condition_fulfilled = false;
                }
            }
        }

        if (!condition_fulfilled)
        {
            dest = l->get_alternative_destination(dest);
            n = dest->get_target();
        }

        if (n > 500)
        {
            cout << "\033[2J";
            cin.clear();
            cout << messages[n - 500 - 1]->get() << endl;
            cout << endl
                 << "Press a key to continue your adventure." << endl;
            cin.get();
            cin.get();
            return location;
        }
    return n;
}

bool Adventure::is_carring(int object)
{
    //im inventory suchen
    return false;
}