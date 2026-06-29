#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair <int,int>;
using pll = pair <ll,ll>;
#define fof(i,a,b) for (int i = a; i <= b; ++i)
#define fob(i,a,b) for (int i = a; i >= b; --i)
#define sz(a) (int) a.size()
#define fi first
#define se second
#define pb push_back
#define pob pop_back
#define eb emplace_back;
#define el "\n"
const int mod = 1e9+7;
const ll infl = 1e18;
const int inf = 2e9;

const int lim = 1e5+1;
struct rollbackdsu{
    int par[lim];
    vector<pii> snapshot;

    rollbackdsu(){memset(par,-1,sizeof(par));}

    int find_set(int v){
        while(par[v] > 0) v = par[v];
        return v;
    }

    bool unite_set(int u, int v){
        u = find_set(u);
        v = find_set(v);
        if (u == v) return 0;
        if (-par[u] < -par[v]) swap(u,v);
        snapshot.pb({v, par[v]});
        par[u] += par[v];
        par[v] = u;
        return 1;
    }

    void rollback(){
        pii c = snapshot.back();
        snapshot.pob();
        par[par[c.fi]] -= c.se;
        par[c.fi] = c.se;
    }
} f;
int cnt = 0;
int ans[lim];