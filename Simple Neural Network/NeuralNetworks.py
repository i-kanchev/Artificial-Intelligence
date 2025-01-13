import numpy as np

def sigmoid(x):
    return 1.0 / (1.0 + np.exp(-x))

def sigmoid_derivative(x):
    return x * (1.0 - x)

def tanh(x):
    return np.tanh(x)

def tanh_derivative(x):
    return 1 - np.tanh(x)**2

def train(input, output, hidden_layers, neurons_count, activation_function_type):
    learning_rate = 0.1
    epochs = 10000

    layer_structure = [input.shape[1]] + [neurons_count] * hidden_layers + [output.shape[1]]

    weights = [np.random.rand(layer_structure[i], layer_structure[i+1]) for i in range(len(layer_structure) - 1)]
    biases = [np.random.rand(1, layer_structure[i+1]) for i in range(len(layer_structure) - 1)]

    for _ in range(epochs):
        # Forward propagation
        layer_input = [input]
        layer_output = []
        
        for i in range(len(weights)):
            input_to_layer = np.dot(layer_input[-1], weights[i]) + biases[i]
            if activation_function_type == 0:
                output_of_layer = sigmoid(input_to_layer)
            else:
                output_of_layer = tanh(input_to_layer)
            layer_input.append(output_of_layer)
            layer_output.append(output_of_layer)

        # Backpropagation
        error = output - layer_output[-1]
        if activation_function_type == 0:
            deltas = [error * sigmoid_derivative(layer_output[-1])]
        else:
            deltas = [error * tanh_derivative(layer_output[-1])]

        for i in range(len(weights) - 2, -1, -1):
            if activation_function_type == 0:
                delta = deltas[-1].dot(weights[i+1].T) * sigmoid_derivative(layer_output[i])
            else:
                delta = deltas[-1].dot(weights[i+1].T) * tanh_derivative(layer_output[i])
            deltas.append(delta)

        deltas.reverse()

        for i in range(len(weights)):
            weights[i] += layer_input[i].T.dot(deltas[i]) * learning_rate
            biases[i] += np.sum(deltas[i], axis=0, keepdims=True) * learning_rate

    return layer_output[-1]

def main():
    # AND, OR, XOR, ALL
    boolean_function = 'ALL'
    # 0 - sigmoid, 1 - hyper-geometric (tanh)
    activation_function_type = 0
    hidden_layers = 1
    neurons_count = 4

    network_input = np.array([[0, 0], [1, 0], [0, 1], [1, 1]])

    if boolean_function == 'AND' or boolean_function == 'ALL':
        network_output = np.array([[0], [0], [0], [1]])

        results = train(network_input, network_output, hidden_layers, neurons_count, activation_function_type)
        
        print('AND:')
        for i in range(0, 4):
            print(f"({network_input[i][0]},{network_input[i][1]}) -> {abs(round(results[i][0], 4))}")
        print()

    if boolean_function == 'OR' or boolean_function == 'ALL':
        network_output = np.array([[0], [1], [1], [1]])

        results = train(network_input, network_output, hidden_layers, neurons_count, activation_function_type)
        
        print('OR:')
        for i in range(0, 4):
            print(f"({network_input[i][0]},{network_input[i][1]}) -> {abs(round(results[i][0], 4))}")
        print()

    if boolean_function == 'XOR' or boolean_function == 'ALL':
        network_output = np.array([[0], [1], [1], [0]])

        results = train(network_input, network_output, hidden_layers, neurons_count, activation_function_type)
        
        print('XOR:')
        for i in range(0, 4):
            print(f"({network_input[i][0]},{network_input[i][1]}) -> {abs(round(results[i][0], 4))}")
        print()

if __name__ == '__main__':
    main()