#include"gates.h"
#include"get_inputs.cpp"
// #include"Part_A.cpp"
#include"Part_B.cpp"

int main(){
    Gate_Variants* variants = read_gateVarient_from("gate_delays.txt");
    Circuit* test= read_circuit_from("given_loop.txt");
    long double maxT = read_gate_delay("delay_constraint.txt");
    // test->print();
    // set_Gates_to_min(test,variants);
    // cout<<Finding_max_delay_in_a_circuit(test,std::numeric_limits<double long>::infinity());
    cout<<part_b(test,variants,7);
}
