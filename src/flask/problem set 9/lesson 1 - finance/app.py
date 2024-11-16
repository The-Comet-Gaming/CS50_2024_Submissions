import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
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
    # Complete the implementation of index in such a way that it displays an HTML table summarizing, for the user currently logged in,
        # which stocks the user owns,

        # the numbers of shares owned,

        # the current price of each stock,

        # and the total value of each holding (i.e., shares times price).

    # Also display the user’s current cash balance along with a grand total (i.e., stocks’ total value plus cash).

    # Odds are you’ll want to execute multiple SELECTs.
        # Depending on how you implement your table(s), you might find GROUP BY HAVING SUM and/or WHERE of interest.

    # Odds are you’ll want to call lookup for each stock.
    return apology("TODO")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # Sub route for users who have been directed to the route from /quote by using the "stock_symbol" button
    if request.method == "POST" and "/quote" in request.headers.get("Referer"):
        stock_symbol = request.form.get("stock_symbol")
        return render_template("buy.html", symbol=stock_symbol)

    # Sub route for users who are directed to the route from /buy by using the "Buy Shares" button
    if request.method == "POST" and request.path == "/buy":
        symbol = request.form.get("symbol")
        if not symbol:
            flash("Enter valid Symbol")
            return apology("Missing Input", 403)

        results = lookup(symbol)
        if not results:
            flash("Enter valid Symbol")
            return apology("Missing Input", 403)

        shares = request.form.get("shares")
        if not shares:
            flash("Must input positive number of shares to buy")
            return apology("Missing Input", 403)

        else:
            shares = int(shares)
            if shares < 1:
                    flash("Must input positive number of shares to buy")
                    return apology("Missing Input", 403)
            # check to see if this ^ if statement is redundant because of the the min="1" tag in the buy.html

        user_id = session.get("user_id")
        cash = float(db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"])
    # might need to convert this ^ into this for readability purposes:
        # cash = db.execute("SELECT cash FROM users WHERE id = ?", session.get("user_id")
        # cash = float(cash[0]["cash"])
        price = results["price"]
        total_price = price * shares
        if cash < total_price:
            flash("Add more 'cash' to your account so that you can continue trading!")
            return apology("Insufficient Funds")

        else:
            db.execute("UPDATE users SET cash = ? WHERE id = ?", cash - total_price, user_id,)

        # cash_transaction db assigning
        description = "Bought x"+str(shares)+" "+results["name"]+" @ "+usd(price)
        db.execute(
            "INSERT INTO cash_transactions (user_id, date, description, amount, transaction_type) VALUES (?, CURRENT_TIMESTAMP, ?, ?, ?)",
            user_id, description, total_price, "withdrawal"
        )

        # stock_transaction db assigning first
        stock_id = db.execute("SELECT id FROM stocks WHERE symbol = ?", symbol)[0]["id"]
        db.execute(
            "INSERT INTO stock_transactions (user_id, stock_id, date, number_of_stocks, amount_per_stock, transaction_type) VALUES (?, ?, CURRENT_TIMESTAMP, ?, ?, ?)",
            user_id, stock_id, shares, price, "buy"
        )

        # shares db assigning
        row = db.execute("SELECT * FROM shares WHERE user_id = ? AND stock_id = ?", user_id, stock_id)
        if not row:
            db.execute("INSERT INTO shares (user_id, stock_id, number_of_stocks) VALUES (?, ?, ?)", user_id, stock_id, shares)

        else:
            db.execute(
                "UPDATE shares SET number_of_stocks = ? WHERE user_id = ? AND stock_id = ?",
                shares + row[number_of_stocks], user_id, stock_id
            )

        flash("Shares Bought!")
        return redirect("/")

    else:
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
            return redirect("/quote")

        results = lookup(symbol)
        if not results:
            flash("Enter valid Symbol")
            return redirect("/quote")

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
            return apology("must provide username", 403)

        password = request.form.get("password")
        if not password:
            return apology("must provide password", 403)

        confirmation = request.form.get("confirmation")
        if not confirmation:
            return apology("must confirm password", 403)
        elif password != confirmation:
            return apology("passwords must match", 403)

        try:
            db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, generate_password_hash(password))
        except ValueError:
            return apology("username already exists", 403)

        user_id = db.execute("SELECT id FROM users WHERE username = ?", username)[0]["id"]
        db.execute("INSERT INTO cash_transactions (user_id, date, description, amount, transaction_type) VALUES (?, CURRENT_TIMESTAMP, ?, ?, ?)", user_id, "Acc. Registration Bonus", 10000.00, "deposit")

        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # Complete the implementation of sell in such a way that it enables a user to sell shares of a stock (that he or she owns).

    # Require that a user input a stock’s symbol, implemented as a select menu whose name is symbol.
        # Render an apology if the user fails to select a stock or if (somehow, once submitted) the user does not own any shares of that stock.

    # Require that a user input a number of shares, implemented as a text field whose name is shares.
        # Render an apology if the input is not a positive integer or if the user does not own that many shares of the stock.

    # Submit the user’s input via POST to /sell.

    # Upon completion, redirect the user to the home page.

    # You don’t need to worry about race conditions (or use transactions).

    return apology("TODO")
