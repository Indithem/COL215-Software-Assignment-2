#include "gates.h"
//return double value and takes a circuit pointer as input
#include<bits/stdc++.h>
using namespace std;
double Finding_max_delay_in_a_circuit(Circuit*given_circuit){
    unordered_map<string,Signal*>&out_puts_of_circuit=given_circuit->primary_outs;
    unordered_map<Signal*,long double>result_outvalues;
    result_outvalues[nullptr]=0;
    vector<Signal*>&dff_in_the_circuit=given_circuit->DFFInps;
    stack<Signal*>temporary_storage;
    for(int i=0;i<dff_in_the_circuit.size();i++){
        temporary_storage.push(dff_in_the_circuit[i]);
    }
    for (auto i = out_puts_of_circuit.begin(); i != out_puts_of_circuit.end(); i++){
        temporary_storage.push(i->second);
    }
    Signal*temp_store;
    double final_answer=0;
    while(temporary_storage.empty()!=true){
        temp_store=temporary_storage.top();
        if((result_outvalues.find(temp_store->get_source()->input1)!=result_outvalues.end())&&(result_outvalues.find(temp_store->get_source()->input2)!=result_outvalues.end())){
            result_outvalues[temp_store]=max(result_outvalues[temp_store->get_source()->input1],result_outvalues[temp_store->get_source()->input2])+temp_store->get_source()->time;
            if(final_answer<result_outvalues[temp_store]){
                final_answer=result_outvalues[temp_store];
            }
            temporary_storage.pop();
            continue;
        }
        if(result_outvalues.find(temp_store->get_source()->input1)==result_outvalues.end()){
            temporary_storage.push(temp_store->get_source()->input1);            
        }
        if(result_outvalues.find(temp_store->get_source()->input2)==result_outvalues.end()){
            temporary_storage.push(temp_store->get_source()->input2);
        }
    }
    return final_answer;
}

