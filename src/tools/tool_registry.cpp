//Mudassir Mohammed
//10/10/2026
// src/tools/tool_registry.cpp — YOU IMPLEMENT (spec §4.3).
//
// A hand-written hash table. No std::unordered_map / std::map / std::set.
// Every function below is a stub so the starter compiles. Replace each body.

#include "tools/tool_registry.h"
#include <vector>
#include <utility>


ToolRegistry::ToolRegistry(): buckets(16) {};    // create your initial buckets and sets size to 16
ToolRegistry::~ToolRegistry() = default;   // unique_ptrs in your buckets free the tools
//returns the 64-bit FNV-1a hash value
std::size_t ToolRegistry::hash(std::string_view key) noexcept {
    std::uint64_t h = 1469598103934665603ULL;                         // "offset basis"
    for (unsigned char c : key) { h ^= c; h *= 1099511628211ULL; }     // "FNV prime"
    return static_cast<std::size_t>(h);
}

// Stores the tool and returns true. Returns false if the tool is null or a tool
// with the same name is already registered.
bool ToolRegistry::add(std::unique_ptr<Tool> tool) {
    // A null tool cannot be stored.
    if (tool == nullptr){
        return false;
    }

    //If name already exists in the hash table, then returns false.
    if (find(tool->name())){
        return false;
    }

    
    //if adding one more tool increases the load factor to more than 0.75
    //grow the hash table and resize by 2.
    if ((((toolCount + 1)*1.0)/bucket_count()) > 0.75){
         //create a twice as big hash table
         std::size_t new_bucket_count = bucket_count()*2;
         std::vector<std::unique_ptr<Tool>> bucket2(new_bucket_count);
        // Move every tool from the old table into the new one.
        for (std::size_t i = 0; i < buckets.size(); i++){
            //ignore the empty indexes in the previous hash.
            if (buckets[i] == nullptr){
                continue;
            }
            // recomputed the new slot with the NEW size
            std::size_t index = hash(buckets[i]->name()) % bucket2.size();

            //if there exists a tool at the index
            while(bucket2[index]){
                //probe until you find an empty index
                index = (index + 1) % bucket2.size();
            }
            // std::move moves only the pointer. The Tool object itself stays where it was allocated
            bucket2[index] = std::move(buckets[i]);
        }

        // The new table becomes the hash table we use
        buckets = std::move(bucket2);

    }
    // Find a slot for the new tool after growing,
    std::size_t index2 = hash(tool->name()) % buckets.size();

    //if there exists a tool at the index
    while (buckets[index2]) {
        //probe until you find an empty index
        index2 = (index2 + 1) % buckets.size();
    }

    // Take ownership. Nothing may use "tool" after the move.
    buckets[index2] = std::move(tool);
    toolCount++;
    return true;
}


Tool* ToolRegistry::find(std::string_view name) const {

    std::size_t index =  hash(name) % buckets.size();
    for (std::size_t i = 0; i < buckets.size(); i++){
        if (buckets[index] == nullptr){
            return nullptr;
        } else

        if ((buckets[index] -> name()) == name){
            return buckets[index].get();
        }
        index = (index + 1) % buckets.size();
    }
       
    return nullptr;   
}

std::size_t ToolRegistry::size() const noexcept {
    return toolCount;   //returns the # of total tools in the table
}

void ToolRegistry::for_each(const std::function<void(const Tool&)>& fn) const {
    for (std::size_t i = 0; i < buckets.size(); i++){
        if (buckets[i] == nullptr){
            continue;
        }
        Tool& registeredTool = *buckets[i];
        fn(registeredTool);
    }
}

std::size_t ToolRegistry::bucket_count() const noexcept {
    return buckets.size(); //returns how many buckets in teh table
}

double ToolRegistry::load_factor() const noexcept {
    return (size()*1.0)/bucket_count();
}

std::size_t ToolRegistry::max_probe_length() const {
    std::size_t probeWorstCase = 0;
    std::size_t probeDistance = 0;
    for (std::size_t i = 0; i < buckets.size(); i++){
        if (buckets[i] == nullptr){
            continue;
        }
            std::size_t homeSlot = hash(buckets[i]->name()) % buckets.size();
            if (i >= homeSlot){
                probeDistance  = 1 + i - homeSlot;
            } else {
                probeDistance =  1 + i + (buckets.size() - homeSlot);
            }

            if (probeDistance > probeWorstCase){
                probeWorstCase = probeDistance;
            }

    }
    return probeWorstCase;
    
}
