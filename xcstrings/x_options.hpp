//
//  x_options.hpp
//  x
//
//  Created by Daher Alfawares on 8/28/17.
//  Copyright © 2017 Daher Alfawares. All rights reserved.
//

#ifndef x_options_hpp
#define x_options_hpp

/* : Example :
 
    namespace arg {
    std::string help = "-help";
    std::string name = "-name-list";
    }

    auto arguments = x::options(argc,argv);
    arguments.map_to({
    {arg::help,    x::option("produces the usage of this tool.")},
    {arg::name,    x::option("takes your first and last names.")}
    });

    if(arguments[arg::help]){
    ... arguments[arg::help].value();
    }

    if(arguments[arg::name]){
    ... arguments[arg::name].values();
    }
 */

#include <string>
#include <vector>
#include <map>

namespace x {
    
    class option {
    public:
        typedef std::string             string;
        typedef std::vector<string>     vector;
        typedef std::map<string,option> map;
        
    public:
        option(string description){
            option_description = description;
            option_is_enabled  = false;
        }
        void operator +=(const string& value){
            this->option_values.push_back(value);
        }
        const vector values() const { return this->option_values; }
        const string value() const {
            if(this->option_values.size()>0)
                return this->option_values[0];
            else
                return string();
        }
        void enable(){ option_is_enabled = true; }
        bool enabled(){ return option_is_enabled; }
        std::string description(){ return option_description; }
        operator bool () { return enabled(); }
        
    private:
        string option_description;
        vector option_values;
        bool   option_is_enabled;
    };
    
    class options {
    public:
        option::map     map;
        option::vector  arguments;

        options(int argc, const char * argv[]);
        void map_to(option::map map);
        std::string print();
        std::string print_values();
        
        inline
        option& operator[](option::string key){
            auto iterator = map.find(key);
            if(iterator != map.end()){
                return iterator->second;
            }
            
            throw -1;
        }
    };
}

#endif /* xctest_options_hpp */
