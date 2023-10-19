#include<bits/stdc++.h>

#include<vector>
#include<string>
#include<iostream>
#include<unordered_map>
class Gate;

class Signal{
private:
    Gate* sourceGate=nullptr;
    std::vector<Gate*> outputGates;
    std::string name;

public:
    Signal(std::string n): name(n) {}

    ~Signal(){
        // delete sourceGate;
    }

    std::string get_name() const{return name;}
    void connect_to(Gate* g) ;
    void connect_from(Gate* g) ;
    
    std::vector<Gate*> get_outs() {return outputGates;}
    Gate* get_source(){return sourceGate;}
};

enum GateType:short unsigned{
    NOTDEFINED=0,
    andGATE=1,
    orGATE,
    nandGATE,
    norGATE,
    notGATE,
    dffGATE
};

const std::vector<std::string> GateTypeStr={"not defined yet","and","or","nand","nor","not","diff"};

class Gate{
public:
    Signal* input1=nullptr;
    Signal* input2=nullptr;
    Signal* output=nullptr;
    long double time=0;
    long double area=0;
    GateType gatetype=NOTDEFINED;

public:
    Gate(Signal* inp1, Signal* inp2, Signal* out, GateType t=NOTDEFINED)\
    :input1(inp1),input2(inp2),output(out),gatetype(t) {
        out->connect_from(this);
        inp1->connect_to(this);
        if (inp2!=nullptr){inp2->connect_to(this);}
    }

    ~Gate(){
        // delete input1, input2;
    }

    std::string get_name() const{return GateTypeStr[gatetype];}
    GateType get_type() const{return gatetype;}
};


class Circuit{
public:
    std::unordered_map<std::string,Signal*> primary_inps;
    std::unordered_map<std::string,Signal*> primary_outs;
    std::unordered_map<std::string,Signal*> internal;

    std::vector<Gate*> Gates;
    std::vector<Signal*> DFFInps;

public:
    Circuit(){}

    // Circuit(std::unordered_map<std::string,Signal*> inps,std::unordered_map<std::string,Signal*>  outs,std::unordered_map<std::string,Signal*> intern, std::vector<Gate*> g)\
    // : primary_inps(inps), primary_outs(outs), Gates(g), internal(intern)
    // {}

    ~Circuit();

    void print() const;

};

//stores time taken to process gate
struct Gate_States{
    double long T_low_area=std::numeric_limits<double long>::infinity();
    double long T_mid_area=std::numeric_limits<double long>::infinity();
    double long T_high_area=std::numeric_limits<double long>::infinity();

    double long A_low_area=std::numeric_limits<double long>::infinity();
    double long A_mid_area=std::numeric_limits<double long>::infinity();
    double long A_high_area=std::numeric_limits<double long>::infinity();

    void insert(const long double& time,const long double& area);
    //gets area of a state while updating time of a Gate
    long double make_state(unsigned short, Gate*) const;
};

class Gate_Variants
{  
public:
    Gate_States andStates;
    Gate_States orStates;
    Gate_States nandStates;
    Gate_States norStates;
    Gate_States notStates;

    //gets area of a state while updating time of a Gate
    long double make_state(unsigned short, Gate*) const;

};



#include<fstream>
#include<sstream>

Signal* get_signal_from_inps(const std::string& inp,const std::unordered_map<std::string,Signal*>& primary_inps, const std::unordered_map<std::string,Signal*>& internal,  const std::unordered_map<std::string,Signal*>& primary_outs)
{
    auto signal = internal.find(inp);
    if (signal==internal.end()){
        signal = primary_inps.find(inp);
        if (signal==primary_inps.end()){
            signal = primary_outs.find(inp);
            if (signal==primary_outs.end()){
                std::cout<<"No signal named "+inp+" is found!";
                exit(404);
            }
        }
    }

    return signal->second;
}

Signal* get_signal_from_outs(const std::string& inp, const std::unordered_map<std::string,Signal*>& internal,  const std::unordered_map<std::string,Signal*>& primary_outs)
{
    auto signal = internal.find(inp);
    if (signal==internal.end()){
        signal = primary_outs.find(inp);
        if (signal==primary_outs.end()){
            std::cout<<"No signal named "+inp+" is found!";
            exit(404);
        }
    }

    return signal->second;
}

