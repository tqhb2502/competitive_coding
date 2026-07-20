# Giant Pizza - https://cses.fi/problemset/task/1684
# 2-SAT: mỗi topping là một biến boolean, mỗi thành viên là một clause (a OR b).
# Xây dựng implication graph, tìm SCC bằng Tarjan (iterative), suy ra đáp án.
import sys


def main():
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); m = int(data[pos + 1]); pos += 2

    N = 2 * m  # 2 node cho mỗi biến: 2*i = "topping i+1 được chọn", 2*i+1 = "không chọn"
    adj = [[] for _ in range(N)]

    # node của literal (sign, x): 2*(x-1) nếu '+', 2*(x-1)+1 nếu '-'.
    # phủ định của một node là node ^ 1.
    plus = ord('+')
    for _ in range(n):
        s1 = data[pos]; x1 = int(data[pos + 1])
        s2 = data[pos + 2]; x2 = int(data[pos + 3])
        pos += 4
        a = 2 * (x1 - 1) + (0 if s1[0] == plus else 1)
        b = 2 * (x2 - 1) + (0 if s2[0] == plus else 1)
        # clause (a OR b): (¬a => b) và (¬b => a)
        adj[a ^ 1].append(b)
        adj[b ^ 1].append(a)

    # Tarjan SCC iterative. comp được đánh số theo thứ tự hoàn thành SCC
    # (sink trước): SCC càng gần sink thì comp càng nhỏ -> reverse topo order.
    indices = [-1] * N
    low = [0] * N
    on_stack = bytearray(N)
    comp = [-1] * N
    it = [0] * N
    scc_stack = []
    work = []
    index_counter = 0
    comp_count = 0

    for start in range(N):
        if indices[start] != -1:
            continue
        work.append(start)
        indices[start] = low[start] = index_counter
        index_counter += 1
        scc_stack.append(start)
        on_stack[start] = 1
        while work:
            v = work[-1]
            av = adj[v]
            advanced = False
            lv = len(av)
            iv = it[v]
            while iv < lv:
                w = av[iv]
                iv += 1
                if indices[w] == -1:
                    it[v] = iv
                    indices[w] = low[w] = index_counter
                    index_counter += 1
                    scc_stack.append(w)
                    on_stack[w] = 1
                    work.append(w)
                    advanced = True
                    break
                elif on_stack[w]:
                    if indices[w] < low[v]:
                        low[v] = indices[w]
            if advanced:
                continue
            it[v] = iv
            if low[v] == indices[v]:
                while True:
                    w = scc_stack.pop()
                    on_stack[w] = 0
                    comp[w] = comp_count
                    if w == v:
                        break
                comp_count += 1
            work.pop()
            if work:
                u = work[-1]
                if low[v] < low[u]:
                    low[u] = low[v]

    out = []
    for i in range(m):
        a = 2 * i
        b = a + 1
        if comp[a] == comp[b]:
            sys.stdout.write("IMPOSSIBLE\n")
            return
        # literal có comp nhỏ hơn (gần sink) được chọn = true
        out.append('+' if comp[a] < comp[b] else '-')

    sys.stdout.write(' '.join(out) + '\n')


main()
