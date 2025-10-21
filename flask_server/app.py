from flask import Flask, request, render_template, jsonify
from datetime import datetime
import psycopg2

app = Flask(__name__)

# --- PostgreSQL connection ---
conn = psycopg2.connect(
    host="localhost",       # or your RPi IP
    database="lux_db",      # your database name
    user="your_user",       # your postgres user
    password="your_pass"    # postgres password
)

# --- Store last received lux value ---
latest_data = {
    "lux": None,
    "timestamp": None
}

@app.route("/")
def index():
    last_10_readings = []
    try:
        with conn.cursor() as cur:
            cur.execute("""
                SELECT lux_value, timestamp 
                FROM lux_readings 
                ORDER BY timestamp DESC 
                LIMIT 10
            """)
            rows = cur.fetchall()
        
        last_10_readings = [
            {"lux": row[0], "timestamp": row[1].strftime("%Y-%m-%d %H:%M:%S")} 
            for row in rows
        ]
    except Exception as e:
        print(f"Database query failed: {e}")
        # Optionally, you can add a placeholder row
        last_10_readings = [{"lux": "N/A", "timestamp": "N/A"}]

    return render_template("index.html", last_10_readings=last_10_readings)

@app.route("/about")
def about():
    return render_template("about.html")

@app.route("/api/lux", methods=["POST"])
def receive_lux():
    global latest_data
    try:
        data = request.get_json()
        lux = data.get("lux")
        if lux is None:
            return jsonify({"status": "error", "message": "Missing 'lux'"}), 400
        
        timestamp = datetime.now()
        latest_data["lux"] = lux
        latest_data["timestamp"] = timestamp.strftime("%Y-%m-%d %H:%M:%S")

        # Insert into database
        with conn.cursor() as cur:
            cur.execute(
                "INSERT INTO lux_readings (lux_value, timestamp) VALUES (%s, %s)",
                (lux, timestamp)
            )
            conn.commit()

        print(f"Received LUX: {lux}")
        return jsonify({"status": "success", "lux": lux})
    except Exception as e:
        return jsonify({"status": "error", "message": str(e)}), 500


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)