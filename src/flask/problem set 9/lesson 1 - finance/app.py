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
    grand_total = cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]
    results = db.execute(
        "SELECT stocks.symbol, shares.number_of_stocks FROM stocks INNER JOIN shares ON stocks.id=shares.stock_id WHERE shares.user_id = ?",
        user_id
    )
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
        symbol = request.form.get("symbol")
        if not symbol:
            flash("Stock Symbol Required")
            return apology("Missing Input", 400)

        shares = request.form.get("shares")
        try:
            shares = floor(float(shares))
        except:
            flash ("Enter valid number of shares")
            return apology("Enter Valid Input", 400)

        if shares < 1:
            flash ("Enter valid number")
            return apology("Enter Valid Input", 400)

        result = lookup(symbol)
        if not result:
            flash ("Enter valid symbol")
            return apology("Enter Valid Input", 400)

        user_id = session.get("user_id")
        if not user_id:
            return apology("User not found", 400)

        try:
            cash_row = db.execute(
                "SELECT cash FROM users WHERE id = ?",
                user_id
                )
        except:
            return apology("Data not found", 400)

        cash = float(cash_row[0]["cash"])
        if cash < result["price"]:
            flash("Add more 'cash' to your account so that you can continue trading!")
            return apology("Insufficient Funds", 403)

        try:
            stock_row = db.execute(
                "SELECT id FROM stocks WHERE symbol = ?",
                result["symbol"]
                )
        except:
            try:
                db.execute(
                    "INSERT INTO stocks (id, stock_name, symbol) VALUES (?, ?)",
                    result["name"], result["symbol"]
                    )
                stock_row = db.execute(
                    "SELECT id FROM stocks WHERE symbol = ?",
                    result["symbol"]
                    )
            except:
                return apology("Data not found", 400)

        stock_id = stock_row[0]["id"]
        shares_total_price = shares * result["price"]
        cash_balance = cash - shares_total_price
        description = "Bought x"+str(shares)+" "+result["name"]+" ("+result["symbol"]+") "+usd(result["price"])

        try:
            try:
                # stock_transactions
                db.execute(
                    "INSERT INTO stock_transactions (user_id, stock_id, date, number_of_stocks, amount_per_stock, transaction_type) VALUES (?, ?, CURRENT_TIMESTAMP, ?, ?, ?)",
                    user_id, stock_id, shares, result[price], "buy"
                    )
            except:
                return apology("#1")

            try:
                # cash_transactions
                db.execute(
                    "INSERT INTO cash_transactions (user_id, date, description, amount, transaction_type) VALUES (?, CURRENT_TIMESTAMP, ?, ?, ?)",
                    user_id, description, result[price], "withdrawal"
                    )
            except:
                return apology("#2")

            try:
                # users (cash)
                db.execute(
                    "UPDATE users SET cash = ? WHERE id = ?",
                    cash_balance, user_id
                    )
            except:
                return apology("#3")

            # shares
            try:
                share_row = db.execute(
                    "SELECT id, number_of_stocks FROM shares WHERE stock_id = (SELECT id FROM stocks WHERE symbol = ?) AND user_id = ?",
                    result["symbol"], user_id
                    )
            except:
                try:
                    db.execute(
                        "INSERT INTO shares (id, user_id, stock_id, number_of_stocks) VALUES (?, ?, ?)",
                        user_id, stock_id, 0
                        )
                    share_row = db.execute(
                        "SELECT id, number_of_stocks FROM shares WHERE stock_id = (SELECT id FROM stocks WHERE symbol = ?) AND user_id = ?",
                        result["symbol"], user_id
                        )
                except:
                    return apology("Unable to complete transaction", 400)

            db.execute(
                "UPDATE shares SET number_of_stocks = ? WHERE id = ?",
                share_row[0]["number_of_stocks"] + shares, share_row[0]["id"]
                )

        except:
            return apology("Unable to complete transaction", 400)

        return redirect("/")

    if request.method == "POST" and "/quote" in request.headers.get("Referer"):
        searched_symbol = request.form.get("searched_symbol")
        if not searched_symbol:
            return render_template("buy.html")

        return render_template("buy.html", symbol=searched_symbol)

    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # Complete the implementation of history in such a way that it displays an HTML table
        # summarizing all of a user’s transactions ever,

        # listing row by row each and every buy and every sell.

    # For each row, make clear whether a stock was bought or sold and include
        # the stock’s symbol,

        # the (purchase or sale) price,

        # the number of shares bought or sold,

        # and the date and time at which the transaction occurred.

    # You might need to alter the table you created for buy or supplement it with an additional table. Try to minimize redundancies.

    return apology("TODO")


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
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

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
        symbol = request.form.get("symbol")
        if not symbol:
            flash("Enter valid Symbol")
            return apology("Missing Input", 400)

        results = lookup(symbol)
        if not results:
            flash("Enter valid Symbol")
            return apology("Missing Input", 400)

        row = db.execute("SELECT * FROM stocks WHERE name = ?", results["name"])
        if not row:
            db.execute("INSERT INTO stocks (name, symbol) VALUES (?, ?)", results["name"], results["symbol"])

        return render_template("quote.html", results=results, request=request)

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

        try:
            db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, generate_password_hash(password))
        except ValueError:
            return apology("username already exists", 400)

        user_id = db.execute("SELECT id FROM users WHERE username = ?", username)[0]["id"]
        db.execute("INSERT INTO cash_transactions (user_id, date, description, amount, transaction_type) VALUES (?, CURRENT_TIMESTAMP, ?, ?, ?)", user_id, "Acc. Registration Bonus", 10000.00, "deposit")

        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session.get("user_id")

    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            flash("Select a symbol from the dropdown select input")
            return apology("Missing Input", 403)

        shares = request.form.get("shares")
        if not shares:
            flash("Input positive number of shares")
            return apology("Missing Input", 403)

        shares = int(shares)
        if shares < 1:
            flash("Input positive number of shares")
            return apology("Missing Input", 403)

        row = db.execute(
            "SELECT stocks.*, shares.number_of_stocks FROM stocks INNER JOIN shares ON stocks.id=shares.stock_id WHERE shares.user_id = ? AND stocks.symbol = ? AND shares.number_of_stocks >= 1",
            user_id, symbol
        )[0]
        if not row:
            return apology("You do not own any shares of that stock", 403)

        if shares > row["number_of_stocks"]:
            return apology("You do not own that many shares", 403)

        # do the db manipulation
        price = lookup(row["symbol"])["price"]
        db.execute(
            "INSERT INTO stock_transactions (user_id, stock_id, date, number_of_stocks, amount_per_stock, transaction_type) VALUES (?, ?, CURRENT_TIMESTAMP, ?, ?, ?)",
            user_id, row["id"], shares, price, "sell"
        )

        description = "Sold x"+str(shares)+" "+row["name"]+" @ "+usd(price)
        total_price = price * shares
        db.execute(
            "INSERT INTO cash_transactions (user_id, date, description, amount, transaction_type) VALUES (?, CURRENT_TIMESTAMP, ?, ?, ?)",
            user_id, description, total_price, "deposit"
        )

        cash = float(db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"])
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash + total_price, user_id,)

        db.execute("UPDATE shares SET number_of_stocks = ? WHERE user_id = ? AND stock_id = ?", row["number_of_stocks"] - shares, user_id, row["id"])

        flash("Stock Sold! (not really)")
        return redirect("/")

    else:
        symbols = db.execute(
            "SELECT stocks.symbol FROM stocks INNER JOIN shares ON stocks.id=shares.stock_id WHERE shares.user_id = ?",
            user_id
        )
        if not symbols:
            return render_template("sell.html")

        return render_template("sell.html", symbols=symbols)
