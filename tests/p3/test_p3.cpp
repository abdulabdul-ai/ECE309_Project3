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
int main(int argc, char** argv) { return mt::run_all(argc, argv); }
