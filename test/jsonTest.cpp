// Copyright (c) 2012-2018 The woodzcl Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Created by woodzcl on 2018/8/6.
//

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <string>
#include <nlohmann/json.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include <sstream>


TEST_CASE("json", "[JSON]") {
    SECTION("memory") {
        nlohmann::json json;

        std::string name = "newbie", name_ref;
        size_t age = 18, age_ref;
        int gender = 0, gender_ref;
        std::string occupy = "manager", occupy_ref;

        json["name"] = name;
        json["age"] = age;
        json["gender"] = gender;
        json["occupy"] = occupy;

        name_ref = json.find("name") != json.end() ? json["name"].get<std::string>() : "";
        age_ref = json.find("age") != json.end() ? json["age"].get<size_t>() : 0;
        gender_ref = json.find("gender") != json.end() ? json["gender"].get<int>() : 0;
        occupy_ref = json.find("ocuppy_ref") != json.end() ? json["occupy"].get<std::string>() : "";
    }
    SECTION("file") {
        const boost::filesystem::path path = "myjsonfile.dat";

        nlohmann::json json, json_ref;

        std::string name = "newbie", name_ref;
        size_t age = 18, age_ref;
        int gender = 0, gender_ref;
        std::string occupy = "manager", occupy_ref;

        json["name"] = name;
        json["age"] = age;
        json["gender"] = gender;
        json["occupy"] = occupy;

        std::ofstream outfile(path.string());
        outfile << json;
        outfile.close();

        std::ifstream infile(path.string());
        infile >> json_ref;
        infile.close();

        std::stringstream ss;
        ss << json_ref;
        std::string str_ss = ss.str();
        std::cout << "string of json: " << str_ss << std::endl;

        name_ref = json_ref.find("name") != json_ref.end() ? json_ref["name"].get<std::string>() : "";
        age_ref = json_ref.find("age") != json_ref.end() ? json_ref["age"].get<size_t>() : 0;
        gender_ref = json_ref.find("gender") != json_ref.end() ? json_ref["gender"].get<int>() : 0;
        occupy_ref = json_ref.find("ocuppy_ref") != json_ref.end() ? json_ref["occupy"].get<std::string>() : "";
    }
}

