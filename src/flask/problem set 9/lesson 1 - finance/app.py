import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from math import floor
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_id = session.get("user_id")
    try:
        try:
            cash_row = db.execute(
                "SELECT cash FROM users WHERE id = ?",
                user_id
            )
        except:
            raise Exception("Database call unsuccessful")

        grand_total = cash = cash_row[0]["cash"]

        try:
            results = db.execute(
                "SELECT stocks.symbol, shares.number_of_stocks FROM stocks INNER JOIN shares ON stocks.id=shares.stock_id WHERE shares.user_id = ? AND shares.number_of_stocks > 0",
                user_id
            )
        except:
            raise Exception("Database call unsuccessful")
    except Exception as ex:
        return apology(ex, 400)

    if not results:
        return render_template("index.html", cash=cash, grand_total=grand_total)

    rows = []
    for result in results:
        share_price = lookup(result["symbol"])["price"]
        total_price = share_price * result["number_of_stocks"]

        row = {"symbol": result["symbol"], "shares": result["number_of_stocks"],
               "share_price": share_price, "total_price": total_price}
        rows.append(row)

        grand_total = grand_total + total_price

    return render_template("index.html", rows=rows, cash=cash, grand_total=grand_total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST" and request.path == "/buy":
        results = lookup(request.form.get("symbol"))
        if not results:
            return apology("Enter valid stock symbol", 400)

        shares = request.form.get("shares")
        if not shares:
            return apology("Missing Input", 400)

        try:
            shares = int(shares)
            if not isinstance(shares, int):
                return apology("Enter valid input", 400)
        except:
            return apology("Enter valid input", 400)

        if shares <= 0:
            return apology("enter positive number", 400)

        user_id = session.get("user_id")
        if not user_id:
            return apology("User not found", 400)

        try:
            # select available cash in account
            try:
                cash_row = db.execute(
                    "SELECT cash FROM users WHERE id = ?",
                    user_id
                )
                cash = float(cash_row[0]["cash"])
            except:
                raise Exception("Cash data could not be retrieved")

            grand_total = results["price"] * shares
            # check if cash is > than grand_total
            if cash < grand_total:
                return apology("Insufficient funds for transaction")

            description = "Bought x" + \
                str(shares)+" "+results["name"] + \
                " ("+results["symbol"]+") @ "+str(usd(results["price"]))
            # insert cash_transaction
            try:
                db.execute(
                    "INSERT INTO cash_transactions (user_id, date, description, amount, transaction_type) VALUES (?, CURRENT_TIMESTAMP, ?, ?, ?)",
                    user_id, description, grand_total, "withdrawal"
                )
            except:
                raise Exception("Cash transaction could not be recorded")

            # remove cash spent from user
            new_cash_value = cash - grand_total
            try:
                db.execute(
                    "UPDATE users SET cash = ? WHERE id = ?",
                    new_cash_value, user_id
                )
            except:
                raise Exception("Account funds could not be removed from account")

            # stock symbol db
            try:
                stock_row = db.execute(
                    "SELECT id FROM stocks WHERE symbol = ?",
                    results["symbol"]
                )
                if not stock_row:
                    db.execute(
                        "INSERT INTO stocks (name, symbol) VALUES (?, ?)",
                        results["name"], results["symbol"]
                    )
                    stock_row = db.execute(
                        "SELECT id FROM stocks WHERE symbol = ?",
                        results["symbol"]
                    )
                stock_id = stock_row[0]["id"]
            except:
                raise Exception("Stock data could not be retrieved")

            # insert stock_transaction db
            try:
                db.execute(
                    "INSERT INTO stock_transactions (user_id, stock_id, date, number_of_stocks, amount_per_stock, transaction_type) VALUES (?, ?, CURRENT_TIMESTAMP, ?, ?, ?)",
                    user_id, stock_id, shares, results["price"], "buy"
                )
            except:
                raise Exception("Stock transaction could not be recorded")

            # shares db
            try:
                shares_row = db.execute(
                    "SELECT number_of_stocks FROM shares WHERE user_id = ? AND stock_id = ?",
                    user_id, stock_id
                )
                if not shares_row:
                    db.execute(
                        "INSERT INTO shares (user_id, stock_id, number_of_stocks) VALUES (?, ?, ?)",
                        user_id, stock_id, shares
                    )
                else:
                    updated_shares_amount = shares + shares_row[0]["number_of_stocks"]
                    db.execute(
                        "UPDATE shares SET number_of_stocks = ? WHERE user_id = ? AND stock_id = ?",
                        updated_shares_amount, user_id, stock_id
                    )
            except:
                raise Exception("Shares data could not be updated")
        except Exception as ex:
            return apology(ex, 400)

        if shares > 1:
            flash("Shares Bought!")
        else:
            flash("Share Bought!")

        return redirect("/")

    if request.method == "POST" and "/quote" in request.headers.get("Referer"):
        searched_symbol = request.form.get("searched_symbol")
        if not searched_symbol:
            return render_template("buy.html")

        return render_template("buy.html", symbol=searched_symbol)

    return render_template("buy.html")


@app.route("/cash", methods=["GET", "POST"])
@login_required
def cash():
    user_id = session.get("user_id")

    if request.method == "POST":
        added_cash = request.form.get("cash")
        if not added_cash:
            return apology("Missing Input", 400)

        try:
            added_cash = float(added_cash)
            if not isinstance(added_cash, float):
                return apology("Enter valid input", 400)
        except:
            return apology("Enter valid input", 400)

        if added_cash <= 0:
            return apology("Enter positive number", 400)

        try:
            # select available cash in account
            try:
                cash_row = db.execute(
                    "SELECT cash FROM users WHERE id = ?",
                    user_id
                )
                current_cash = float(cash_row[0]["cash"])
            except:
                raise Exception("Cash data could not be retrieved")

            new_cash_amount = current_cash + added_cash
            # insert cash_transaction
            try:
                db.execute(
                    "INSERT INTO cash_transactions (user_id, date, description, amount, transaction_type) VALUES (?, CURRENT_TIMESTAMP, ?, ?, ?)",
                    user_id, "Added cash to Acc.", added_cash, "deposit"
                )
            except:
                raise Exception("Cash transaction could not be recorded")

            try:
                db.execute(
                    "UPDATE users SET cash = ? WHERE id = ?",
                    new_cash_amount, user_id
                )
            except:
                raise Exception("Cash data could not be updated")
        except Exception as ex:
            return apology(ex, 400)

        flash("Cash added to account!")
        return redirect("/cash")
    else:
        # get rows data for cash_history
        try:
            results = db.execute(
                "SELECT date, description, amount, transaction_type FROM cash_transactions WHERE user_id = ?",
                user_id
            )
        except:
            return apology("Could not access database at this time", 400)

        if not results:
            return render_template("cash.html")

        rows = []
        running_total = 0
        for result in results:
            if result["transaction_type"] == "deposit":
                running_total = running_total + result["amount"]
            else:
                running_total = running_total - result["amount"]

            row = {"date": result["date"], "description": result["description"], "amount": result["amount"],
                   "running_total": running_total, "transaction_type": result["transaction_type"]}
            rows.append(row)

        return render_template("cash.html", rows=rows, grand_total=running_total)


@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    if request.method == "POST":
        password = request.form.get("password")
        if not password:
            return apology("must provide password", 400)

        confirmation = request.form.get("confirmation")
        if not confirmation:
            return apology("must confirm password", 400)
        elif password != confirmation:
            return apology("passwords must match", 400)

        user_id = session.get("user_id")
        try:
            rows = db.execute(
                "SELECT * FROM users WHERE id = ?",
                user_id
            )
        except:
            return apology("Could not retrieve data from db", 400)

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], password):
            return apology("Could not process request", 405)

        return redirect("/confirm_change_password")
    else:
        return render_template("password.html")


