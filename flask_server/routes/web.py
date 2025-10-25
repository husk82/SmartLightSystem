from flask import Blueprint, render_template
from database.db import fetch_last_readings

web_bp = Blueprint("web", __name__)

@web_bp.route("/")

def index():
    last_10_readings = fetch_last_readings()
    if not last_10_readings:
        last_10_readings = [{"lux": "N/A", timestamp:"N/A"}]
    return render_template("index.html", last_10_readings)

@web_bp.route("/about")
def about():
    return render_template("about.html")
 
