#pragma once
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