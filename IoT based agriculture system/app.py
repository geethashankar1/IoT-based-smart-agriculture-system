from flask import Flask, request, jsonify, render_template

app = Flask(__name__)
data_store = {}

@app.route('/api/data', methods=['POST'])
def receive_data():
    global data_store
    data_store = request.json
    return jsonify({"status": "success", "received": data_store})

@app.route('/')
def dashboard():
    return render_template("dashboard.html", data=data_store)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
