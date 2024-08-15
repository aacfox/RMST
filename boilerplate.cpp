module;
#define cauto const auto
export module boilerplate;
export import std;
export {
	using namespace std;
	using namespace std::chrono;
	using namespace std::filesystem;
}

export namespace boil {
	inline namespace utilities {
		enum class order: signed char {
			equal = 0,
			equivalent = equal,
			less = -1,
			greater = 1,
			unordered = -128,
		};
		constexpr auto now = &high_resolution_clock::now;
		template <class N> concept arithmetic = floating_point<N> or integral<N>;
		template <arithmetic T> constexpr auto max_v = numeric_limits<T>::max();
		template <arithmetic T> constexpr auto min_v = numeric_limits<T>::min();
		auto today() noexcept {
			return year_month_day{ floor<days>(system_clock::now()) };
		}
		auto o_clock() noexcept {
			return hh_mm_ss{ floor<seconds>(system_clock::now()) - floor<days>(system_clock::now()) };
		}
		auto hash_combine = [](cauto& x, cauto& y) noexcept {
			size_t origin{ hash{}(x.origin) };
			return origin ^ (hash{}(x.exit) + 0x517cc1b727220a95
								  + (origin >> 2) + (origin << 6)); };
		auto subrange(auto pair) { return ranges::subrange{ get<0>(pair),get<1>(pair) }; }
		template<class Path> requires same_as<remove_cvref_t<Path>, path>
		void create_file(Path&& p) { ofstream{ p }; }
	}

	inline namespace functions {
		template <arithmetic T>
		auto randomizer(T min = min_v<T>, T max = max_v<T>) {
			random_device seeder;
			default_random_engine generator{ seeder() };
			if constexpr (is_floating_point<T>())
				return bind(uniform_real_distribution{ min, max }, generator);
			else if constexpr (is_integral<T>())
				return bind(uniform_int_distribution{ min, max }, generator);
		}

		template <class...Args, invocable<Args...> Fx>
		auto benchmark(ostream& output, Fx&& func, Args&&...args) {
			constexpr size_t iters{ 1000 };
			auto const epoch = now();
			for (size_t i{}; i != iters; ++i)
				invoke(forward<Fx>(func), forward<Args>(args)...);
			auto const delta{ now() - epoch };
			println(output, "Benchmark completed with {}ns.",
					  delta.count() / iters);
			return delta;
		}
	}
}