#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "data_structures/Graph.h"
#include <vector>
#include <algorithm>
#include <iostream>

// Template function declarations
template<class T>
bool drivingRelax(Edge<T>* edge) {
    if (edge->getOrig()->getDist() + edge->getDriving() < edge->getDest()->getDist()) {
        edge->getDest()->setDist(edge->getOrig()->getDist() + edge->getDriving());
        edge->getDest()->setPath(edge);
        return true;
    }
    return false;
}

template<class T>
bool walkingRelax(Edge<T>* edge) {
    if (edge->getOrig()->getDist() + edge->getWalking() < edge->getDest()->getDist()) {
        edge->getDest()->setDist(edge->getOrig()->getDist() + edge->getWalking());
        edge->getDest()->setPath(edge);
        return true;
    }
    return false;
}

inline void avoidEdges(Graph<int>* g, const std::vector<std::pair<int, int>>& avoidSegments) {
    for (auto v : g->getVertexSet()) {
        for (auto e : v->getAdj()) {
            e->setAvoid(false);
        }
    }

    for (auto pair : avoidSegments) {
        Vertex<int>* srcVertex1 = g->findVertex(pair.first);
        Vertex<int>* srcVertex2 = g->findVertex(pair.second);

        if (!srcVertex1 || !srcVertex2) { continue; }

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

template <class T>
void Dijkstra(Graph<T>* g, const int& origin, const std::vector<int>& avoidNodes = {}, const std::vector<std::pair<int, int>>& avoidSegments = {}, bool isWalking = false) {
    for (auto v : g->getVertexSet()) {
        v->setDist(INF);
        v->setPath(nullptr);
        v->setVisited(std::find(avoidNodes.begin(), avoidNodes.end(), v->getInfo()) != avoidNodes.end());
    }

    avoidEdges(g, avoidSegments);

    auto s = g->findVertex(origin);
    if (!s) return;
    s->setDist(0);

    MutablePriorityQueue<Vertex<T>> q;
    q.insert(s);

    while (!q.empty()) {
        auto v = q.extractMin();
        if (v->isVisited()) continue;
        v->setVisited(true);

        for (auto e : v->getAdj()) {
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
std::vector<T> getPath(Graph<T>* g, const int& origin, const int& dest) {
    std::vector<T> res;
    auto v = g->findVertex(dest);
    if (!v || v->getDist() == INF) return res;

    while (v) {
        res.push_back(v->getInfo());
        v = (v->getPath()) ? v->getPath()->getOrig() : nullptr;
    }

    std::reverse(res.begin(), res.end());
    if (res.empty() || res[0] != origin) std::cout << "Origin not found!!" << std::endl;
    return res;
}

#endif // DIJKSTRA_H
