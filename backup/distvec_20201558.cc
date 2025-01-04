#include<iostream>
#include<vector>
#include<fstream>
#include<tuple>
#include<string>
using namespace std;
//int nd[101][101];//node - node cost
pair<int,int> disTable[101][101];//disTable[i][j] : i에서 j가는거리, first : 다음노드, second : cost
vector<pair<int,int>> cost_v[101];//i에서 cost[i].first로 가는데, 비용은 cost[i].second
vector<tuple<int, int, string>> msg_v;
vector<tuple<int,int,int>> chg_v;
int N;//# of node
// void initNode();
void readTopology(ifstream &tpFile);
void initTable();
void readMessages(ifstream &msgFile);
void readChanges(ifstream &chgFile);
void writeOutput(ofstream &outFile);
void printToOutput(ofstream &outFile);
char* eraseSpace(char* str);
void exchangeInfo();
void bfAlgorithm();
void edgeChagne(int i);
int main(int argc, char* argv[]){
    if(argc!=4){
        cout << "usage : distvec topologyfile messagesfile changesfile\n";
        return 0;
    }
    ifstream topologyFile;
    ifstream messageFile;
    ifstream changeFile;
    topologyFile.open(argv[1]);
    messageFile.open(argv[2]);
    changeFile.open(argv[3]);
    if(!(topologyFile.is_open()&&messageFile.is_open()&&changeFile.is_open())){
        //세 파일이 모두 성공한게 아니라면. 즉, 하나라도 안열리면
        cout << "Error: open input file.\n";
        return 0;
    }
    ofstream outputFile;
    outputFile.open("output_dv.txt");
    if(!outputFile.is_open()){
        cout << "Error: open output file.\n";
        return 0;
    }
    //initNode();
    readTopology(topologyFile);
    readMessages(messageFile);
    readChanges(changeFile);
    writeOutput(outputFile);
    

    //성공했으면
    cout << "Complete. Output file written to output_dv.txt.\n";
    topologyFile.close();
    messageFile.close();
    changeFile.close();
    outputFile.close();
    return 0;
}
// void initNode(){
//     for(int i=0;i<=100;i++){
//         for(int j=0;j<=100;j++){
//             nd[i][j] = -999;
//             if(i==j){
//                 nd[i][j] = 0;
//             }
//         }
//     }
// }
void initTable(){
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if(i==j){
                disTable[i][j] = {i,0};//자기자신한테 오는거 ㅇㅇ
            }else{
                disTable[i][j] = {-1,-999};
            }
        }
    }
    for(int i=0;i<N;i++){
        for(int j=0;j<(int)cost_v[i].size();j++){
            int desti = cost_v[i][j].first;
            int cost = cost_v[i][j].second;
            if(cost==-999)continue;
            disTable[i][desti] = {desti, cost};
            disTable[desti][i] = {i, cost};
        }
    }
}
void readTopology(ifstream &tpFile){
    int s, e, cost;
    tpFile >> N;
    while(true){
        tpFile >> s >> e >> cost;
        if(tpFile.eof())break;;
        // nd[s][e] = cost;
        // nd[e][s] = cost;
        cost_v[s].push_back({e,cost});
        cost_v[e].push_back({s,cost});
    }
    initTable();
    return ;
}
// char* eraseSpace(char* str) {
//     int i = 0;
//     while (str[i] == ' ') {
//         i++;
//     }
//     return str + i;
// }
void readMessages(ifstream &msgFile){
    int s, e;
    string str;
    while(true){
        msgFile >> s >> e;
        if(msgFile.eof())break;
        getline(msgFile, str);
        //msg = eraseSpace(str);
        msg_v.push_back({s,e,str});
    }
    return ;
}
void readChanges(ifstream &chgFile){
    int s, e, cost;
    while(true){
        chgFile >> s >> e >> cost;
        if(chgFile.eof())break;
        chg_v.push_back({s, e, cost});
    }
    return ;
}
void bfAlgorithm(int now){
    //N개 노드
    disTable[now][now] = {now,0};
    for(int i=1;i<=N-1;i++){
        for(int j=0;j<N;j++){
            for(auto k : cost_v[j]){
                int source = j;
                int desti = k.first;
                int cost = k.second;
              //  cout << j << ' ' << desti << ' '<<cost << '\n';
                if(now==source)continue;
                if(now==desti)continue;
                if(cost==-999)continue;
                if(disTable[now][source].second == -999)continue;
                if(disTable[now][desti].second==-999||disTable[now][desti].second > disTable[now][source].second + cost){
                    disTable[now][desti].second = disTable[now][source].second + cost;
                    disTable[now][desti].first = disTable[now][source].first; 
                }else if(disTable[now][desti].second == disTable[now][source].second + cost){
                    if(disTable[now][desti].first > disTable[now][source].first){
                        disTable[now][desti].second = disTable[now][source].second + cost;
                        disTable[now][desti].first = disTable[now][source].first; 
                    }
                }
            }
        }
    }
    return ;
}
void exchangeInfo(){
    for(int i=0; i<N;i++){
        bfAlgorithm(i); 
    }
    return ;    
}

void writeOutput(ofstream &outFile){
    exchangeInfo();
    printToOutput(outFile);
    for(int i=0;i<(int)chg_v.size();i++){
        edgeChagne(i);
        //체인지 반영
        initTable();
        exchangeInfo();
        printToOutput(outFile);
    }
}
void edgeChagne(int i){//i번째 change
    int s, e, cost;
    bool find1, find2;
    find1 = false;
    find2 = false;
    tie(s,e,cost) = chg_v[i];
    for(int i=0;i<(int)cost_v[s].size();i++){
        if(cost_v[s][i].first==e){
            cost_v[s][i].second = cost;
            find1 = true;
        }
    }
    for(int i=0;i<(int)cost_v[e].size();i++){
        if(cost_v[e][i].first==s){
            cost_v[e][i].second = cost;
            find2 = true;
        }
    }
    //find1이 true면 find2 도 당연히 true가 되긴함
    if(!find1){
        cost_v[s].push_back({e,cost});
    }
    if(!find2){
        cost_v[e].push_back({s,cost});
    }
}
void printToOutput(ofstream &outFile){
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if(disTable[i][j].second==-999)continue;
            outFile << j << ' ' << disTable[i][j].first << ' ' << disTable[i][j].second<<'\n';
        }
        outFile<<'\n';
    }
    for(auto msg : msg_v){
        int source, desti;
        bool infFlag=false;
        string str;
        tie(source, desti, str) = msg;
        outFile << "from " << source << " to " << desti;
        if(disTable[source][desti].second==-999){
            outFile <<" cost infinite";
            infFlag=true;
        }else{
            outFile <<" cost " << disTable[source][desti].second;
        }
        outFile << " hops ";
        if(!infFlag){        
            int hop = source;
            while(hop!=desti){
                outFile << hop << " ";
                hop = disTable[hop][desti].first;
            }
        }else{
            outFile <<"unreachable ";
        }

        outFile << "message" << str << '\n';
    }
    outFile << '\n';
}
