import pandas as pd
import numpy as np
import joblib
import os

# === Helper: Check if an array is nearly sorted ===
def is_nearly_sorted(arr):
    n = len(arr)
    if n <= 1:
        return 1.0
    sorted_count = sum(1 for i in range(n - 1) if arr[i] <= arr[i + 1])
    return sorted_count / (n - 1)

# === Step 1: Check for the test file ===
if os.path.exists("test_array.csv"):
    print("🟢 Detected existing test_array.csv — using it for prediction.")
    data = pd.read_csv("test_array.csv", header=None)
    arr = data.values.flatten().tolist()
else:
    print("❌ test_array.csv not found!")
    exit()

# === Step 2: Extract features (MUST match trainmodel.py) ===
features = [
    np.mean(arr),
    np.std(arr),
    is_nearly_sorted(arr),
    len(arr)
]

# === Step 3: Load trained model ===
try:
    model = joblib.load("sorting_model.pkl")  # ✅ correct file name
    print("✅ ML model loaded successfully.\n")
except FileNotFoundError:
    print("❌ Error: sorting_model.pkl not found. Please train the model first.")
    exit()

# === Step 4: Predict best sorting algorithm ===
prediction = model.predict([features])[0]

print("🔹 SMART SORTING ADVISOR 🔹")
print(f"📈 For array of size {len(arr)}, predicted best algorithm: 🧠 {prediction}")
