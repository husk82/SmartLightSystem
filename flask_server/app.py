from flask import Flask, request, render_template, jsonify
from datetime import datetime

app = Flask(__name__)

# Store last received lux value
latest_data = {
    "lux": None,
    "timestamp": None
}

@app.route("/")
def index():
    return render_template("index.html", data=latest_data)

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
        
        latest_data["lux"] = lux
        latest_data["timestamp"] = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        print(f"Received LUX: {lux}")
        return jsonify({"status": "success", "lux": lux})
    except Exception as e:
        return jsonify({"status": "error", "message": str(e)}), 500


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)