#include"Part_B_helper.cpp"
#include"Part_A.cpp"

long double part_b(Circuit* inp_circuit, Gate_Variants* variants, long double time_constraint){

    const int n = inp_circuit->Gates.size();
    string iter_str=string(n,'0');

    double long current_valid_area = std::numeric_limits<double long>::infinity();
    
    
    while (iter_str!="LOOP COMPLETED")
    {
    helper next_iter = Calculating_suitable_area(iter_str,inp_circuit,variants,current_valid_area);

    if(Finding_max_delay_in_a_circuit(inp_circuit,time_constraint)<=time_constraint){
        current_valid_area=min(next_iter.iter_area,current_valid_area);
    }

    iter_str=next_iter.next_iter;
    }

    return current_valid_area;

}