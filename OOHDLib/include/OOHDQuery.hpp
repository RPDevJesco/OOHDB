#include "OOHD.hpp"
#include <string>
#include <vector>
#include <unordered_map>

class OOHDQuery {
public:
    OOHDQuery(OOHD& db) : database(db) {}

    QueryResult execute(const std::string& queryString);

    struct QueryParams {
        std::string objectType;
        std::unordered_map<std::string, std::pair<std::string, std::string>> conditions;
        std::vector<std::string> selectAttributes;
        size_t limit = 0;
    };

    QueryParams parseQuery(const std::string& queryString);
    std::vector<std::unordered_map<std::string, std::string>> formatResults(const QueryResult& results, const QueryParams& params);

private:
    OOHD& database;
    Query buildQuery(const QueryParams& params);
};