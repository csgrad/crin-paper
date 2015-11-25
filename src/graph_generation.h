#ifndef GRAPH_H
#define GRAPH_H

#include "ll.h"
#include "hypergraph.h"

llnode * getRandomVertices(int n, int max);
hypergraph * generateRandomGraph(int numVertices, int maxHyperEdgeSize, int maxHyperEdges);
hypergraph * generatePentagonGraph(int m);
hypergraph * generatePoleGraph(int m);
hypergraph * generateGeometricGraph(int sides);

#endif