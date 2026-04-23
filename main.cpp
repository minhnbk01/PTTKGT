#include <iostream>
#include <vector> 
#include <algorithm> 
#include <cmath> 
using namespace std; 
class QuanLyKho { 
private: 
    int n; // Số lượng đồ vật 
    int C; // Sức chứa của 1 thùng 
    vector<int> w; // Danh sách trọng lượng các món hàng
    int best_bins; // Kỷ lục số thùng ít nhất tìm được (Upper Bound) 

    // THAM LAM (First Fit)
    int firstFit(const vector<int>& weights) { 
        vector<int> danh_sach_thung; 
        for (int i = 0; i < weights.size(); i++) { 
            bool da_xep = false; 
            for (int j = 0; j < danh_sach_thung.size(); j++) { 
                if (danh_sach_thung[j] + weights[i] <= C) { 
                    danh_sach_thung[j] += weights[i]; 
                    da_xep = true; 
                    break; 
                } 
            } 
            if (!da_xep) { 
                danh_sach_thung.push_back(weights[i]); 
            } 
        } 
        return danh_sach_thung.size(); 
    } 

    // THAM LAM (Best Fit)
    int bestFit(const vector<int>& weights) { 
        vector<int> danh_sach_thung; 
        for (int i = 0; i < weights.size(); i++) { 
            int khoang_trong_min = C+1;
            int vi_tri_thung_tot_nhat = -1; 
            
            for (int j = 0; j < danh_sach_thung.size(); j++) { 
                if (danh_sach_thung[j] + weights[i] <= C) { 
                    int khoang_trong_con_lai = C - (danh_sach_thung[j] + weights[i]); 
                    if (khoang_trong_con_lai < khoang_trong_min) { 
                        khoang_trong_min = khoang_trong_con_lai; 
                        vi_tri_thung_tot_nhat = j; 
                    } 
                } 
            } 
            if (vi_tri_thung_tot_nhat != -1) { 
                danh_sach_thung[vi_tri_thung_tot_nhat] += weights[i]; 
            } else { 
                danh_sach_thung.push_back(weights[i]); 
            } 
        } 
        return danh_sach_thung.size(); 
    } 

    

    int firstFitDecreasing(const vector<int>& weights) {
        vector<int> sorted_w = weights;
        sort(sorted_w.rbegin(), sorted_w.rend());
        return firstFit(sorted_w);
    }

    int bestFitDecreasing(const vector<int>& weights) {
        vector<int> sorted_w = weights;
        sort(sorted_w.rbegin(), sorted_w.rend());
        return bestFit(sorted_w);
    }

    // QUY HOẠCH ĐỘNG (DP - Knapsack) 
    int thuatToanQuyHoachDong() { 
        if (n == 0) return 0;

        int so_thung_da_dung = 0; 
        vector<bool> da_xep_vao_thung(n, false); 
        int hang_con_lai = n; 
        
        while (hang_con_lai > 0) {
            vector<int> dp(C + 1, 0); 
            vector<int> truy_vet_item(C + 1, -1);
            vector<int> truy_vet_cap(C + 1, -1);
            
            for (int i = 0; i < n; i++) { 
                if (da_xep_vao_thung[i]) continue; 
                for (int c = C; c >= w[i]; c--) { 
                    if (dp[c - w[i]] + w[i] > dp[c]) { 
                        dp[c] = dp[c - w[i]] + w[i]; 
                        truy_vet_item[c] = i;
                        truy_vet_cap[c] = c - w[i];
                    } 
                } 
            } 

            if (dp[C] == 0) {
                for (int i = 0; i < n; i++) {
                    if (!da_xep_vao_thung[i]) {
                        da_xep_vao_thung[i] = true;
                        hang_con_lai--;
                        so_thung_da_dung++;
                        break;
                    }
                }
                continue;
            }

            int suc_chua_hien_tai = C; 
            while (suc_chua_hien_tai > 0 && truy_vet_item[suc_chua_hien_tai] != -1) {
                int item = truy_vet_item[suc_chua_hien_tai];
                da_xep_vao_thung[item] = true;
                hang_con_lai--;
                suc_chua_hien_tai = truy_vet_cap[suc_chua_hien_tai];
            } 
            so_thung_da_dung++; 
        } 
        return so_thung_da_dung; 
    } 

