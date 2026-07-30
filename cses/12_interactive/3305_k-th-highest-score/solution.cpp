#include <bits/stdc++.h>
using namespace std;

static const long long POSINF = (long long)4e18;
static const long long NEGINF = (long long)-4e18;

int n;
long long k;
vector<long long> fval, sval;
vector<char> fq, sq;

// Gửi một query và đọc giá trị trả về; endl bảo đảm flush stdout.
long long query(char c, int i) {
    cout << c << ' ' << i << endl; // endl -> flush, bắt buộc với bài tương tác
    long long x;
    cin >> x;
    return x;
}

// Lấy F[i] với biên ảo và cache: i<=0 -> +INF, i>n -> -INF, còn lại hỏi một lần.
long long getF(int i) {
    if (i <= 0) return POSINF;
    if (i > n)  return NEGINF;
    if (!fq[i]) { fval[i] = query('F', i); fq[i] = 1; } // chưa hỏi thì hỏi và cache
    return fval[i];
}

// Lấy S[i] với biên ảo và cache, tương tự getF.
long long getS(int i) {
    if (i <= 0) return POSINF;
    if (i > n)  return NEGINF;
    if (!sq[i]) { sval[i] = query('S', i); sq[i] = 1; }
    return sval[i];
}

int main() {
    if (!(cin >> n >> k)) return 0;

    fval.assign(n + 2, 0);
    sval.assign(n + 2, 0);
    fq.assign(n + 2, 0);
    sq.assign(n + 2, 0);

    // Khoảng tìm kiếm cho i (số phần tử lấy từ F): i thuộc [max(0,k-n), min(k,n)].
    int lo = (int)max(0LL, k - n);
    int hi = (int)min(k, (long long)n);
    long long ans = -1;

    // Binary search điểm cắt i*: partition top-i của F + top-j của S trùng top-k.
    while (lo <= hi) {
        int i = lo + (hi - lo) / 2;
        long long j = k - i;

        long long Fi  = getF(i);          // F[i]   (+INF nếu i==0)
        long long Sj1 = getS((int)j + 1); // S[j+1] (-INF nếu vượt mảng)
        long long Sj  = getS((int)j);     // S[j]   (+INF nếu j==0)
        long long Fi1 = getF(i + 1);      // F[i+1] (-INF nếu vượt mảng)

        bool A = Fi > Sj1;  // điều kiện A: không lấy quá nhiều từ F
        bool B = Sj > Fi1;  // điều kiện B: đã lấy đủ từ F

        if (A && B) {
            ans = min(Fi, Sj); // partition đúng -> đáp án là phần tử nhỏ nhất được chọn
            break;
        } else if (!A) {
            hi = i - 1;        // F[i] quá nhỏ -> lấy ít hơn từ F
        } else {               // !B
            lo = i + 1;        // cần lấy thêm từ F
        }
    }

    cout << "! " << ans << endl; // trả lời cuối cùng, endl -> flush
    return 0;
}
