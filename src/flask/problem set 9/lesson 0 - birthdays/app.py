import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")

#Maximum days for Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov & Dec
max_days_in_month = [31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        name = request.form.get("name")
        if not name:
            return redirect("/")

        month = request.form.get("month")
        if not month:
            return redirect("/")
        try:
            month = int(month)
        except ValueError:
            return redirect("/")
        if month < 1 or month > 12:
            return redirect("/")

        day = request.form.get("day")
        if not day:
            return redirect("/")
        try:
            day = int(day)
        except ValueError:
            return redirect("/")
        if day < 1 or day > max_days_in_month[month]:
            return redirect("/")

        # Insert data into database
        db.execute("INSERT INTO birthdays (name, month, day) VALUES(?, ?, ?)", name, month, day)

        return redirect("/")

    else:
        # Query for all birthdays
        birthdays = db.execute("SELECT * FROM birthdays")

        # Render birthdays page
        return render_template("index.html", birthdays=birthdays)


@app.route("/delete", methods=["GET", "POST"])
def delete():
    id = request.form.get("id")
    birthday = db.execute("SELECT * FROM birthdays WHERE id = ?", id)
    if birthday:
        birthday = birthday[0]  # Get the first result

    return render_template("delete.html", birthday=birthday)


@app.route("/delete_confirmed", methods=["POST"])
def delete_confirmed():
    id = request.form.get("id")
    if id:
        db.execute("DELETE FROM birthdays WHERE id = ?", id)

    return redirect("/")


@app.route("/edit", methods=["POST"])
def edit():
    id = request.form.get("id")
    birthday = db.execute("SELECT * FROM birthdays WHERE id = ?", id)
    if birthday:
        birthday = birthday[0]  # Get the first result

    return render_template("edit.html", birthday=birthday)


@app.route("/edit_confirmed", methods=["GET", "POST"])
def edit_confirmed():
    id = request.form.get("id")
    id = int(id)

    name = request.form.get("name")
    if not name:
        return redirect("/")

    month = request.form.get("month")
    if not month:
        return redirect("/")
    try:
        month = int(month)
    except ValueError:
        return redirect("/")
    if month < 1 or month > 12:
        return redirect("/")

    day = request.form.get("day")
    if not day:
        return redirect("/")
    try:
        day = int(day)
    except ValueError:
        return redirect("/")
    if day < 1 or day > max_days_in_month[month]:
        return redirect("/")

    db.execute("UPDATE birthdays SET name = ?, month = ?, day = ? WHERE id = ?", name, month, day, id)

    return redirect("/")
