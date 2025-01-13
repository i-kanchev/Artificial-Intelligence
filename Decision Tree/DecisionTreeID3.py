import csv
import numpy
import random

index_names = ["type", "age", "menopause", "tumor-size", "inv-nodes",
                   "node-caps", "deg-malig", "breast", "breast-quad", "irradiat"]

class Node:
    def __init__(self, name, attribute, data):
        self.name = name
        self.attribute = attribute
        # self.data = copy.deepcopy(data)
        self.data = data
        self.is_leaf = False
        self.value = None
        self.parent = None
        self.children = []

class DecisionTreeID3:
    def __init__(self, data, N, K, G):
        self.max_depth = N
        self.min_entries = K
        self.min_info_gain = G
        self.root = Node('breast-cancer', 'all', data)
        self.build_nodes(self.root, [False] * 9, 1)

    def build_nodes(self, node : Node, used, depth):
        data = make_data_table(node.data, index_names)

        entropy = self.entropy(data)

        if entropy == 0:
            node.is_leaf = True
            node.value = next(iter(data['type']))
            return

        # N
        if depth == self.max_depth:
            return

        # K
        if len(node.data) < self.min_entries:
            return
        
        gains = []
        
        for i in range(1 , len(index_names)):
            gains.append(self.information_gain(node, data, i, entropy))

        # G
        if max(gains) < self.min_info_gain:
            return

        for i in range(1 , len(index_names)):
            if used[i - 1] == True or gains[i - 1] != max(gains):
                continue

            for attr in data[index_names[i]]:
                sub_data = []
                for entry in node.data:
                    if entry[i] == attr:
                        sub_data.append(entry)

                new_node = Node(index_names[i], attr, sub_data)
                node.children.append(new_node)
                new_node.parent = node
                used[i - 1] = True
                self.build_nodes(new_node, used, depth + 1)
                used[i - 1] = False

    def information_gain(self, node : Node, data, index, general_entropy):
        total_entropy = 0
        for attr in data[index_names[index]]:
            sub_data = []
            for entry in node.data:
                if entry[index] == attr:
                    sub_data.append(entry)
            total_entropy += (len(sub_data) / len(node.data)) * self.entropy(make_data_table(sub_data, index_names))
        return general_entropy - total_entropy

    def entropy(self, data):
        if 'recurrence-events' not in data['type'] or 'no-recurrence-events' not in data['type']:
            return 0
        
        num1 = data['type']['recurrence-events']
        num2 = data['type']['no-recurrence-events']
        probability1 = num1 / (num1 + num2)
        probability2 = num1 / (num1 + num2)
        return - (probability1 * numpy.log2(probability1)) - (probability2 * numpy.log2(probability2))

def split_data(data, train_set, test_set):
    recurrence_data = []
    no_recurrence_data = []

    for entry in data:
        if entry[0] == "recurrence-events":
            recurrence_data.append(entry)
        else:
            no_recurrence_data.append(entry)
    
    random.shuffle(recurrence_data)
    random.shuffle(no_recurrence_data)

    split_index_recurrence = int(len(recurrence_data) * 0.8)
    split_index_no_recurrence = int(len(no_recurrence_data) * 0.8)

    train_set.extend(recurrence_data[:split_index_recurrence])
    train_set.extend(no_recurrence_data[:split_index_no_recurrence])
    test_set.extend(recurrence_data[split_index_recurrence:])
    test_set.extend(no_recurrence_data[split_index_no_recurrence:])

    random.shuffle(train_set)

def make_data_table(set, index_names):
    table = {}

    for name in index_names:
        table[name] = {}

    for entry in set:
            index = 0
            for data in entry:
                name = index_names[index]
                if data != "?":
                    if data in table[name]:
                        table[name][data] += 1
                    else:
                        table[name][data] = 1
                index += 1
    
    return table

def traverse_tree(decision_tree : DecisionTreeID3, entry):
    node = decision_tree.root

    while node.is_leaf != True:
        if node.children == []:
            index = random.randint(0, len(node.data) - 1)
            return node.data[index][0]
        
        name = node.children[0].name
        next_node = None
        for child in node.children:
            attribute = child.attribute
            if attribute == entry[index_names.index(name)]:
                next_node = child
                break

        if next_node == None:
            return ''
        
        node = next_node
    
    return node.value

def check_set(set, decision_tree):
    correct_count = 0

    for entry in set:
        if entry[0] == traverse_tree(decision_tree, entry):
            correct_count += 1

    return correct_count

def ten_fold_cross_validation(train_set, N, K, G):
    print("10-Fold Cross-Validation Results:")

    folds = []

    for i in range(0, 10):
        bound = int(len(train_set) * 0.1)

        fold = train_set[:bound]

        # remove the data for this fold from the train set
        del train_set[:bound]

        # calculate accuracy
        decision_tree = DecisionTreeID3(train_set, N, K, G)
        reduced_error_pruning(decision_tree.root, decision_tree)
        accuracy = check_set(fold, decision_tree) / len(fold)
        folds.append(accuracy)
        print("Accuracy Fold", i + 1,": ", round(accuracy * 100, 2), "%")

        # return the data for this fold
        train_set.extend(fold)

    print()
    print("Average Accuracy: ", round(numpy.mean(folds) * 100, 2), "%")
    print("Standard Deviation: ", round(numpy.std(folds) * 100, 2), "%")
    print()

def reduced_error_pruning(node : Node, decision_tree):
    if node.is_leaf == True or node.children == []:
        return

    err_percent_parent = check_set(node.data, decision_tree) / len(node.data)
    err_percent_children = 0

    for child in node.children:
        err_percent_children += check_set(child.data, decision_tree) / len(child.data)
    err_percent_children /= len(node.children)

    if err_percent_children > err_percent_parent:
        node.children = []
    
    for child in node.children:
        reduced_error_pruning(child, decision_tree)

def main():
    data = []
    with open('breast+cancer/breast-cancer.data', 'r') as file:
        csv_reader = csv.reader(file)
        for row in csv_reader:
            data.append(row)

    train_set = []
    test_set = []

    split_data(data, train_set, test_set)

    # data for pre-pruning
    # N - max depth
    # K - min number of entries for a node
    # G - min information gain
    N = 7
    K = 10
    G = 0.05

    decision_tree = DecisionTreeID3(train_set, N, K, G)

    # post-pruning
    reduced_error_pruning(decision_tree.root, decision_tree)

    print("Train Set Accuracy:")
    print("Accuracy: ", round(check_set(train_set, decision_tree) / len(train_set) * 100, 2), "%")
    print()

    ten_fold_cross_validation(train_set, N, K, G)

    print("Test Set Accuracy:")
    print("Accuracy: ", round(check_set(test_set, decision_tree) / len(test_set) * 100, 2), "%")

if __name__ == "__main__":
    main()