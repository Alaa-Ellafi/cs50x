import os
import datetime
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
    user_id = session["user_id"]
    transaction = db.execute(
        "SELECT symbol, SUM(shares) AS shares, SUM(shares) * price AS total, price FROM transactions WHERE user_id = ? GROUP BY symbol ", user_id)
    cash_db = db.execute("SELECT cash FROM users WHERE id =?", user_id)
    cash = cash_db[0]["cash"]
    tot = cash
    for row in transaction:
        tot += row["total"]
    return render_template("index.html", database=transaction, cash=cash, tot=tot)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")
    else:
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Please Enter A Symbol", 400)
        stock = lookup(symbol)
        if stock == None:
            return apology("Symbol Does Not Exist", 400)
        shares = request.form.get("shares")
        try:
            new_shares = int(shares)
            if new_shares <= 0:
                return apology("Enter a positive number", 400)
        except ValueError:
            return apology("Enter a number", 400)
        price = float(stock["price"])
        total = new_shares * price
        user_id = session["user_id"]
        cash_db = db.execute("SELECT cash FROM users WHERE id =?", user_id)
        cash = cash_db[0]["cash"]
        if cash < total:
            return apology("Not Enough Cash", 400)
        cash = cash - total
        db.execute("UPDATE users SET cash =? WHERE id =?", cash, user_id)
        date = datetime.datetime.now()
        db.execute("INSERT INTO transactions (user_id, symbol, shares, price, date) VALUES (?, ?, ?, ?, ?)",
                   user_id, symbol, new_shares, price, date)
        flash("Bought!")
        return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    transaction_db = db.execute("SELECT * FROM transactions WHERE user_id=?", user_id)
    return render_template("history.html", transactions=transaction_db)

@app.route("/add_cash", methods=["GET", "POST"])
@login_required
def add_cash():

    if request.method == "GET":
        return render_template("add.html")
    else:
        new_cash = request.form.get("new_cash")
        if not new_cash:
            return apology("Give a number please")
        user_id = session["user_id"]
        cash = db.execute("SELECT cash FROM users WHERE id =?", user_id)
        user_cash = cash[0]["cash"]
        update_cash = user_cash + int(new_cash)
        db.execute("UPDATE users SET cash =? WHERE id =?", update_cash, user_id)
        return redirect("/")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 400)

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
    if request.method == "GET":
        return render_template("quote.html")
    else:
        symbol = request.form.get("symbol")
        print(f"Received symbol: {symbol}")
        if not symbol:
            return apology("Enter a symbol", 400)

        stock = lookup(symbol)
        print(f"Lookup result: {stock}")
        if stock == None:
            return apology("Symbol does not exist", 400)
        return render_template("quoted.html", price=stock["price"], symbol=stock["symbol"])


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")
    else:
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        if not username:
            return apology("Please enter a username", 400)
        if not password:
            return apology("Please enter a password", 400)
        if not confirmation:
            return apology("Please confirm your password", 400)
        if password != confirmation:
            return apology("No match ", 400)
        hash = generate_password_hash(password)
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        if len(rows) != 0:
            return apology("User already exists", 400)
        db.execute("INSERT INTO users (username,hash) VALUES (?,?)", username, hash)
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        session["user_id"] = rows[0]["id"]
        return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session["user_id"]
    symbols = db.execute("SELECT symbol FROM transactions WHERE user_id = ?", user_id)
    symbol_list = [row['symbol'] for row in symbols]
    if request.method == "GET":
        return render_template("sell.html", symbol_list=symbol_list)
    else:
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Enter a symbol", 400)
        if symbol not in symbol_list:
            return apology("stock not owned", 400)
        stock = lookup(symbol)
        if stock == None:
            return apology("stock does not exist")
        shares = request.form.get("shares")
        try:
            new_shares = int(shares)
            if new_shares < 0:
                return apology("Enter a positive number", 400)
        except ValueError:
            return apology("Enter a number", 400)
        nbr = db.execute(
            "SELECT SUM(shares) AS total_shares FROM transactions WHERE user_id =? AND symbol =? ", user_id, symbol)
        count = nbr[0]["total_shares"]
        if count < new_shares:
            return apology("Not enough shares")
        price = float(stock["price"])
        total = new_shares * price
        cash_db = db.execute("SELECT cash FROM users WHERE id =?", user_id)
        cash = cash_db[0]["cash"]
        cash = cash + total
        new_shares = (-1)*new_shares
        db.execute("UPDATE users SET cash =? WHERE id =?", cash, user_id)
        date = datetime.datetime.now()
        db.execute("INSERT INTO transactions (shares ,user_id,symbol,date,price) VALUES (?,?,?,?,?)",
                   new_shares, user_id, symbol, date, price)
        sum = db.execute(
            "SELECT SUM(shares) AS shares FROM transactions WHERE user_id=? AND symbol=?", user_id, symbol)
        if sum[0]["shares"] == 0:
            db.execute("DELETE FROM transactions WHERE user_id=? AND symbol=?", user_id, symbol)
        flash("Sold!")
        return redirect("/")
