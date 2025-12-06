#include <budget/budget.hpp>

nlohmann::json budget::getCurrentData() {
    soci::session db(soci::sqlite3, "budget.db");
    nlohmann::json current;
    int runningIncome = 0;
    int runningExpenses = 0;

    current["incomes"] = nlohmann::json::array();
    current["expenses"] = nlohmann::json::array();

    soci::rowset<soci::row> incomes = (db.prepare << "SELECT * FROM incomes");

    for (const auto& row : incomes) {
        current["incomes"].push_back({{"name", row.get<std::string>(0)}, {"amount", row.get<int>(1)}});
        runningIncome += row.get<int>(1);
    }

    soci::rowset<soci::row> expenses = (db.prepare << "SELECT * FROM expenses");

    for (const auto& row : expenses) {
        current["expenses"].push_back({{"name", row.get<std::string>(0)}, {"amount", row.get<int>(1)}, {"url", row.get<std::string>(2)}});
        runningExpenses += row.get<int>(1);
    }

    current["profit"] = runningIncome - runningExpenses;
    
    if (current["profit"].get<int>() > 0) {
        current["profitClass"] = "Green";
    } else {
        current["profitClass"] = "Red";
    }

    current["totalIn"] = runningIncome;
    current["totalEx"] = runningExpenses;

    return current;
}

void budget::setIncome(const std::string& name, const int amount) {
    soci::session db(soci::sqlite3, "budget.db");
    const bool deleting = amount == 0;
    
    if (deleting) {
        db << "DELETE FROM incomes WHERE name = :name", soci::use(name);
        return;
    }

    const soci::rowset rows = (db.prepare << "SELECT * FROM incomes WHERE name=:name", soci::use(name));

    for (const auto& row : rows) {
        db << "UPDATE incomes SET amount = :amount WHERE name = :name", soci::use(amount), soci::use(name);

        return;
    }

    db << "INSERT INTO incomes (name, amount) VALUES (:name, :amount)", soci::use(name), soci::use(amount);
}

void budget::setExpense(const std::string& name, const int amount, const std::string&url) {
    soci::session db(soci::sqlite3, "budget.db");
    const bool deleting = amount == 0;
    
    if (deleting) {
        db << "DELETE FROM expenses WHERE name = :name", soci::use(name);
        return;
    }

    const soci::rowset rows = (db.prepare << "SELECT * FROM expenses WHERE name=:name", soci::use(name));

    for (const auto& row : rows) {
        db << "UPDATE expenses SET amount = :amount WHERE name = :name", soci::use(amount), soci::use(name);
        return;
    }

    db << "INSERT INTO expenses (name, amount, url) VALUES (:name, :amount, :url)", soci::use(name), soci::use(amount), soci::use(url);
}

budget::NameAmount budget::parseIncomeBody(const std::string& body) {
    std::string name = body;
    std::string amount;
    
    while (name.find("+") != std::string::npos) {
        name.replace(name.find("+"), 1, " ");
    }

    name.replace(0, name.find("=") + 1, "");
    amount = name;
    amount.replace(0, amount.find("&") + 1, "");
    amount.replace(0, amount.find("=") + 1, "");
    name.replace(name.find("&"), name.size() - 1 - name.size(), "");

    budget::NameAmount nA;
    
    nA.name = name;
    nA.amount = amount;

    return nA;
}

budget::NameAmountUrl budget::parseExpenseBody(const std::string& body) {
    std::string name = body;
    std::string amount, url;
		
    while (name.find("+") != std::string::npos) {
        name.replace(name.find("+"), 1, " ");
    }

    name.replace(0, name.find("=") + 1, "");
    amount = name;
    amount.replace(0, amount.find("&") + 1, "");
    amount.replace(0, amount.find("=") + 1, "");
    url = amount;
    url.replace(0, url.find("&") + 1, "");
    url.replace(0, url.find("=") + 1, "");
    amount.replace(amount.find("&"), amount.size() - 1, "");
    name.replace(name.find("&"), name.size() - 1, "");

    url.replace(url.find("%3A"), 3, ":");

    while (url.find("%2F") != std::string::npos) {
        url.replace(url.find("%2F"), 3, "/");
    }

    budget::NameAmountUrl expense;
    expense.name = name;
    expense.amount = amount;
    expense.url = url;

    return expense;
}

crow::response budget::genIncomeResponse(const std::string& body) {
    budget::NameAmount income;
    crow::response response;
    
    try {
        income = budget::parseIncomeBody(body);
    } catch (std::exception& e) {
        response.code = 303;
        response.add_header("Location", "/error");
    }		

    if (income.name == "" || income.amount == "") {
        response.code = 303;
        response.add_header("Location", "/error");
    } else {
        try {
            budget::setIncome(income.name, std::stoi(income.amount));
            response.code = 303;
            response.add_header("Location", "/");
        } catch (std::exception& e) {
            response.code = 303;
            response.add_header("Location", "/error");
        }
    }

    return response;
}

crow::response budget::genExpenseResponse(const std::string& body) {
    budget::NameAmountUrl expense;
    crow::response response;
    
    try {
        expense = budget::parseExpenseBody(body);
    } catch (std::exception& e) {
        response.code = 303;
        response.add_header("Location", "/error");
    }		

    if (expense.name == "" || expense.amount == "") {
        response.code = 303;
        response.add_header("Location", "/error");
    } else {
        try {
            budget::setExpense(expense.name, std::stoi(expense.amount), expense.url);
            response.code = 303;
            response.add_header("Location", "/");
        } catch (std::exception& e) {
            response.code = 303;
            response.add_header("Location", "/error");
        }
    }

    return response;
}