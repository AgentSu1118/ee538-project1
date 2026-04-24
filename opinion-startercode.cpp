#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <string>
using namespace std;

/********************DO NOT EDIT**********************/
// Function prototype. Defined later.
void read_opinions(string filename); // reads file into opinions vector and updates total_nodes as needed
void read_edges(string filename); // reads file into edge_list, defined later
void build_adj(); // convert edge_list to adjacency matrix

int total_nodes = 0; // We keep track of the total number of nodes based on largest node id.

// #define MATRIX
/****************************************************************/

/******** Create adjacency matrix and vector of opinions */
// simple vector to hold each node's opinion (0 or 1)
std::vector<int> opinions;

#ifdef MATRIX
    // global adjacency matrix initialized later
    std::vector<std::vector<int>> adj;
#else
    // global adjacency list initialized later
    std::vector<std::vector<int>> adj_list;
#endif
// edge list: each row contains {source, target}
std::vector<std::vector<int>> edge_list;

// AI-GENERATED START: Complexity instrumentation helpers
struct ComplexityStats {
    long long build_edge_visits = 0;
    long long majority_neighbor_checks = 0;
    long long update_calls = 0;
};

ComplexityStats complexity_stats;

size_t estimate_space_bytes()
{
    size_t bytes = 0;

    bytes += opinions.capacity() * sizeof(int);
    bytes += edge_list.capacity() * sizeof(std::vector<int>);
    for (const auto& row : edge_list) {
        bytes += row.capacity() * sizeof(int);
    }

    #ifdef MATRIX
        bytes += adj.capacity() * sizeof(std::vector<int>);
        for (const auto& row : adj) {
            bytes += row.capacity() * sizeof(int);
        }
    #else
        bytes += adj_list.capacity() * sizeof(std::vector<int>);
        for (const auto& neighbors : adj_list) {
            bytes += neighbors.capacity() * sizeof(int);
        }
    #endif

    return bytes;
}

void print_complexity_report(double total_runtime_ms, double simulation_runtime_ms, int iterations)
{
    const size_t estimated_space = estimate_space_bytes();
    cout << "\n=== Complexity Report ===" << endl;
    #ifdef MATRIX
        cout << "Mode: MATRIX" << endl;
    #else
        cout << "Mode: LIST" << endl;
    #endif
    cout << fixed << setprecision(3);
    cout << "Measured total runtime (ms): " << total_runtime_ms << endl;
    cout << "Measured simulation runtime (ms): " << simulation_runtime_ms << endl;
    cout << "Estimated memory footprint (bytes): " << estimated_space << endl;
    cout << "Measured counters: edge visits in build = " << complexity_stats.build_edge_visits
         << ", neighbor checks = " << complexity_stats.majority_neighbor_checks
         << ", update calls = " << complexity_stats.update_calls << endl;

    #ifdef MATRIX
        cout << "Theoretical time complexity: build O(N^2 + E), per-iteration update O(N^2), total O(T*N^2)" << endl;
        cout << "Theoretical space complexity: O(N^2 + E + N)" << endl;
    #else
        cout << "Theoretical time complexity: build O(N + E), per-iteration update O(N + E), total O(T*(N + E))" << endl;
        cout << "Theoretical space complexity: O(N + E)" << endl;
    #endif

    cout << "Iterations executed: " << iterations << endl;
    cout << "=========================" << endl;
}
// AI-GENERATED END: Complexity instrumentation helpers

void build_adj()
{
    #ifdef MATRIX
        adj.resize(total_nodes, vector<int>(total_nodes, 0));
    #else
        adj_list.resize(total_nodes);
    #endif
    for(const auto& edge: edge_list){
        int source = edge[0];
        int target = edge[1];
        complexity_stats.build_edge_visits++;
        #ifdef MATRIX
            adj[source][target] = 1; // directed edge from source to target
        #else
            adj_list[target].push_back(source); // directed edge from source to target
        #endif
    }
}

double calculate_fraction_of_ones()
{
    double count_ones = 0;
    for(int opinion: opinions){
        if(opinion == 1) count_ones++;
    }
    return count_ones / total_nodes;
}

