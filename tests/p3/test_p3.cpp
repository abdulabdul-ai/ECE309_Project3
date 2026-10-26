// tests/p3/test_p3.cpp — YOUR P3 TEST SUITE.
//
// The five tests below are worked examples. Keep them, and add your own
// until you have AT LEAST 15 (spec §6 lists what must be covered).
// Name every test <area>_<what>: reg_ for the registry, ws_ for the
// workspace, calc_ for the calculator.
//
// Build & run:   cmake --build build && ./build/test_p3
// One area:      ./build/test_p3 reg_        (names starting with "reg_")
// One test:      ./build/test_p3 =reg_add_and_find

#include "../mini_test.h"
#include "../test_helpers.h"     // TempDir, write_text, args1, ...

#include "tools/calculator.h"
#include "tools/tool_registry.h"
#include "tools/workspace.h"

#include <cstdio>
#include <memory>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace {

// A tool whose name you choose — use it to fill the registry in tests.
class NamedTool : public Tool {
public:
    explicit NamedTool(std::string n) : n_(std::move(n)), schema_(JsonValue::object()) {}
    std::string name() const override { return n_; }
    std::string description() const override { return "test tool"; }
    const JsonValue& schema() const override { return schema_; }
    ToolResult invoke(const JsonValue&, ToolContext&) override { return ToolResult::success(n_); }
private:
    std::string n_;
    JsonValue   schema_;   // an empty JSON object: {}
};

// Tokenize with the provided tokenizer, then run YOUR calc_evaluate.
bool eval(const std::string& expr, double* value, std::string* error) {
    std::vector<CalcToken> tokens;
    if (!calc_tokenize(expr, &tokens, error)) return false;
    return calc_evaluate(tokens, value, error);
}

}  // namespace

// ---------------------------------------------------------------- examples

TEST(reg_add_and_find) {
    ToolRegistry r;
    auto t = std::make_unique<NamedTool>("alpha");
    Tool* raw = t.get();                        // remember the address before moving
    REQUIRE(r.add(std::move(t)));
    CHECK(r.size() == 1);
    CHECK(r.find("alpha") == raw);              // same object, not a copy
    CHECK(r.find("beta") == nullptr);
}

TEST(reg_duplicate_keeps_original) {
    ToolRegistry r;
    auto first = std::make_unique<NamedTool>("alpha");
    Tool* original = first.get();
    REQUIRE(r.add(std::move(first)));
    CHECK(!r.add(std::make_unique<NamedTool>("alpha")));
    CHECK(r.find("alpha") == original);
    CHECK(r.size() == 1);
}

TEST(ws_rejects_parent_escape) {
    TempDir d;
    Workspace ws(d.path());
    fs::path out;
    std::string reason;
    REQUIRE(ws.resolve("notes/todo.txt", &out, &reason));   // sanity: accepts *something*
    CHECK(out == ws.root() / "notes" / "todo.txt");
    CHECK(!ws.resolve("../etc/passwd", &out, &reason));
    CHECK(!reason.empty());
}

TEST(calc_precedence) {
    double v = 0;
    std::string err;
    REQUIRE(eval("2 + 3 * 4", &v, &err));
    CHECK(v == 14);
}

TEST(calc_division_by_zero) {
    double v = 0;
    std::string err;
    CHECK(!eval("1 / 0", &v, &err));
    CHECK(err == "division by zero");
}

// ------------------------------------------------------------- your tests
//
// Still to write (spec §6 has the full list). Suggested names:
//   reg_null_rejected, reg_for_each_visits_each_once
//   reg_10000_tools               REQUIRED: print bucket_count / load_factor /
//                                 max_probe_length, check the bound
//   reg_alternative_hash          REQUIRED: longest chain of a weak hash
//   ws_<case>                     REQUIRED: every row of the table in spec §4.4
//   calc_<case>                   left-to-right, parentheses, negatives

//T1
TEST(reg_null_rejected){
    ToolRegistry r;
    CHECK(r.size() == 0);

    CHECK(r.add(nullptr) == false);
    CHECK(r.size() == 0);

    CHECK(r.add(std::make_unique<EchoTool>("uno")) == true);
    CHECK(r.size() == 1);

    CHECK(r.find("absent") == nullptr);
    CHECK(r.size() == 1);

}

