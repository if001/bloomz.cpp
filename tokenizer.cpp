#include <cstdio>
#include <iostream>
#include <dlfcn.h>
#include <vector>
#include <string>

const char *LIB = "./tokenizer_rs/target/debug/libtokenizer.so";

std::vector<int32_t> to_vector(const int32_t* arr, size_t size) {
    return std::vector<int32_t>(arr, arr + size);
}

const int32_t* to_array(const std::vector<int32_t>& vec) {
    return vec.data();
}
size_t size_of_array(const int32_t* arr) {
    size_t size = 0;
    while (arr[size] != '\0') {
        ++size;
    }
    return size;
}

std::string decode(const std::vector<int32_t> ids) {    
    const std::string empty = {""};
    void* handle = dlopen(LIB, RTLD_LAZY);
    if (!handle) {
        std::cerr << "Error loading library: " << dlerror() << std::endl;
        return empty;
    }

    using DecodeFn = char*(*)(const int32_t*, const int);
    DecodeFn decode = reinterpret_cast<DecodeFn>(dlsym(handle, "decode"));
    if (!decode) {
        std::cerr << "Error loading symbol: " << dlerror() << std::endl;
        return empty;
    }
    char *text = decode(ids.data(), ids.size());

    dlclose(handle);
    return std::string(text);
}

const std::vector<int32_t> encode(std::string input) {
    const char *text = input.c_str();
    void* handle = dlopen(LIB, RTLD_LAZY);
    if (!handle) {
        std::cerr << "Error loading library: " << dlerror() << std::endl;
        return {};
    }

    using EncodeFn = int32_t* (*)(const char*);
    EncodeFn encode = reinterpret_cast<EncodeFn>(dlsym(handle, "encode"));
    if (!encode) {
        std::cerr << "Error loading symbol: " << dlerror() << std::endl;
        return {};
    }

    const int32_t *ids = encode(text);    
    dlclose(handle);
    size_t l = size_of_array(ids);
    return to_vector(ids, l);
}

// int main(int argc, char ** argv) {
//     printf("tokenize test..\n");    
//     const std::vector<int32_t> ids = encode("応答");
//     size_t l = ids.size();
//     printf("size: %ld\n", l);
//     printf("encode:\n");
//     for(int i = 0; i < l-1; i++) {
//         std::cout << ids[i] << std::endl;
//     }
//     printf("-------------\n");

//     std::string r = decode(ids);
//     std::cout <<  "decode: " << r << '\n' <<std::endl;
//     return 0;
// }