// For a given node, count majority opinion among its neighbours. Tie -> 0.
int get_majority_friend_opinions(int node)
{
    int count_source = 0;
    int count_ones = 0;
    #ifdef MATRIX
        for(int source = 0; source < total_nodes; source++){
            complexity_stats.majority_neighbor_checks++;
            if(adj[source][node] == 1){
                count_source++;
                if(opinions[source] == 1) count_ones++;
            }
        }
    #else
        for(int source: adj_list[node]){
            complexity_stats.majority_neighbor_checks++;
            count_source++;
            if(opinions[source] == 1) count_ones++;
        }
    #endif
    if(count_source == 0) return opinions[node];
    else if (count_ones > count_source / 2.0) return 1;
    else return 0;    
}

// Calculate new opinions for all voters and return if anyone's opinion changed
bool update_opinions()
{
    complexity_stats.update_calls++;
    bool changed = false;
    vector<int> new_opinions = opinions;
    for(int node = 0; node < total_nodes; node++){
        new_opinions[node] = get_majority_friend_opinions(node);
        if(new_opinions[node] != opinions[node]) changed = true;
    }
    opinions = new_opinions;
    return changed;
}

int main() {
    // no preallocation; vectors grow on demand

    // AI-GENERATED START: Runtime timers
    auto total_start = chrono::steady_clock::now();
    // AI-GENERATED END: Runtime timers

    
    #ifdef USE_LARGE_DATA
        read_opinions("opinions_large.txt");
        read_edges("edge_list_large.txt");
    #else
        read_opinions("opinions.txt");
        read_edges("edge_list.txt");
    #endif

    // convert edge list into adjacency matrix once we know total_nodes
    build_adj();
    
    cout << "Total nodes: " << total_nodes << endl;
    
    // Run simulation
    int max_iterations = 30;
    int iteration = 0;
    bool opinions_changed = true;
    bool consensus_reached = false;
    
    // Print initial state
    cout << "Iteration " << iteration << ": fraction of 1's = " 
         << calculate_fraction_of_ones() << endl;

    // AI-GENERATED START: Runtime timers
    auto simulation_start = chrono::steady_clock::now();
    // AI-GENERATED END: Runtime timers
    
    /// (6)  //////////////////////////////////////////////
    while(opinions_changed && iteration < max_iterations && !consensus_reached){
        iteration++;
        opinions_changed = update_opinions();
        consensus_reached = (calculate_fraction_of_ones() == 1.0 || calculate_fraction_of_ones() == 0.0);
        if(opinions_changed && iteration < max_iterations && !consensus_reached)
            cout << "Iteration " << iteration << ": fraction of 1's = " 
                << calculate_fraction_of_ones() << endl;
    }

    // AI-GENERATED START: Runtime timers
    auto simulation_end = chrono::steady_clock::now();
    // AI-GENERATED END: Runtime timers

    ////////////////////////////////////////////////////////
    // Print final result
    double final_fraction = calculate_fraction_of_ones();
    cout << "Iteration " << iteration << ": fraction of 1's = " 
         << final_fraction << endl;
    
    if(final_fraction == 1.0)
        cout << "Consensus reached: all 1's" << endl;
    else if(final_fraction == 0.0)
        cout << "Consensus reached: all 0's" << endl;
    else
        cout << "No consensus reached after " << iteration << " iterations" << endl;

    // AI-GENERATED START: Complexity reporting
    auto total_end = chrono::steady_clock::now();
    const double simulation_runtime_ms = chrono::duration<double, milli>(simulation_end - simulation_start).count();
    const double total_runtime_ms = chrono::duration<double, milli>(total_end - total_start).count();
    print_complexity_report(total_runtime_ms, simulation_runtime_ms, iteration);
    // AI-GENERATED END: Complexity reporting
    
    return 0;
}


/*********** Functions to read files **************************/ 

// Read opinion vector from file.
void read_opinions(string filename)
{
    ifstream file(filename);
    int id, opinion;
    while(file >> id >> opinion)
    {
        opinions.push_back(opinion);
        if(id >= total_nodes) total_nodes = id+1;
    }
    file.close();
}

// Read edge list from file and update total nodes as needed.
void read_edges(string filename)
{
    ifstream file(filename);
    int source, target;
    
    while(file >> source >> target)
    {
        edge_list.push_back({source, target});
        if(source >= total_nodes) total_nodes = source+1;
        if(target >= total_nodes) total_nodes = target+1;
    }
    file.close();
}

/********************************************************************** */