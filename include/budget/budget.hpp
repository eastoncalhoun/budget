#pragma once

#include <string>

#include <crow.h>
#include <nlohmann/json.hpp>
#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>

namespace budget {
    struct NameAmount {
        std::string name;
        std::string amount;
    };

    struct NameAmountUrl : NameAmount {
        std::string url;
    };

    NameAmount parseIncomeBody(const std::string& body);
    NameAmountUrl parseExpenseBody(const std::string& body);
    crow::response genIncomeResponse(const std::string& body);
    crow::response genExpenseResponse(const std::string& body);

    nlohmann::json getCurrentData();
    void setIncome(const std::string& name, const int amount);
    void setExpense(const std::string& name, const int amount, const std::string& url);
}