#include"Part_B_helper.cpp"
#include"Part_A.cpp"

void inline print_progress(const string& s,const long double& ans){
    // float progress;
    // int n=s.length();

    // int i=0;
    // for(i=0;i<n;i++){
    //     if(s[i]=='0'){break;}
    // }


    // int barWidth = 70;

    std::cout << "minimum area found so far..."<<ans
    <<"\n";
//     int pos = barWidth * i/n;
//     for (int i = 0; i < barWidth; ++i) {
//         if (i < pos) std::cout << "=";
//         else if (i == pos) std::cout << ">";
//         else std::cout << " ";
//     }
//     std::cout << "] " << i << " %\r";
    std::cout.flush();
//     std::cout << std::endl;
}

long double part_b(Circuit* inp_circuit, Gate_Variants* variants, long double time_constraint){

    const int n = inp_circuit->Gates.size();
    string iter_str=string(n,'2');

    double long current_valid_area = std::numeric_limits<double long>::infinity();
    
    
    while (iter_str!="LOOP COMPLETED")
    {
    helper next_iter = Calculating_suitable_area(iter_str,inp_circuit,variants,current_valid_area);

    if(Finding_max_delay_in_a_circuit(inp_circuit,time_constraint)<=time_constraint){
        current_valid_area=min(next_iter.iter_area,current_valid_area);
        print_progress(iter_str,current_valid_area);   
        write_to_file("minimum_area.txt",current_valid_area);
    }

    iter_str=next_iter.next_iter;
    }

    return current_valid_area;

}