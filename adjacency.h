#ifndef ADJACENCY_H
#define ADJACENCY_H

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <deque>
#include <unordered_set>

namespace adj{
    class NodeI {
        friend class Adjacency;
        std::string name;
    protected:
        void setName(std::string &in) {
            name = in;

        }
    public:
        std::string& getName(){
            return name;

        }
        virtual const type_info* typeInfo(){
            return &typeid(*this);

        }
        //static CRUD
    };

    template<class T>
    class Node : public NodeI {
        friend class Adjacency;
    protected:
        T data;
    public:
        T &getData(){
            return data;

        }
        T &operator*(){
            return data;

        }
        T *operator->(){
            return &data;

        }
        virtual const type_info* typeInfo(){
            return &typeid(T);

        }
    };
    template<class T>
    Node<T> *cast(NodeI *nodei){
        if(nodei->typeInfo() == &typeid (T))
            return (Node<T>*)nodei;

        return NULL;

    }
    struct Edge : std::pair<const type_info*, std::string>{
        Edge(){}
        Edge(const type_info*tyn, std::string sin) : std::pair<const type_info*, std::string>(tyn,sin){}
        bool is_open = true;
        int weight = 0;

    };
    class Gateways {
        friend class Adjacency;
        typedef typename std::set<Edge>::iterator Iterator;
        std::set<Edge> edges;
        std::set<Edge> hidden_edges;

    public:
        template <class T>
        struct Looper{
            Iterator iterator;
            const type_info *type;
            Iterator end_iterator;
            bool next(){
                iterator++;
                if(iterator == end_iterator){
                    return false;

                }
                if(iterator->first != type){
                    return false;

                }
                return true;

            }
            const Edge &current(){
                return *iterator;

            }
            bool valid(){
                return type != NULL && iterator->first == type;

            }
        };
        template<class T>
        Looper<T> getEdgesByType() {
            const type_info *t = &typeid(T);;
            Edge lb(t, "");
            auto it = edges.lower_bound(lb);
            if(it != edges.end() && it->first == t){
                return {it, t, edges.end()};

            }
            return {it, NULL, edges.end()};

        }
        template<class T>
        Looper<T> getHiddenEdgesByType() {
            const type_info *t = &typeid(T);;
            Edge lb(t, "");
            auto it = hidden_edges.lower_bound(lb);
            if(it != hidden_edges.end() && it->first == t){
                return {it, t, hidden_edges.end()};

            }
            return {it, NULL, hidden_edges.end()};

        }
        std::set<Edge> &getEdges() {
            return edges;

        }
        std::set<Edge> &getHiddenEdges() {
            return hidden_edges;

        }
        bool addEdge(Edge &edgein){
            auto it =edges.insert(edgein);
            return it.second;

        }
        bool addHiddenEdge(Edge &edgein){
            auto it =hidden_edges.insert(edgein);
            return it.second;

        }
        bool removeEdge(Edge &edgein){
            bool it =edges.erase(edgein);
            return it;

        }
        bool removeHiddenEdge(Edge &edgein){
            bool it = hidden_edges.erase(edgein);
            return it;

        }
        Edge &getFirstEdge(Edge &edgein){
            auto it =edges.lower_bound(edgein);
            if (it != edges.end()) {
                edgein = *it;
                return edgein;

            }
            return edgein;

        }
    };

    class Adjacency
    {
    public:        
        static Adjacency &singleton(){
            static Adjacency adjacency;
            return adjacency;
            
        }
        typedef typename std::map<std::pair<const type_info*, std::string>, NodeI*>::iterator Iterator;

