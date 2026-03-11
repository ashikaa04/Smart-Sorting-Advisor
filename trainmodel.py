import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score, confusion_matrix, classification_report
import joblib

# ----------------------------------------
# STEP 1 — Load the labeled dataset
# ----------------------------------------
df = pd.read_csv("labeled_arrays.csv")

# Helper: convert string "[1,2,3]" → [1,2,3]
def parse_array(arr_str):
    arr_str = arr_str.strip("[]\" ")  # remove brackets, quotes, spaces
    return list(map(int, arr_str.split(",")))

# ----------------------------------------
# STEP 2 — Feature Extraction
# ----------------------------------------
def extract_features(arr):
    size = len(arr)
    arr_range = max(arr) - min(arr)
    sortedness = sum(arr[i] <= arr[i+1] for i in range(len(arr)-1)) / (len(arr)-1)
    unique_ratio = len(set(arr)) / len(arr)
    variance = np.var(arr)
    mean = np.mean(arr)
    return [size, arr_range, sortedness, unique_ratio, variance, mean]

X, y = [], []
for _, row in df.iterrows():
    try:
        arr = parse_array(row["Array"])
        features = extract_features(arr)
        X.append(features)
        y.append(row["BestSort"])
    except Exception as e:
        continue

X = np.array(X)
y = np.array(y)

# ----------------------------------------
# STEP 3 — Train/Test Split
# ----------------------------------------
X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.2, random_state=42, stratify=y
)

# ----------------------------------------
# STEP 4 — Random Forest Model Training
# ----------------------------------------
model = RandomForestClassifier(
    n_estimators=300,
    max_depth=15,
    random_state=42,
    n_jobs=-1
)

model.fit(X_train, y_train)

# ----------------------------------------
# STEP 5 — Evaluation
# ----------------------------------------
y_pred = model.predict(X_test)

accuracy = accuracy_score(y_test, y_pred)
print(f"✅ Model Accuracy: {accuracy*100:.2f}%\n")

print("📊 Classification Report:")
print(classification_report(y_test, y_pred))

# ----------------------------------------
# STEP 6 — Confusion Matrix
# ----------------------------------------
print("Confusion Matrix:")
print(confusion_matrix(y_test, y_pred))

# ---------------------------------------
