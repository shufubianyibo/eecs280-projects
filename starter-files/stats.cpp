#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>

//EFFECTS: returns the count (sample size) of the numbers in v
int count(std::vector<double> v){
    return v.size();
}

//REQUIRES: v is not empty
//EFFECTS: returns the sum of the numbers in v
double sum(std::vector<double> v){
    assert(!v.empty());
    double total = 0.0;
    for(double i : v){
        total += i;
    }
    return total;
}

//REQUIRES: v is not empty
//EFFECTS: returns the arithmetic mean of the numbers in v
//  http://en.wikipedia.org/wiki/Arithmetic_mean
double mean(std::vector<double> v){
    assert(!v.empty());
    return sum(v) / count(v);
}

//REQUIRES: v is not empty
//EFFECTS: returns the median of the numbers in v
//  https://en.wikipedia.org/wiki/Median#Finite_data_set_of_numbers
double median(std::vector<double> v){
    assert(!v.empty());
    std::sort(v.begin(), v.end());
    if (count(v)%2 == 1) {
        return v[count(v)/2];
    } else {
        return (v[count(v)/2 - 1] + v[count(v)/2]) / 2.0;
    }
}

//REQUIRES: v is not empty
//EFFECTS: returns the min number in v
double min(std::vector<double> v){
    assert(!v.empty());
    double minimum = v[0];
    for(double i : v){
        if(i < minimum){
            minimum = i;
        }
    }
    return minimum;
}

//REQUIRES: v is not empty
//EFFECTS: returns the max number in v
double max(std::vector<double> v){
    assert(!v.empty());
    double maximum = v[0];
    for(double i : v){
        if(i > maximum){
            maximum = i;
        }
    }
    return maximum;
}

//REQUIRES: v contains at least 2 elements
//EFFECTS: returns the corrected sample standard deviation of the numbers in v
//  http://en.wikipedia.org/wiki/Standard_deviation#Corrected_sample_standard_deviation
double stdev(std::vector<double> v){
    assert(count(v) >= 2);
    double m = mean(v);
    double sum_sq_diff = 0.0;
    for(double i : v){
        sum_sq_diff += (i - m) * (i - m);
    }
    return sqrt(sum_sq_diff / (count(v) - 1));
}

//REQUIRES: v is not empty
//          p is between 0 and 1, inclusive
//EFFECTS: returns the percentile p of the numbers in v like Microsoft Excel.
//  Refer to the project spec for the formula to use.
//  NOTE: the definition in the spec uses indexing from 1.  You will need to
//  adapt it to use indexing from 0.
double percentile(std::vector<double> v, double p){
    assert(!v.empty());
    assert(p >= 0.0 && p <= 1.0);   
    if (p == 1.0)
    {
       return max(v);
    } else if (p == 0.0)
    {
        return min(v);
    } else {
        std::sort(v.begin(), v.end());
        double n = count(v);
        double rank = p * (n - 1) + 1;
        double intpart = 0;
        double fractpart = 0;
        fractpart = modf(rank, &intpart);
        return v[intpart - 1] + fractpart * (v[intpart] - v[intpart - 1]);

    }
    
    
}

//REQUIRES: v and criteria have the same number of elements
//EFFECTS: returns a new, filtered version of v containing the elements (and
//         only those elements) at v[x] where criteria[x] is equal to target,
//         in the same order as they originally appear in v.
//         Note: For this function, compare values to the criteria using ==.
std::vector<double> filter(std::vector<double> v,
    std::vector<double> criteria,
    double target){
    assert(v.size() == criteria.size());
    std::vector<double> result;
        for (int i = 0; i < criteria.size(); i++)
        {
            if (criteria[i] == target)
            {
                result.push_back(v[i]);
            }
            
        }
        return result;

    }