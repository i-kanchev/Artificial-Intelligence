import csv
import numpy
import random

def numberfy_data(data):
    for entry in data:
        for i in range(4):
            entry[i] = float(entry[i])

def split_data(data, train_set, test_set):
    setosa_data = []
    versicolor_data = []
    virginica_data = []

    for entry in data:
        if entry[4] == 'Iris-setosa':
            setosa_data.append(entry)
        elif entry[4] == 'Iris-versicolor':
            versicolor_data.append(entry)
        else:
            virginica_data.append(entry)
    
    random.shuffle(setosa_data)
    random.shuffle(versicolor_data)
    random.shuffle(virginica_data)

    split_index_setosa = int(len(setosa_data) * 0.8)
    split_index_versicolor = int(len(versicolor_data) * 0.8)
    split_index_virginica = int(len(versicolor_data) * 0.8)

    train_set.extend(setosa_data[:split_index_setosa])
    train_set.extend(versicolor_data[:split_index_versicolor])
    train_set.extend(virginica_data[:split_index_virginica])

    test_set.extend(setosa_data[split_index_setosa:])
    test_set.extend(versicolor_data[split_index_versicolor:])
    test_set.extend(virginica_data[split_index_virginica:])

    random.shuffle(train_set)

def knn(k, entry, data):
    dist = []

    for curr in data:
        dist_curr = (entry[0] - curr[0]) ** 2 + (entry[1] - curr[1]) ** 2 + (entry[2] - curr[2]) ** 2 + (entry[3] - curr[3]) ** 2
        dist.append([dist_curr, curr[4]])

    dist = sorted(dist)

    setosa_value = 0
    versicolor_value = 0
    virginica_value = 0

    for i in range(1, k + 1):
        if dist[i][1] == 'Iris-setosa':
            setosa_value += dist[i][0]
        elif dist[i][1] == 'Iris-versicolor':
            versicolor_value += dist[i][0]
        else:
            virginica_value += dist[i][0]

    min_value = float('inf')
    if setosa_value != 0 and setosa_value < min_value:
        min_value = setosa_value
    if versicolor_value != 0 and versicolor_value < min_value:
        min_value = versicolor_value
    if virginica_value != 0 and virginica_value < min_value:
        min_value = virginica_value

    if setosa_value != 0 and setosa_value == min_value:
        return 'Iris-setosa'
    elif versicolor_value != 0 and versicolor_value == min_value:
        return 'Iris-versicolor'
    else:
        return 'Iris-virginica'

def check_set(k, set):
    correct_count = 0

    for entry in set:
        if entry[4] == knn(k, entry, set):
            correct_count += 1

    return correct_count

def ten_fold_cross_validation(k ,train_set):
    print("10-Fold Cross-Validation Results:")

    folds = []

    for i in range(0, 10):
        bound = int(len(train_set) * 0.1)

        fold = train_set[:bound]

        # remove the data for this fold from the train set
        del train_set[:bound]

        # calculate accuracy
        accuracy = check_set(k, fold) / len(fold)
        folds.append(accuracy)
        print("Accuracy Fold", i + 1,": ", round(accuracy * 100, 2), "%")

        # return the data for this fold
        train_set.extend(fold)

    print()
    print("Average Accuracy: ", round(numpy.mean(folds) * 100, 2), "%")
    print("Standard Deviation: ", round(numpy.std(folds) * 100, 2), "%")
    print()

def main():
    K = 5
    data = []
    with open('iris/iris.data', 'r') as file:
        csv_reader = csv.reader(file)
        for row in csv_reader:
            data.append(row)

    numberfy_data(data)

    train_set = []
    test_set = []

    split_data(data, train_set, test_set)

    print("Train Set Accuracy:")
    print("Accuracy: ", round(check_set(K, train_set) / len(train_set) * 100, 2), "%")
    print()

    ten_fold_cross_validation(K, train_set)

    print("Test Set Accuracy:")
    print("Accuracy: ", round(check_set(K, test_set) / len(test_set) * 100, 2), "%")

if __name__ == "__main__":
    main()