        std::map< std::pair<const type_info*, std::string>, NodeI* > adjacency;
        std::map< NodeI*, Gateways> gateways;
        template<class T>
        struct Looper{
           Iterator iterator;
           const type_info *type;
           Iterator end_iterator;
           bool next(){
               iterator++;
               if(iterator == end_iterator){
                   return false;

               }if(iterator->first.first != type){
                   return false;

               }
               return true;

           }
           bool valid(){
               return type != NULL && iterator->first.first == type;

           }
           Node<T> *current(){
               return cast<T>(iterator->second);

           }
        };
        template<class T>
        struct LooperGraph {
            adj::Adjacency *adj;
            std::deque<Node<T> *> dnodes;
            Edge origin;
            std::unordered_set<const Edge *> origin_edges;
            std::deque<const Edge *> dedges;
            std::unordered_set<const Edge *> visited_edges;
            std::unordered_set<NodeI *> visited;
            size_t notEnd(){
                return dnodes.size();

            }
            size_t notEndEdges(){
                return dedges.size();

            }
            Node<T>* at(size_t index){
                return dnodes[index];

            }
            const Edge* edgeAt(size_t index){
                return dedges[index];

            }
            Node<T> *front(){
                return dnodes.front();

            }
            Node<T> *popFront(){
                Node<T> *out = dnodes.front();
                dnodes.pop_front();
                return out;

            }
            Node<T> *back(){
                return dnodes.back();
            }
            Node<T> *popBack(){
                Node<T> *out = dnodes.back();
                dnodes.pop_back();
                return out;

            }
            const Edge *frontEdge(){
                return dedges.front();

            }
            const Edge *popFrontEdge(){
                const Edge *out = dedges.front();
                dedges.pop_front();
                return out;
            }
            const Edge *backEdge(){
                return dedges.back();

            }
            const Edge *popBackEdge(){
                const Edge *out = dedges.back();
                dedges.pop_back();
                return out;
            }
            void clear(){
                visited.clear();
                dnodes.clear();
                dedges.clear();
                visited_edges.clear();

            }
            void clearVisited(){
                visited.clear();
                visited_edges.clear();

            }
            void clearVisitedEdgesExceptOrigins(int depth = 2){
                if(dedges.size() == depth){
                    for(auto it = visited_edges.begin(); it != visited_edges.end(); ){
                        if(origin_edges.find(*it) == origin_edges.end()){
                            it = visited_edges.erase(it);

                        }else{
                            ++it;

                        }
                    }
                }
            }
            void setOriginEdges(Node<T> *origin){
                adj::Gateways::Looper<T> eloop_graph = adj->looperEdgesByType<T>(origin);
                if(eloop_graph.type != NULL){
                    origin_edges.clear();
                    do {
                        const adj::Edge *edge = &(*eloop_graph.iterator);
                        if(edge){
                            if(origin_edges.insert(edge).second){

                            }
                        }else{
                            std::cout << "error... ";

                        }
                    }while(eloop_graph.next());
                }

            }
            void beginNode(Node<T> *current){
                if(!dnodes.size()){
                    dnodes.push_back(current);
                    visited.insert(current);

                }
            }
            void beginEdge(Node<T> *current){
                if(!dedges.size()){
                    origin.first = &typeid(T);
                    origin.second = current->getName();
                    dedges.push_back(&origin);
                    visited_edges.insert(&origin);

                }
            }
            Node<T> *nextNode(){
                Node<T> *current = dnodes.front();
                dnodes.pop_front();
                return current;

            }
            bool bfsNodes(NodeI *current, bool only_one = false){
                adj::Gateways::Looper<T> eloop_graph = adj->looperEdgesByType<T>(current);
                return bfs(eloop_graph,only_one);

            }
            bool bfsHiddenNodes(NodeI *current, bool only_one = false){
                adj::Gateways::Looper<T> eloop_graph = adj->looperHiddenEdgesByType<T>(current);
                return bfs(eloop_graph,only_one);

            }
            bool bfs(adj::Gateways::Looper<T> &eloop_graph, bool only_one = false){
                bool out = false;
                if(eloop_graph.valid()){
                    do {
                        adj::Node<T> *node = adj->readNode<T>(eloop_graph.current());
                        if(node){
                            if(visited.insert(node).second){
                                dnodes.push_back(node);
                                out=true;
                                if(only_one){
                                    break;

                                }
                            }
                        }else{
                            std::cout << "error... ";

                        }
                    }while(eloop_graph.next());
                }
                return out;

            }
            bool dfsNodes(NodeI *current, bool only_one = false){
                adj::Gateways::Looper<T> eloop_graph = adj->looperEdgesByType<T>(current);
                return dfs(eloop_graph,only_one);

            }
            bool dfsHiddenNodes(NodeI *current, bool only_one = false){
                adj::Gateways::Looper<T> eloop_graph = adj->looperHiddenEdgesByType<T>(current);
                return dfs(eloop_graph,only_one);

            }
            bool dfs(adj::Gateways::Looper<T> &eloop_graph, bool only_one = false){
                bool out = false;
                if(eloop_graph.valid()){
                    do {
                        adj::Node<T> *node = adj->readNode<T>(eloop_graph.current());
                        if(node){
                            if(visited.insert(node).second){
                                dnodes.push_front(node);
                                out=true;
                                if(only_one){
                                    break;

                                }
                            }
                        }else{
                            std::cout << "error... ";

                        }
                    }while(eloop_graph.next());
                }
                return out;

            }
            const Edge *nextEdge(){
                const Edge *current = dedges.front();
                dedges.pop_front();
                return current;

            }
            bool bfsEdges(NodeI *current, bool only_one = false){
                adj::Gateways::Looper<T> eloop_graph = adj->looperEdgesByType<T>(current);
                return bfsEdges(eloop_graph, only_one);

            }
            Node<T> * bfsEdges(const Edge *edge, bool only_one = false){
                Node<T> *nodi= adj->readNode<T>(edge->second);
                if(nodi){
                    bfsEdges(nodi, only_one);

                }
                return nodi;

            }
            bool bfsEdges(adj::Gateways::Looper<T> &eloop_graph, bool only_one = false){
                bool out = false;
                if(eloop_graph.valid()){
                    do {
                            const Edge &edge = eloop_graph.current();
                            if(edge.is_open && visited_edges.insert(&edge).second){
                                dedges.push_back(&edge);
                                out=true;
                                if(only_one){
                                    break;

                                }
                            }
                    }while(eloop_graph.next());
                }
                return out;

            }
            bool dfsEdges(NodeI *current, bool only_one = false){
                adj::Gateways::Looper<T> eloop_graph = adj->looperEdgesByType<T>(current);
                return dfsEdges(eloop_graph, only_one);

            }
            Node<T> * dfsEdges(const Edge *edge, bool only_one = false){
                Node<T> *nodi= adj->readNode<T>(edge->second);
                if(nodi){
                    dfsEdges(nodi, only_one);

                }
                return nodi;

            }
            bool dfsEdges(adj::Gateways::Looper<T> &eloop_graph, bool only_one = false){
                bool out = false;
                if(eloop_graph.valid()){
                    do {
                            const Edge &edge = eloop_graph.current();
                            if(edge.is_open && visited_edges.insert(&edge).second){
                                dedges.push_front(&edge);
                                out=true;
                                if(only_one){
                                    break;

                                }
                            }
                    }while(eloop_graph.next());
                }
                return out;

            }
        };

