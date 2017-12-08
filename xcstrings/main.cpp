//
//  main.cpp
//  strings_fix
//
//  Created by Daher Alfawares on 12/7/17.
//  Copyright © 2017 Daher Alfawares. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <boost/regex.hpp>
#include "x_options.hpp"

class foo {
    virtual void abc() = 0;
};

class bar : public foo {
    void abc(){}
};

bar b;


namespace xcs {
    struct content {
        typedef std::string string;
        typedef std::set<string> set;
        typedef std::map<string,set> map;
        map strings;
        content(std::string filename){
            
            const std::size_t buffer_size = 1024*1024;
            char buffer[buffer_size] = {0};
            int line_count = 0;
            
            boost::regex keyRegex {"\\s*\\\"(\\w+)\\\".+=.+"};
            
            std::ifstream in(filename);
            if(in.is_open()){
                while(in.getline(buffer, buffer_size)){
                    line_count ++;
                    std::string line = buffer;
                    if(line[0] == char(-17)){
                        line.erase(line.begin(),line.begin()+3);
                    }
                    auto match = boost::regex_match(line, keyRegex);
                    if(match){
                        boost::smatch what;
                        boost::regex_search(line, what, keyRegex);
                        std::string key = what[1];
                        this->insert(key,line);
                    } else {
                        std::cout << "// ignoring -> " << line << std::endl;
                    }
                }
            }
        }
        
        void insert(string key, string line){
            if(strings.find(key) != this->strings.end()){
                this->strings[key].insert(line);
            } else {
                this->strings[key] = set {line};
            }
        }
    };
    
    class sort {
    public:
        sort(const content& content){
            // print (sorted)
            for(auto string:content.strings){
                for(auto line:string.second){
                    std::cout << line << std::endl;
                }
            }
        }
    };
    
    class check {
    public:
        check(const content& content){
            int errors = 0;
            for(auto string:content.strings){
                if(string.second.size()>1){
                    std::cerr << "error " << errors++ << ": duplicate key - " << string.first << std::endl;
                    for(auto line:string.second){
                        std::cerr << "\t-> " << line << std::endl;
                    }
                }
            }
            
            if(errors>0){
                std::cerr << errors << " errors found." << std::endl;
            }
        }
    };
}


namespace arg {
    std::string help = "-help";
    std::string check = "-check";
    std::string sort = "-sort";
}



int main(int argc, const char * argv[]) {
    // insert code here...
    if(argc == 1){
        std::cout << "//usage: xcstrings -(command) file" << std::endl;
        std::cout << "//usage: xcstrings -help" << std::endl;
        return -1;
    }
    std::cout << "//xcstrings v1.0" << std::endl;
    std::cout << "//cmd: " << argv[0] << std::endl;

    auto arguments = x::options(argc,argv);
    arguments.map_to({
        {arg::help, x::option("Produces the usage of this tool.")},
        {arg::check, x::option("Will print an error if there are any duplicate keys. Usage: -dups <file>.")},
        {arg::sort, x::option("Sorts keys in a strings file.Usage: -sort <file>.")},
    });
    

    if(arguments[arg::help]){
        std::cout
        << "xcstrings is a command line tool that parses xcode strings file. "
        << "You can use it to check for errors and sort the file by key."
        << std::endl;
        std::cout << arguments.print();
        return 0;
    }
    
    if(arguments[arg::check]){
        auto in = arguments[arg::check].value();
        if(in == ""){
            std::cerr << "Error no input." << std::endl;
            return -2;
        }
        
        xcs::content content(arguments[arg::check].value());
        xcs::check check(content);
    }
   
    if(arguments[arg::sort]){
        auto in = arguments[arg::sort].value();
        if(in == ""){
            std::cerr << "Error no input." << std::endl;
            return -2;
        }
        
        xcs::content content(arguments[arg::sort].value());
        xcs::sort sort(content);
    }
    return 0;
}
