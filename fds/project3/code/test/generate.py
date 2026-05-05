# 该代码全部由 Gemini 生成,未经人工修改,仅仅用于生成大规模测试样例,仅供参考

import random
import heapq
import os

def generate_graph(V, E):
    """生成连通的无向图"""
    edges = set()
    # 1. 保证图绝对连通：先生成一棵包含所有顶点的生成树
    for i in range(2, V + 1):
        u = random.randint(1, i - 1)
        edges.add((min(u, i), max(u, i)))
        
    # 2. 填充剩余的边，直到达到目标 E
    while len(edges) < E:
        u = random.randint(1, V)
        v = random.randint(1, V)
        if u != v:
            edge = (min(u, v), max(u, v))
            edges.add(edge)
            
    # 3. 赋予随机权重 (1 到 100) 并生成邻接表
    graph_edges = []
    adj = {i: [] for i in range(1, V + 1)}
    for u, v in edges:
        w = random.randint(1, 100)
        graph_edges.append((u, v, w))
        adj[u].append((v, w))
        adj[v].append((u, w))
        
    return graph_edges, adj

def get_valid_dijkstra_sequence(adj, start, V):
    """模拟 Dijkstra 算法，生成一个绝对合法的 YES 序列"""
    dist = {i: float('inf') for i in range(1, V + 1)}
    dist[start] = 0
    know = {i: False for i in range(1, V + 1)}
    seq = []
    
    for _ in range(V):
        # 寻找当前未确定的、距离最小的顶点集合
        min_d = float('inf')
        candidates = []
        for i in range(1, V + 1):
            if not know[i]:
                if dist[i] < min_d:
                    min_d = dist[i]
                    candidates = [i]
                elif dist[i] == min_d:
                    candidates.append(i)
                    
        if not candidates:
            break
            
        # 随机挑选一个符合资格的极小值点，保证多分支情况下的随机性
        u = random.choice(candidates)
        know[u] = True
        seq.append(u)
        
        # 松弛邻居
        for v, w in adj[u]:
            if not know[v] and dist[u] + w < dist[v]:
                dist[v] = dist[u] + w
                
    # 把不连通的节点补在最后（理论上上面已保证连通）
    for i in range(1, V + 1):
        if not know[i]:
            seq.append(i)
            
    return seq

def write_test_case(filename, V, E, K):
    """将测试样例写入文件"""
    print(f"正在生成 {filename} ... (V={V}, E={E})")
    graph_edges, adj = generate_graph(V, E)
    
    with open(filename, 'w') as f:
        # 写入 V 和 E
        f.write(f"{V} {E}\n")
        # 写入所有边
        for u, v, w in graph_edges:
            f.write(f"{u} {v} {w}\n")
            
        # 写入 K
        f.write(f"{K}\n")
        
        # 生成 K 个序列 (一半 YES，一半 NO)
        for i in range(K):
            start_node = random.randint(1, V)
            valid_seq = get_valid_dijkstra_sequence(adj, start_node, V)
            
            if i % 2 == 0:
                # 写入合法序列 (YES)
                f.write(" ".join(map(str, valid_seq)) + "\n")
            else:
                # 写入非法序列 (NO)：保留起点不变，反转后续所有节点的处理顺序
                invalid_seq = [valid_seq[0]] + valid_seq[1:][::-1]
                f.write(" ".join(map(str, invalid_seq)) + "\n")
                
    print(f"{filename} 生成完毕！\n")

if __name__ == "__main__":
    # 1. 生成 Test 5: 稠密图基础验证 (V=25, E=260 > 10*V)
    write_test_case("test5_dense.txt", 25, 260, 4)
    
    # 2. 生成极限数据: 极限稀疏图 (V=1000, E=9000 < 10*V)
    write_test_case("test_max_sparse.txt", 1000, 9000, 10)
    
    # 3. 生成极限数据: 极限稠密图 (V=1000, E=100000 > 10*V)
    write_test_case("test_max_dense.txt", 1000, 100000, 10)