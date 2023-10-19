#include<fstream>
#include<sstream>
#include"gates.h"

Signal* get_signal_from_inps(const std::string& inp,const std::unordered_map<std::string,Signal*>& primary_inps, const std::unordered_map<std::string,Signal*>& internal,  const std::unordered_map<std::string,Signal*>& primary_outs)
{
    auto signal = internal.find(inp);
    if (signal==internal.end()){
        signal = primary_inps.find(inp);
        if (signal==primary_inps.end()){
            signal = primary_outs.find(inp);
            if (signal==primary_outs.end()){
                std::cout<<"No signal named "+inp+" is found!";
                abort();
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
            abort();
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
            abort();
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
            abort();
        }
    }

    return res;
}