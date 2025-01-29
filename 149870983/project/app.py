import os
import requests
from datetime import datetime
from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from werkzeug.security import check_password_hash, generate_password_hash

# Configure application
app = Flask(__name__)
app.secret_key = os.urandom(16)
db = SQL("sqlite:///project.db")

@app.route("/")
def index():
    return render_template("index.html")
@app.route("/home")
def home():
    return render_template("home.html")
@app.route("/profile", methods=["GET", "POST"])
def profile():
    if request.method == "POST":
      
        if request.form.get("action") == "register":
            username = request.form.get("username")
            email = request.form.get("email")
            password = request.form.get("password")
            if not username or not email or not password:
                flash("Must provide all fields", "danger")
                return redirect("/profile")
            existing_user = db.execute("SELECT * FROM users WHERE username = ?", username)
            if existing_user:
                flash("Username already exists", "danger")
                return redirect("/profile")
            hash = generate_password_hash(password)
            db.execute("INSERT INTO users (username, email, hash) VALUES (?, ?, ?)", username, email, hash)
            flash("Registered successfully! Please log in.", "success")
            return redirect("/profile")

        # Handle login
        elif request.form.get("action") == "login":
            username = request.form.get("username")
            password = request.form.get("password")
            if not username or not password:
                flash("Must provide all fields", "danger")
                return redirect("/profile")
            rows = db.execute("SELECT * FROM users WHERE username = ?", username)
            if len(rows) != 1 or not check_password_hash(rows[0]["hash"], password):
                flash("Invalid username or password", "danger")
                return redirect("/profile")
            session["user_id"] = rows[0]["id"]
            return redirect("/")
    else:
        return render_template("profile.html")
def fetch_current_conditions(zip_code):
    url = "http://www.airnowapi.org/aq/observation/zipCode/current"
    params = {
        'format': 'json',
        'zipCode': zip_code,
        'api_key': '215ED6FA-110A-4399-88FE-E806EE42AF17'
    }
    response = requests.get(url, params=params)
    if response.status_code == 200:
        data = response.json()
        return data
    else:
        raise Exception(f"Error fetching data: {response.status_code}")


current_conditions = fetch_current_conditions('90210')  # Example zip code
print(current_conditions)
@app.route('/calculator', methods=['GET', 'POST'])
def calculator():
    if request.method == 'POST':

        electricity = float(request.form['electricity'])
        fuel = float(request.form['fuel'])
        mileage = float(request.form['mileage'])
        fuel_type = request.form['fuel_type']
        waste = float(request.form['waste'])


        lbs_per_metric_ton = 2204.6


        electricity_emissions = (electricity * 919.1) / (1000 * lbs_per_metric_ton)


        if fuel_type == 'gasoline':
            fuel_emissions = (fuel * 19.6) / lbs_per_metric_ton
        else:
            fuel_emissions = (fuel * 22.4) / lbs_per_metric_ton


        if fuel_type == 'gasoline':
            transport_emissions = (mileage * 0.888) / lbs_per_metric_ton
        else:
            transport_emissions = (mileage * 1.021) / lbs_per_metric_ton


        waste_emissions = (waste * 0.94) / lbs_per_metric_ton


        total_emissions = electricity_emissions + fuel_emissions + transport_emissions + waste_emissions
        user_id = session["user_id"]

        db.execute("""
    INSERT INTO user_calculations (user_id, total_carbon_footprint, transportation, energy, food, calculation_date)
    VALUES (?)
""",( user_id, total_emissions, transport_emissions, electricity_emissions + fuel_emissions, waste_emissions, datetime.now().strftime("%Y-%m-%d")))


        return render_template('calculator.html', total_emissions=total_emissions)


    return render_template('calculator.html')
@app.route("/track")
def track():
    if "user_id" not in session:
        return redirect("/profile")

    try:

        calculations = db.execute("SELECT * FROM user_calculations WHERE user_id = ? ORDER BY calculation_date DESC", (session["user_id"],))


        labels = [calc['calculation_date'] for calc in calculations]  # Assuming the date is stored as 'calculation_date'
        emission_data = [calc['total_carbon_footprint'] for calc in calculations]  # Assuming 'total_carbon_footprint' stores the emissions value


        return render_template("track.html", labels=labels, emission_data=emission_data)
    except Exception as e:

        print(f"An error occurred: {e}")
        return "An internal error occurred. Please try again later.", 500

if __name__ == "__main__":
    app.run(debug=True)
@app.route("/insights", methods=["POST", "GET"])
def insights():
    if "user_id" not in session:
        return redirect("/profile")

    try:
        if request.method == "POST":
            selected_area = request.form.get("zip_code")
            if not selected_area:
                return "No area selected.", 400


            airnow_data = fetch_current_conditions(selected_area)
            if not airnow_data:
                raise Exception("No data found for the selected area")

            aqi = airnow_data[0]['AQI']
            air_quality_category = airnow_data[0]['Category']['Name']

            recommendations = []
            if aqi <= 50:
                recommendations = ["The air quality is good. Keep up the good practices!"]
            elif aqi <= 100:
                recommendations = ["The air quality is moderate. Consider reducing outdoor activities if you are sensitive."]
            elif aqi <= 150:
                recommendations = ["The air quality is unhealthy for sensitive groups. Reduce outdoor activities if you belong to this group."]
            elif aqi <= 200:
                recommendations = ["The air quality is unhealthy. Limit outdoor activities and use air purifiers."]
            elif aqi <= 300:
                recommendations = ["The air quality is very unhealthy. Avoid outdoor activities and stay indoors."]
            else:
                recommendations = ["The air quality is hazardous. Stay indoors and avoid all outdoor activities."]

            return render_template("insights.html", recommendations=recommendations, air_quality_category=air_quality_category, aqi=aqi)

        calculations = db.execute("SELECT * FROM user_calculations WHERE user_id = ? ORDER BY calculation_date DESC", (session["user_id"],))
        recent_calculation = calculations[0] if calculations else None
        total_emissions = recent_calculation['total_carbon_footprint'] if recent_calculation else 0

        return render_template("insights.html", recommendations=[], air_quality_category='', aqi=0, total_emissions=total_emissions)

    except Exception as e:
        print(f"An error occurred: {e}")
        return "An internal error occurred. Please try again later.", 500
