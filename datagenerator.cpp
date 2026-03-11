#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <unordered_set>
#include <sstream>
using namespace std;

// Convert array to a unique string for hashing (used to check duplicates)
string arrayToString(const vector<int>& arr) {
    stringstream ss;
    for (size_t i = 0; i < arr.size(); ++i) {
        ss << arr[i];
        if (i != arr.size() - 1)
            ss << ",";
    }
    return ss.str();
}

// Generate a random array
vector<int> generateRandomArray(int size, int minValue, int maxValue) {
    vector<int> arr(size);
    for (int i = 0; i < size; ++i) {
        arr[i] = minValue + rand() % (maxValue - minValue + 1);
    }
    return arr;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    // Configuration
    int minSize = 5;
    int maxSize = 1000;
    int minValue = -1000;
    int maxValue = 1000;
    long long maxArrays = 100000; // upper limit (1 million, adjust as needed)

    unordered_set<string> uniqueArrays;
    uniqueArrays.reserve(maxArrays * 2);
    
    ofstream outFile("arrays_dataset.csv");
    if (!outFile.is_open()) {
        cerr << "❌ Error: Could not open file for writing!" << endl;
        return 1;
    }

    cout << "⚙️  Generating unique arrays..." << endl;
    long long count = 0;

    // Keep generating until we reach the maximum or can't find new ones
    while (count < maxArrays) {
        int size = minSize + rand() % (maxSize - minSize + 1);
        vector<int> arr = generateRandomArray(size, minValue, maxValue);
        string arrStr = arrayToString(arr);

        // Only write if array is unique
        if (uniqueArrays.insert(arrStr).second) {
            outFile << arrStr << "\n";
            count++;

            // Print progress every 10,000 arrays
            if (count % 10000 == 0)
                cout << "✅ " << count << " unique arrays generated..." << endl;
        }

        // Safety break to prevent infinite loop if space exhausted
        if (uniqueArrays.size() >= maxArrays)
            break;
    }

    outFile.close();
    cout << "\n🎉 Done! " << count << " unique arrays saved to arrays_dataset.csv" << endl;
    return 0;
}