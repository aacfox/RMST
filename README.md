# RMST
C++ Implementation of [a randomized expected linear time MST algorithm by D. Karger et al(1995)](./README.md#randomized-om-mst-algo).
>[!CAUTION]
>Time complexity is currently semilinear because I'm struggling with the last of subroutines of the algorithm.
  
## Introduction 
For I am Constantine, the "C" ~~in C++~~ in my nickname! And this is my half-assed but whole-hearted attempt to understand and implement the mentioned algo. I can hardly explain why of all probable topics I deem this the most exciting, so here we go.  
In the wild there exist other deadly cool algos for the same purpose (which is finding MSTs of graphs) but this one just rubs me the right way and was easier to comprehend and to finally implement. [Others](./README.md#other-cool-mst-algos) are deterministic with time complexities being great always without degenerating in worst cases.
  
## Get started
>[!WARNING]
>My imple uses feateres of `C++23`, that are already supported by `MSVC`. There's no and will never be any endeavor to make up for that with `macros` or whatnot from my side. So if you wanna run my code on older versions of compilers, that is entirely on your own. My sincere apologies...


## References
### Randomized O(m) MST algo
Karger, David R.; Klein, Philip N.; Tarjan, Robert E. (1995). "A randomized linear-time algorithm to find minimum spanning trees".  
Original paper: https://dl.acm.org/doi/pdf/10.1145/201019.201022  
Wikipedia's article: https://en.wikipedia.org/wiki/Expected_linear_time_MST_algorithm

### Other cool MST algos
* S. PETTIE and V. RAMACHANDRAN "An Optimal Minimum Spanning Tree Algorithm" (2002) with provebly optimal but unknown TC
  https://web.eecs.umich.edu/~pettie/papers/jacm-optmsf.pdf
* B. Chazelle "A Minimum Spanning Tree Algorithm with InverseAckermann Type Complexity" (2000)
  https://dl.acm.org/doi/pdf/10.1145/355541.355562
