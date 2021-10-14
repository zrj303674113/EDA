#ifndef ALGORITHM2_HPP
#define ALGORITHM2_HPP

#include <iostream>
#include <assert.h>
#include <algorithm>

using namespace std;

void ReverseTopo(vector<int>& TopoPath){

    //copy the toposort result
    vector<int> RTopoPath(TopoPath);
    //reverse the topopath
    reverse(RTopoPath.begin(),RTopoPath.end());
    return;
}


void Caculate(int* TopTable,vector<int>& TopoPath,vector<Pin>& PinList, vector<Component>& ComList,vector<AdjItem>& AdjList){
    
    int Co_time=1,Setup_time=1,Hold_time=1;
    int SuSlack=0,HoldSlack=0,ComportDelay=0;
    int S_require = 0; 
    int H_require = Hold_time; 
    int Logic_delay = 0.1;

    int total = PinList.size()+ComList.size();
    int offset = PinList.size();
    
    int AdjHash[total];
    GetAdjHash(total,AdjHash,AdjList);
    
    vector<int> Pre;
    
    vector<result> R(20);       //  

//Preorder traversal
    for(int i = 0; i<total; ++i){
        int v = TopoPath[i];
        if(v<offset && PinList[TopTable[v]].CLK){	
            
            //This is clock pin
            int index=AdjHash[v];
            int num=AdjList[index].AdjEdge.size();
            
            //Traversal adjacency table        
                for(int i=0; i<num; ++i){
                    int j =  AdjList[index].AdjEdge[i].AdjIndex;
                    if(ComList[TopTable[j]].FF){
                        ComList[TopTable[j]].CYCLE = PinList[TopTable[v]].CLK_Frequent;
                    }
               }
                       
        }else if(v<offset && !PinList[TopTable[v]].CLK){    
            
            //This is input port，ex_delay=1
            int ex_delay = 1;
            int setup_slack = 0;
            int arrival_time = 0;
            
            //Traversal adjacency table,caculate the delay   (AOE net)
                int index=AdjHash[v];
                int num=AdjList[index].AdjEdge.size();
            
                for(int i=0; i<num; ++i){

                    int j = AdjList[index].AdjEdge[i].AdjIndex;
                    
                       
                    //Judge whether the pin and the next point are on the same FPGA
                    int cable = 0 , Ttdm_delay = 0;
                    if(PinList[TopTable[v]].FPGA_ID != ComList[TopTable[j]].FPGA_ID && !AdjList[index].AdjEdge[i].TDM_ID){
                        cable = 2;
                    }else if(AdjList[index].AdjEdge[i].TDM_ID){
                        Ttdm_delay = (20+AdjList[index].AdjEdge[i].TDM_RATIO/4)/312.5;
                    }    

                    if(!ComList[TopTable[j]].FF && !ComList[TopTable[j]].END ){
                      
                      // Pin ——>logic delay
                        arrival_time = ex_delay + Logic_delay + cable +Ttdm_delay;
                        //P[j].Pre_Index = v;
                        //Ve[j] = arrival_time;
                    
                    }else if(ComList[TopTable[j]].FF){

                       //Pin —>ff                      
                            arrival_time = ex_delay + Co_time + cable + Ttdm_delay; 
                            S_require = ComList[TopTable[j]].CYCLE - Setup_time;
                            setup_slack = arrival_time - S_require;
                            Hold_time = arrival_time - H_require;

                            if(setup_slack >= 0 && Hold_time >= 0){
                                SuSlack += setup_slack;
                                HoldSlack += Hold_time;
                                //P[j].Pre_Index = v;
                                //Ve[j] = arrival_time; 
                                cout<<""<<endl; 

                            }

                    }else{break;}

                }



        }else if(v<offset){

        }

      

    

    return;
}






void F_Travesal(int x,int* TopTable,vector<int>& TopoPath,vector<Pin>& PinList, vector<Component>& ComList,vector<AdjItem>& AdjList,int* AdjHash){

    int k = TopoPath[x];
    int index=AdjHash[k];
    int n=AdjList[index].AdjEdge.size();
            
    for(int t=0; t<n; ++t){

        int x= AdjList[index].AdjEdge[t].AdjIndex;

        //Judge whether the pin and the next point are on the same FPGA
        int cable = 0 , Ttdm_delay = 0;
        if(ComList[TopTable[k]].FPGA_ID != ComList[TopTable[x]].FPGA_ID && !AdjList[index].AdjEdge[t].TDM_ID){
            cable = 2;
        }else if(AdjList[index].AdjEdge[t].TDM_ID){
            Ttdm_delay = (20+AdjList[index].AdjEdge[t].TDM_RATIO/4)/312.5;
        }

        if(ComList[TopTable[x]].FF){
            //This is ff
        /* 
            arrival_time = ex_delay + Co_time + cable + Ttdm_delay; 
            S_require = ComList[TopTable[x]].CYCLE - Setup_time;
            setup_slack = arrival_time - S_require;
            Hold_time = arrival_time - H_require;
        */
        }else if(ComList[TopTable[x]].END){
            //This is output port


            break;
         }else{
            //This is logic delay

        }
    
    }
    
    
    
    
    
    
    return;
}





#endif