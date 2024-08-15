# RMST
C++ Implementation of [a randomized expected linear time MST algorithm by D. Karger et al(1995)](./README.md#randomized-om-mst-algo).
>[!CAUTION]
>Time complexity is currently semilinear because I'm struggling with the last of the subroutines of the algorithm.
## Introduction 
For I am Constantine, the 'C' ~~in C++~~ in my nickname! And this is my half-assed but whole-hearted attempt to understand and implement the mentioned algo. I can hardly explain why of all the probable topics I deem this the most exciting, so here we go.  
In the wild, there exist other deadly cool algos for the same purpose (which is finding MSTs[^1] of graphs, in case you forgot) but this one just rubs me the right way and was easier to comprehend and finally implement. [Others](./README.md#other-cool-mst-algos) are deterministic with time complexities being great always without degenerating in worst cases.  
[^1]: [Minimum spanning tree](https://en.wikipedia.org/wiki/Minimum_spanning_tree) (or minimum spanning forest - which is a more general notion).

## What it does
Eats graph, poos its MSF[^1] in the same format!  

### Limitations so far  
[The program](./UI.exe) supports only input files with `.csv` extension and formatted as ***edge list*** with ***semicolon*** as a delimiter and ***integral*** weights. See [example](./docs/wikipedia_graph.csv). To fulfill more complicated tasks source code provides more powerful tools not included in the executable ~~'cause of my lazy ass~~ to keep things simple and focus resources of humankind on [much bigger problem]().
<!-- !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! EMPTY LINK ABOVE -->
## Get started
The simplest case:
1. Place [program](./UI.exe) AND your input files in the same folder (if you want them processed at once), or COPY a path to the desired location to the clipboard (to process files one by one)
2. Run program
   * If you chose the first option press consequently `2`, then `Enter`.  
     Proceed to the final step.  
   * Otherwise press `1`, `Enter`, paste the path from the clipboard, `Enter`.  
     If the message "Success!" popped, proceed to the next step.
3. Press `3`, `Enter`, `0`, `Enter`. A list of found files should show up.
4. Enter the index of the desired file.
5. Enjoy the results! For a proper and safe exit from the program enter `-1`.  
   p.s. just kidding, you may as well just throw your device out of the window

***For any more advanced techniques, we'll have to dive into the source code. So first of all, let's clone the repository and build the project.***  
>[!WARNING]
>* My imple uses features of `C++23`, already supported by `MSVC`. There's no and will never be any endeavour to make up for that with `macros` or whatnot from my side. So if you want to run my code on older versions of compilers, that is entirely on your own. My sincere apologies...  
>* All source code files are provided with a `.cpp` extension while containing module interface code, so make sure the compiler interprets them as one.

### The algorithm's source code
The actual implementation of the algorithm is in a [separate file](./graph.cpp), designated as a member function template `mst()` of a class template named `Graph`, parameterized by the maximum theoretically possible `number of vertices` in a given graph for a given application, and by `type` representing weights of edges, respectively. 
 
Any given graph is stored in a data member as a plain `vector<Graph::Edge>` representing an edge list. But `mst()` is oblivious of this fact, so feel free to change the storage until a type-complete instance of `Graph` constitutes the `std::ranges::range` concept, and the derefencing of iterators returns `Graph::Edge`.  

`Graph` also has several `named constructors`. Specifically:
* `Graph::from_grid` — creates a graph from any 2D-range (i.e. supporting two consecutive subscript `operator[]`)
* `Graph::from_csv` — creates a graph from any `std::istream` formatted as explained above

Check [source code](./graph.cpp) for the full signature and documentation.  

### Dependencies
***The imple is self-contained. No third-party library is required.***  
As per below is just a description of the internal relations.
* [boilerplate.cpp](./boilerplate.cpp) is self-explanatory. Contains a bunch of utils, and in the common case should not pique any of your interest, as I tried to name those utils properly. If any confusion arises, check it to understand, what one or the other does. This module imported in every other file.
* [main.cpp](./main.cpp) — driver code; look at how UI.exe is built from the inside. Depends on the other both.
  
## References
### Randomized O(m) MST algo
Karger, David R.; Klein, Philip N.; Tarjan, Robert E. (1995). "A randomized linear-time algorithm to find minimum spanning trees".  
Original paper: https://dl.acm.org/doi/pdf/10.1145/201019.201022  
Wikipedia's article: https://en.wikipedia.org/wiki/Expected_linear_time_MST_algorithm

### Other cool MST algos
* S. PETTIE and V. RAMACHANDRAN "An Optimal Minimum Spanning Tree Algorithm" (2002) with provably optimal but unknown TC
  https://web.eecs.umich.edu/~pettie/papers/jacm-optmsf.pdf
* B. Chazelle "A Minimum Spanning Tree Algorithm with InverseAckermann Type Complexity" (2000)
  https://dl.acm.org/doi/pdf/10.1145/355541.355562
