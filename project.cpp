#include <cstdio>
#include <string>
#include <vector>
#include <list>

//Global Variables
std::vector< std::list < int> > adjacency_list;
std::vector<bool> visited;
std::vector<int> discovery;
std::vector<int> low;
std::vector<int> parent;
std::vector<bool> isArticulation; //articulation points boolean vector

int max_value = 0; // var for storing the max vertex found in a subgraph
int vertices_in_subgraph = 0; // number of vertices in subgraph
int clock_time = 0; // time variable
int ap_points = 0; // number of articulation points

//dfs
void dfs(int u){
    clock_time++;
    int childCount = 0;
    visited[u] = true;
    discovery[u] = low[u] = clock_time;
    std::list<int>::iterator i;
    for(i = adjacency_list[u].begin(); i!= adjacency_list[u].end();i++){
        int v = (*i);
        if(!visited[v]){
            childCount++;
            parent[v] = u;
            dfs(v);
            low[u] = std::min(low[u],low[v]);
            if(low[v] >= discovery[u] && parent[u] != -1){ //non root case
                isArticulation[u] = true;
            }
            int temp = std::max(u,v);
            max_value = std::max(temp,max_value);
            vertices_in_subgraph++;

        }
        else if(parent[u] != v){
            low[u] = std::min(low[u],discovery[v]);
        }
    }//root case
    if(((parent[u] != -1) && (isArticulation[u] == true)) || ((parent[u] == -1) && (childCount > 1))){
         isArticulation[u] = true;
         ap_points++;
    }
}

int main(){
   
    //read from input
    int vertices,edges;
    if(scanf("%d",&vertices) != 1){
        printf("Failed to read number of vertices.\n");
        exit(1);
    }
    if(scanf("%d",&edges) != 1){
        printf("Failed to read number of edges.\n");
        exit(1);
    }
    
    //reserve memory and initialize vectors
    adjacency_list = std::vector< std::list<int> >(vertices);
    visited = std::vector<bool>(vertices, false);
    discovery = std::vector<int>(vertices, -1);
    low = std::vector<int>(vertices, -1);
    parent = std::vector<int>(vertices, -1);
    isArticulation = std::vector<bool>(vertices, false);

    //read edges from input
    int u,v;
    for(int i = 0;i < edges;i++){
        if(scanf("%d %d",&u,&v) != 2){
            printf("Failed to read edge.%d %d\n",u,v);
            exit(1);
        }
        adjacency_list[(u-1)].push_back((v-1));
        adjacency_list[(v-1)].push_back((u-1));
    }

    std::vector<int>subgraph_max_values; // vector for storing the maximum values found in a subgraph
    for(int i = 0; i < vertices; i++){
        if(visited[i] == false){
            max_value = 0;
            dfs(i);
            subgraph_max_values.push_back(max_value);
        }
    }

    //print number of subgraphs
    int graph_roots = 0;
    for(int i = 0;i < vertices;i++){
        if(parent[i] == -1)
            graph_roots++;
    }
    printf("%d\n",graph_roots);


    //order vector with max values using count sort
    int count[vertices+1] = {0};
    int output[subgraph_max_values.size()];

    for(unsigned int i = 0; i < subgraph_max_values.size();i++){
        count[subgraph_max_values[i]] = count[subgraph_max_values[i]]+1;
    }
    for(int i = 1; i < vertices;i++){
        count[i] = count[i] + count[i-1];
    }
    for(int i = subgraph_max_values.size(); i >= 0; i--){
        output[count[subgraph_max_values[i]]-1] = subgraph_max_values[i];
        count[subgraph_max_values[i]] = count[subgraph_max_values[i]] -1;
    }
    for(unsigned int i = 0; i < subgraph_max_values.size();i++){
        subgraph_max_values[i] = output[i];
    }



    //print vector with max values
    for(unsigned int i = 0; i < subgraph_max_values.size()-1;i++){
        printf("%d ", subgraph_max_values[i]+1);
    }
    printf("%d\n", subgraph_max_values[subgraph_max_values.size()-1]+1);

    //print number of articulation points
    printf("%d\n",ap_points);

    //re-initialize for second run
    //remove articulation points in order to not be visited by dfs
    for(int i = 0; i < vertices; i++){
        if(isArticulation[i] != true){
            visited[i] = false;
            discovery[i] = -1;
            parent[i] = -1;
        }
    }
    //second run of dfs
    clock_time = 0; //restart clock_time
    int temp = 0;
    for(int i = 0; i < vertices; i++){
        if(visited[i] == false){
            vertices_in_subgraph = 1;
            dfs(i);
            temp = std::max(temp,vertices_in_subgraph);
        }
    }
    //print maximum number of vertices in a subgraph after removing articulation points
    printf("%d\n",temp);
    
    return 0;
}
