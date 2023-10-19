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

void Gate_States::insert(const long double& time,const long double& area){
    if(time==std::numeric_limits<double long>::infinity()){return;}
    long double old_time, old_area;
    if(area<=A_low_area){
        old_time=T_low_area; old_area=A_low_area;
        T_low_area=time; A_low_area=area;
        insert(old_time,old_area);
        return;
    }
    else if (area<=A_mid_area){
        old_time=T_mid_area; old_area=A_mid_area;
        T_mid_area=time; A_mid_area=area;
        insert(old_time,old_area);
        return;
    }
    else {
        T_high_area=time; A_high_area=area;
        return;
    }
}

long double Gate_States::make_state(unsigned short a, Gate* g) const{
    switch (a)
    {
    case 0:
        g->time=T_low_area;
        g->area=A_low_area;
        return A_low_area;
        break;
    case 1:
        g->time=T_mid_area;
        g->area=A_mid_area;
        return A_mid_area;
        break;
    case 2:
        g->time=T_high_area;
        g->area=A_high_area;
        return A_high_area;
        break;
    }
    return -2;
}

long double Gate_Variants::make_state(unsigned short a, Gate* g) const{
    switch (g->gatetype)
    {
    case andGATE:
        return andStates.make_state(a,g);
    case orGATE:
        return orStates.make_state(a,g);
    case nandGATE:
        return nandStates.make_state(a,g);
    case norGATE:
        return norStates.make_state(a,g);
    case notGATE:
        return notStates.make_state(a,g);
    case dffGATE:
        return 0;
    }
    return -1;
}
