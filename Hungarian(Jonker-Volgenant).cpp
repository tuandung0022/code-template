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

const int lim = 1001;
int n;
int matchx[lim], matchy[lim]; // matchx[i]=j: trái i ghép với phải j, và ngược lại. matchy[0] = "nguồn ảo" — trick để xử lý start đồng nhất
int fx[lim], fy[lim];         // potential (nhãn) đỉnh trái và phải
int trace[lim];                 // trace[j] = đỉnh phải "cha" của j trong đường tăng (khác Kuhn-Munkres: trace lưu đỉnh PHẢI, không phải trái)
int dist[lim];                  // dist[j] = reduced cost nhỏ nhất từ start đến j (Dijkstra distance, tích lũy dần, không reset giữa chừng)
int c[lim][lim];              // ma trận chi phí
bool vis_y[lim];               // vis_y[j] = đỉnh phải j đã được "chốt" vào cây Dijkstra
int res;                         // tổng chi phí

// find_path: tìm đường tăng từ start bằng Dijkstra trên reduced cost
// d: tham chiếu, lưu delta cuối cùng (dùng để update potential liên tục)
// Trả về đỉnh phải tự do cuối đường tăng
int find_path(int start, int &d){
    // Trick nguồn ảo: coi j=0 là đỉnh phải ảo, luôn "ghép sẵn với start"
    // -> vòng lặp xử lý start đồng nhất với mọi đỉnh trái khác, không cần case đặc biệt
    matchy[0] = start;

    // Khởi tạo Dijkstra: dist = vô cực, chưa thăm đỉnh nào
    fof(j,1,n){
        dist[j] = 1e9;
        trace[j] = 0;
        vis_y[j] = 0;
    }
    vis_y[0] = 1;    // chốt nguồn ảo ngay từ đầu
    int matched_y = 0; // đỉnh phải vừa được chốt (bắt đầu từ nguồn ảo j=0)

    do {
        int cur_x = matchy[matched_y]; // đỉnh trái ghép với đỉnh vừa chốt. Lần đầu: cur_x = matchy[0] = start
        d = 1e9;
        int ny = 0; // đỉnh phải chưa chốt có dist nhỏ nhất (sẽ chốt tiếp)

        // Relaxation: cập nhật dist[] từ cur_x sang các đỉnh phải chưa chốt
        fof(j,1,n){
            if (!vis_y[j]){
                // Reduced cost = c[i][j] - fx[i] - fy[j], luôn >= 0 nhờ potential
                // Đây là "khoảng cách thực" trong Dijkstra sau khi trừ nhãn
                int cdist = c[cur_x][j] - fx[cur_x] - fy[j];
                if (cdist < dist[j]){
                    dist[j] = cdist;
                    trace[j] = matched_y; // j đến gần hơn qua matched_y
                }
                // Tìm ny = đỉnh phải chưa chốt có dist nhỏ nhất (Dijkstra greedy)
                if (dist[j] < d){
                    d = dist[j];
                    ny = j;
                }
            }
        }

        // Update potential ngay sau mỗi bước chốt — điểm khác biệt với Kuhn-Munkres
        // Kuhn-Munkres update lazily (khi BFS thất bại)
        // Jonker-Volgenant update eagerly (sau mỗi bước Dijkstra)
        // d = dist[ny] = delta nhỏ nhất qua biên cắt hiện tại
        fof(j,0,n){
            if (vis_y[j]){
                fx[matchy[j]] += d; // trái đã chốt: tăng potential
                fy[j] -= d;         // phải đã chốt: giảm potential
                                    // -> giữ nguyên reduced cost các cạnh đã tight
            }
            else dist[j] -= d;      // phải chưa chốt: "kéo" dist về 0 <=> tương đương shift trục Dijkstra, tránh dùng heap
        }
        // Chốt ny vào cây Dijkstra
        matched_y = ny;
        vis_y[ny] = 1;
    // Dừng khi ny là đỉnh phải chưa ghép (matchy[ny] == 0) -> tìm được đường tăng
    // Không bao giờ thất bại vì Dijkstra luôn tìm được đường (khác Kuhn-Munkres)
    } while (matchy[matched_y] != 0);
    return matched_y; // đỉnh phải tự do cuối đường tăng
}

// re_match: flip đường tăng kết thúc tại y
// Đi ngược theo trace[] từ y về nguồn ảo (j=0)
// trace lưu đỉnh PHẢI cha -> flip theo hướng đỉnh phải (ngược với Kuhn-Munkres flip theo trái)
void re_match(int y){
    do {
        int pre_y = trace[y];       // đỉnh phải cha của y trong đường tăng
        matchy[y] = matchy[pre_y];  // y nhận đỉnh trái của cha pre_y=0: matchy[0]=start -> start ghép với y ✓
        matchx[matchy[y]] = y;      // cập nhật ngược: đỉnh trái vừa gán biết mình ghép với y
        y = pre_y;                  // đi ngược lên
    } while (y != 0);               // dừng khi về nguồn ảo j=0
}

void hungarian(){
    fof(i,1,n)
        matchx[i] = matchy[i] = fx[i] = fy[i] = 0; // khởi tạo: chưa ghép, potential=0
    fof(i,1,n){
        int d = 0;
        // find_path luôn thành công (không cần vòng do-while như Kuhn-Munkres)
        // vì Dijkstra đảm bảo tìm được đường tăng trong đúng n bước
        int last_y = find_path(i, d);
        re_match(last_y);
    }
}

int main(){
    ios_base::sync_with_stdio(0); 
    cin.tie(0); cout.tie(0);
    cin >> n;
    fof(i,1,n) fof(j,1,n) cin >> c[i][j];
    hungarian();
    ll total = 0;
    fof(i,1,n) total += c[i][matchx[i]];
    cout << "Min cost: " << total << "\n";
    fof(i,1,n)
        cout << i << " - " << matchx[i] << " (w=" << c[i][matchx[i]] << ")\n";
    return 0;
}