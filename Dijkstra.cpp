#include "data_structures/Graph.h"

using namespace std;

// Relax an edge for driving routes, updating destination vertex if a shorter path is found
// @param edge Pointer to the edge to be relaxed
template <class T>
bool drivingRelax(Edge<T> *edge) {
    if (edge->getOrig()->getDist() + edge->getDriving() < edge->getDest()->getDist()) {
        edge->getDest()->setDist(edge->getOrig()->getDist() + edge->getDriving());
        edge->getDest()->setPath(edge);
        return true;
    }
    return false;
}

// Relax an edge for walking routes, updating destination vertex if a shorter path is found
// @param edge Pointer to the edge to be relaxed
template <class T>
bool walkingRelax(Edge<T> *edge) {
    if (edge->getOrig()->getDist() + edge->getWalking() < edge->getDest()->getDist()) {
        edge->getDest()->setDist(edge->getOrig()->getDist() + edge->getWalking());
        edge->getDest()->setPath(edge);
        return true;
    }
    return false;
}

// Mark specific edges as avoided based on given segment pairs
// @param g Pointer to the graph
// @param avoidSegments Vector of vertex pairs to avoid
inline void avoidEdges(Graph<int>* g, const std::vector<std::pair<int, int>>& avoidSegments) {
    for (auto v : g->getVertexSet()) {
        for (auto e : v->getAdj()) {
            e->setAvoid(false);
        }
    }

    for (auto pair:avoidSegments) {

        Vertex<int>* srcVertex1 = g->findVertex(pair.first);
        Vertex<int>* srcVertex2 = g->findVertex(pair.second);

        if (!srcVertex1 || !srcVertex2) {continue;}

        for (auto edge : srcVertex1->getAdj()) {
            if (edge->getDest()->getInfo() == pair.second) {
                edge->setAvoid(true);
            }
        }

        for (auto edge : srcVertex2->getAdj()) {
            if (edge->getDest()->getInfo() == pair.first) {
                edge->setAvoid(true);
            }
        }
    }
}

// Implement Dijkstra's shortest path algorithm with options to avoid nodes and edges
// @param g Pointer to the graph
// @param origin Starting vertex ID
// @param avoidNodes Vector of vertex IDs to avoid
// @param avoidSegments Vector of vertex pairs to avoid
// @param isWalking Flag to determine walking (true) or driving (false) route
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

// Reconstruct the shortest path between origin and destination vertices
// @param g Pointer to the graph
// @param origin Starting vertex ID
// @param dest Destination vertex ID
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