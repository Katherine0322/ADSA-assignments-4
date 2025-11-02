#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// convert a letter into a numeric cost (A-Z -> 0-25, a-z -> 26-51)
int getCost(char c){
    if ('A' <= c && c <= 'Z') return c - 'A';
    return c - 'a' + 26;
}

// split helper: split a CSV-like string by commas
vector<string> splitByComma(const string& s){
    vector<string> parts;
    string cur;
    for(char ch : s){
        if(ch == ','){
            parts.push_back(cur);
            cur.clear();
        }else{
            cur.push_back(ch);
        }
    }
    parts.push_back(cur);
    return parts;
}

// find the parent of a node (basic union-find function)
int findSet(int x, vector<int>& parent){
    if(parent[x]==x) return x;
    parent[x] = findSet(parent[x], parent); // path compression
    return parent[x];
}

// join two sets together (union operation)
void unionSet(int a, int b, vector<int>& parent){
    a = findSet(a, parent);
    b = findSet(b, parent);
    if (a != b) parent[b] = a;
}

int main(){
    // 
    string country_s, build_s, destroy_s;
    cin >> country_s >> build_s >> destroy_s;

    // split into rows
    vector<string> country = splitByComma(country_s);
    vector<string> build   = splitByComma(build_s);
    vector<string> destroy = splitByComma(destroy_s);

    int n = (int)country.size();
    vector<pair<int, pair<int, int>>> edges; // (weight, (u,v))
    long long base = 0;

    // create edge list (only upper triangle to avoid duplicates)
    for(int i=0; i<n; i++){
        for(int j=i+1; j<n; j++){
            if(country[i][j]=='1'){
                int d = getCost(destroy[i][j]);
                base += d;                         // assume destroy all first
                edges.push_back({-d, {i, j}});     // saving if we keep it
            }else{
                int b = getCost(build[i][j]);
                edges.push_back({ b, {i, j}});     // cost to build a new road
            }
        }
    }

    // sort edges by weight (smallest first)
    sort(edges.begin(), edges.end());

    // union-find init
    vector<int> parent(n);
    for(int i=0;i<n;i++) parent[i]=i;

    long long total = base;

    // Kruskal: pick edges if they connect different components
    for(int i=0; i<(int)edges.size(); i++){
        int w = edges[i].first;
        int u = edges[i].second.first;
        int v = edges[i].second.second;
        if (findSet(u, parent) != findSet(v, parent)){
            unionSet(u, v, parent);
            total += w;  // add cost (or saving if negative)
        }
    }

    cout << total << endl; // 
    return 0;
}