Circuit* read_circuit_from(std::string input_file_name){
    using namespace std;

    Circuit* constructed_circuit=new Circuit;

    std::unordered_map<std::string,Signal*>& primary_inps=constructed_circuit->primary_inps;
    std::unordered_map<std::string,Signal*>& primary_outs=constructed_circuit->primary_outs;
    std::unordered_map<std::string,Signal*>& internal=constructed_circuit->internal;
    std::vector<Gate*>& gates=constructed_circuit->Gates;
    std::vector<Signal*>& DFFInps=constructed_circuit->DFFInps;


    std::ifstream file(input_file_name);

    string line, word;
    while (std::getline(file,line))
    {
        if (line.empty()){continue;}
        std::istringstream lstream(line);

        lstream >> word;
        if (word.substr(0,2)=="//"){continue;}

        if (word=="PRIMARY_INPUTS"){
            while (!lstream.eof()){lstream >> word;primary_inps.insert({word,new Signal{word}});}
            continue;
        }

        if (word=="PRIMARY_OUTPUTS"){
            while (!lstream.eof()){lstream >> word;primary_outs.insert({word,new Signal{word}});}
            continue;
        }
        
        if (word=="INTERNAL_SIGNALS"){
            while (!lstream.eof()){lstream >> word;internal.insert({word,new Signal{word}});}
            continue;
        }

        string inp,inp2,out;
        lstream>>inp>>inp2>>out;

        if (word=="INV"){
            Signal* inputSignal = get_signal_from_inps(inp,primary_inps, internal, primary_outs);
            Signal* outpt = get_signal_from_outs(inp2,internal,primary_outs);

            gates.push_back(new Gate{inputSignal,nullptr,outpt,notGATE});
            continue;
        }

        if (word=="DFF"){
            Signal* inputSignal = get_signal_from_inps(inp,primary_inps, internal, primary_outs);
            Signal* outpt = get_signal_from_outs(inp2,internal,primary_outs);

            DFFInps.push_back(inputSignal);
            gates.push_back(new Gate{inputSignal,nullptr,outpt,dffGATE});
            continue;
        }

        if (word=="AND2"){
            Signal* inputSignal = get_signal_from_inps(inp,primary_inps, internal, primary_outs);
            Signal* input2Signal = get_signal_from_inps(inp2,primary_inps, internal, primary_outs);
            Signal* outpt = get_signal_from_outs(out,internal,primary_outs);

            gates.push_back(new Gate{inputSignal,input2Signal,outpt,andGATE});
            continue;
        }

        if (word=="NAND2"){
            Signal* inputSignal = get_signal_from_inps(inp,primary_inps, internal, primary_outs);
            Signal* input2Signal = get_signal_from_inps(inp2,primary_inps, internal, primary_outs);
            Signal* outpt = get_signal_from_outs(out,internal,primary_outs);

            gates.push_back(new Gate{inputSignal,input2Signal,outpt,nandGATE});
            continue;
        }

        if (word=="OR2"){
            Signal* inputSignal = get_signal_from_inps(inp,primary_inps, internal, primary_outs);
            Signal* input2Signal = get_signal_from_inps(inp2,primary_inps, internal, primary_outs);
            Signal* outpt = get_signal_from_outs(out,internal,primary_outs);

            gates.push_back(new Gate{inputSignal,input2Signal,outpt,orGATE});
            continue;
        }
        
        if (word=="NOR2"){
            Signal* inputSignal = get_signal_from_inps(inp,primary_inps, internal, primary_outs);
            Signal* input2Signal = get_signal_from_inps(inp2,primary_inps, internal, primary_outs);
            Signal* outpt = get_signal_from_outs(out,internal,primary_outs);

            gates.push_back(new Gate{inputSignal,input2Signal,outpt,norGATE});
            continue;
        }
        
        else {
            cout<<word<<" is not a valid gate!";
            exit(404);
        }
    }

    return constructed_circuit;
}

// const std::vector<std::string> Gate_Literals ={"nd","AND2","OR2","NAND2","NOR2","INV","DFF"};

