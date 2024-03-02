#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <cstring>
#include "svm.h" // Include libsvm header

// Function to load data from a file into vectors
void load_data(const char* filename, std::vector<std::vector<double>>& features, std::vector<double>& labels) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        double label;
        iss >> label; // Assuming the label is in the first column
        labels.push_back(label);

        std::vector<double> feature_vector;
        double feature;
        while (iss >> feature) {
            feature_vector.push_back(feature);
        }
        features.push_back(feature_vector);
    }
    file.close();
}

int main() {
    std::vector<std::vector<double>> features_train, features_test;
    std::vector<double> labels_train, labels_test;

    // Load training data
    load_data("train_data.txt", features_train, labels_train);

    // Load test data
    load_data("test_data.txt", features_test, labels_test);

    // Define SVM parameters
    struct svm_parameter param;
    param.svm_type = C_SVC;
    param.kernel_type = RBF;
    param.gamma = 0.5;
    param.C = 10;
    param.probability = 1;

    // Train SVM model
    struct svm_problem prob;
    prob.l = labels_train.size();
    prob.y = labels_train.data();
    prob.x = new struct svm_node*[prob.l];
    for (int i = 0; i < prob.l; i++) {
        std::vector<double>& feature_vector = features_train[i];
        prob.x[i] = new struct svm_node[feature_vector.size() + 1];
        for (size_t j = 0; j < feature_vector.size(); j++) {
            prob.x[i][j].index = j + 1;
            prob.x[i][j].value = feature_vector[j];
        }
        prob.x[i][feature_vector.size()].index = -1;
    }

    struct svm_model* model = svm_train(&prob, &param);

    // Test SVM model
    int correct = 0;
    for (size_t i = 0; i < features_test.size(); i++) {
        std::vector<double>& feature_vector = features_test[i];
        struct svm_node* x = new struct svm_node[feature_vector.size() + 1];
        for (size_t j = 0; j < feature_vector.size(); j++) {
            x[j].index = j + 1;
            x[j].value = feature_vector[j];
        }
        x[feature_vector.size()].index = -1;

        double predicted_label = svm_predict(model, x);
        if (predicted_label == labels_test[i]) {
            correct++;
        }

        delete[] x;
    }

    std::cout << "Accuracy: " << (double)correct / features_test.size() << std::endl;

    // Clean up
    svm_free_and_destroy_model(&model);
    for (int i = 0; i < prob.l; i++) {
        delete[] prob.x[i];
    }
    delete[] prob.x;

    return 0;
}

