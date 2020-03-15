#include "bridgesim.h"
int main()
{
    Network n;
    n.input_t();
    n.input_bridges();
    n.process_bridges();
    n.output_bridges();
    n.setup_final_lan();
    n.process_lan();
}
