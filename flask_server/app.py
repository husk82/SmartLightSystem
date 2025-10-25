from flask import Flask
from routes.api import api_bp
from routes.web import web_bp
from config import HOST, PORT, DEBUG

app = Flask(__name__)

app.register_blueprint(api_bp, url_prefix="/api")
app. register_blueprint(web_bp)

if __name__ == "__main__":
    app.run(host=HOST, port=PORT, debug=DEBUG)
