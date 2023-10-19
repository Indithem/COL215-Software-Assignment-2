#include"gates.h"
#include"get_inputs.cpp"
#include"Part_B.cpp"

int main(int argn, char* argv[]){
    // char part = *argv[1];
    // string circuit_file = argv[2];
    // string gate_delay = argv[3];
    // string longest_del_fp = argv[4];

    char part = 'B';
    string circuit_file = "circuit.txt";
    string gate_delay = "gate_delays.txt";
    string longest_del_fp = "longest_delay.txt";


    Circuit* circuit = read_circuit_from(circuit_file);
    Gate_Variants* vars = read_gateVarient_from(gate_delay);
    long double maxT = read_gate_delay(longest_del_fp);

    if (part=='A'){
        set_Gates_to_min(circuit,vars);
        write_to_file("longest_delay.txt",Finding_max_delay_in_a_circuit(circuit,std::numeric_limits<double long>::infinity()));
    }
    else if (part=='B'){
        write_to_file("minimum_area.txt",part_b(circuit,vars,maxT));
    }
    else{
        cout<<part<<" is not in valid format(A|B)...assuming part A";
        set_Gates_to_min(circuit,vars);
        write_to_file("longest_delay.txt",Finding_max_delay_in_a_circuit(circuit,std::numeric_limits<double long>::infinity()));
    }
}
