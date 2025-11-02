#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
using namespace std;

//convert a letter into a numeric cost(A-Z -> 0-25, a-z -> 26-51)
int getCost(char c){
    if (c>'A'&&c<='Z')return c-'A';
    return c-'a'+26;
}

//find the parent of a node(basic union-find function)
int findSet(int x, vector<int>& parent){
    if(parent[x]==x)return x;
    parent[x]=findSet(parent[x], parent);//path compression to make it faster
    return parent[x];
}

//join two sets together (union operation)
void unionSet(int a, int b, vector<int>&parent){
    a=findSet(a, parent);
    b=findSet(b, parent);
    if (a!=b)parent[b]=a;
}

int main(){
    string country_s, build_s, destroy_s;
    cin>>country_s>>build_s>>destroy_s;

    //split the three input strings by commas and store them separately
    vector<string> country, build, destroy;
    string temp =" ";
    for(char c: country_s){
        if(c==','){
            country.push_back(temp);
            temp=" ";
        }else temp += c;
    }
    build.push_back(temp);

    temp=" ";
    for(char c: destroy_s){
        if(c==','){
            destroy.push_back(temp);
            temp=" ";
        }else temp+=c;
    }
    destroy.push_back(temp);

    int n = country.size();
    vector<pair<int, pair<int, int>>>edges;
    int base = 0;

    //create edge list(just the upper triangle to avoid duplicates)
    for(int i=0; i<n; i++){
        for(int j=i+1; j<n; j++){
            if(country[i][j]=='1'){
                int d=getCost(destroy[i][j]);
                base += d; //assume we destroy all roads first
                edges.push_back({-d, {i, j}});//negative = saving if keep it
            }else{
                int b= getCost(build[i][j]);
                edges.push_back({b, {i,j}}); //cost to build a new road
            }
        }
    }
    
    //sort all edges from smallest to largest cost 
    sort(edges.begin(), edges.end());

    //set up the union-find parent array
    vector<int> parent(n);
    for(int i=0; i<n;i++) parent[i]=i;

    int total=base;

    //main Kruskal loop(build MST and minimize cost)
    for(int i=0; i<edges.size();i++){
        int w = edges[i].first;
        int u = edges[i].second.first;
        int v = edges[i].second.second;

        //if u and v are not connected yet, connect them
        if (findSet(u, parent)!=findSet(v, parent)){
            unionSet(u,v,parent);
            total += w;  //add cost (or saving if w<0)
        }
    }
    cout<<total<<endl; //print the final answer cost (including savings)

    return 0;
}