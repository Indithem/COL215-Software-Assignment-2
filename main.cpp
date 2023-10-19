#include"gates.h"
#include"get_inputs.cpp"
#include"Part_A.cpp"

int main(){
    Gate_Variants* variants = read_gateVarient_from("gate_delays.txt");
    Circuit* test= read_circuit_from("circuit.txt");
    test->print();
    set_Gates_to_min(test,variants);
    cout<<Finding_max_delay_in_a_circuit(test);
}
