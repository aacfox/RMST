module;
#define cauto const auto
export module graph;
import boilerplate;
using namespace boil;

template<arithmetic Weight = bool> class Graph {
public:
	struct Edge {
		size_t origin{};
		size_t exit{};
		Weight weight{ 1 };
		bool operator==(const Edge&) const = default;
		auto operator<=>(const Edge& other) const { return weight <=> other.weight; }
	};
private:
	size_t _fresh_id{ max_v<size_t> };
	vector<Edge> _edge_list;
public:
	Graph() = default;
	Graph(auto&& edges): _edge_list{ forward_like<decltype(_edge_list)>(edges) } {
		_fresh_id = ranges::max(_edge_list, {}, [](cauto& x) {return max(x.origin, x.exit); });
	}
	template<ranges::range Grid, class Pred = ranges::equal_to> requires
		requires (Grid grid, Pred pred) {
		ranges::begin(grid[0]);
		{ pred(grid[0][0], grid[0][0]) } -> convertible_to<bool>;
	}
	static Graph from_grid(Grid&& grid, Pred pred = {}) {
		Graph graph;
		size_t max_id{};
		for (size_t x{}; x != grid.size(); ++x) {
			for (size_t y{}, width{ grid[0].size() }; y != width; ++y) {
				auto id = [&width, &max_id](cauto i, cauto j) {
					max_id = max(i * width + j, max_id);
					return i * width + j; };
				cauto origin{ id(x, y) };
				if (cauto exit{ id(x + 1, y) }; x + 1 != grid.size()) {
					if (pred(grid[x][y], grid[x + 1][y]))
						graph._edge_list.emplace_back(origin, exit);
					if (pred(grid[x + 1][y], grid[x][y]))
						graph._edge_list.emplace_back(exit, origin);
				}
				if (cauto exit{ id(x, y + 1) }; y + 1 != grid[0].size()) {
					if (pred(grid[x][y], grid[x][y + 1]))
						graph._edge_list.emplace_back(origin, exit);
					if (pred(grid[x][y + 1], grid[x][y]))
						graph._edge_list.emplace_back(exit, origin);
				}
			}
		}
		graph._fresh_id = move(max_id);
		return graph;
	}
	template<class T> requires derived_from<remove_cvref_t<T>, istream>
	static Graph from_csv(T&& input) {
		Graph graph;
		size_t max_id{};
		while (input.good()) {
			Weight weight{}; auto origin{ max_id }, exit{ max_id };
			input >> origin;
			input.ignore(max_v<streamsize>, ';');
			input >> exit;
			input.ignore(max_v<streamsize>, ';');
			input >> weight;
			input.ignore(max_v<streamsize>, '\n');
			max_id = max(max_id, origin);
			graph._edge_list.emplace_back(move(origin), move(exit), move(weight));
		}
		graph._fresh_id = move(max_id);
		return graph;
	}
	auto fresh_id() const noexcept { return _fresh_id + 1; }
	auto begin() noexcept { return _edge_list.begin(); }
	auto cbegin() const noexcept { return _edge_list.cbegin(); }
	auto end() noexcept { return _edge_list.end(); }
	auto cend() const noexcept { return _edge_list.cend(); }
	template <size_t Maximum_vertices = 1024> Graph mst() const {
		using Supernode = bitset<N>;
		using edge_set = unordered_set < Edge, decltype([](cauto& edge) noexcept {
			return hash<size_t>{}(edge.origin) ^ hash<size_t>{}(edge.exit); }),
			decltype([](cauto& x, cauto& y) {
			if (x.weight == y.weight and (
				x.origin == y.origin and x.exit == y.exit or
				x.exit == y.origin and y.exit == x.origin))
				return true; else return false; }) > ;
		//TL;DR above: graph is interpreted as undirected
		using edge_multimap = unordered_multimap<Supernode, Edge>;
		if (N < fresh_id()) throw (length_error("Default template argument for number of vertices (which is 1024) proved insufficient."));
		edge_multimap edges; //bitset-based hash map for easier node combining
		vector<Supernode> supernodes(fresh_id()); //vector for tracking remaining supernodes
		vector nodes(fresh_id(), supernodes.end()); //disjoint set of original nodes
		//the latter two can be interpreted as nonoptimized-memory-wise hash_maps 
		//vertice_ID -> respective_type
		for (cauto edge : *this) {
			supernodes[edge.origin].set(edge.origin);
			nodes[edge.origin] = supernodes.begin() + edge.origin;
			edges.emplace(supernodes[edge.origin], move(edge));
		}
		auto iter{ edges.cend() }, sentinel{ iter };
		//this is one of the subroutines of boruvka(); should be used once
		//on the original graph, incase it contains self loops or redundant edges
		auto trim_selfs_redundants = [&](edge_multimap& some_edges) {
			for (unordered_map<Supernode, decltype(iter)> connections;
				  cauto & supernode : supernodes
				  | views::filter([](cauto& x) { return x.any(); }))
				for (tie(iter, sentinel) = some_edges.equal_range(supernode);
					  iter != sentinel;) {
				if (supernode[iter->second.exit]) { //i.e. a self-loop
					some_edges.erase(iter++);
					continue;
				} //else check if the edge's redundant. For that,
				  // firstly, check if the connection wasn't considered before yet or was already.
				if (auto connected{ connections.find(*nodes[iter->second.exit]) };
					 connected == connections.cend())//first case
					connections[supernode] = iter++;
				else if (iter->second.weight < connected->second->second.weight)
					some_edges.erase(exchange(connected->second, iter++));
				else some_edges.erase(iter++);
				connections.clear();
			}
		};
		trim_selfs_redundants(edges);
		//The actual Randomized Expected Linear-time MST algorithm:
		auto rmst = [&](this auto self, edge_multimap& input) {
			edge_set boruvkas;
			auto boruvka = [&]() {
				edge_set buffer;
				//choosing-incident-edges cycle:
				for (auto lightest{ iter }; cauto & node : supernodes
					  | views::filter([](cauto& x) { return x.any(); })) {
					tie(iter, sentinel) = input.equal_range(node);
					lightest = ranges::min_element(iter, sentinel, [](cauto& x, cauto& y) {
						return x.second.weight < y.second.weight; });
					if (lightest != sentinel) buffer.insert(input.extract(lightest).mapped());
				}
				//contracting cycle:
				for (auto absorbing{ supernodes.end() }, absorbed{ absorbing };
					  cauto & edge : buffer) {
					absorbing = nodes[edge.origin];
					absorbed = nodes[edge.exit];
					if (auto [combined, changing] = tuple{ *absorbing | *absorbed,
						 edge_multimap{} }; absorbing != absorbed) {
						auto update_keys = [&](cauto& node) {
							for (tie(iter, sentinel) = input.equal_range(node); iter != sentinel;) {
								auto temp{ input.extract(iter++) };
								temp.key() = combined;
								changing.insert(move(temp));
							}
						};
						update_keys(*absorbing);
						update_keys(*absorbed);
						input.merge(changing);
						for (size_t i{}; i != fresh_id(); ++i)
							if ((*absorbed)[i]) nodes[i] = absorbing;
						*absorbing = move(combined);
						absorbed->reset();
					}
				}
				boruvkas.merge(buffer);
				trim_selfs_redundants(input);
			};
			auto erase_f_heavies = [&](const edge_set& forest) { //probable signature
				//perform some magic modificated linear time mst verification algo 
				//for deleting F_heavy edges from contracted graph,
				//given the forest of its subgraph
			};
			if (input.empty()) return edge_set{};
			boruvka();
			boruvka();
			edge_multimap subgraph;
			subgraph.reserve(input.bucket_count()); //to prevent rehashing
			for (auto yes_no{ randomizer(0, 1) }; cauto edge : input)
				if (yes_no()) subgraph.emplace(move(edge));
			erase_f_heavies(self(subgraph));
			boruvkas.merge(self(input));
			return boruvkas;
		};
		edge_set mst_edges{ rmst(edges) };
		Graph mst; //time to restore invariants:
		mst._fresh_id = _fresh_id;
		for (mst._edge_list.reserve(2 * mst_edges.size());
			  auto & edge: mst_edges) {
			mst._edge_list.emplace_back(edge.exit, edge.origin, edge.weight);
			mst._edge_list.emplace_back(move(edge));
		}
		return mst;
	}
	path mermaid(optional<string_view> name) const {
		print("{}_{}", today(), o_clock());
		return {};
	}
};

