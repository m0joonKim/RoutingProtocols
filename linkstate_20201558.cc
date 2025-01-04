#include<iostream>
#include<vector>
#include<fstream>
#include<tuple>
#include<string>
#include<queue>
using namespace std;
//int nd[101][101];//node - node cost
pair<int,int> lsTable[101][101];//lsTable[i][j] : i에서 j가는거리, first : 다음노드, second : cost
vector<pair<int,int>> cost_v[101];//i에서 cost[i].first로 가는데, 비용은 cost[i].second
vector<tuple<int, int, string>> msg_v;
vector<tuple<int,int,int>> chg_v;
int N;//# of node
// void initNode();
struct cmp{
    bool operator()(tuple<int,int,int> a, tuple<int, int,int> b){ // s - e - cost
        if( get<2>(a) == get<2>(b)){
            return get<1>(a) > get<1>(b);   
        }
        return get<2>(a) > get<2>(b); 
    }
};
void readTopology(ifstream &tpFile);
void initTable();
void readMessages(ifstream &msgFile);
void readChanges(ifstream &chgFile);
void writeOutput(ofstream &outFile);
void printToOutput(ofstream &outFile);
char* eraseSpace(char* str);
void exchangeInfo();
void dijkstra(int now);
void edgeChagne(int i);
int main(int argc, char* argv[]){
    if(argc!=4){
        cout << "usage : linkstate topologyfile messagesfile changesfile\n";
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
    outputFile.open("output_ls.txt");
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
    cout << "Complete. Output file written to output_ls.txt.\n";
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
                lsTable[i][j] = {i,0};//자기자신한테 오는거 ㅇㅇ
            }else{
                lsTable[i][j] = {-1,-999};
            }
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
void dijkstra(int now){
    //N개 노드
    int s,e,cost;
    int par[101]={0,};

    priority_queue<tuple<int,int,int>, vector<tuple<int,int,int>>, cmp> pq;
    pq.push({now,now,0});
    while(!pq.empty()){
        tie(s,e,cost) = pq.top();
        pq.pop();
        par[e] = s;
        //여기서 뽑았다는건, 현재 트리에서 가장 가까운노드라는거고 ls에 넣어야된다는거?
        if(lsTable[now][e].second<cost&&lsTable[now][e].second!=-999)continue;
        //now - s 거리를 알고있는 상태에서, s - e 연결하는 노드를 뽑아온거. 총 거리가 그리고 cost.

        //이제 e에서 어디론가 가는 노드들 추가.
        for(auto nd : cost_v[e]){
            int ee, ecost, totalcost;
            tie(ee, ecost) = nd;
            if(ecost == -999)continue;//끊긴엣지 죽이고
            if(now == ee)continue;
            totalcost = cost + ecost;//e까지 온 거리 + e에서 ee가는 거리.
            if(lsTable[now][ee].second>totalcost||lsTable[now][ee].second==-999){
                //토탈코스트가 더 작던가, 아직 간적도 없던가 -> 무조건 갱신
                par[ee] = e;
                lsTable[now][ee].second = totalcost;
                if(lsTable[now][e].first==now){//나 직전놈이, 다음노드가 시작노드라면 -> 시작노드임
                    lsTable[now][ee].first = ee;
                }else{
                    lsTable[now][ee].first = lsTable[now][e].first;
                }
                pq.push({e,ee,totalcost});
            }else if(lsTable[now][ee].second==totalcost){
                //같으면 -> 부모보고 판단
                if(par[ee]>e){//원래 부모보다 작아야 갱신
                    par[ee] = e;
                    if(lsTable[now][e].first==now){//나 직전놈이, 다음노드가 시작노드라면 -> 시작노드임
                        lsTable[now][ee].first = ee;
                    }else{
                        lsTable[now][ee].first = lsTable[now][e].first;
                    }
                    pq.push({e,ee,totalcost});
                }
            }

        }
    }

    return ;
}
void exchangeInfo(){
    for(int i=0; i<N;i++){
        dijkstra(i); 
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
            if(lsTable[i][j].second==-999)continue;
            outFile << j << ' ' << lsTable[i][j].first << ' ' << lsTable[i][j].second<<'\n';
        }
        outFile<<'\n';
    }
    for(auto msg : msg_v){
        int source, desti;
        bool infFlag=false;
        string str;
        tie(source, desti, str) = msg;
        outFile << "from " << source << " to " << desti;
        if(lsTable[source][desti].second==-999){
            outFile <<" cost infinite";
            infFlag=true;
        }else{
            outFile <<" cost " << lsTable[source][desti].second;
        }
        outFile << " hops ";
        if(!infFlag){        
            int hop = source;
            while(hop!=desti){
                outFile << hop << " ";
                hop = lsTable[hop][desti].first;
            }
        }else{
            outFile <<"unreachable ";
        }

        outFile << "message" << str << '\n';
    }
    outFile << '\n';
}
