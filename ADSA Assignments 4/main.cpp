#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// turn a letter into a number (A-Z = 0~25, a-z = 26~51)
int getCost(char c){
    if ('A' <= c && c <= 'Z') return c - 'A';
    return c - 'a' + 26;
}

// take away spaces and weird things like \r or \t
static inline void trimSpaces(string &s){
    string t;
    t.reserve(s.size());
    for(char ch : s){
        if (ch!=' ' && ch!='\r' && ch!='\t') t.push_back(ch);
    }
    s.swap(t);
}

// split the string by commas, like cutting an apple into pieces
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

// find the boss of a node (used for union-find)
int findSet(int x, vector<int>& parent){
    if(parent[x]==x) return x;
    parent[x] = findSet(parent[x], parent); // make the road shorter next time
    return parent[x];
}

// make two groups become friends (connect them)
void unionSet(int a, int b, vector<int>& parent){
    a = findSet(a, parent);
    b = findSet(b, parent);
    if (a != b) parent[b] = a;
}

int main(){
    string country_s, build_s, destroy_s;
    // read three strings, all in one line or maybe in different lines
    if(!(cin >> country_s >> build_s >> destroy_s)) return 0;

    // cut them into small parts (rows)
    vector<string> country = splitByComma(country_s);
    vector<string> build   = splitByComma(build_s);
    vector<string> destroy = splitByComma(destroy_s);

    // clean up each part so no extra spaces stay inside
    for (size_t i=0;i<country.size();++i) trimSpaces(country[i]);
    for (size_t i=0;i<build.size();++i)   trimSpaces(build[i]);
    for (size_t i=0;i<destroy.size();++i) trimSpaces(destroy[i]);

    int n = (int)country.size();

    // check if something went wrong with input size
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

    vector<pair<int, pair<int, int>>> edges; // store roads and their cost
    long long base = 0;

    // check all pairs (i, j) but only the upper half so we don’t repeat
    for(int i=0; i<n; i++){
        for(int j=i+1; j<n; j++){
            if(country[i][j]=='1'){
                int d = getCost(destroy[i][j]);
                base += d;                         // first, pretend we destroy all roads
                edges.push_back({-d, {i, j}});     // negative = we save money if we keep it
            }else{
                int b = getCost(build[i][j]);
                edges.push_back({ b, {i, j}});     // positive = we pay to build a road
            }
        }
    }

    // sort roads by smallest cost first
    sort(edges.begin(), edges.end());

    // make each city be its own parent first
    vector<int> parent(n);
    for(int i=0;i<n;i++) parent[i]=i;

    long long total = base;

    // Kruskal’s way: connect cities with cheapest roads first
    for(int k=0; k<(int)edges.size(); k++){
        int w = edges[k].first;
        int u = edges[k].second.first;
        int v = edges[k].second.second;
        if (findSet(u, parent) != findSet(v, parent)){
            unionSet(u, v, parent);
            total += w;  // add the cost (if w < 0, we save money)
        }
    }

    cout << total << endl; // show the final number
    return 0;
}