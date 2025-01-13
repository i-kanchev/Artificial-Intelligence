import matplotlib.pyplot as plt
import numpy
import random

class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

def init_centroids(K, points):
    centroids = {}

    max_x = max(points, key=lambda p: p.x).x
    min_x = min(points, key=lambda p: p.x).x
    max_y = max(points, key=lambda p: p.y).y
    min_y = min(points, key=lambda p: p.y).y

    # for _ in range(K):
    #     rnd_x = random.uniform(min_x, max_x)
    #     rnd_y = random.uniform(min_y, max_y)
    #     c = Point(rnd_x, rnd_y)
    #     centroids[c] = []

    rnd_x = random.uniform(min_x, max_x)
    rnd_y = random.uniform(min_y, max_y)
    c = Point(rnd_x, rnd_y)
    centroids[c] = []

    last_centroid = c
    removed_points = []

    for _ in range(K - 1):
        farthest_point = max(points, key=lambda p: (p.x - last_centroid.x) ** 2 + (p.y - last_centroid.y) ** 2)
        removed_points.append(farthest_point)
        points.remove(farthest_point)

        centr = Point(farthest_point.x, farthest_point.y)
        centroids[centr] = []
        last_centroid = centr

    points.extend(removed_points)
    return centroids

def recenter(centroids):
    centroid_points = list(centroids.keys())

    for centroid_point in centroid_points:
        x_values = [point.x for point in centroids[centroid_point]]
        y_values = [point.y for point in centroids[centroid_point]]

        if len(x_values) == 0:
            centroids.pop(centroid_point)
            continue

        center_x = numpy.mean(x_values)
        center_y = numpy.mean(y_values)

        c = Point(center_x, center_y)
        centroids[c] = centroids.pop(centroid_point)

def clusterize(points, centroids):
    flag = True

    while(flag):
        centroid_points = []
        new_centroids = {}

        for centroid in centroids:
            centroid_points.append(centroid)
            new_centroids[centroid] = []

        for point in points:
            closest_centroid = None
            closest_distance = float('inf')

            for centroid in centroid_points:
                distance = (point.x - centroid.x) ** 2 + (point.y - centroid.y) ** 2

                if distance < closest_distance:
                    closest_distance = distance
                    closest_centroid = centroid
            
            new_centroids[closest_centroid].append(point)

        if new_centroids == centroids:
            flag = False
        else:
            centroids = new_centroids
        
        recenter(centroids)

    return centroids

def draw(centroids):   
    plt.figure(figsize=(8, 6))

    for centroid in centroids:
        random_color = "#{:02x}{:02x}{:02x}".format(random.randint(0, 255), random.randint(0, 255), random.randint(0, 255))
        
        for point in centroids[centroid]:
            plt.scatter(point.x, point.y, color=random_color)
        
        plt.scatter(centroid.x, centroid.y, marker='^', s=100, color='black')

    plt.title('kMeans')
    plt.show()

def main():
    K = 8
    # file_name = 'Datasets/normal/normal.txt'
    file_name = 'Datasets/unbalance/unbalance.txt'

    points = []
    with open(file_name, 'r') as file:
        for row in file:
            x, y = map(float, row.split())
            p = Point(x, y)
            points.append(p)

    centroids = init_centroids(K, points)

    centroids = clusterize(points, centroids)

    draw(centroids)

if __name__ == "__main__":
    main()