#include<bits/stdc++.h>
using namespace std;
const int N = 1e3+10;

struct ChuyenXe{
    int ma_tram_ke_tiep;
    int ma_tuyen;
} tr[N];

bool isTramValid(ChuyenXe tram) {
    return tram.ma_tram_ke_tiep != 0 && tram.ma_tuyen != 0;
}

map<string,int> ma_tram;
int count_tram, n;
string ds_ten_tram[N];
bool visited[N];

vector <ChuyenXe> ds_ke[N];

void dfs(int u, int ma_tram_dich){
    if (u == ma_tram_dich) {
        stack <ChuyenXe> st;
        int tam = ma_tram_dich;
        st.push({ma_tram_dich, 0});

        while (isTramValid(tr[tam])) {
            st.push(tr[tam]);
            tam = tr[tam].ma_tram_ke_tiep;
        }

        cout << "\n== Cach di ===\n" ;
        while (!st.empty()) {
            ChuyenXe x = st.top();
            cout << ds_ten_tram[x.ma_tram_ke_tiep] << endl;
            if (x.ma_tuyen != 0) cout << "Len xe buyt so: " << x.ma_tuyen << " ==> Di den: " << endl;
            st.pop();
        }
        return;
    }

    visited[u] = true;
    for (int i = 0;  i < ds_ke[u].size(); i++) {
        ChuyenXe x = ds_ke[u][i];
        if (!visited[x.ma_tram_ke_tiep]) {
            tr[x.ma_tram_ke_tiep] = {u, x.ma_tuyen};
            dfs(x.ma_tram_ke_tiep, ma_tram_dich);
        }
    }
    visited[u] = false;
}

void doc_du_lieu() {
    ifstream infile("danhsach.txt");

    infile >> n;
    for (int i = 1; i <= n; i++) {
        int ma_tuyen, so_luong_tram, ma_tram_truoc;
        infile >> ma_tuyen >> so_luong_tram;

        // == Nhap va danh so thu tu cac tram
        for (int j = 1; j <= so_luong_tram; j++) {
            string ten_tram;
            infile >> ten_tram;
            if (ma_tram[ten_tram] == 0) {
                ma_tram[ten_tram] = ++count_tram;
                ds_ten_tram[count_tram] = ten_tram;
            }


            int ma_tram_hien_tai = ma_tram[ten_tram];

            if (j > 1) {
                ds_ke[ma_tram_truoc].push_back({ma_tram_hien_tai, ma_tuyen});
                ds_ke[ma_tram_hien_tai].push_back({ma_tram_truoc, ma_tuyen});
            }

            ma_tram_truoc = ma_tram_hien_tai;
            //cout << ma_tram_hien_tai << " ";
        }
        //cout << endl;
    }

    for (int i = 1; i <= count_tram; i++) {
        cout << "ID: " << i << ", Name: " << ds_ten_tram[i] << endl;
    }
    cout << endl;

    infile.close();
}
int main() {
    doc_du_lieu();
    int start_id, end_id;
    cout << "Nhap ma tram dau: ";
    cin >> start_id;
    cout << "Nhap ma tram dich: ";
    cin >> end_id;

    dfs(start_id, end_id);
}
