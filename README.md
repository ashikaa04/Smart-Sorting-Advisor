# Smart Sorting Advisor

An AI-powered system that predicts the best sorting algorithm for a given dataset using Machine Learning.

## Overview

This project combines C++, Python, and Machine Learning to analyze array characteristics and recommend the most efficient sorting algorithm.

The system automatically:

1. Generates random datasets
2. Benchmarks multiple sorting algorithms
3. Labels the fastest algorithm
4. Trains a machine learning model
5. Predicts the best sorting method for new arrays

## Technologies Used

- C++
- Python
- Flask
- Machine Learning (Random Forest)
- NumPy
- Pandas
- Scikit-learn

## Project Workflow

1. Dataset Generation (C++)
2. Labeling best algorithm (C++)
3. Train ML model (Python)
4. Predict best sorting algorithm (Python)

## Sorting Algorithms Evaluated

- Bubble Sort
- Insertion Sort
- Selection Sort
- Quick Sort
- Shell Sort
- Counting Sort
- Radix Sort
- Bucket Sort

## Machine Learning Model

Random Forest Classifier is used to predict the best sorting algorithm based on features like:

- Array size
- Variance
- Range
- Sortedness
- Unique element ratio

## How to Run

### Generate dataset
