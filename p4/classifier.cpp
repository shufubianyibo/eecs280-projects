#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <cmath>
#include <limits>
#include "csvstream.hpp"
using namespace std;

class Trainer {
public:
Trainer();
~Trainer();
void train(csvstream &train_stream);
void only_train(csvstream &train_stream);
void test(csvstream &test_stream);
void train_with_test(csvstream &train_stream);

private:
int total_post();
int unique_words();
int num_post_with_word(const std::string &word);
int num_post_with_label(const std::string &label);
int num_label_with_word(const std::string &label, 
    const std::string &word);
double log_proir_probability(const std::string &label);
double log_likelihood(const std::string &label, 
    const std::string &word);
std::set<std::string> unique_words_from_text(const std::string &str);
double calculate_log_probability(const std::set<std::string> &words, 
                                     const std::string &label);
std::pair<std::string, double> predict(const std::set<std::string> &words);
void print_training_data();


int total_posts_ = 0;
std::set<std::string> vocabulary_;
std::set<std::string> labels_;
std::map<std::string, int> posts_per_label_;
std::map<std::string, int> posts_per_word_;
std::map<std::string, std::map<std::string, int>> posts_with_word_per_label_;
std::vector<std::map<std::string, std::string>> all_rows_;
};

Trainer::Trainer() {
}

Trainer::~Trainer() {
}

// Returns stored total
int Trainer::total_post() {
    return total_posts_;
}

// Returns size of stored vocabulary set
int Trainer::unique_words() {
    return vocabulary_.size();
}

// Finds the word in the stored map
int Trainer::num_post_with_word(const std::string &word) {
    auto it = posts_per_word_.find(word);
    if (it != posts_per_word_.end()) {
        return it->second;
    }
    return 0; // Word was not found
}

// Finds the label in the stored map
int Trainer::num_post_with_label(const std::string &label) {
    auto it = posts_per_label_.find(label);
    if (it != posts_per_label_.end()) {
        return it->second;
    }
    return 0; // Label was not found
}

// Finds the label, then the word, in the nested map
int Trainer::num_label_with_word(const std::string &label, const std::string &word) {
    auto label_it = posts_with_word_per_label_.find(label);
    if (label_it == posts_with_word_per_label_.end()) {
        return 0; // Label not found
    }
    
    auto word_it = label_it->second.find(word);
    if (word_it == label_it->second.end()) {
        return 0; // Word not found for this label
    }
    
    return word_it->second;
}

// Calculates using the new helper functions
double Trainer::log_proir_probability(const std::string &label) {
    int c = num_post_with_label(label);
    int total = total_post();
    return log(static_cast<double>(c) / static_cast<double>(total));
}

// Calculates using the new helper functions
double Trainer::log_likelihood(const std::string &label, const std::string &word) {
    int w = num_post_with_word(word);
    int w_in_c = num_label_with_word(label, word);
    int c = num_post_with_label(label);
    int total = total_post();
    if (total == 0 || c == 0) {
        return 0.0;
    } else if (w == 0) {
        return log(1.0 / static_cast<double>(total));
    } else if (w_in_c == 0) {
        return log(static_cast<double>(w) / static_cast<double>(total));
    } else {
        return log(static_cast<double>(w_in_c) / static_cast<double>(c));
    }
}

// Tokenize the content of a post
std::set<std::string> Trainer::unique_words_from_text(const std::string &str) {
    std::istringstream source(str);
    std::set<std::string> words;
    std::string word;
    while (source >> word) {
        words.insert(word);
    }
    return words;
}

// Calculates the log-probability score for one post and one label
double Trainer::calculate_log_probability(const std::set<std::string> &words,
                                          const std::string &label) {
    
    double score = log_proir_probability(label);

    for (const auto &word : words) {
        score += log_likelihood(label, word);
    }
    
    return score;
}

// Finds the label with the highest score for a given post
std::pair<std::string, double> Trainer::predict(const std::set<std::string> &words) {
    
    double best_score = -std::numeric_limits<double>::infinity();
    std::string best_label = "";

    for (const auto &label : labels_) {
        double current_score = calculate_log_probability(words, label);
        
        if (current_score > best_score) {
            best_score = current_score;
            best_label = label;
        } else if (current_score == best_score) {
            if (label < best_label) {
                best_label = label;
            }
        }
    }
    
    return {best_label, best_score};
}

