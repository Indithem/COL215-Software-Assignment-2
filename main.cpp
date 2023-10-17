#include"gates.h"
#include"get_inputs.cpp"

int main(){
    Circuit* test= read_from("circuit.txt");
    test->print();
}
