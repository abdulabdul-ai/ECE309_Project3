// src/tools/tool_registry.cpp — YOU IMPLEMENT (spec §4.3).
//
// A hand-written hash table. No std::unordered_map / std::map / std::set.
// Every function below is a stub so the starter compiles. Replace each body.

#include "tools/tool_registry.h"
#include <vector>


ToolRegistry::ToolRegistry(): buckets(16) {};    // TODO: create your initial buckets
ToolRegistry::~ToolRegistry() = default;   // unique_ptrs in your buckets free the tools

std::size_t ToolRegistry::hash(std::string_view key) noexcept {
    std::uint64_t h = 1469598103934665603ULL;                         // "offset basis"
    for (unsigned char c : key) { h ^= c; h *= 1099511628211ULL; }     // "FNV prime"
    return static_cast<std::size_t>(h);
    // if ((bucket_count() & (bucket_count() - 1)) == 0){
    //     index = hash() & (bucket_count() - 1);
    // }
    // else {index = key % bucket_count();}
    // return index;
    // return 0;   // TODO: FNV-1a (spec Appendix A) or another real string hash
}

bool ToolRegistry::add(std::unique_ptr<Tool> tool) {
    //std::size_t index = hash(tool);
    //std::size_t bucketsProbed = 0;
    //while (bucketsProbed++ < )

    if (tool == nullptr){
        return false;
    }

    if (( toolCount + 1)/tool.bucket_count() > tool.load_factor()){
        new_bucket_count() = buckets_count()*2;
        for (std::size_t i = 0; i < new_bucket_count; i++){

        }
    }
    else if (tool == tool.find(name)){
        if (tool.load_factor() > 0.75){
            bucket_count = bucket_count*2;
        }
    }
    else{
        return true;
    }
    return true;   // TODO
}

Tool* ToolRegistry::find(std::string_view name) const {
    //bool found = false;
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
//       bucketIndex = (hashCode + i) % hashTable⇢length
         
//       // An empty-since-start bucket implies the key is not in the table
//       if (hashTable[bucketIndex] is empty-since-start) {
//          foundEmptySinceStart = true
//       }
//       else if (hashTable[bucketIndex] is not empty-after-removal) {
//          // Check if the non-empty bucket has the key

//          if (key == hashTable[bucketIndex]⇢key) {
//             // Update the value
//             hashTable[bucketIndex]⇢value = value
//             return true
//          }
       
    return nullptr;   // TODO
}

std::size_t ToolRegistry::size() const noexcept {
    return toolCount;   // TODO
}

void ToolRegistry::for_each(const std::function<void(const Tool&)>& /*fn*/) const {

    // TODO
}

std::size_t ToolRegistry::bucket_count() const noexcept {
    return buckets.size();
   //return 0;   // TODO
}

double ToolRegistry::load_factor() const noexcept {
    return (size()*1.0)/bucket_count();
    //return 0.0;   // TODO
}

std::size_t ToolRegistry::max_probe_length() const {
    std::size_t probeWorstCase = 0;
    std::size_t probeDistance = 0;
    for (std::size_t i = 0; i < buckets.size(); i++){
        if (buckets[i] == nullptr){
            continue;
        }
            std::size_t homeSlot = hash(buckets[i]->name()) % buckets.size();
            probeDistance++;
            


            
            probeDistance = probeDistance - i;
            
        if (probeDistance > probeWorstCase){
            probeWorstCase = probeDistance;
        }

    }


    return probeWorstCase;
    
}
