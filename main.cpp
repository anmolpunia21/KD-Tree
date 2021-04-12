#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;
#define XMIN 0
#define XMAX 400
#define YMIN 0
#define YMAX 400
#define x_axis "x"
#define y_axis "y"

struct point
{
    int id, x, y;
    point(int id_, int x_, int y_)
    {
        id = id_;
        x = x_;
        y = y_;
    }
};

class Node
{
public:
    string axis; // Axis = {x/y}
    int value;

    bool isLeft; // Is this Node a left child of Parent Node
    bool isLeaf; // Is this Node a leaf Node
    bool isRoot; // Is this Node

    Node *left;   // Left child pointer
    Node *right;  // Right child pointer
    Node *parent; // Pointer to parent

    vector<point> leafs;
    vector<int> region;

    Node() {}
    ~Node() {}

    Node(vector<point> dataset) // root
    {
        isLeaf = false;
        isRoot = true;
        parent = NULL;

        sort(dataset.begin(), dataset.end(), [](point &a, point &b) { return a.x < b.x; });
        region.push_back(dataset[0].x);
        region.push_back(dataset[dataset.size() - 1].x);
        sort(dataset.begin(), dataset.end(), [](point &a, point &b) { return a.y < b.y; });
        region.push_back(dataset[0].y);
        region.push_back(dataset[dataset.size() - 1].y);
    }
    Node(Node *parent_) // internal
    {
        isRoot = false;
        parent = parent_;
        left = NULL;
        right = NULL;
        isLeaf = false;
    }
    Node(vector<point> dataset, Node *parent_) // leaf
    {
        isRoot = false;
        parent = parent_;
        left = NULL;
        right = NULL;
        isLeaf = true;
        leafs = dataset;
    }
};

class KdTree
{
public:
    Node *root = NULL;
    int alpha;

