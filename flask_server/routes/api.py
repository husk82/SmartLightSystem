from flask import Blueprint, request, jsonify
from datetime import datetime
from database.db import insert_lux

api_bp = Blueprint("api", __name__)

latest_data = {"lux": None, "timestamp": None}

@api_bp.route("/lux", methods=["POST"])

def receive_lux():
    try:
        data = request.get_json()
        lux = data.get("lux")
        if lux is None:
            return jsonify({"status": "Error", "message": "Missing 'lux'"}), 400
        timestamp = datetime.now()
        latest_data["lux"] = lux
        latest_data["timestamp"] = timestamp.strftime("%Y-%m-%d %H:%M:%S")

        if insert_lux(lux, timestamp):
            print(f"Received LUX: {lux}")
            return jsonify({"status": "success", "lux": lux})
        else:
            return jsonify({"status": "error", "message": "DB insert failed"}),500
    except Exception as e:
        return jsonify({"status": "error", "message": str(e)}), 500

    





