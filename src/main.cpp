#include <inja/inja.hpp>
#include <crow.h>

#include <budget/budget.hpp>

int main() {
    inja::Environment env;
	crow::SimpleApp app;

    CROW_ROUTE(app, "/")([&env]() {
		int totalIn = 42;
		int totalOut = 64;
		
		return env.render_file("./static/page.html", budget::getCurrentData());
	});

	CROW_ROUTE(app, "/help")([&env]() {
		return env.render_file("./static/help.html", budget::getCurrentData());
	});

	CROW_ROUTE(app, "/error")([&env]() {
		return env.render_file("./static/error.html", budget::getCurrentData());
	});

	CROW_ROUTE(app, "/api/set_income").methods(crow::HTTPMethod::POST)([&env](const crow::request& request){
		return budget::genIncomeResponse(request.body);
	});

	CROW_ROUTE(app, "/api/set_expense").methods(crow::HTTPMethod::POST)([&env](const crow::request& request){
		return budget::genExpenseResponse(request.body);
	});

	app.port(8080).multithreaded().run();
    
    return 0;
}
