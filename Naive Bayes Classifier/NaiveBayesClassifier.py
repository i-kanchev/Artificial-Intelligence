import csv
import math
import numpy
import random

def split_data(data, train_set, test_set):
    republicans_data = []
    democrats_data = []

    for entry in data:
        if entry[0] == "republican":
            republicans_data.append(entry)
        else:
            democrats_data.append(entry)
    
    random.shuffle(republicans_data)
    random.shuffle(democrats_data)

    split_index_republicans = int(len(republicans_data) * 0.8)
    split_index_democrats = int(len(democrats_data) * 0.8)

    train_set.extend(republicans_data[:split_index_republicans])
    train_set.extend(democrats_data[:split_index_democrats])
    test_set.extend(republicans_data[split_index_republicans:])
    test_set.extend(democrats_data[split_index_democrats:])

    random.shuffle(train_set)

def update_info_republicans(set, republicans):
    republicans_count = 0
    
    for entry in set:
        if entry[0] == "republican":
            republicans_count += 1
            index = 0
            for question in entry[1:]:
                if question != "?":
                    republicans[question][index] += 1
                index += 1

    return republicans_count

def update_info_democrats(set, democrats):
    democrats_count = 0
    
    for entry in set:
        if entry[0] == "democrat":
            democrats_count += 1
            index = 0
            for question in entry[1:]:
                if question != "?":
                    democrats[question][index] += 1
                index += 1

    return democrats_count

def beyes_classifier(entry, republicans, democrats, republicans_count, democrats_count):
    republican_possibility = math.log(republicans_count / (republicans_count + democrats_count))
    democrat_possibility = math.log(democrats_count / (republicans_count + democrats_count))

    # used to fix the problem of 0 probability
    lambda_val = 1
    values_count = 2

    for index in range(1, len(entry)):
        if entry[index] != "?":
            republican_possibility += math.log(republicans[entry[index]][index - 1] + lambda_val / republicans_count + values_count * lambda_val)
            democrat_possibility += math.log(democrats[entry[index]][index - 1] + lambda_val / democrats_count + values_count * lambda_val)
    
    return "republican" if republican_possibility > democrat_possibility else "democrat"

def check_set(set, republicans, democrats, republicans_count, democrats_count):
    correct_count = 0

    for entry in set:
        if entry[0] == beyes_classifier(entry, republicans, democrats, republicans_count, democrats_count):
            correct_count += 1

    return correct_count

def ten_fold_cross_validation(train_set, republicans, democrats, republicans_count, democrats_count):
    print("10-Fold Cross-Validation Results:")

    folds = []

    for i in range(0, 10):
        min_bound = int(len(train_set) * i * 0.1)
        max_bound = int(len(train_set) * (i + 1) * 0.1)

        fold = train_set[min_bound:max_bound]

        temp_republicans = {
            'y': [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
            'n': [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]}
        temp_democrats = {
            'y': [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
            'n': [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]}
        
        temp_republicans_count = update_info_republicans(fold, temp_republicans)
        temp_democrats_count = update_info_democrats(fold, temp_democrats)

        # remove the data for this fold from the train set
        for j in range(0, 16):
            republicans['y'][j] -= temp_republicans['y'][j]
            republicans['n'][j] -= temp_republicans['n'][j]
            democrats['y'][j] -= temp_democrats['y'][j]
            democrats['n'][j] -= temp_democrats['n'][j]

        republicans_count -= temp_republicans_count
        democrats_count -= temp_democrats_count

        # calculate accuracy
        accuracy = check_set(fold, republicans, democrats, republicans_count, democrats_count) / len(fold)
        folds.append(accuracy)
        print("Accuracy Fold", i + 1,": ", round(accuracy * 100, 2), "%")

        # return the data for this fold
        for j in range(0, 16):
            republicans['y'][j] += temp_republicans['y'][j]
            republicans['n'][j] += temp_republicans['n'][j]
            democrats['y'][j] += temp_democrats['y'][j]
            democrats['n'][j] += temp_democrats['n'][j]

        republicans_count += temp_republicans_count
        democrats_count += temp_democrats_count

    print()
    print("Average Accuracy: ", round(numpy.mean(folds) * 100, 2), "%")
    print("Standard Deviation: ", round(numpy.std(folds) * 100, 2), "%")
    print()

def main():
    data = []
    with open('congressional+voting+records/house-votes-84.data', 'r') as file:
        csv_reader = csv.reader(file)
        for row in csv_reader:
            data.append(row)

    train_set = []
    test_set = []

    split_data(data, train_set, test_set)

    republicans = {
        'y': [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        'n': [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]}
    democrats = {
        'y': [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        'n': [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]}

    republicans_count = update_info_republicans(train_set, republicans)
    democrats_count = update_info_democrats(train_set, democrats)

    print("Train Set Accuracy:")
    print("Accuracy: ", round(check_set(train_set, republicans, democrats, republicans_count, democrats_count) / len(train_set) * 100, 2), "%")
    print()

    ten_fold_cross_validation(train_set, republicans, democrats, republicans_count, democrats_count)

    print("Test Set Accuracy:")
    print("Accuracy: ", round(check_set(test_set, republicans, democrats, republicans_count, democrats_count) / len(test_set) * 100, 2), "%")

if __name__ == "__main__":
    main()