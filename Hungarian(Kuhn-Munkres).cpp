#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ld = long double;
using ull = unsigned long long;
using pii = pair <int,int>;
using piii = pair <int,pii>;
#define name "kuhn-munkres"
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
const int mod = 1e9+7;
const int inf = 1e9;

const int lim = 1001;
int n;
int matchX[lim], matchY[lim]; // matchX[i]=j: trái i ghép với phải j, và ngược lại
int fx[lim], fy[lim];         // potential (nhãn) của đỉnh trái và phải
int trace[lim];                 // trace[j]=i: đỉnh trái i dẫn đến j trong BFS đồng thời đánh dấu j đã được thăm (trace[j]!=0)
int c[lim][lim];              // ma trận chi phí
int s;                           // đỉnh trái đang xét (source)
// Reduced cost của cạnh (i,j)
// Luôn >= 0 nhờ điều kiện feasibility: fx[i] + fy[j] <= c[i][j]
// Bằng 0 <=> cạnh "tight" <=> được phép đi qua trong BFS
int get_weight(int i, int j){
    return c[i][j] - fx[i] - fy[j];
}

// BFS tìm đường tăng từ s trên các cạnh tight (get_weight == 0)
// Đường tăng có dạng: s -> j1 -> matchY[j1] -> j2 -> matchY[j2] -> ... -> j_free
// Trả về đỉnh phải tự do cuối đường, hoặc 0 nếu không tìm được
int find_path(){
    queue<int> q;
    fof(i,1,n) trace[i] = 0; // reset trace, đồng thời reset "đã thăm"
    q.push(s);                // bắt đầu BFS từ đỉnh trái s
    while(!q.empty()){
        int i = q.front(); q.pop();
        fof(j,1,n){
            if (!trace[j] && !get_weight(i,j)){ // j chưa thăm VÀ cạnh (i,j) tight
                trace[j] = i;                    // ghi cha: j được dẫn đến bởi i
                if (!matchY[j]) return j;        // j chưa ghép -> tìm được đường tăng!
                q.push(matchY[j]);               // j đã ghép -> đẩy matchY[j] vào queue để tiếp tục BFS từ đỉnh trái matchY[j]
            }
        }
    }
    return 0; // BFS thất bại, không có đường tăng trên cạnh tight hiện tại
}

// Cập nhật potential khi BFS thất bại
// Mục tiêu: tạo thêm ít nhất 1 cạnh tight mới mà không vi phạm feasibility
// delta = min reduced cost qua "biên cắt" (từ đỉnh đã thăm sang đỉnh chưa thăm)
void update_label(){
    int d = 1e9;
    // Tái dựng tập đỉnh đã thăm từ trace[] (không cần lưu vis riêng trong BFS)
    bool vis_x[lim], vis_y[lim];
    fill(vis_x, vis_x+n+1, 0);
    fill(vis_y, vis_y+n+1, 0);
    vis_x[s] = 1; // s luôn đã thăm (nguồn BFS)
    fof(j,1,n){
        if (trace[j] != 0){       // j đã được thăm trong BFS vừa thất bại
            vis_x[matchY[j]] = 1; // -> đỉnh trái ghép với j cũng đã thăm
            vis_y[j] = 1;         // -> j đã thăm bên phải
        }
    }
    // Tìm delta = min get_weight(i,j) với i đã thăm, j chưa thăm
    // Đây là "khoảng cách" nhỏ nhất để mở thêm cạnh tight mới
    fof(i,1,n){
        if (vis_x[i]){
            fof(j,1,n){
                if (!vis_y[j] && get_weight(i,j) < d)
                    d = get_weight(i,j);
            }
        }
    }
    // Cập nhật potential:
    // - đỉnh trái đã thăm: fx[i] += d  -> reduced cost từ i giảm đi d
    // - đỉnh phải đã thăm: fy[j] -= d  -> giữ nguyên reduced cost các cạnh tight cũ
    // Kết quả: ít nhất 1 cạnh tight mới xuất hiện, feasibility vẫn duy trì
    fof(t,1,n){
        if (vis_x[t]) fx[t] += d;
        if (vis_y[t]) fy[t] -= d;
    }
}

// Flip đường tăng kết thúc tại y
// Đi ngược theo trace[] từ y về s, xen kẽ gán lại matchX và matchY
void update(int y){
    int x, nx;
    do {
        x  = trace[y];   // đỉnh trái cha của y (dẫn đến y trong BFS)
        nx = matchX[x];  // đỉnh phải x đang ghép hiện tại (sẽ bị "đẩy" lên trên)
        matchX[x] = y;   // x ghép với y (cạnh mới)
        matchY[y] = x;   // y ghép với x (cạnh mới)
        y = nx;          // đi ngược lên: xét đỉnh phải bị đẩy
    } while (y != 0);    // dừng khi y=0: x=s chưa ghép -> đầu đường tăng
}

void hungarian(){
    // Khởi tạo: chưa ghép, potential = 0
    fof(i,1,n) matchX[i] = matchY[i] = fx[i] = fy[i] = 0;
    int y;
    fof(i,1,n){
        s = i; // ghép đỉnh trái i
        // Lặp: tìm đường tăng, nếu không được thì update potential rồi thử lại
        // Mỗi lần update_label() tạo thêm ít nhất 1 cạnh tight
        // -> sau tối đa n lần update, BFS chắc chắn tìm được đường tăng
        do {
            y = find_path();
            if (y == 0) update_label();
        } while (y == 0);
        update(y); // flip đường tăng tìm được
    }
}