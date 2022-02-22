#include "adjacency.h"

int main(int argc, char *argv[])
{

        adj::Adjacency adj;

        adj::Node<int>* node_int = adj.createNode<int>("int");
        adj::Node<float>* node_float = adj.createNode<float>("int");
        adj::Node<double>* node_double = adj.createNode<double>("double");
        adj::Node<double>* node_double2 = adj.createNode<double>("double1");
        adj::Node<double>* node_double3 = adj.createNode<double>("double2");
        adj::Node<double>* node_doublenull = adj.createNode<double>("double");

        adj::Node<char>* node_char = adj.createNode<char>("int");
        adj::Node<std::string>* node_str = adj.createNode<std::string>("int");
        **node_int = 1;
        **node_double = 5.3;

        std::cout << "Nodes" << std::endl;
        adj::Adjacency::Looper<double> looper = adj.getNodesByType<double>();
        if(looper.valid()){
            do{
                std::cout << looper.current()->getName() << std::endl;

            }while(looper.next());
        }

        adj::Adjacency::Looper<adj::Adjacency> looper2 = adj.getNodesByType<adj::Adjacency>();
        if(looper2.valid()){
            do{
                std::cout << looper2.current()->getName()  << std::endl;

            }while(looper2.next());
        }

        adj::Node<int>* node_int2 = adj.readNode<int>("int");

        adj.deleteNode<int>("int");

        adj.edgeNodes(node_float, node_double, true, 0, false, 0);
        adj.edgeNodes(node_float, node_double2, true, 0, false, 0);
        adj.edgeNodes(node_float, node_double3);
        adj.edgeNodes(node_char, node_str);
        adj.unEdgeNodes(node_float, node_double);
        node_double = adj.getFirstNodeFromFirstEdge<double>(node_float);
        auto edges = adj.readEdges(node_float);
        adj::Gateways::Looper<double> eloop = adj.looperEdgesByType<double>(node_float);
        /*Loop through a type*/
        std::cout << "Edges" << std::endl;
        if(eloop.valid()){
            do {
                std::cout << eloop.current().second << " - ";
                adj::Node<double> *node = adj.readNode<double>(eloop);
                std::cout << node->getName() << std::endl;

            }while(eloop.next());
        }
        /*create a graph*/
        adj::Node<std::string>* node_str1 = adj.createNode<std::string>("int1");
        adj::Node<std::string>* node_str2 = adj.createNode<std::string>("int2");
        adj::Node<std::string>* node_str3 = adj.createNode<std::string>("int3");
        adj::Node<std::string>* node_str4 = adj.createNode<std::string>("int4");
        adj::Node<std::string>* node_str5 = adj.createNode<std::string>("int5");
        adj::Node<std::string>* node_str6 = adj.createNode<std::string>("int6");
        adj::Node<std::string>* node_str7 = adj.createNode<std::string>("int7");
        adj::Node<std::string>* node_str8 = adj.createNode<std::string>("int8");
        adj::Node<std::string>* node_str9 = adj.createNode<std::string>("int9");
        adj::Node<std::string>* node_str10 = adj.createNode<std::string>("int10");

        adj.edgeNodes(node_str1, node_str2);
        adj.edgeNodes(node_str2, node_str3);
        adj.edgeNodes(node_str3, node_str4);
        adj.edgeNodes(node_str4, node_str5);
        adj.edgeNodes(node_str5, node_str6);
        adj.edgeNodes(node_str6, node_str7);
        adj.edgeNodes(node_str7, node_str1);
        adj.edgeNodes(node_str4, node_str8);
        adj.edgeNodes(node_str5, node_str9);
        adj.edgeNodes(node_str9, node_str7);
        adj.edgeNodes(node_str7, node_str10);
        adj.edgeNodes(node_str4, node_str10);
        adj.edgeNodes(node_str5, node_str10);
        adj.edgeNodes(node_str9, node_str10);
        adj.edgeNodes(node_str10, node_str2);
        adj.edgeNodes(node_str10, node_str3);
        adj.edgeNodes(node_str10, node_str4);
        adj.edgeNodes(node_str10, node_str5);
        {
            adj::Adjacency::LooperGraph<std::string> eloop_graph = adj.bfsLooperGraphEdges<std::string>(node_str1);
            /*Loop through a type*/
            std::cout << "Edges Graph" << std::endl;
            while(eloop_graph.notEndEdges()){
                const adj::Edge *edge = eloop_graph.nextEdge();
                std::cout << edge->second << " - " ;
                //adj::Node<std::string> *node1 = adj::cast<std::string>(adj.readNode(*edge));
                adj::Node<std::string> *node1 = adj.readNode<std::string>(*edge);
                bool done = eloop_graph.bfsEdges(node1);

                //this does the same...
                adj::Node<std::string> *node2 = eloop_graph.bfsEdges(edge);
                std::cout << node2->getName() << std::endl;

            }
        }
        {
            adj::Adjacency::LooperGraph<std::string> eloop_graph = adj.looperGraph<std::string>(node_str1);
            std::cout << "Nodes dfs Graph" << std::endl;
            /*Loop through a type*/
            while(eloop_graph.notEnd()){
                adj::Node<std::string> *node = eloop_graph.nextNode();
                std::cout << node->getName()<< std::endl;;
                eloop_graph.dfsNodes(node);

            }
        }

        {
            adj::Adjacency::LooperGraph<std::string> eloop_graph = adj.bfsGraphNodes<std::string>(node_str1);
            std::cout << "Nodes Graph Bfs" << std::endl;
            /*Loop through a type*/
            while(eloop_graph.notEnd()){
                adj::Node<std::string> *node = eloop_graph.nextNode();
                std::cout << node->getName()<< std::endl;;
                eloop_graph.bfsNodes(node);

            }
        }

        {
            adj::Adjacency::LooperGraph<std::string> eloop_graph = adj.bfsGraphHiddenNodes<std::string>(node_str1);
            /*Loop through a type*/
            std::cout << "Nodes Graph bfs Hidden" << std::endl;
            while(eloop_graph.notEnd()){
                adj::Node<std::string> *node = eloop_graph.nextNode();
                std::cout << node->getName()<< std::endl;;
                eloop_graph.bfsHiddenNodes(node);

            }
        }
        {
            adj::Adjacency::LooperGraph<std::string> eloop_graph = adj.dfsGraphHiddenNodes<std::string>(node_str1);
            /*Loop through a type*/
            std::cout << "Nodes Graph dfs Hidden" << std::endl;
            while(eloop_graph.notEnd()){
                adj::Node<std::string> *node = eloop_graph.nextNode();
                std::cout << " --- " << node->getName()<< std::endl;;
                eloop_graph.dfsHiddenNodes(node);

            }
        }

        adj.deleteNode(node_str10);
        adj.deleteNode<std::string>("int1");
        adj.deleteNode(node_str3);
        adj.deleteNode(node_str7);



        /*Combinatoria*/
        adj::Node<std::string>* a = adj.createNode<std::string>("a");
        adj::Node<std::string>* b = adj.createNode<std::string>("b");
        adj::Node<std::string>* c = adj.createNode<std::string>("c");
        adj::Node<std::string>* d = adj.createNode<std::string>("d");
        adj::Node<std::string>* e = adj.createNode<std::string>("e");
        std::vector<adj::Node<std::string>*> letters = {a,b,c,d, e};

        for(int i=0; i<letters.size();i++){
            adj::Node<std::string>* from = letters[i];
            std::cout <<"from "<< from->getName()<< std::endl;;
            for(int o=i+1; o<letters.size();o++){
                if(i==o){
                    continue;
                }
                adj::Node<std::string>* to = letters[o];
                adj.edgeNodes(from,to);
                std::cout << "to " << to->getName()<< std::endl;;

            }
        }
        {
            for(int i=0; i<letters.size();i++){
                adj::Adjacency::LooperGraph<std::string> eloop_graph = adj.looperGraph<std::string>(letters[i]);
                std::cout << "Nodes dfs Graph" << std::endl;
                /*Loop through a type*/
                while(eloop_graph.notEnd()){
                    adj::Node<std::string> *node = eloop_graph.front();
                    bool done =eloop_graph.dfsNodes(node, true);
                    if(!done){
                        for(int i=0; i<eloop_graph.notEnd(); i++){
                            std::cout << eloop_graph.at(i)->getName() << " - ";

                        }
                        eloop_graph.popFront();
                        std::cout <<std::endl;

                    }
                }
            }
        }
        {
            /*Loop through a type*/
            for(int i=0; i<letters.size();i++){
                std::cout << "Edges Graph Combinatoria" << std::endl;
                adj::Adjacency::LooperGraph<std::string> eloop_graph = adj.looperGraphEdges<std::string>();
                eloop_graph.beginEdge(letters[i]); //crea un edge con el nodo pasado
                eloop_graph.setOriginEdges(letters[i]); //inserta los edges del origin
                while(eloop_graph.notEndEdges()){
                    const adj::Edge *edge = eloop_graph.backEdge();
                    adj::Node<std::string> *node1 = adj.readNode<std::string>(*edge);

                    bool done = eloop_graph.bfsEdges(node1, true);
                    if(!done){
                        for(int i=0; i<eloop_graph.notEndEdges(); i++){
                            std::cout << eloop_graph.edgeAt(i)->second << " - ";

                        }
                        eloop_graph.clearVisitedEdgesExceptOrigins(2);

                        eloop_graph.popBackEdge();
                        std::cout <<std::endl;

                    }

                }
            }

        }

    return 0;

}
