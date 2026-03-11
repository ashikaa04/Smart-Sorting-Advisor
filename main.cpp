#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>

using namespace std;

// Utility: clear terminal screen
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Utility: pause
void wait() {
    cout << "\nPress ENTER to continue...";
    cin.ignore();
    cin.get();
}

// Header display
void printHeader() {
    clearScreen();
    cout << "===========================================\n";
    cout << "     🚀 SMART SORTING ADVISOR SYSTEM\n";
    cout << "===========================================\n";
}

// Check if file exists (no filesystem required)
bool fileExists(const string &filename) {
    ifstream file(filename);
    return file.good();
}

// Option 1: Generate dataset
void generateDataset() {
    printHeader();
    cout << "⚙️  Generating dataset using C++...\n";
    system("datagenerator.exe");
    wait();
}

// Option 2: Label dataset
void labelDataset() {
    printHeader();
    cout << "🏷️  Labeling dataset (this may take several minutes)...\n";
    system("labelling.exe");
    wait();
}

// Option 3: Train ML model
void trainModel() {
    printHeader();
    cout << "🧠 Training ML model using Python...\n";
    system("python trainmodel.py");
    wait();
}

// Option 4: Predict algorithm
void predictAlgorithm() {
    printHeader();
    if (!fileExists("sorting_model.pkl")) {
        cout << "❌ Model not found! Please train the model first.\n";
    } else {
        cout << "🔍 Predicting best sorting algorithm using trained model...\n";
        system("python smartsortpredictor.py");
    }
    wait();
}

// Option 5: Generate & test random array
void generateAndTest() {
    printHeader();
    int n;
    cout << "Enter number of elements for random test array: ";
    cin >> n;

    ofstream testFile("test_array.csv");
    srand(time(0));
    for (int i = 0; i < n; i++) {
        testFile << (rand() % 10000);
        if (i != n - 1) testFile << ",";
    }
    testFile.close();

    cout << "\n✅ Random test array of size " << n << " saved to test_array.csv.\n";
    cout << "🔎 Now predicting best algorithm using ML model...\n";
    this_thread::sleep_for(chrono::milliseconds(500));

    if (fileExists("sorting_model.pkl")) {
        system("python smartsortpredictor.py");
    } else {
        cout << "\n❌ Error: Model file not found. Please train it first.\n";
    }
    wait();
}

int main() {
    int choice;
    while (true) {
        printHeader();
        cout << "1️⃣  Generate Dataset (C++)\n";
        cout << "2️⃣  Label Dataset (C++)\n";
        cout << "3️⃣  Train ML Model (Python)\n";
        cout << "4️⃣  Predict Best Algorithm (Python)\n";
        cout << "5️⃣  Generate & Test Random Array\n";
        cout << "6️⃣  Exit\n";
        cout << "-------------------------------------------\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: generateDataset(); break;
            case 2: labelDataset(); break;
            case 3: trainModel(); break;
            case 4: predictAlgorithm(); break;
            case 5: generateAndTest(); break;
            case 6:
                cout << "\n👋 Exiting Smart Sorting Advisor. Goodbye!\n";
                return 0;
            default:
                cout << "\n❌ Invalid choice. Please try again.\n";
                wait();
        }
    }
}
