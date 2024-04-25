#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <climits>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // Add your code here
        unsigned long long w[5] = {0}; // Store vlaues
        int length = k.length();
        int numSegs = (length + 5) / 6; // Calculate num segments

        for ( int i = 0; i < 5; ++i)
        {
          int segmentStart = std::max(0, length -6 * (i+1));
          int segmentEnd = length - 6 * i;
          unsigned long long segmentValue = 0;
          unsigned long long base = 1;

          for (int j = segmentEnd - 1; j >= segmentStart; --j)
          {
            unsigned long long charValue =  letterDigitToNumber(k[j]);
            unsigned long long addValue = charValue * base;
            
            if(segmentValue > ULLONG_MAX - addValue)
            {
              std::cerr << "Potential Overflow error" << std::endl;
              return 0;
            }
          
            segmentValue += addValue;
            base *= 36;
            //std::cout << "debug char " << k[j] << " value " << letterDigitToNumber(k[j]) 
                          // << " Segment Value " << segmentValue << std::endl;
            //std::cout << "Segemnt " << i << ": " << segmentValue << std::endl;
          }
          
          w[4-i] = segmentValue;
        }

        unsigned long long hash = 0;
        for (int i = 0; i < 5; ++i)
        {
          hash += rValues[i] * w[i];
          std::cout << "w[" << i << "]: " << hash << std::endl;
        }
        
        return hash;

    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it
        if(isdigit(letter))
        {
          return 26 + letter - '0';
        }
        else
        {
          letter = std::tolower(letter);
          return letter - 'a';
        }

    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
