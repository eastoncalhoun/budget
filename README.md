# Budget Tracker - Example Suffer Project
A lightweight C++ web application for tracking personal income and expenses with real-time budget calculations.

# Features

- Immediate access to expenses pay page's with URLs inputted
- Automatic profit/loss calculations with color coding
- Server-side HTML rendering with Inja templates
- SQLite database for persistent storage
- Multithreaded web server

# Tech Stack

- HTML
- CSS
- C++

# Quick Start

```bash
suffer install json
suffer install soci
suffer install Crow
suffer install inja

suffer import soci
suffer import Crow
suffer import inja

make
make run
```
Server runs on http://localhost:8080

# API Endpoints
- GET / - Dashboard
- GET /help - Help page
- GET /error - Error page
- POST /api/set_income - Body: name=Salary&amount=5000
- POST /api/set_expense - Body: name=Rent&amount=1200&url=https://example.com

# Database
- incomes: name TEXT, amount INTEGER
- expenses: name TEXT, amount INTEGER, url TEXT

</ul>

# Dependencies
Managed by Suffer

- Crow - Web framework
- Inja - Template engine
- SOCI - Database access
- nlohmann/json - JSON handling

# Security Note
⚠️ Example project only. For production, add: input validation, authentication, HTTPS, CSRF protection, and rate limiting.