    KdTree() {}
    KdTree(vector<point> dataset, int alpha_)
    {
        Node *head = new Node(dataset); // creating head Node
        root = head;                    // assign head Node to root Node
        alpha = alpha_;
    }
    void build(Node *root, vector<point> dataset)
    {

        if (dataset.size() <= alpha)
        {

            root->isLeaf = true;
            root->leafs = dataset;
            return;
        }
        // cout << dataset.size() << ' ' << root << '\n';
        string spread = find_spread(dataset);
        int half = dataset.size() / 2 + dataset.size() % 2;
        // cout << dataset.size() << ' ';
        vector<point> l, r;
        if (spread == x_axis)
        {
            root->axis = x_axis;
            sort(dataset.begin(), dataset.end(), [](point &a, point &b) { return a.x < b.x; });

            int median = dataset[half - 1].x;
            vector<point> l, r;
            for (point p : dataset)
            {
                if (p.x <= median)
                {
                    l.push_back(p);
                }
                else
                    r.push_back(p);
            }
            root->value = median;
            // cout<<dataset.size()<<' '<<l.size()<<' '<<r.size()<<'\n';
            if (l.size() == 0 and r.size() != 0)
            {
                Node *rightChild = new Node(dataset, root);
                root->right = rightChild;
                rightChild->isLeft = false;
            }
            else if (r.size() == 0 and l.size() != 0)
            {
                Node *leftChild = new Node(dataset, root);
                root->left = leftChild;
                leftChild->isLeft = true;
            }
            else if (l.size() != 0 and r.size() != 0)
            {
                Node *leftChild = new Node(root);
                root->left = leftChild;
                leftChild->isLeft = true;
                build(leftChild, l);

                Node *rightChild = new Node(root);
                root->right = rightChild;
                rightChild->isLeft = false;
                build(rightChild, r);
            }
        }
        else
        {
            root->axis = y_axis;
            sort(dataset.begin(), dataset.end(), [](point &a, point &b) { return a.y < b.y; });

            int median = dataset[half - 1].y;
            vector<point> l, r;
            for (point p : dataset)
            {
                if (p.y <= median)
                {
                    l.push_back(p);
                }
                else
                    r.push_back(p);
            }
            root->value = median;
            // cout << dataset.size() << ' ' << l.size() << ' ' << r.size() << '\n';
            if (l.size() == 0 and r.size() != 0)
            {
                Node *rightChild = new Node(dataset, root);
                root->right = rightChild;
                rightChild->isLeft = false;
            }
            else if (r.size() == 0 and l.size() != 0)
            {
                Node *leftChild = new Node(dataset, root);
                root->left = leftChild;
                leftChild->isLeft = true;
            }
            else if (l.size() != 0 and r.size() != 0)
            {
                Node *leftChild = new Node(root);
                root->left = leftChild;
                leftChild->isLeft = true;
                build(leftChild, l);

                Node *rightChild = new Node(root);
                root->right = rightChild;
                rightChild->isLeft = false;
                build(rightChild, r);
            }
        }
    }
    string find_spread(vector<point> dataset)
    {
        int x_spread, y_spread;
        sort(dataset.begin(), dataset.end(), [](point &a, point &b) { return a.x < b.x; });
        x_spread = dataset[dataset.size() - 1].x - dataset[0].x;
        sort(dataset.begin(), dataset.end(), [](point &a, point &b) { return a.y < b.y; });
        y_spread = dataset[dataset.size() - 1].y - dataset[0].y;
        return x_spread >= y_spread ? x_axis : y_axis;
    }
    void printLevelOrder(Node *root)
    {
        queue<Node *> q;

        q.push(root);
        while (q.empty() == false)
        {
            int n = q.size();
            while (n--)
            {
                Node *node = q.front();
                // cout << node << "\n";
                q.pop();

                if (node->left != NULL)
                    q.push(node->left);

                if (node->right != NULL)
                    q.push(node->right);

                if (node->isLeaf)
                {
                    // cout << "hello" << '\n';
                    cout << '[';
                    for (point leaf : node->leafs)
                    {
                        // cout << node << " leaf " << leaf.id << ' ' << leaf.x << ' ' << leaf.y << '\n';
                        cout << " (" << leaf.x << ", " << leaf.y << ")";
                    }
                    cout << ']';
                }
                else
                {
                    // cout << node << " internal " << node->axis << '=' << node->value << '\n';
                    cout << node->axis << '=' << node->value << ' ';
                }
            }
            cout << '\n';
        }
    }
};
struct CompareDistance
{
    bool operator()(pair<int, point> const &p1, pair<int, point> const &p2)
    {
        return p1.first < p2.first;
    }
};
void search(Node *root, priority_queue<pair<int, point>, vector<pair<int, point>>, CompareDistance> &pq, unordered_set<Node *> &isVisited, int x, int y, int k)
{
    if (root == NULL)
        return;
    // if (root->isLeaf)
    // {
    //     for (point leaf : root->leafs)
    //     {
    //         cout << root << " recurion " << leaf.id << ' ' << leaf.x << ' ' << leaf.y << '\n';
    //     }
    // }
    if (pq.empty())
    {
        if (root->isLeaf)
        {
            int i = 0;
            for (; i < root->leafs.size() && pq.size() != k; i++)
            {
                int d1 = (int)sqrt(pow((root->leafs[i].x - x), 2) + pow((root->leafs[i].y - y), 2));
                pq.push(make_pair(d1, root->leafs[i]));
            }
            if (i != root->leafs.size())
            {
                for (; i < root->leafs.size(); i++)
                {
                    int d1 = (int)sqrt(pow((root->leafs[i].x - x), 2) + pow((root->leafs[i].y - y), 2));
                    int d2 = (int)sqrt(pow((pq.top().second.x - x), 2) + pow((pq.top().second.y - y), 2));
                    if (d1 < d2)
                    {
                        pq.pop();
                        pq.push(make_pair(d1, root->leafs[i]));
                    }
                }
            }
            isVisited.insert(root);
            return;
        }
        else
        {
            if (root->axis == x_axis)
            {
                if (x <= root->value)
                    search(root->left, pq, isVisited, x, y, k);
                else
                    search(root->right, pq, isVisited, x, y, k);
            }
            else
            {
                if (y <= root->value)
                    search(root->left, pq, isVisited, x, y, k);
                else
                    search(root->right, pq, isVisited, x, y, k);
            }
        }
    }

    if (isVisited.find(root) != isVisited.end())
        return;

    if (!root->isLeaf)
    {

        if (pq.size() == k)
        {
            vector<pair<Node *, bool>> v;
            Node *curr = root;
            while (curr->parent)
            {
                v.push_back({curr->parent, curr->isLeft});
                curr = curr->parent;
            }
            int x1, y1, x2, y2;
            x1 = curr->region[0];
            y1 = curr->region[2];
            x2 = curr->region[1];
            y2 = curr->region[3];
            for (int i = v.size() - 1; i >= 0; i--)
            {
                if (v[i].first->axis == x_axis)
                {
                    if (v[i].second)
                    {
                        x2 = v[i].first->value;
                    }
                    else
                    {
                        x1 = v[i].first->value;
                    }
                }
                else
                {
                    if (v[i].second)
                    {
                        y2 = v[i].first->value;
                    }
                    else
                    {
                        y1 = v[i].first->value;
                    }
                }
            }

            // double rect_x = (x1 + x2) / 2;
            // double rect_y = (y1 + y2) / 2;
            // double rect_width = x2 - x1;
            // double rect_height = y2 - y1;

            // double dx = max(abs(x - rect_x) - rect_width / 2, 0.0);
            // double dy = max(abs(y - rect_y) - rect_height / 2, 0.0);
            double dx = max(x1 - x, max(0, x - x2));
            double dy = max(y1 - y, max(0, y - y2));

            int d1 = (int)(sqrt(dx * dx + dy * dy));
            int d2 = (int)sqrt(pow((pq.top().second.x - x), 2) + pow((pq.top().second.y - y), 2));
            if (d1 >= d2)
            {
                return;
            }
        }
    }

    if (root->isLeaf)
    {
        int i = 0;
        for (; i < root->leafs.size() && pq.size() != k; i++)
        {
            int d1 = (int)sqrt(pow((root->leafs[i].x - x), 2) + pow((root->leafs[i].y - y), 2));
            pq.push({d1, root->leafs[i]});
        }
        if (i != root->leafs.size())
        {
            for (; i < root->leafs.size(); i++)
            {
                int d1 = (int)sqrt(pow((root->leafs[i].x - x), 2) + pow((root->leafs[i].y - y), 2));
                int d2 = (int)sqrt(pow((pq.top().second.x - x), 2) + pow((pq.top().second.y - y), 2));
                if (d1 < d2)
                {
                    pq.pop();
                    pq.push({d1, root->leafs[i]});
                }
            }
        }
        isVisited.insert(root);
        return;
    }

    if (root->left != NULL && isVisited.find(root->left) == isVisited.end())
    {
        search(root->left, pq, isVisited, x, y, k);
    }
    if (root->right != NULL && isVisited.find(root->right) == isVisited.end())
    {
        search(root->right, pq, isVisited, x, y, k);
    }
    isVisited.insert(root);
    return;
}

