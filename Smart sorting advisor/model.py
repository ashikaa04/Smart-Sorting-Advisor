import pandas as pd
from sklearn.tree import DecisionTreeClassifier, export_text

# 1. Load training data from CSV
data = pd.read_csv("training_data.csv")

# 2. Prepare features and labels
X = data[["size", "sortedness", "range"]]
y = data["label"]

# 3. Train a Decision Tree
clf = DecisionTreeClassifier(max_depth=5, random_state=42)  # limit depth for simpler rules
clf.fit(X, y)

# 4. Show the learned decision rules in text form (for debugging)
print("Learned Decision Tree rules:")
print(export_text(clf, feature_names=list(X.columns)))

# 5. Export rules to C++ function
def export_to_cpp(clf, feature_names):
    tree_ = clf.tree_
    feature_name = [
        feature_names[i] if i != -2 else "undefined!"
        for i in tree_.feature
    ]
    
    def recurse(node, depth):
        indent = "    " * depth
        if tree_.feature[node] != -2:
            name = feature_name[node]
            threshold = tree_.threshold[node]
            return (f"{indent}if ({name} <= {threshold:.6f}) {{\n" +
                    recurse(tree_.children_left[node], depth + 1) +
                    f"{indent}}} else {{\n" +
                    recurse(tree_.children_right[node], depth + 1) +
                    f"{indent}}}\n")
        else:
            classes = clf.classes_
            class_id = tree_.value[node].argmax()
            return f"{indent}return \"{classes[class_id]}\";\n"
    
    return "std::string predictAlgorithm(double size, double sortedness, double range) {\n" + recurse(0, 1) + "}\n"

cpp_code = export_to_cpp(clf, ["size", "sortedness", "range"])

with open("decision_tree.h", "w") as f:
    f.write("#pragma once\n#include <string>\n\n")
    f.write(cpp_code)

print("\nC++ code exported to decision_tree.h")
