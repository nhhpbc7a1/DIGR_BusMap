#include<bits/stdc++.h>
using namespace std;
const int N = 1e3+10;

struct ChuyenXe{
    int ma_tram_ke_tiep;
    int ma_tuyen;
};

ChuyenXe tr[N];

bool isTramValid(ChuyenXe tram) {
    return tram.ma_tram_ke_tiep != 0 && tram.ma_tuyen != 0;
}

map<string,int> ma_tram;
int count_tram, n;
string ds_ten_tram[N];
bool visited[N];

vector <ChuyenXe> ds_ke[N];

string JSON_string(stack<ChuyenXe> st) {
    // Tạo chuỗi JSON để lưu trữ kết quả
    string result = "[";
    bool first = true;

    while (!st.empty()) {
        ChuyenXe x = st.top();
        st.pop();

        if (x.ma_tuyen != 0) {
            if (!first) {
                result += ",";
            }

            result += "{";
            result += "\"tram\": \"" + ds_ten_tram[x.ma_tram_ke_tiep] + "\"";
            result += ", \"xe_buyt\": " + to_string(x.ma_tuyen);
            result += ", \"noi_den\": \"" + ds_ten_tram[st.top().ma_tram_ke_tiep] + "\"";
            result += "}";
        }
            first = false;
    }

    result += "],";
    return result;
}

// Hàm DFS tìm đường đi từ trạm 'u' đến trạm 'ma_tram_dich'
string dfs(int u, int ma_tram_dich) {
    if (u == ma_tram_dich) {
        // Tìm thấy trạm đích, truy vết đường đi
        stack<ChuyenXe> st;
        int tam = ma_tram_dich;
        st.push({ma_tram_dich, 0});  // Đẩy trạm đích vào ngăn xếp

        // Truy vết ngược dựa vào mảng tr
        while (isTramValid(tr[tam])) {
            st.push(tr[tam]);
            tam = tr[tam].ma_tram_ke_tiep;
        }

        // Chuyển đường đi sang chuỗi JSON
        return JSON_string(st);
    }

    visited[u] = true;  // Đánh dấu trạm hiện tại là đã thăm
    string result = "";

    // Duyệt qua tất cả các trạm kề của u
    for (ChuyenXe x : ds_ke[u]) {
        if (!visited[x.ma_tram_ke_tiep]) {
            // Lưu thông tin truy vết
            tr[x.ma_tram_ke_tiep] = {u, x.ma_tuyen};
            // Đệ quy tìm đường từ trạm kề
            result += dfs(x.ma_tram_ke_tiep, ma_tram_dich);
        }
    }

    visited[u] = false;  // Quay lui
    return result;
}
 
void doc_du_lieu() {
    // Mở file "ds_tuyen.txt" để đọc dữ liệu
    ifstream infile("ds_tuyen.txt");

    // Đọc số lượng tuyến xe buýt
    infile >> n;

    // Duyệt qua từng tuyến xe buýt
    for (int i = 1; i <= n; i++) {
        int ma_tuyen, so_luong_tram, ma_tram_truoc = 0;

        // Đọc mã tuyến và số lượng trạm trên tuyến
        infile >> ma_tuyen >> so_luong_tram;

        // Duyệt qua từng trạm trên tuyến xe buýt hiện tại
        for (int j = 1; j <= so_luong_tram; j++) {
            string ten_tram;

            // Đọc tên trạm từ file
            infile >> ten_tram;

            // Kiểm tra xem trạm đã có mã định danh chưa
            if (ma_tram[ten_tram] == 0) {
                // Nếu chưa có, gán mã định danh mới
                ma_tram[ten_tram] = ++count_tram;
                ds_ten_tram[count_tram] = ten_tram; // Lưu tên trạm
            }

            // Lấy mã trạm hiện tại
            int ma_tram_hien_tai = ma_tram[ten_tram];

            // Nếu không phải trạm đầu tiên trên tuyến
            if (j > 1) {
                // Kết nối trạm trước đó với trạm hiện tại
                ds_ke[ma_tram_truoc].push_back({ma_tram_hien_tai, ma_tuyen});
                ds_ke[ma_tram_hien_tai].push_back({ma_tram_truoc, ma_tuyen});
            }

            // Cập nhật trạm hiện tại trở thành trạm trước đó
            ma_tram_truoc = ma_tram_hien_tai;
        }
    }

    // Đóng file sau khi đọc xong
    infile.close();
}

void xuat_danh_sach_tram() {
    ofstream outfile("ds_tram.json");
    outfile << "[" << endl;
    for (int i = 1; i <= count_tram; i++) {
        outfile << "{\"id\":" << i << ", \"name\": \""<< ds_ten_tram[i] << "\"}";
        if (i != count_tram) outfile << ",";
        outfile << endl;
    }
    outfile << "]" << endl;
    outfile.close();
}

int main() {
    doc_du_lieu();
    xuat_danh_sach_tram();

    int start_id, end_id;
    cin >> start_id;
    cin >> end_id;

    string result = dfs(start_id, end_id);
    if (result[result.length() - 1] == ',')
        result.erase(result.length() - 1, 1);
    cout << "{ \"answers\":[" << result << "]}" << endl;
    return 0;
}
