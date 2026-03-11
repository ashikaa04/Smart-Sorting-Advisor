import os
import subprocess
from flask import Flask, request, jsonify, send_from_directory
import joblib
import numpy as np
import pandas as pd
import time

app = Flask(__name__)

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
WEB_DIR = os.path.join(BASE_DIR, "webui")

# ---------------- Serve UI ----------------
@app.route('/')
def home():
    return send_from_directory(WEB_DIR, 'index.html')

@app.route('/<path:path>')
def serve_static(path):
    return send_from_directory(WEB_DIR, path)

# ---------------- Core Functionalities ----------------
@app.route('/generate', methods=['POST'])
def generate_dataset():
    exe_path = os.path.join(BASE_DIR, "datagenerator.exe")
    if not os.path.exists(exe_path):
        return jsonify({"error": "❌ datagenerator.exe not found!"}), 404

    start = time.time()
    subprocess.run([exe_path])
    duration = round(time.time() - start, 2)

    # Check if CSV exists
    dataset_path = os.path.join(BASE_DIR, "dataset.csv")
    if os.path.exists(dataset_path):
        rows = sum(1 for _ in open(dataset_path)) - 1
        return jsonify({"message": f"✅ Dataset generated successfully ({rows} arrays, {duration}s)."})
    else:
        return jsonify({"message": f"✅ Dataset generated successfully ({duration}s)."})


@app.route('/label', methods=['POST'])
def label_dataset():
    exe_path = os.path.join(BASE_DIR, "labelling.exe")
    if not os.path.exists(exe_path):
        return jsonify({"error": "❌ labelling.exe not found!"}), 404

    start = time.time()
    subprocess.run([exe_path])
    duration = round(time.time() - start, 2)

    labeled_path = os.path.join(BASE_DIR, "labeled_dataset.csv")
    if os.path.exists(labeled_path):
        rows = sum(1 for _ in open(labeled_path)) - 1
        return jsonify({"message": f"🏷️ Dataset labeled successfully ({rows} arrays, {duration}s)."})
    else:
        return jsonify({"message": f"🏷️ Dataset labeled successfully ({duration}s)."})


@app.route('/train', methods=['POST'])
def train_model():
    py_path = os.path.join(BASE_DIR, "trainmodel.py")
    start = time.time()
    subprocess.run(["python", py_path])
    duration = round(time.time() - start, 2)

    model_path = os.path.join(BASE_DIR, "sorting_model.pkl")
    if os.path.exists(model_path):
        return jsonify({"message": f"🧠 Model trained successfully in {duration}s!"})
    else:
        return jsonify({"error": "⚠️ Model training failed!"}), 500


@app.route('/generate_test', methods=['POST'])
def generate_test_array():
    data = request.get_json()
    size = int(data.get("size", 100))
    arr = np.random.randint(0, 1000, size)
    df = pd.DataFrame([arr])
    df.to_csv(os.path.join(BASE_DIR, "test_array.csv"), index=False)
    return jsonify({
        "message": f"📄 Random test array of size {size} saved!",
        "array": arr.tolist()
    })


@app.route('/manual_test', methods=['POST'])
def manual_test_array():
    data = request.get_json()
    user_input = data.get("array", "")
    try:
        arr = [int(x.strip()) for x in user_input.split(",") if x.strip()]
        if len(arr) == 0:
            return jsonify({"error": "⚠️ Array cannot be empty!"}), 400
        df = pd.DataFrame([arr])
        df.to_csv(os.path.join(BASE_DIR, "test_array.csv"), index=False)
        return jsonify({
            "message": f"📄 Manual array saved successfully ({len(arr)} elements)!",
            "array": arr
        })
    except Exception:
        return jsonify({"error": "⚠️ Invalid input format. Use comma-separated integers."}), 400


@app.route('/predict', methods=['POST'])
def predict_algorithm():
    model_path = os.path.join(BASE_DIR, "sorting_model.pkl")
    test_file = os.path.join(BASE_DIR, "test_array.csv")

    if not os.path.exists(model_path):
        return jsonify({"error": "❌ sorting_model.pkl not found!"}), 404
    if not os.path.exists(test_file):
        return jsonify({"error": "❌ test_array.csv not found!"}), 404

    try:
        model = joblib.load(model_path)
        df = pd.read_csv(test_file)

        features_full = [
            df.shape[1],
            float(df.mean(axis=1)),
            float(df.std(axis=1)),
            float(df.max(axis=1) - df.min(axis=1))
        ]

        expected = getattr(model, "n_features_in_", len(features_full))
        features = features_full[:expected]

        prediction = model.predict([features])[0]

        descriptions = {
            "BubbleSort": "Simple but slow — ideal for very small or nearly sorted arrays.",
            "InsertionSort": "Fast for small datasets and almost-sorted data.",
            "QuickSort": "Best all-round performer — efficient for large random data.",
            "ShellSort": "Good compromise between insertion and quick sort.",
            "CountingSort": "Excellent for integer data in a known range.",
            "RadixSort": "Fastest for large integer arrays with limited digits.",
            "SelectionSort": "Simple and easy to understand, but not efficient for big data."
        }

        complexities = {
            "BubbleSort": [1, "O(n²)"],
            "InsertionSort": [2, "O(n²)"],
            "SelectionSort": [2, "O(n²)"],
            "ShellSort": [3, "O(n log² n)"],
            "QuickSort": [5, "O(n log n)"],
            "CountingSort": [6, "O(n+k)"],
            "RadixSort": [6, "O(nk)"]
        }

        score, complexity = complexities.get(prediction, [4, "O(n log n)"])
        info = descriptions.get(prediction, "Efficient sorting algorithm detected.")

        return jsonify({
            "message": f"🔹 Predicted best algorithm: {prediction}",
            "algorithm": prediction,
            "description": info,
            "complexity": complexity,
            "score": score
        })

    except Exception as e:
        return jsonify({"error": f"⚠️ Prediction failed: {str(e)}"}), 500


if __name__ == '__main__':
    app.run(debug=True)