        Adjacency(){

        }

        template<class T>
        Node<T> *createNode(std::string namein){
            const type_info *t = &typeid(T);
            std::pair<const type_info*, std::string> p(t, namein);
            if(adjacency.find(p) == adjacency.end()){
                Node<T> * in = new Node<T>();
                in->setName(namein);
                adjacency[p] = in;
                return in;

            }
            return NULL;

        }
        template<class T>
        bool deleteGateway(Node<T> * in){
            const type_info *t = &typeid(T);
            Edge p(t, in->getName());
            auto it = gateways.find(in);
            if(it != gateways.end()){
                //recupera los hidden edges y se auto elimina
                for(auto it2 = it->second.getHiddenEdges().begin(); it2 != it->second.getHiddenEdges().end(); it2++){
                    //busca el nodo, buscas su gateway, eliminas de los edges al menda
                   auto it_node = adjacency.find(*it2);
                   if(it_node != adjacency.end()){
                       NodeI *nodei = it_node->second;
                       auto it_gate = gateways.find(nodei);
                       if(it_gate != gateways.end()){
                           bool a =it_gate->second.getEdges().erase(p);

                       }
                   }
                }
                for(auto it2 = it->second.getEdges().begin(); it2 != it->second.getEdges().end(); it2++){
                    //busca el nodo, buscas su gateway, eliminas de los hiddenedges al menda
                    auto it_node = adjacency.find(*it2);
                    if(it_node != adjacency.end()){
                        NodeI *nodei = it_node->second;
                        auto it_gate = gateways.find(nodei);
                        if(it_gate != gateways.end()){
                            bool a = it_gate->second.getHiddenEdges().erase(p);

                        }
                    }
                }
                gateways.erase(it);
                return true;

            }
            return false;

        }
        template<class T>
        bool deleteNode(std::string namein){
            const type_info *t = &typeid(T);
            std::pair<const type_info*, std::string> p(t, namein);
            auto it = adjacency.find(p);
            if(it != adjacency.end()){
                deleteGateway((Node<T> *)it->second);
                delete it->second;
                adjacency.erase(it);
                return true;

            }
            return false;

        }
        template<class T>
        bool deleteNode(Node<T> * in){
            const type_info *t = &typeid(T);
            std::pair<const type_info*, std::string> p(t, in->getName());
            auto it = adjacency.find(p);
            if(it != adjacency.end()){
                if(it->second == in){
                    deleteGateway(in);
                    delete it->second;
                    adjacency.erase(it);
                    return true;

                }
            }
            return false;

        }
        template<class T>
        bool updateNode(Node<T> &nodein, std::string namein){
            return false;

        }
        template<class T>
        Node<T> *readNode(std::string namein){
            const type_info *t = &typeid(T);
            std::pair<const type_info*, std::string> p(t, namein);
            auto it = adjacency.find(p);
            if(it == adjacency.end()){
                return NULL;

            }
            return adj::cast<T>(it->second);

        }
        template<class T>
        Node<T> *readNode(adj::Gateways::Looper<T> &eloop){
            return readNode<T>(eloop.current());

        }
        template<class T>
        Node<T> *readNode(const Edge &p){
            auto it = adjacency.find(p);
            if(it == adjacency.end()){
                return NULL;

            }
            return  adj::cast<T>(it->second);

        }
        std::set<Edge> &readEdges(NodeI* nodeiin) {
               return gateways[nodeiin].getEdges();

        }
        std::set<Edge> &readHiddenEdges(NodeI* nodeiin) {
               return gateways[nodeiin].getHiddenEdges();

        }
        template<class T>
        Gateways::Looper<T> looperEdgesByType(NodeI *nodein) {
            Gateways &in = gateways[nodein];
            return  in.getEdgesByType<T>();

        }
        template<class T>
        Gateways::Looper<T> looperHiddenEdgesByType(NodeI *nodein) {
            Gateways &in = gateways[nodein];
            return  in.getHiddenEdgesByType<T>();

        }
        template<class I, class O>
        bool edgeNodes(Node<I>* nodeiin, Node<O>*nodeoin, bool is_open = true, int weight = 0, bool is_openh = false, int weighth = 0){
            const type_info *i = &typeid(I);
            Edge pi(i, nodeiin->getName());
            pi.is_open = is_openh;
            pi.weight = weighth;
            const type_info *o = &typeid(O);
            Edge po(o, nodeoin->getName());
            po.is_open = is_open;
            po.weight = weight;
            Gateways &in = gateways[nodeiin];
            Gateways &out = gateways[nodeoin];

            bool output = in.addEdge(po);
            output &= out.addHiddenEdge(pi);
            return output;

        }
        template<class I, class O>
        bool unEdgeNodes(Node<I>* nodeiin, Node<O>*nodeoin){
            const type_info *i = &typeid(I);
            Edge pi(i, nodeiin->getName());
            const type_info *o = &typeid(O);
            Edge po(o, nodeoin->getName());

            Gateways &in = gateways[nodeiin];
            Gateways &out = gateways[nodeoin];

            bool output = in.removeEdge(po);
            output &= out.removeHiddenEdge(pi);
            return output;

        }