inline int randomNumber(int n)
{
    return (rand() % (n + 1));
}

vector<point> generateDataSet(int n)
{
    vector<point> v;

    for (int i = 0; i < n; i++)
    {
        point p(i, randomNumber(XMAX), randomNumber(YMAX));
        v.push_back(p);
    }
    return v;
}

void naive(priority_queue<pair<int, point>, vector<pair<int, point>>, CompareDistance> &pq, int x, int y, int k, vector<point> dataset)
{
    for (point p : dataset)
    {
        int d1 = (int)sqrt(pow((p.x - x), 2) + pow((p.y - y), 2));

        if (pq.size() < k)
            pq.push(make_pair(d1, p));
        else
        {
            int d2 = (int)sqrt(pow((pq.top().second.x - x), 2) + pow((pq.top().second.y - y), 2));
            if (d1 < d2)
            {
                pq.pop();
                pq.push(make_pair(d1, p));
            }
        }
    }
}
int main()
{
    vector<point> dataset;
    priority_queue<pair<int, point>, vector<pair<int, point>>, CompareDistance> pq;
    priority_queue<pair<int, point>, vector<pair<int, point>>, CompareDistance> npq;
    unordered_set<Node *> isVisited;
    dataset = generateDataSet(30000);
    int alpha, k, x, y, choice;

    while (true)
    {
        int temp;
        cout << "Press 1 for Entering value of Alpha & K\nPress 2 for Exit\n";
        cin >> temp;
        if (temp == 2)
        {
            break;
        }
        cout << "Enter Alpha & K: ";
        cin >> alpha >> k;
        KdTree obj(dataset, alpha);
        obj.build(obj.root, dataset);
        while (true)
        {

            cout << "Press 1 for Visualize\nPress 2 for Intiate k-nn search for query.csv\nPress 3 for Exit\n";
            cin >> choice;
            if (choice == 3)
                break;
            else if (choice == 1)
            {
                obj.printLevelOrder(obj.root);
            }
            else if (choice == 2)
            {
                fstream f;
                f.open("query.csv", ios::in);
                if (f.is_open())
                {
                    string tp;
                    int i = 0;
                    int avg_kd = 0;
                    int avg_naive = 0;
                    while (getline(f, tp))
                    {
                        i++;
                        vector<string> tokens;
                        stringstream check1(tp);
                        string intermediate;
                        while (getline(check1, intermediate, ' '))
                        {
                            tokens.push_back(intermediate);
                        }
                        x = stoi(tokens[0]);
                        y = stoi(tokens[1]);
                        if (i == 1)
                            continue;
                        // cout<<x<<' '<<y<<'\n';
                        auto start = high_resolution_clock::now();
                        search(obj.root, pq, isVisited, x, y, k);
                        auto stop = high_resolution_clock::now();
                        auto duration = duration_cast<microseconds>(stop - start);
                        cout << "KDTree: " << duration.count() << " ms; ";
                        avg_kd += duration.count();
                        // while (!pq.empty())
                        // {
                        //     pair<int, point> t = pq.top();
                        //     pq.pop();
                        //     cout << t.first << ' ' << t.second.x << ' ' << t.second.y << '\n';
                        // }
                        start = high_resolution_clock::now();
                        naive(npq, x, y, k, dataset);
                        stop = high_resolution_clock::now();
                        duration = duration_cast<microseconds>(stop - start);
                        cout << "Naive: " << duration.count() << " ms\n";
                        avg_naive += duration.count();
                        // while (!npq.empty())
                        // {
                        //     pair<int, point> t = npq.top();
                        //     npq.pop();
                        //     cout << t.first << ' ' << t.second.x << ' ' << t.second.y << '\n';
                        // }
                    }
                    cout << "KDTree(Avg): " << avg_kd / i << " ms; Naive(Avg): " << avg_naive / i << " ms\n";
                    f.close();
                }
            }
        }
    }

    // while (!pq.empty())
    // {
    //     pair<int, point> t = pq.top();
    //     pq.pop();
    //     cout << t.first << ' ' << t.second.x << ' ' << t.second.y << '\n';
    // }
    // naive(npq, 36, 36, 10, dataset);
    // cout << '\n';
    // while (!npq.empty())
    // {
    //     pair<int, point> t = npq.top();
    //     npq.pop();
    //     cout << t.first << ' ' << t.second.x << ' ' << t.second.y << '\n';
    // }

    return 0;
}
