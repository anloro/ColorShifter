/**
 * @file   utils.h
 * @brief  .
 * @author Ángel Lorente Rogel
 * @date   12/11/2021
 */

#pragma once

#include <map>
#include <vector> 
#include <algorithm>
#include <numeric> 


template<typename A, typename B>
std::pair<B,A> flip_pair(const std::pair<A,B> &pair){
    return std::pair<B,A>(pair.second, pair.first);
}

template<typename A, typename B>
std::multimap<B,A> flipMap(const std::map<A, B> &src){
    std::multimap<B,A> dst;
    std::transform(src.begin(), src.end(), 
                    std::inserter(dst, dst.begin()), flip_pair<A,B>);
    return dst;
}

template <typename T>
std::vector<T> get_sorted_indexes(const std::vector<T> &v) {
    // Returns the indexes of a sorted vector

    // initialize original index locations
    std::vector<T> idx(v.size());
    std::iota(idx.begin(), idx.end(), 0);

    // sort indexes based on comparing values in v
    // using std::stable_sort instead of std::sort
    // to avoid unnecessary index re-orderings
    // when v contains elements of equal values 
    std::stable_sort(idx.begin(), idx.end(),
        [&v](std::size_t i1, std::size_t i2) {return v[i1] > v[i2];});

    return idx;
}