#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ld = long double;
using ull = unsigned long long;
using pii = pair <int,int>;
using piii = pair <int,pii>;
using pll = pair <ll,ll>;
#define fof(i,a,b) for (int i = a; i <= b; ++i)
#define fob(i,a,b) for (int i = a; i >= b; --i)
#define foff(i,a,b,s) for (int i = a; i <= b; i += s)
#define fobb(i,a,b,s) for (int i = a; i >= b; i -= s)
#define foe(a,b) for (auto &a:b)
#define all(a) a.begin(),a.end()
#define all1(a) a.begin()+1,a.end()
#define fi first
#define se second
#define pb push_back
#define pob pop_back
#define pf push_front
#define pof pop_front
#define el "\n"
const int mod = 1e9+7;
const ll infl = 1e18;
const int inf = 2e9;

const int N = 2e5+1;
int n;
vector<int> adj[N];
int sz[N];

void dfs_sz(int u, int p){
    sz[u] = 1;
    for (int v:adj[u]){
        if (v == p) continue;
        dfs_sz(v,u);
        sz[u] += sz[v];
    }
}

int find_centroid(int u, int p){
    for (int v:adj[u]){
        if (v == p) continue;
        if (sz[v] > n/2) return find_centroid(v,u);
    }
    return u;
}

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    
    cin >> n;
    fof(i,1,n-1){
        int u,v;
        cin >> u >> v;
        adj[u].pb(v);
        adj[v].pb(u);
    }
    dfs_sz(1,0);
    cout << find_centroid(1,0);
    return 0;
}