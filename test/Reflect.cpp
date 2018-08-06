// Copyright (c) 2012-2018 The woodzcl Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Created by woodzcl on 2018/8/1.
//

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "CMemBlock.h"

#define STRING(flag) #flag

template<typename T>
class Helper {
private:
    T _t;
public:
    static const char *s_typename;
public:
     void init(T t);
     T getValue() const;
};

#define TYPE_HELPER(type) \
template<> const char* Helper<type>::s_typename = STRING(type);


template<typename T> void Helper<T>::init(T t) {
    _t = t;
}
template<typename T> T Helper<T>::getValue() const {
    return _t;
}

TYPE_HELPER(int);

TEST_CASE("reflect", "[TEST]") {
    printf("%s.\n", Helper<int>::s_typename);
    Helper<int> h1, h2;
    h1.init(10);
    h2.init(20);
    printf("h1=%d, h2=%d.\n", h1.getValue(), h2.getValue());
}