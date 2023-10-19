/* Helpers
inputs:i(base3),Ciruit,gate_variants,Area
Outputs:Struct{next i,Area}*/
#include"gates.h"
using namespace std;
struct helper{
    string next_iter;
    long double iter_area;
};
helper Calculating_suitable_area(string iterator,Circuit*given_circuit,Gate_Variants*types_of_gates,double long Present_valid_area){
    string present_iterator=iterator;
    vector<Gate*>types_ofgates=given_circuit->Gates;
    double long checking_area=std::numeric_limits<double long>::infinity();
    bool check_completion;
    do{
        checking_area=0;
        for(int i=0;i<present_iterator.size();i++){
            checking_area+=types_of_gates->make_state(present_iterator[i],types_ofgates[i]);
            if(checking_area>=Present_valid_area){
                break;
            }
        }
        check_completion=Add_1(present_iterator);
    }while(check_completion && checking_area>=Present_valid_area);
    helper output;
    if(check_completion==0){
        if(checking_area<Present_valid_area){
            output.next_iter="LOOP COMPLETED";
            output.iter_area=checking_area;
        }
        else{
            output.next_iter="LOOP COMPLETED";
            output.iter_area=std::numeric_limits<double long>::infinity();
        }
    }
    else{
        output.next_iter=present_iterator;
        output.iter_area=checking_area;
    }
    return output;
}
bool Add_1(string& iterator){
    int checker;
    int carry=1;
    for(int i=0;i<iterator.size();i++){
        checker=(iterator[iterator.size()-1-i])-'0';
        checker=checker+carry;
        if(checker==3){
            carry=1;
            iterator[iterator.size()-1-i]='0';
        }
        else{
            carry=0;
            iterator[iterator.size()-1-i]=checker+'0';
        }
    }
    return (carry!=1);
}