        template<class I>
        Node<I> *getFirstNodeFromFirstEdge(NodeI* nodeiin){
            const type_info *i = &typeid(I);
            Edge pi(i,"");
            Gateways &in = gateways[nodeiin];

            pi=in.getFirstEdge(pi);
            auto it = adjacency.find(pi);
            if(it == adjacency.end()){
                return NULL;

            }
            return (Node<I> *)it->second;

        }
        template<class T>
        Looper<T> getNodesByType(){
            const type_info *t = &typeid(T);
            std::pair<const type_info*, std::string> lb(t, "");
            auto ilb = adjacency.lower_bound(lb);
            if(ilb != adjacency.end() && ilb->first.first == t){
                //auto iup = adjacency.lower_bound(ub);
                return {ilb, t, adjacency.end()};

            }
            return {ilb, NULL, adjacency.end()};

        }
        template<class T>
        LooperGraph<T> bfsLooperGraphEdges(NodeI *nodein){
            LooperGraph<T> out;
            out.adj = this;
            out.bfsEdges(nodein);
            return out;

        }
        template<class T>
        LooperGraph<T> looperGraphEdges(){
            LooperGraph<T> out;
            out.adj = this;
            return out;

        }
        template<class T>
        LooperGraph<T> dfsLooperGraphEdges(NodeI *nodein){
            LooperGraph<T> out;
            out.adj = this;
            out.dfsEdges(nodein);
            return out;

        }
        template<class T>
        LooperGraph<T> looperGraph(Node<T> *nodein){
            LooperGraph<T> out;
            out.adj = this;
            out.beginNode(nodein);
            return out;

        }
        template<class T>
        LooperGraph<T> bfsGraphNodes(NodeI *nodein){
            LooperGraph<T> out;
            out.adj = this;
            out.bfsNodes(nodein);
            return out;

        }
        template<class T>
        LooperGraph<T> bfsGraphHiddenNodes(NodeI *nodein){
            LooperGraph<T> out;
            out.adj = this;
            out.bfsHiddenNodes(nodein);
            return out;

        }
        template<class T>
        LooperGraph<T> dfsGraphNodes(NodeI *nodein){
            LooperGraph<T> out;
            out.adj = this;
            out.dfsNodes(nodein);
            return out;

        }
        template<class T>
        LooperGraph<T> dfsGraphHiddenNodes(NodeI *nodein){
            LooperGraph<T> out;
            out.adj = this;
            out.dfsHiddenNodes(nodein);
            return out;

        }
    };

}
#endif // ADJACENCY_H
