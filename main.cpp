module;
#define cauto const auto
export module main;
import boilerplate;
using namespace boil;

export int main() {
	enum class button { quit = -1, confirmed, change_path, tests, main_menu };
	using enum button;
	using integral_graph = Graph<intmax_t>;
	for (auto [graph, pressed, temp_button, default_path, temp_string, found, i] = tuple{
		integral_graph{}, main_menu, int{}, current_path(), string{}, false, directory_iterator() };
		pressed != quit;
		  print("3. Return to main menu\n-1. Quit\n"),
		  cin >> temp_button,
		  pressed = static_cast<button>(temp_button)) {
		auto traverse_csvs = [&](cauto& apath, cauto& action) {
			for (i = directory_iterator(apath);
				  i != directory_iterator(); ++i, ++temp_button)
				if (i->path().extension() == ".csv") action();
		};
		auto show_graphs = [&]() {
			graph = integral_graph::from_csv(ifstream{ i->path() });
			auto formatted_out = [](auto&& graph) {
				for (bool delimeter{}; cauto & edge : graph) {
					print("{}->{}={}{}", edge.origin, edge.exit, edge.weight,
							(delimeter ? '\n' : ' '));
					delimeter = !delimeter;
				}
				print("___________________________\n");
			};
			print("Original graph from {}:\n", i->path().filename().string());
			formatted_out(graph);
			print("Its MST:\n");
			formatted_out(graph.mst());
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
2. Run tests
)", "\033[2J\033[H", current_path().string());
			break;
		case change_path: print("Enter valid directory: ");
			cin >> temp_string;
			if (path new_path{ temp_string }; exists(new_path)) {
				if (is_directory(new_path)) current_path(new_path);
				else current_path(new_path.remove_filename());
				print("Success!\n");
			} else
				print("Such a path doesn't exist...\n2. Try again\n");
			break;
		case confirmed:
		{
			found = false;
			temp_button = 0;
			traverse_csvs(current_path(), [&]() {
				print("{}. {}\n", temp_button, i->path().filename().string());
				found = true;
			});
			if (not found) { print("No .csv files found\n"); break; }
			print("Which one? ");
			cin >> temp_button;
			print("Performing some magic...\n");
			for (i = directory_iterator(current_path());
				  temp_button != 0; --temp_button, ++i);
			show_graphs();
		}
		break;
		case tests:
			traverse_csvs(default_path, show_graphs);
			print("These tests prove that boruvka rules!\n");
			break;
		}
	}
	print("Goodbye!\n");
}
