bool isNumber(std::string s) {

    // Trimed begin and end
    auto begin = find_if(s.begin(), s.end(), [](char c){return !isspace(c);});
    if(begin != s.end()){ // check for empty string

        auto end = find_if(s.rbegin(), s.rend(), [](char c){return !isspace(c);}).base();

        // Count the symbols up until the end or the exponent marker
        unsigned numPlus = 0;
        unsigned numMinus = 0;
        unsigned numRadix = 0;
        unsigned numDigit = 0;
        auto sitter = begin;
        char c = *sitter;
        while(sitter != end && c != 'e'){
                 if(c == '+') numPlus++;
            else if(c == '-') numMinus++;
            else if(c == '.') numRadix++;
            else if(isdigit(c)) numDigit++;
            else                return false; // Invalid character
            sitter = next(sitter);
            c = *sitter;
        }
        auto eLoc = sitter; // If number is non-exponential, this will point to end

        // Check that the counts are valid
        if( numPlus <= 1 && numMinus <= 1 && numRadix <= 1 && numPlus + numMinus <= 1 && 0 < numDigit){

            // Check that +/- is at the start
            c = *begin;
            if( numPlus + numMinus == 0 || c == '+' || c == '-'){

                // If there is an exponent check that it has valid syntax
                if(eLoc != end){

                    // Advance forward to start of exponent (check that the exponent is not empty)
                    auto eBegin = next(eLoc);
                    if(eBegin != end){

                        // Count the symbols up until the end
                        numPlus = 0;
                        numMinus = 0;
                        numRadix = 0;
                        numDigit = 0;
                        sitter = eBegin;
                        c = *sitter;
                        while(sitter != end){
                                 if(c == '+') numPlus++;
                            else if(c == '-') numMinus++;
                            else if(c == '.') numRadix++;
                            else if(isdigit(c)) numDigit++;
                            else                return false; // Invalid character
                            sitter = next(sitter);
                            c = *sitter;
                        }
                        if( numPlus <= 1 && numMinus <= 1 && numRadix == 0 && numPlus + numMinus <= 1 && 0 < numDigit){

                            // Check that +/- is at the start
                            c = *eBegin;
                            if( numPlus + numMinus == 0 || c == '+' || c == '-'){
                                return true;
                            }
                        }
                    }
                }
                else{
                    return true;
                }
            }
        }
    }

    return false;
}