Gate_Variants* read_gateVarient_from(std::string input_file_name){
    using namespace std;
    Gate_Variants* res = new Gate_Variants;
    
    std::ifstream file(input_file_name);
    string line, word;
    while (std::getline(file,line))
    {
        if (line.empty()){continue;}
        std::istringstream lstream(line);

        lstream >> word;
        if (word.substr(0,2)=="//"){continue;}

        string gate;
        lstream>>gate;
        double time, area;
        if(gate=="AND2"){
            lstream>>time>>area;
            res->andStates.insert(time,area);
        }
        else if(gate=="OR2"){
            lstream>>time>>area;
            res->orStates.insert(time,area);
        }
        else if(gate=="NAND2"){
            lstream>>time>>area;
            res->nandStates.insert(time,area);
        }
        else if(gate=="NOR2"){
            lstream>>time>>area;
            res->norStates.insert(time,area);
        }
        else if(gate=="INV"){
            lstream>>time>>area;
            res->notStates.insert(time,area);
        }

        else{
            cout<<gate<<" is not a valid gate in line "<<line;
            exit(404);
        }
    }

    return res;
}

long double read_gate_delay(std::string file_path){
    std::ifstream file(file_path);
    long double ans;

    file>>ans;
    return ans;
}

void write_to_file(std::string file_path,long double ans){
    std::ofstream file(file_path);

    file<<ans;
    return;
}

/* Helpers
inputs:i(base3),Ciruit,gate_variants,Area
Outputs:Struct{next i,Area}*/
using namespace std;
struct helper{
    string next_iter;
    long double iter_area;
};

bool Add_1(string& iterator){
    int checker;
    int carry=-1;
    for(int i=0;i<iterator.size();i++){
        checker=(iterator[iterator.size()-1-i])-'0';
        checker=checker+carry;
        if(checker==-1){
            carry=-1;
            iterator[iterator.size()-1-i]='2';
        }
        else{
            carry=0;
            iterator[iterator.size()-1-i]=checker+'0';
            break;
        }
    }
    return (carry!=-1);
}

helper Calculating_suitable_area(string iterator,Circuit*given_circuit,Gate_Variants*types_of_gates,double long Present_valid_area){
    string present_iterator=iterator;
    const vector<Gate*>&types_ofgates=given_circuit->Gates;
    double long checking_area=std::numeric_limits<double long>::infinity();
    bool check_completion;
    do{
        checking_area=0;
        for(int i=0;i<present_iterator.size();i++){
            checking_area+=types_of_gates->make_state(present_iterator[i]-'0',types_ofgates[i]);
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


//return double value and takes a circuit pointer as input
using namespace std;

void set_Gates_to_min(Circuit* c, Gate_Variants* v){
    for(Gate* gate: c->Gates){
        v->make_state(2,gate);
    }
}

long double Finding_max_delay_in_a_circuit(Circuit*given_circuit, long double max_time){
    unordered_map<string,Signal*>&out_puts_of_circuit=given_circuit->primary_outs;
    unordered_map<string,Signal*>&inputs_of_circuit=given_circuit->primary_inps;
    unordered_map<Signal*,long double>result_outvalues;
    result_outvalues[nullptr]=0;
    vector<Signal*>&dff_in_the_circuit=given_circuit->DFFInps;
    stack<Signal*>temporary_storage;
    for(auto i=inputs_of_circuit.begin();i!=inputs_of_circuit.end();i++){
        result_outvalues[i->second]=0;
    }
    for(int i=0;i<dff_in_the_circuit.size();i++){
        temporary_storage.push(dff_in_the_circuit[i]);
    }
    for (auto i = out_puts_of_circuit.begin(); i != out_puts_of_circuit.end(); i++){
        temporary_storage.push(i->second);
    }
    Signal*temp_store;
    long double final_answer=0;
    while(temporary_storage.empty()!=true){
        temp_store=temporary_storage.top();
        if(temp_store->get_source()->get_type()==dffGATE){
            result_outvalues[temp_store]=0;
            temporary_storage.pop();   
        }
        else{
            if((result_outvalues.find(temp_store->get_source()->input1)!=result_outvalues.end())&&(result_outvalues.find(temp_store->get_source()->input2)!=result_outvalues.end())){
                result_outvalues[temp_store]=max(result_outvalues[temp_store->get_source()->input1],result_outvalues[temp_store->get_source()->input2])+temp_store->get_source()->time;
                if(final_answer<result_outvalues[temp_store]){
                    final_answer=result_outvalues[temp_store];
                    if(final_answer>max_time){return std::numeric_limits<double long>::infinity();}
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
    }
    return final_answer;
}



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

#include<cassert>


void Signal::connect_to(Gate* g) {
    outputGates.push_back(g);
}

void Signal::connect_from(Gate* g) {
    if (sourceGate!=nullptr){
        std::cout<<"Signal "+name+" was connected from two inputs, previously connected to "+sourceGate->get_name();
        exit(404);
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
