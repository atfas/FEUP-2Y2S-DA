#include "data_structures/Graph.h"

using namespace std;

template <class T>
bool drivingRelax(Edge<T> *edge) { // d[u] + w(u,v) < d[v]
    if (edge->getOrig()->getDist() + edge->getDriving() < edge->getDest()->getDist()) { // we have found a better way to reach v
        edge->getDest()->setDist(edge->getOrig()->getDist() + edge->getDriving()); // d[v] = d[u] + w(u,v)
        edge->getDest()->setPath(edge); // set the predecessor of v to u; in this case the edge from u to v
        return true;
    }
    return false;
}

template <class T>
bool walkingRelax(Edge<T> *edge) {
    if (edge->getOrig()->getDist() + edge->getWalking() < edge->getDest()->getDist()) { // we have found a better way to reach v
        edge->getDest()->setDist(edge->getOrig()->getDist() + edge->getWalking()); // d[v] = d[u] + w(u,v)
        edge->getDest()->setPath(edge); // set the predecessor of v to u; in this case the edge from u to v
        return true;
    }
    return false;
}

inline void avoidEdges(Graph<int>* g, const std::vector<std::pair<int, int>>& avoidSegments) {
    for (const auto& pair:avoidSegments) {
        Vertex<int>* srcVertex = g->findVertex(pair.first);

        if (!srcVertex) {continue;}

        for (auto edge : srcVertex->getAdj()) {
            if (edge->getDest()->getInfo() == pair.second) {
                edge->setAvoid(true);
            }
        }
    }
}

template <class T>
void Dijkstra(Graph<T> * g, const int &origin, const vector<int>& avoidNodes = {}, const std::vector<std::pair<int, int>>& avoidSegments = {}, bool isWalking = false) {
    for(auto v : g->getVertexSet()) {
        v->setDist(INF);
        v->setPath(nullptr);

        if (std::find(avoidNodes.begin(), avoidNodes.end(), v->getInfo()) != avoidNodes.end()) {
            v->setVisited(true);
        } else {
            v->setVisited(false);
        }
    }

    avoidEdges(g, avoidSegments);

    auto s = g->findVertex(origin);
    if (!s) return;
    s->setDist(0);

    MutablePriorityQueue<Vertex<T>> q;
    q.insert(s);

    while(!q.empty()) {
        auto v = q.extractMin();

        if (v->isVisited()) continue;

        v->setVisited(true);

        for(auto e : v->getAdj()) {
            if (e->getAvoid()) continue;

            auto dest = e->getDest();

            if (dest->isVisited()) continue;

            auto oldDist = dest->getDist();

            bool relaxed = isWalking ? walkingRelax(e) : drivingRelax(e);

            if (relaxed) {
                if (oldDist == INF) {
                    q.insert(dest);
                } else {
                    q.decreaseKey(dest);
                }
            }
        }
    }
}

template <class T>
static std::vector<T> getPath(Graph<T> * g, const int &origin, const int &dest) {
    std::vector<T> res;
    auto v = g->findVertex(dest);
    if (v == nullptr || v->getDist() == INF) { // missing or disconnected
        return res;

    }
    res.push_back(v->getInfo());
    while(v->getPath() != nullptr) {
        v = v->getPath()->getOrig();
        res.push_back(v->getInfo());
    }
    reverse(res.begin(), res.end());
    if(res.empty() || res[0] != origin) {
        std::cout << "Origin not found!!" << std::endl;
    }
    return res;
}