    // HÀM HỖ TRỢ: TÍNH CHẶN DƯỚI (Lower Bound cho B&B) 
    int tinhChanDuoi(int vi_tri_hang, int so_thung_hien_tai, const vector<int>& khong_gian_thung) { 
        int tong_trong_luong_con_lai = 0;
        for (int i = vi_tri_hang; i < n; i++) { 
            tong_trong_luong_con_lai += w[i]; 
        } 
        
        int tong_khong_gian_trong = 0; 
        for (int space : khong_gian_thung) { 
            tong_khong_gian_trong += space; 
        } 
        
        if (tong_trong_luong_con_lai <= tong_khong_gian_trong) return so_thung_hien_tai; 
        
        int trong_luong_du = tong_trong_luong_con_lai - tong_khong_gian_trong; 
        return so_thung_hien_tai + ceil((double)trong_luong_du / C); 
    } 

    // NHÁNH VÀ CẬN (Branch & Bound) 
    void nhanhVaCan(int vi_tri_hang, int so_thung_hien_tai, vector<int>& khong_gian_thung) { 
        if (so_thung_hien_tai >= best_bins) return; 
        
        if (vi_tri_hang == n) { 
            best_bins = min(best_bins, so_thung_hien_tai); 
            return; 
        } 
        
        int bound = tinhChanDuoi(vi_tri_hang, so_thung_hien_tai, khong_gian_thung); 
        if (bound >= best_bins) return; 
        
        // Thử đặt vào thùng cũ
        for (int i = 0; i < so_thung_hien_tai; i++) { 
            if (khong_gian_thung[i] >= w[vi_tri_hang]) { 
                khong_gian_thung[i] -= w[vi_tri_hang];
                nhanhVaCan(vi_tri_hang + 1, so_thung_hien_tai, khong_gian_thung); 
                khong_gian_thung[i] += w[vi_tri_hang]; 
            } 
        } 
        
        // Mở thùng mới
        khong_gian_thung.push_back(C - w[vi_tri_hang]); 
        nhanhVaCan(vi_tri_hang + 1, so_thung_hien_tai + 1, khong_gian_thung); 
        khong_gian_thung.pop_back(); 
    } 

public: 
    // Constructor 
    QuanLyKho(int _n, int _C, const vector<int>& _w) { 
        n = _n; 
        C = _C; 
        w = _w; 
        best_bins = n; // Mặc định xấu nhất: mỗi món 1 thùng 
    } 

    // Hàm xuất kết quả 
    void giaiQuyet() { 
        // 1. Tham lam trên thứ tự GỐC
        int kq_first_fit = firstFit(w); 
        int kq_best_fit = bestFit(w);

        // 2. Tham lam trên thứ tự GIẢM DẦN (FFD, BFD)
        int kq_ffd = firstFitDecreasing(w);
        int kq_bfd = bestFitDecreasing(w);

        // 3. Quy hoạch động
        sort(w.rbegin(), w.rend());
        int kq_dp = thuatToanQuyHoachDong();

        // Upper bound tốt nhất từ tất cả heuristic + DP
        best_bins = min({kq_first_fit, kq_best_fit, kq_ffd, kq_bfd, kq_dp});

        // 4. Nhánh và Cận (dùng bản sorted cho B&B)
        sort(w.rbegin(), w.rend());
        vector<int> khong_gian_cac_thung; 
        nhanhVaCan(0, 0, khong_gian_cac_thung); 
        
        // In kết quả
        cout << "\n[+] 1. Thuat toan Tham Lam (First Fit) dung: " << kq_first_fit << " thung"; 
        cout << "\n[+] 2. Thuat toan Tham Lam (Best Fit) dung: " << kq_best_fit << " thung"; 
        cout << "\n[+] 3. First Fit Decreasing (FFD) dung: " << kq_ffd << " thung"; 
        cout << "\n[+] 4. Best Fit Decreasing (BFD) dung: " << kq_bfd << " thung"; 
        cout << "\n[+] 5. Thuat toan Quy Hoach Dong (DP) dung: " << kq_dp << " thung"; 
        cout << "\n[+] 6. Thuat toan Nhanh va Can (B&B) dung: " << best_bins << " thung\n"; 
        cout << "=> KET LUAN CHUNG: So thung toi uu tuyet doi la: " << best_bins << " thung.\n"; 
    } 
}; 
int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int n, C;
    cout << "=== DO AN PTTKGT - CASE STUDY 5: QUAN LY KHO ===\n";
    int testCase = 1;
    while (cin >> n >> C) {
        cout << "\nTEST CASE " << testCase++ << endl;
        cout << "So luong hang (n): " << n << "\nSuc chua moi thung (C): " << C << endl;
        
        vector<int> w(n);
        cout << "Trong luong cac mon hang: ";
        for (int i = 0; i < n; i++) {
            cin >> w[i];
            cout << w[i] << " ";
        }
        cout << "\n";
        
        QuanLyKho he_thong(n, C, w);
        he_thong.giaiQuyet();
        cout << "\n";
    }

    return 0;
}