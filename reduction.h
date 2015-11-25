/*
 * Author: Jason Ernst
 * Date Started: Tuesday December 11th, 2007
 * Reduction Algorithm
 */

hypergraph * reduction(hypergraph * H, hypergraph * G);
void reduction_rules(hypergraph * H, hypergraph * G, edgeSet ** EprimeBar);

edgeSet * generateEprimeBar(hypergraph * Hprime);
edgeSet * generateEstar(hypergraph * H, edgeSet * EprimeBar);

hypergraph * generateGadget(int max);
hypergraph * sortHyperGraph(hypergraph * H);

bool isInterConnectionGraphOf(hypergraph * H, hypergraph * testGraph);

bool rule1(hypergraph * H, hypergraph * G, edgeSet ** EprimeBar);
bool rule2(hypergraph * H, hypergraph * G, edgeSet ** EprimeBar);
bool rule3(hypergraph * H, hypergraph * G, edgeSet ** EprimeBar);
bool rule4(hypergraph * H, hypergraph * G, edgeSet ** EprimeBar);
