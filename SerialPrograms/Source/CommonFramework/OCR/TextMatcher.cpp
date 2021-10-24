/*  Text Inference Tools
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include <cmath>
#include <vector>
#include "Common/Cpp/SpinLock.h"
#include "Common/Cpp/Exception.h"
#include "StringNormalization.h"
#include "TextMatcher.h"

#include <iostream>
using std::cout;
using std::endl;

namespace PokemonAutomation{
namespace OCR{

void print(const std::vector<size_t>& v){
    std::string str = "{";
    bool first = true;
    for (size_t x : v){
        if (!first){
            str += ", ";
        }
        first = false;
        str += std::to_string(x);
    }
    str += "}";
    cout << str << endl;
}


size_t levenshtein_distance(const QString& x, const QString& y){
    size_t xlen = x.size();
    size_t ylen = y.size();

    std::vector<size_t> v0(ylen + 1);
    std::vector<size_t> v1(ylen + 1);

    for (size_t i = 0; i <= ylen; i++){
        v0[i] = i;
    }

    for (size_t i = 0; i < xlen; i++){
        v1[0] = i + 1;

        for (size_t j = 0; j < ylen; j++){
            size_t delete_cost = v0[j + 1] + 1;
            size_t insert_cost = v1[j] + 1;
            size_t sub_cost = v0[j];
            if (x[(int)i] != y[(int)j]){
                sub_cost += 1;
            }
            v1[j + 1] = std::min({delete_cost, insert_cost, sub_cost});
        }

        std::swap(v0, v1);
    }

    return v0[ylen];
}
size_t levenshtein_distance_substring(const QString& substring, const QString& fullstring){
    size_t xlen = fullstring.size();
    size_t ylen = substring.size();

    std::vector<size_t> v0(ylen + 1);
    std::vector<size_t> v1(ylen + 1);

    for (size_t i = 0; i <= ylen; i++){
        v0[i] = i;
    }
//    print(v0);

    size_t min = ylen;

    for (size_t i = 0; i < xlen; i++){
        v1[0] = 0;

        for (size_t j = 0; j < ylen; j++){
            size_t delete_cost = v0[j + 1] + 1;
            size_t insert_cost = v1[j] + 1;
            size_t sub_cost = v0[j];
            if (fullstring[(int)i] != substring[(int)j]){
                sub_cost += 1;
            }
            v1[j + 1] = std::min({delete_cost, insert_cost, sub_cost});
        }

        std::swap(v0, v1);

        min = std::min(min, v0[ylen]);
//        print(v0);
    }

    return min;
}


std::map<size_t, std::vector<uint64_t>> binomial_table;
SpinLock binomial_lock;
std::vector<uint64_t> binomial_row_u64(size_t degree){
    std::vector<uint64_t> row;

    uint64_t a = (uint64_t)degree;
    uint64_t b = 1;
    {
        row.emplace_back(1);
    }
    {
        row.emplace_back(a);
        a--;
        b++;
    }
    while (a > b){
        row.emplace_back(row.back() * a / b);
        a--;
        b++;
    }

    return row;
}
uint64_t binomial_coefficient_u64(size_t degree, size_t index){
    if (degree > 62){
        PA_THROW_StringException("Cannot go beyond degree 62.");
    }

    SpinLockGuard lg(binomial_lock, "binomial_coefficient_u64()");

    auto iter = binomial_table.find(degree);
    std::vector<uint64_t>& row = iter != binomial_table.end()
        ? iter->second
        : binomial_table[degree] = binomial_row_u64(degree);

    if (index > degree / 2){
        index = degree - index;
    }

    return row[index];
}


double random_match_probability(size_t total, size_t matched, double random_match_chance){
    if (total > 62){
        PA_THROW_StringException("Cannot go beyond degree 62.");
    }

    double c_match = 1 - random_match_chance;

    double misses[62];
    {
        double miss = 1;
        misses[0] = miss;
        for (size_t c = 1; c <= total - matched; c++){
            miss *= c_match;
            misses[c] = miss;
        }
    }

    double hits = 1;

    size_t m = 0;
    for (; m < matched; m++){
        hits *= random_match_chance;
    }

    double probability = 0;
    for (; m < total; m++){
        double binomial = (double)binomial_coefficient_u64(total, m);
        probability += hits * binomial * misses[total - m];
        hits *= random_match_chance;
    }
    probability += hits;
    return probability;
}



void match_substring(
    StringMatchResult& results,
    const std::map<QString, std::set<std::string>>& database, double random_match_chance,
    const QString& text, double log10p_spread
){
//    result.original_text = text;
//    result.normalized_text = normalize(text);

    QString normalized = normalize(text);

    //  Search for exact match of candidate.
    auto iter = database.find(normalized);
    if (iter != database.end()){
        results.exact_match = true;
        double probability = random_match_probability(normalized.size(), normalized.size(), random_match_chance);
        double log10p = std::log10(probability);
        for (const auto& target : iter->second){
            results.add(
                log10p,
                StringMatchData{text, normalized, normalized, target}
            );
        }
        return;
    }


    for (auto item : database){
        double token_length = item.first.size();

        size_t distance = levenshtein_distance_substring(item.first, normalized);
        size_t matched = token_length - distance;
        if (matched == 0){
            continue;
        }

        double probability = random_match_probability(token_length, matched, random_match_chance);
        double log10p = std::log10(probability);

        if (distance == 0){
            results.exact_match = true;
        }

        for (const auto& slug : item.second){
            results.add(log10p, StringMatchData{text, normalized, item.first, slug});
            results.clear_beyond_spread(log10p_spread);
        }
    }
}








}
}
