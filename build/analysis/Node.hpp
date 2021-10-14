#ifndef NODE_HPP_
#define NODE_HPP_

#include <iostream>
#include <vector>

using namespace std;

//file path
#define design_node "D:\\EDA\\Sample\\grpout_1\\design.node"  
#define design_clk  "D:\\EDA\\Sample\\grpout_1\\design.clk"
#define design_are  "D:\\EDA\\Sample\\grpout_1\\design.are"
#define design_net  "D:\\EDA\\Sample\\grpout_1\\design.net"
#define design_tdm  "D:\\EDA\\Sample\\grpout_1\\design.tdm"

//macro
#define NPOS    string::npos

//data structure
class Node
{
    public:
        int DEGREE_IN;
        int DEGREE_OUT;
        int FPGA_ID;//@FPGAi
        int NODE_ID;//@gi or gpi
        Node(void);
        Node(int fpga_id, int node_id);
};
class Pin : public Node
{
    public:
        bool valid;//0---vcc/gnd 
        bool CLK; //0---no clk  1---have clk
        int CLK_Frequent;
        Pin(int fpga_id, int node_id);
};
class Component : public Node
{
    public:
        bool FF;//0---not FF  1---FF
        bool CLK;//0---no clk  1---have clk
        bool END;//0---logic_delay  1---output port
        int visited;//被访问次数
        vector<int> CLK_Frequent;//frequent list
        int CYCLE;//CLK_Frequent
        Component(int fpga_id, int node_id);
};

Node::Node(void)
{
    this->DEGREE_IN=0;
    this->DEGREE_OUT=0;
    this->FPGA_ID=-1;
    this->NODE_ID=-1;
}
Node::Node(int fpga_id, int node_id)
{
    this->DEGREE_IN=0;
    this->DEGREE_OUT=0;
    this->FPGA_ID=fpga_id;
    this->NODE_ID=node_id;
}
Pin::Pin(int fpga_id, int node_id): Node(fpga_id,node_id)
{
    this->valid=1;//need to be checked
}
Component::Component(int fpga_id, int node_id): Node(fpga_id,node_id)
{
    this->CLK=0;
    this->FF=0;
    this->END=0;
    this->CLK_Frequent.clear();
    this->CYCLE = 0;
}

//graph adjlist
struct Edge
{
    int AdjIndex;
    int DELAY;
    int TDM_ID;
    int TDM_RATIO;
};

struct AdjItem
{
    bool Hash;//1---still not deleted yet
    int VIndex;//top index, which needs "TopTable" 
    vector<Edge> AdjEdge;
};

struct result{
    int fnum;              //front node
    int bnum;              //back node
    int fpga_id1;          //front node FPGA
    int fpga_id2;          //back bode FPGA
    int cable;
    int cycle;
    int tdm_time;
    int su_time;
    int hold_time;
};

struct Pre{
    int Pre_Index;         //TopoPath
    double farrival_time; //front arrival time
    double barrival_time;// back arrival time
};



//register info
void register_pin(vector<Pin>& PinList, int fpga_id, int pin_id)
{
    Pin pin=Pin(fpga_id,pin_id);
    PinList.push_back(pin);
    return;
}

void register_component(vector<Component>& ComList, int fpga_id, int com_id)
{
    Component com=Component(fpga_id,com_id);
    ComList.push_back(com);
    return;   
}

#endif