//T2
TEST(reg_for_each_visits_each_once){
    ToolRegistry r;
    r.add(std::make_unique<EchoTool>("uno"));
    r.add(std::make_unique<EchoTool>("dos"));
    r.add(std::make_unique<EchoTool>("tres"));
    r.add(std::make_unique<EchoTool>("quatro"));
    r.add(std::make_unique<EchoTool>("cinco"));
    r.add(std::make_unique<EchoTool>("seis"));
    r.add(std::make_unique<EchoTool>("siete"));

    int count_1 = 0;   
    int count_2 = 0;
    int count_3 = 0;
    int count_4 = 0;
    int count_5 = 0;
    int count_6 = 0;
    int count_7 = 0;
    int count_total = 0;

    r.for_each([&](const Tool& tool){
        count_total++;
        if (tool.name() == "uno"){
            count_1++;
        }
        if (tool.name() == "dos"){
            count_2++;
        }
        if (tool.name() == "tres"){
            count_3++;
        }
        if (tool.name() == "quatro"){
            count_4++;
        }
        if (tool.name() == "cinco"){
            count_5++;
        }
        if (tool.name() == "seis"){
            count_6++;
        }
        if (tool.name() == "siete"){
            count_7++;
        }
    });

    CHECK(count_1 == 1);
    CHECK(count_2 == 1);
    CHECK(count_3 == 1);
    CHECK(count_4 == 1);
    CHECK(count_5 == 1);
    CHECK(count_6 == 1);
    CHECK(count_7 == 1);
    CHECK(count_total == 7);


}

//T3
TEST(reg_10000_tools){
    ToolRegistry r; //Initiliaze variables
    Tool* firstPointer = nullptr;
    bool loadFactorValid = true;
    int nameCountValid = 10000;

    //Traverse through a loop 10,000 times to add tolls
    for (int i = 0; i < 10000; i++){ 
        r.add(std::make_unique<NamedTool>("tool_" + std::to_string(i)));
        // get the pointer address for the first tool
        if (i == 0){
            firstPointer = r.find("tool_0");
        }
        //Every iterations in the loop, check if load_factor ever crosses 1.0
        if (r.load_factor() > 1.0){
            loadFactorValid = false;
        }
    }



    // Go through the tools list to see if every name can 
    // be found (has an address). If not, remove from the 10000 count
    for (int i = 0; i < 10000; i++){
        if (r.find("tool_" + std::to_string(i)) == nullptr){
            nameCountValid--;
        }
    }
    // Check if every name found by checking if number is equal to 10000
    CHECK(nameCountValid == 10000);
    //Check if pointer obtained after first insert is the same after last
    CHECK(r.find("tool_0") == firstPointer);
    CHECK(loadFactorValid == true);
    CHECK(r.max_probe_length() <= 64);

    // Prints the values, had to use chatGPT to find out how to convert between different types using type cast.
    std::printf("Bucket Count: %d\n", static_cast<int>(r.bucket_count()));
    std::printf("Load Factor: %.2f\n", r.load_factor());
    std::printf("Max Probe Length: %d\n", static_cast<int>(r.max_probe_length()));

}

//T4
TEST(reg_alternative_hash){
    ToolRegistry r;
    int largest_chain = 0;

    // Insert all tools
    for (int i = 0; i < 10000; i++){
        r.add(std::make_unique<NamedTool>("tool_" + std::to_string(i)));
    }
    std::vector<int> bucketCounter(r.bucket_count(),0);
    // Use the alt. hash
    for (int i = 0; i < 10000; i++){
        std::string name = "tool_" + std::to_string(i);                           // rebuild the name
        std::size_t alt_hash = static_cast<unsigned char>(name[0]) + name.size(); // first character + length
        bucketCounter[alt_hash % bucketCounter.size()]++;                         // count it in its bucket
    }
    for (std::size_t i = 0; i < bucketCounter.size(); i++){
        if(bucketCounter[i]> largest_chain){
            largest_chain = bucketCounter[i];
        }
    }
    
    std::printf("Weak Hash's longest chain: %d | Real Numbers: %d\n", largest_chain, static_cast<int>(r.max_probe_length()));
}

