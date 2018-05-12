// problem: Codeforces 103E
// two steps. 1. convert the original graph with bipartite match 2. find the maximal closed graph with maximum flow algorithm
#include <cstdio>
#include <iostream>
#include <cstring>
using namespace std;
#define INF 2000000000

bool match(int x0, int points[][300], int matched_set[], int matched_pt[], int size[])
{
    bool visited[300]; // in the effort to find an expandable path, whether yi is alr visited
    memset(visited, 0, 300 * sizeof(bool));
    int xi = 0;
    int reachable_set[300], pt_predecessor[300];
    reachable_set[0] = x0;
    int count = 1, point;
    while (1)
    {
        int index = 0; // index in reachable_set
        bool flag = 0; // whether ever visited a point
        while (index < count)
        {
            int xi = reachable_set[index];
            for (int j = 0; j < size[xi]; j++)
            {
                point = points[xi][j];
                if (!visited[point])
                {
                    pt_predecessor[point] = xi;
                    visited[point] = 1;
                    flag = 1;
                    if (matched_pt[point] == -1) // this point is not matched
                    {
                        // found an expandable path! update match along this expandable path
                        int x = -1, y = point, tmp;
                        while (x != x0)
                        {
                            x = pt_predecessor[y];
                            matched_pt[y] = x;
                            tmp = y;
                            y = matched_set[x];
                            matched_set[x] = tmp;
                        }
                        return 1;
                    }
                    else // this point is alr matched
                    {
                        // find the set this pt is matched with and add it to reachable set
                        reachable_set[count++] = matched_pt[point];
                    }
                }
            }
            index ++;
        }
        if (!flag) break; // no visit
    }
    return 0;
}

int min(int a, int b)
{
    if (a < b) return a;
    else return b;
}

int main()
{
    int n, k, size[300];
    cin >> n;
    int points[300][300];
    for (int i = 0; i < n; i++)
    {
        cin >> size[i]; // size of set i
        for (int j = 0; j < size[i]; j++)
        {
            cin >> k; // k is between 1 and n
            points[i][j] = k - 1;
        }
    }
    int value[300];
    for (int i = 0; i < n; i++)
    {
        cin >> value[i];
    }
    // match: Hungary algorithm
    // there exists a match that matches each set to a pt
    // matched_set[i]: which point is matched to set i
    int matched_set[300], matched_pt[300]; // each set xi, matched to who; each point yj, matched to who
    for (int i = 0; i < 300; i ++)
    {
        matched_set[i] = -1;
        matched_pt[i] = -1;
    }
    while (1)
    {
        int flag = 0;
        for (int i = 0; i < n; i++)
            if (matched_set[i] == -1)
            {
                //cout<<i<<endl;
                match(i, points, matched_set, matched_pt, size);
                flag = 1;
            }
        if (!flag) break; // all sets are matched
    }
    // MATCH FINISHED!
    // Next step: construct a graph based on the relationship above. sets are nodes in the graph
    int point, correlated_set;
    int set_graph_out[302][302], set_graph_in[302][302], flow_capacity_out[302][302], flow_capacity_in[302][302],
        size_set_succ[302], size_set_pred[302];
    memset(size_set_pred, 0, sizeof(size_set_pred));
    memset(size_set_succ, 0, sizeof(size_set_succ));
    int flag;
    for (int i = 0; i < n; i++)
    {
        flag = 0;
        for (int j = 0; j < size[i]; j++)
        {
            point = points[i][j];
            correlated_set = matched_pt[point];
            if (correlated_set == i) // connection to oneself is prohibited
            {
                flag = 1;
                continue;
            }
            k = size_set_succ[i]; // add a member as the successor of i
            set_graph_out[i][k] = correlated_set;
            flow_capacity_out[i][correlated_set] = INF;
            size_set_succ[i]++;

            k = size_set_pred[correlated_set]; // add a member as the predecessor of correlated_set
            set_graph_in[correlated_set][k] = i;
            flow_capacity_in[correlated_set][i] = INF;
            size_set_pred[correlated_set]++;
        }
    }
    // point n: s; point n+1: t
    int sum_positive = 0;
    for (int i = 0; i < n; i++)
    {
        value[i] = -value[i]; // in order to perform maximum flow
        if (value[i] > 0)
        {
            set_graph_out[n][size_set_succ[n]++] = i;
            flow_capacity_out[n][i] = value[i];
            set_graph_in[i][size_set_pred[i]++] = n;
            flow_capacity_in[i][n] = value[i];
            sum_positive += value[i];
        }
        else
        {
            set_graph_out[i][size_set_succ[i]++] = n + 1;
            flow_capacity_out[i][n + 1] = -value[i];
            set_graph_in[n + 1][size_set_pred[n + 1]++] = i;
            flow_capacity_in[n + 1][i] = -value[i];
        }
    }

    // now perform Edmonds-Karp, with points 0..n
    int flow_used_out[302][302], flow_used_in[302][302];
    memset(flow_used_out, 0, sizeof(flow_used_out));
    memset(flow_used_in, 0, sizeof(flow_used_in));
    int queue[302], pred[302], flow_max[302], visited[302], direction[302];
    int cnt = 0;
    while (1)
    {
        flag = 0;
        queue[0] = n; pred[n] = -1; flow_max[n] = INF;
        int head = 0, tail = 0, curr, nxt;
        memset(visited, 0, sizeof visited); // initialize for bfs
        while (head <= tail)
        {
            curr = queue[head];
            for (int i = 0; i < size_set_succ[curr]; i++)
            {
                nxt = set_graph_out[curr][i];
                if (!visited[nxt] && flow_capacity_out[curr][nxt] > flow_used_out[curr][nxt])
                {
                    visited[nxt] = 1;
                    flag = 1;
                    queue[++tail] = nxt;
                    pred[nxt] = curr;
                    direction[nxt] = 1;
                    flow_max[nxt] = min(flow_max[curr], flow_capacity_out[curr][nxt] - flow_used_out[curr][nxt]); // max flow addition
                    if (nxt == n + 1) // has successfully found an update path
                    {
                        flag = 2;
                        break;
                    }
                }
            }
            if (flag !=2)
            for (int i = 0; i < size_set_pred[curr]; i++)
            {
                nxt = set_graph_in[curr][i];
                if (!visited[nxt] && flow_used_in[curr][nxt] > 0)
                {
                    flag = 1;
                    visited[nxt] = 1;
                    queue[++tail] = nxt;
                    pred[nxt] = curr;
                    direction[nxt] = 0;
                    flow_max[nxt] = min(flow_max[curr], flow_used_in[curr][nxt]);
                }
            }
            head ++;
            if (flag == 2) // has reached destination
            {
                curr = n + 1;
                int flow_addition = flow_max[n + 1], prev;
                while (curr != n)
                {
                    prev = pred[curr];
                    if (direction[curr])
                        {
                            flow_used_out[prev][curr] += flow_addition;
                            flow_used_in[curr][prev] += flow_addition;
                        }
                    else 
                        {
                            flow_used_in[prev][curr] -= flow_addition;
                            flow_used_out[curr][prev] -= flow_addition;
                        }
                    curr = prev;
                }
                break;
            }
        }
        if (flag != 2) break; // cannot reach t, no longer has an expandable path!
    }

    int sum = 0;
    for (int i = 0; i < n; i++)
        sum += flow_used_out[n][i];
    // finally output result, take the negative of maxima flow
    printf("%d\n", -(sum_positive - sum));
    return 0;
}