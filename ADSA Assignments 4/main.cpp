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

// remove spaces and CR (\r) inside a token
static inline void trimSpaces(string &s){
    string t;
    t.reserve(s.size());
    for(char ch : s){
        if (ch!=' ' && ch!='\r' && ch!='\t') t.push_back(ch);
    }
    s.swap(t);
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
    string country_s, build_s, destroy_s;
    // Read three tokens (works for one-line or multi-line inputs)
    if(!(cin >> country_s >> build_s >> destroy_s)) return 0;

    // split into rows
    vector<string> country = splitByComma(country_s);
    vector<string> build   = splitByComma(build_s);
    vector<string> destroy = splitByComma(destroy_s);

    // trim spaces/CR in each row token to avoid length mismatch
    for (size_t i=0;i<country.size();++i) trimSpaces(country[i]);
    for (size_t i=0;i<build.size();++i)   trimSpaces(build[i]);
    for (size_t i=0;i<destroy.size();++i) trimSpaces(destroy[i]);

    int n = (int)country.size();

    // guard: malformed input -> avoid out-of-range
    if ((int)build.size()!=n || (int)destroy.size()!=n){
        cout << 0 << endl;
        return 0;
    }
    for (int i=0;i<n;i++){
        if ((int)country[i].size()!=n || (int)build[i].size()!=n || (int)destroy[i].size()!=n){
            cout << 0 << endl;
            return 0;
        }
    }

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
    for(int k=0; k<(int)edges.size(); k++){
        int w = edges[k].first;
        int u = edges[k].second.first;
        int v = edges[k].second.second;
        if (findSet(u, parent) != findSet(v, parent)){
            unionSet(u, v, parent);
            total += w;  // add cost (or saving if negative)
        }
    }

    cout << total << endl; // flush
    return 0;
}
