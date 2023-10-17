#include"gates.h"
#include<cassert>


void Signal::connect_to(Gate* g) {
    outputGates.push_back(g);
}

void Signal::connect_from(Gate* g) {
    if (sourceGate!=nullptr){
        std::cout<<"Signal "+name+" was connected from two inputs, previously connected to "+sourceGate->get_name();
        abort();
    }
    sourceGate=g;
}

Circuit::~Circuit(){
    for(auto s: primary_outs){delete s.second;}
    for(auto s: primary_inps){delete s.second;}
    for(auto s: internal){delete s.second;}
    for(Gate* g: Gates){delete g;}
}

void Circuit::print() const{
    using namespace std;

    cout<<"Printing circuit\nPrimary inps are\n";
    for(auto s: primary_inps){
        cout<<endl<<s.second->get_name()<<" connected to gates";
        for(auto gate:s.second->get_outs()){
            cout<<" "+gate->get_name();
        }
    }
    cout<<"\n\nPrimaryouts are";
    for(auto s: primary_outs){
        cout<<endl<<s.second->get_name()<<" connected from gate ";
        cout<<s.second->get_source()->get_name();
    }
    cout<<"\n\nINternals are";
    for(auto s: internal){
        cout<<endl<<s.second->get_name()<<" connected from gate ";
        cout<<s.second->get_source()->get_name();
        cout<<endl<<s.second->get_name()<<" connected to gates";
        for(auto gate:s.second->get_outs()){
            cout<<" "+gate->get_name();
        }
    }

    cout<<"\n\nGates are\n";
    for(auto g:Gates){
        cout<<g->output->get_name()<<"<--"<<g->get_name()<<"from "<<g->input1->get_name()<<" and ";
        if(g->input2!=nullptr){cout<<g->input2->get_name();}
        
        cout<<"\n";
    }
}