@app.route("/confirm_change_password", methods=["GET", "POST"])
@login_required
def confirm_change_password():
    if request.method == "POST":
        password = request.form.get("password")
        if not password:
            return apology("must provide password", 400)

        confirmation = request.form.get("confirmation")
        if not confirmation:
            return apology("must confirm password", 400)
        elif password != confirmation:
            return apology("passwords must match", 400)

        user_id = session.get("user_id")
        try:
            rows = db.execute(
                "SELECT * FROM users WHERE id = ?",
                user_id
            )
        except:
            return apology("Could not retrieve data from db", 400)

        # Ensure username exists and password is correct
        if len(rows) != 1 or check_password_hash(rows[0]["hash"], password):
            return apology("New password cannot be the same as an old password", 405)

        try:
            db.execute(
                "UPDATE users SET hash = ? WHERE id = ?",
                generate_password_hash(password), user_id
            )
        except:
            return apology("Database cannot be reached at this time", 400)

        flash("Password changed!")
        return redirect("/")
    else:
        return render_template("password.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session.get("user_id")

    try:
        rows = db.execute(
            "SELECT stock_transactions.date, stocks.symbol, stock_transactions.number_of_stocks, stock_transactions.amount_per_stock, stock_transactions.transaction_type FROM stock_transactions INNER JOIN stocks ON stock_transactions.stock_id=stocks.id WHERE stock_transactions.user_id = ?",
            user_id
        )
    except:
        return apology("Could not access database at this time", 400)

    if not rows:
        return render_template("history.html")

    return render_template("history.html", rows=rows)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 404)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 405)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        results = lookup(request.form.get("symbol"))
        if not results:
            return apology("Enter valid Symbol", 400)

        return render_template("quote.html", results=results)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        if not username:
            return apology("must provide username", 400)

        password = request.form.get("password")
        if not password:
            return apology("must provide password", 400)

        confirmation = request.form.get("confirmation")
        if not confirmation:
            return apology("must confirm password", 400)
        elif password != confirmation:
            return apology("passwords must match", 400)

        acc_create_error_msg = "Account creation has failed please try again later"

        try:
            try:
                db.execute(
                    "INSERT INTO users (username, hash) VALUES (?, ?)",
                    username, generate_password_hash(password)
                )
            except ValueError:
                return apology(acc_create_error_msg, 400)

            try:
                user_id_row = db.execute(
                    "SELECT id FROM users WHERE username = ?",
                    username
                )

                user_id = user_id_row[0]["id"]
            except:
                raise IndexError(acc_create_error_msg)

            try:
                db.execute(
                    "INSERT INTO cash_transactions (user_id, date, description, amount, transaction_type) VALUES (?, CURRENT_TIMESTAMP, ?, ?, ?)",
                    user_id, "Acc. Registration Bonus", 10000.00, "deposit"
                )
            except:
                raise Exception(acc_create_error_msg)
        except Exception as ex:
            return apology(ex, 400)
        else:
            return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session.get("user_id")

    if request.method == "POST":
        results = lookup(request.form.get("symbol"))
        if not results:
            flash("Stock symbol required")
            return apology("Select valid stock symbol", 400)

        shares = request.form.get("shares")
        if not shares:
            return apology("Missing shares input", 400)

        try:
            shares = int(shares)
            if not isinstance(shares, int):
                return apology("Enter whole number for shares input", 400)
        except:
            return apology("Enter valid shares input", 400)

        if shares <= 0:
            return apology("Enter positive number", 400)

        try:
            # stock symbol db
            try:
                stock_row = db.execute(
                    "SELECT id FROM stocks WHERE symbol = ?",
                    results["symbol"]
                )
                if not stock_row:
                    return apology("Stock data could not be retrieved", 400)

                stock_id = stock_row[0]["id"]
            except:
                raise Exception("Could not connect to database for stock_id retrieval")

            try:
                shares_row = db.execute(
                    "SELECT number_of_stocks FROM shares WHERE user_id = ? AND stock_id = ?",
                    user_id, stock_id
                )
                if not shares_row:
                    return apology("Shares data could not be retrieved", 400)

                number_of_stocks = shares_row[0]["number_of_stocks"]
            except:
                raise Exception("Could not connect to database for number of shares retrieval")
        except Exception as ex:
            return apology(ex, 400)

        # check if symbol is one that user owns any shares off
        if number_of_stocks < shares:
            return apology("Insufficient number of shares to sell", 400)

        try:
            # insert stock_transaction db
            try:
                db.execute(
                    "INSERT INTO stock_transactions (user_id, stock_id, date, number_of_stocks, amount_per_stock, transaction_type) VALUES (?, ?, CURRENT_TIMESTAMP, ?, ?, ?)",
                    user_id, stock_id, shares, results["price"], "sell"
                )
            except:
                raise Exception("Stock transaction could not be recorded")

            # shares db
            try:
                shares_row = db.execute(
                    "SELECT number_of_stocks FROM shares WHERE user_id = ? AND stock_id = ?",
                    user_id, stock_id
                )
                if not shares_row:
                    return apology("Shares data could not be retrieved")
                else:
                    updated_shares_amount = shares_row[0]["number_of_stocks"] - shares
                    db.execute(
                        "UPDATE shares SET number_of_stocks = ? WHERE user_id = ? AND stock_id = ?",
                        updated_shares_amount, user_id, stock_id
                    )
            except:
                raise Exception("Shares data could not be updated")

            grand_total = results["price"] * shares
            description = "Sold x" + \
                str(shares)+" "+results["name"] + \
                " ("+results["symbol"]+") @ "+str(usd(results["price"]))
            # insert cash_transaction
            try:
                db.execute(
                    "INSERT INTO cash_transactions (user_id, date, description, amount, transaction_type) VALUES (?, CURRENT_TIMESTAMP, ?, ?, ?)",
                    user_id, description, grand_total, "deposit"
                )
            except:
                raise Exception("Cash transaction could not be recorded")

            # select available cash in account
            try:
                cash_row = db.execute(
                    "SELECT cash FROM users WHERE id = ?",
                    user_id
                )
                cash = float(cash_row[0]["cash"])
            except:
                raise Exception("Cash data could not be retrieved")

            # add cash paid out to user
            new_cash_value = cash + grand_total
            try:
                db.execute(
                    "UPDATE users SET cash = ? WHERE id = ?",
                    new_cash_value, user_id
                )
            except:
                raise Exception("Account funds could not be removed from account")
        except Exception as ex:
            return apology(ex, 400)

        return redirect("/")
    else:
        try:
            symbols = db.execute(
                "SELECT symbol FROM stocks INNER JOIN shares ON stocks.id = shares.stock_id WHERE user_id = ?",
                user_id
            )
        except:
            return apology("Database could not be reached", 400)

        return render_template("sell.html", symbols=symbols)
