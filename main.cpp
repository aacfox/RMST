module;
#define cauto const auto
export module main;
import boilerplate;
import graph;
using namespace boil;

export int main() {
	enum class button { quit = -1, confirmed, change_path, all, main_menu };
	using enum button;
	using integral_graph = Graph<intmax_t>;
	using dir_iter = recursive_directory_iterator;
	for (auto [graph, pressed, temp_button, temp_string, found, i] = tuple{
		integral_graph{}, main_menu, int{}, string{}, false, dir_iter() };
		pressed != quit;
		  print("3. Return to main menu\n-1. Quit\n"),
		  cin >> temp_button,
		  pressed = static_cast<button>(temp_button)) {
		auto traverse_csvs = [&](cauto& action) {
			for (i = dir_iter(current_path());
				  i != dir_iter(); ++i, ++temp_button)
				if (i->path().extension() == ".csv") action();
		};
		auto show_graphs = [&]() {
			graph = integral_graph::from_csv(ifstream{ i->path() });
			auto formatted_out = [](auto&& graph) {
				for (cauto& edge : graph)
					print("{};{};{}\n", edge.origin, edge.exit, edge.weight);
				print("___________________________\n");
			};
			print("Original graph from {}:\n", i->path().filename().string());
			formatted_out(graph);
			print("Its MST:\n");
			formatted_out(graph.mst());
			print("Mermaid code is created as well.\n_______________________\n");
			graph.mermaid(ofstream{ path{ i->path() }
			.replace_extension(".mm").filename() });
		};
		switch (pressed) {
		case main_menu: [[fallthrough]];
		default: print(R"(
{}!!!__Welcome to Randomized MST-algo__!!!

*Please, consider that only .csv files (semicolon separeted) with edge lists 
(by the current code base) and graphs with integral edge weights 
(by this exact UI) are supported!
Check the source code and feel free to add new functionality!

Current path: {}
0. Show its contents
1. Change current path
2. Run all
)", "\033[2J\033[H", current_path().string());
			break;
		case change_path: print("Enter valid directory: ");
			cin >> temp_string;
			if (path new_path{ temp_string }; exists(new_path)) {
				if (is_directory(new_path)) current_path(new_path);
				else current_path(new_path.remove_filename());
				print("Success!\n");
			} else
				print("Such a path doesn't exist...\n1. Try again\n");
			break;
		case confirmed:
		{
			found = false;
			temp_button = 0;
			traverse_csvs([&]() { print("{}. {}\n", temp_button, i->path()
												 .filename().string()); found = true;
			});
			if (not found) { print("No .csv files found\n"); break; }
			print("Which one? ");
			cin >> temp_button;
			print("Performing some magic...\n");
			for (i = dir_iter(current_path());
				  temp_button != 0; --temp_button, ++i);
			show_graphs();
		}
		break;
		case all:
			traverse_csvs(show_graphs);
			break;
		}
	}
	print("Goodbye!\n");
}
