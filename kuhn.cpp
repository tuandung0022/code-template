#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ld = long double;
using ull = unsigned long long;
using pii = pair <int,int>;
using piii = pair <int,pii>;
#define fof(i,a,b) for (int i = a; i <= b; i++)
#define fob(i,a,b) for (int i = a; i >= b; i--)
#define foff(i,a,b,s) for (int i = a; i <= b; i += s)
#define fobb(i,a,b,s) for (int i = a; i >= b; i -= s)
#define foe(a,b) for (auto &a:b)
#define sz(a) (int) a.size()
#define all(a) a.begin(),a.end()
#define all1(a) a.begin()+1,a.end()
#define fi first
#define se second
#define pb push_back
#define pob pop_back
#define pf push_front
#define pof pop_front
#define el "\n"
const int mod =  1e9+7;
const ll inf = 1e18;

int m,n;
vector<vector<int>> adj;
vector<int> matchX,matchY,seen;
int timer;
int res;

struct kuhn_dfs {
    int matchX[1001], matchY[1001], seen[1001];
    int timer;

    bool dfs(int u){
        if (seen[u] == timer) return 0;
        seen[u] = timer;
        for (int v : adj[u]){
            if (matchY[v] == -1 || dfs(matchY[v])){
                matchX[u] = v; matchY[v] = u;
                return 1;
            }
        }
        return 0;
    }

    void solve(){
        fill(matchX, matchX+m+1, -1);
        fill(matchY, matchY+n+1, -1);
        fill(seen, seen+m+1, 0);
        timer = 0;
        fof(u,1,m){
            ++timer;
            if (dfs(u)) res++;
        }
    }
} kuhn_dfs_solver;

struct kuhn_bfs {
    int matchX[1001], matchY[1001];
    int prevX[1001]; 
    int bfs(int s){
        fill(prevX+1, prevX+n+1, -1); // reset trace
        queue<int> q;
        q.push(s);
        while (!q.empty()){
            int u = q.front(); q.pop();
            for (int v : adj[u]){
                if (prevX[v] != -1) continue; // v đã thăm
                prevX[v] = u;                  // ghi cha: v được dẫn đến bởi u
                if (matchY[v] == -1) return v; // v tự do -> tìm được đường tăng!
                q.push(matchY[v]);             // v đã ghép -> đẩy matchY[v] vào queue
            }
        }
        return -1;
    }

    // Flip đường tăng kết thúc tại đỉnh phải endV
    // Đi ngược theo prevX[] từ endV về s
    void augment(int endV){
        int v = endV;
        while (v != -1){
            int u  = prevX[v];       // đỉnh trái dẫn đến v
            int pv = matchX[u];      // đỉnh phải u đang ghép (sẽ bị đẩy lên)
            matchX[u] = v;
            matchY[v] = u;
            v = pv;                  // đi ngược lên
        }
    }

    void solve(){
        fill(matchX, matchX+m+1, -1);
        fill(matchY, matchY+n+1, -1);
        fof(u,1,m){
            if (matchX[u] == -1){    // u chưa ghép -> thử tìm đường tăng
                int endV = bfs(u);
                if (endV != -1){
                    augment(endV);
                    res++;
                }
            }
        }
    }
} kuhn_bfs_solver;

int main(){
    ios_base::sync_with_stdio(0); 
    cin.tie(0); cout.tie(0);

    cin >> m >> n;
    int u, v;
    while (cin >> u >> v)
        adj[u].push_back(v);

    kuhn_bfs_solver.solve();
    auto& match = kuhn_bfs_solver.matchX;
    cout << res << el;
    fof(u,1,m)
        if (match[u] != -1)
            cout << u << " " << match[u] << el;

    return 0;
}