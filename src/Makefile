all: crin-paper

crin-paper: hypergraph.c graph_generation.c hypergraph.h ll.c ll.h reduction.h reduction.c
	gcc hypergraph.c graph_generation.c reduction.c test_geometric_graph.c -lm -o test_geometric_graph
	gcc hypergraph.c graph_generation.c reduction.c test_pentagon_graph.c -lm -o test_pentagon_graph
	gcc hypergraph.c graph_generation.c reduction.c test_random_graph.c -lm -o test_random_graph
	gcc hypergraph.c graph_generation.c reduction.c test_pole_graph.c -lm -o test_pole_graph
	gcc hypergraph.c graph_generation.c reduction.c test_interconnect_checking.c -lm -o test_interconnect_checking
	
clean:
	rm -rf test_geometric_graph
	rm -rf test_random_graph
	rm -rf test_pole_graph
	rm -rf test_pentagon_graph
	rm -rf test_interconnect_checking