void Trainer::print_training_data() {
    cout << "training data:" << endl;
    
    for (const auto &r : all_rows_) {
        cout << "  label = " << r.at("tag") 
             << ", content = " << r.at("content") << endl;
    }
}

void Trainer::train(csvstream &train_stream) {
    std::map<std::string, std::string> row;

    // --- 1. Data Ingestion Pass ---
    while (train_stream >> row) {
        all_rows_.push_back(row); // Store row for printing
        
        total_posts_++;
        
        std::string label = row["tag"];
        std::string text = row["content"];

        // Add label to set and increment its count
        labels_.insert(label);
        posts_per_label_[label]++;

        // Tokenize the text into a set of unique words for this post
        std::istringstream iss(text);
        std::string word;
        std::set<std::string> words_in_post;
        while (iss >> word) {
            words_in_post.insert(word);
        }

        // Update counts for each unique word in the post
        for (const auto& w : words_in_post) {
            vocabulary_.insert(w);
            posts_per_word_[w]++;
            posts_with_word_per_label_[label][w]++;
        }
    }

}

void Trainer::train_with_test(csvstream &train_stream){
    train(train_stream);
    cout << "trained on " << total_post() << " examples" << endl;
}

void Trainer::only_train(csvstream &train_stream) {
    train(train_stream);

    print_training_data();

    cout << "trained on " << total_post() << " examples" << endl;

    cout << "vocabulary size = " << unique_words() << endl;

    cout << endl << "classes:" << endl;
    for (const auto &label : labels_) {
        int count = num_post_with_label(label);
        double prior = log_proir_probability(label);
        cout << "  " << label << ", " << count << " examples" <<
            ", log-prior = " << prior << endl;
    }

    cout << "classifier parameters:" << endl;
    for (const auto &label : labels_) {
        for (const auto &word : vocabulary_) {
            int count = num_label_with_word(label, word);
            if (count > 0) {
                double ll = log_likelihood(label, word);
                cout << "  " << label << ":" << word << ", count = " << count <<
                    ", log-likelihood = " << ll << endl;
            }
        }
    }
}

void Trainer::test(csvstream &test_stream) {
    cout << "\ntest data:" << endl; 

    int correct_count = 0;
    int total_count = 0;
    std::map<std::string, std::string> row;

    while (test_stream >> row) {
        total_count++;
        
        std::string correct_label = row["tag"]; 
        std::string content = row["content"];      
        
        // 1. Tokenize the post's content
        std::set<std::string> words = unique_words_from_text(content);
        
        // 2. Get the prediction
        std::pair<std::string, double> prediction = predict(words);
        std::string predicted_label = prediction.first;
        double log_prob_score = prediction.second;

        // 3. Check for correctness
        if (predicted_label == correct_label) {
            correct_count++;
        }
        
        // 4. Print the formatted output for this post 
        cout << "  correct = " << correct_label 
             << ", predicted = " << predicted_label
             << ", log-probability score = " << log_prob_score << endl;
        cout << "  content = " << content << endl;
        cout << endl; 
    }

    // Print the final performance summary 
    cout << "performance: " << correct_count << " / " << total_count
         << " posts predicted correctly" << endl;
}

int main(int argc, char *argv[]) {
    cout.precision(3);
    
    if (argc != 2 && argc != 3) {
        cout << "Usage: classifier.exe TRAIN_FILE [TEST_FILE]" << endl;
        return 1;
    }

    Trainer my_classifier;

    if (argc == 2) {
        try {
        csvstream train_stream(argv[1]);
        my_classifier.only_train(train_stream);
        } catch (const csvstream_exception &e) {
            cout << "Error opening file: " << argv[1] << endl;
            return 1;
        }
    }
    


    if (argc == 3) {
        try {
            csvstream train_stream(argv[1]);
            my_classifier.train_with_test(train_stream);
            csvstream test_stream(argv[2]);
            my_classifier.test(test_stream);

        } catch (const csvstream_exception &e) {
            cout << "Error opening file: " << argv[2] << endl;
            return 1;
        }
    }
    return 0;
  
    // cout << endl;
    // cout << "Hello World!\n";
}