//T5
 TEST(ws_accept_row_paths){
    TempDir root;
    Workspace ws(root.path());
    
    fs::path out;
    std::string reason;

    CHECK(ws.resolve("notes/todo.txt", &out, &reason)); 
    CHECK(out == (ws.root() / "notes" / "todo.txt"));

    CHECK(ws.resolve("./notes//todo.txt", &out, &reason)); 
    CHECK(out == (ws.root() / "notes" / "todo.txt"));

    CHECK(ws.resolve(".", &out, &reason));
    CHECK(out == (ws.root())); 

    CHECK(ws.resolve("a/../b", &out, &reason)); 
    CHECK(out == (ws.root() / "b")); 

    CHECK(ws.resolve("a/..", &out, &reason)); 
    CHECK(out == (ws.root())); 

    CHECK(ws.resolve("new/dir/file.txt", &out, &reason)); 
    CHECK(out == (ws.root() / "new" / "dir" / "file.txt"));

    CHECK(ws.resolve("my..notes.txt", &out, &reason)); 
    CHECK(out == (ws.root() / "my..notes.txt")); 

    reason = "";
    CHECK(!(ws.resolve("", &out, &reason))); 
    CHECK(!reason.empty()); 



 }

//T6
 TEST(calc_no_parenthesis){
    double v = 0;
    std::string err;
    REQUIRE(eval("2 + 3 * 4", &v, &err));
    CHECK(v == 14);

    v = 0;
    err = "";
    REQUIRE(eval("10 - 4 - 3", &v, &err));
    CHECK(v == 3);

    v = 0;
    err = "";
    REQUIRE(eval("100 / 10 / 5", &v, &err));
    CHECK(v == 2);





 }

 //T7
 TEST(calc_with_parentheis){
    double v = 0;
    std::string err = "";
    REQUIRE(eval("(2 + 3) * 4", &v, &err));
    CHECK(v == 20);

    v = 0;
    err = "";
    REQUIRE(eval("2 * (3 + 4) - 5", &v, &err));
    CHECK(v == 9);

    v = 0;
    err = "";
    REQUIRE(eval("-3 * -3", &v, &err));
    CHECK(v == 9);

    v = 0;
    err = "";
    REQUIRE(eval("7 / 2", &v, &err));
    CHECK(v == 3.5);

    v = 0;
    err = "";
    REQUIRE(!eval("1 / (2 - 2)", &v, &err));
    CHECK(err == "division by zero");
 }

//T7
 TEST(ws_rejects_dotdot){
    TempDir root;
    Workspace ws(root.path());
    
    fs::path out;
    std::string reason;
    reason = "";
    std::string row9 = std::string("notes") + '\0' + "/x";
    CHECK(!(ws.resolve(row9, &out, &reason))); 
    CHECK(!reason.empty()); 

    reason = "";
    CHECK(!(ws.resolve("/etc/passwd", &out, &reason))); 
    CHECK(!reason.empty()); 

    reason = "";
    CHECK(!(ws.resolve("..", &out, &reason))); 
    CHECK(!reason.empty()); 

    reason = "";
    CHECK(!(ws.resolve("../etc/passwd", &out, &reason))); 
    CHECK(!reason.empty()); 

    reason = "";
    CHECK(!(ws.resolve("a/../../b", &out, &reason))); 
    CHECK(!reason.empty()); 

    reason = "";
    CHECK(!(ws.resolve("notes/../../ws/notes/todo.txt", &out, &reason))); 
    CHECK(!reason.empty()); 
 }

// //T8
 TEST(reg_hash_order){
    CHECK(ToolRegistry::hash("ab") != ToolRegistry::hash("ba"));

}

// //T9
 TEST(calc_my_expressions){
    double v = 0;
    std::string err = "";
    REQUIRE(eval("-0 + (2 + 3) * -1 * (-4)", &v, &err));
    CHECK(v == 20);

    v = 0;
    err = "";
    REQUIRE(eval("-10.5 * 5.5", &v, &err));
    CHECK(v == -57.75);
 }

// //T10
 TEST(calc_operator_unary){
    double v = 0;
    std::string err = "";
    REQUIRE(eval("2 +2 - 3 /-3 +1 *2", &v, &err));
    CHECK(v == 7);
 }


 int main(int argc, char** argv) { return mt::run_all(argc, argv); }
