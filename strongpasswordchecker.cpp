// Password Req: 
//   - at least one lowercase letter
//   - at least one uppercase letter, 
//   - at least one digit.
//   - NOT contain three repeating characters in a row
int strongPasswordChecker(std::string s) {

    // ---------------------
    // Step 0. Initial Vars
    // ---------------------
    unsigned len = s.length();

    unsigned minLength = 6;
    unsigned maxLength = 20;
    unsigned minReps = 1;       // Minimum number of repeated characters in a row (aaa => 2 reps)

    // Ascii lookup
    unsigned asciiDigLow = 48;
    unsigned asciiDigHigh = 57;
    unsigned asciiUCLow = 65;
    unsigned asciiUCHigh = 90;
    unsigned asciiLCLow = 97;
    unsigned asciiLCHigh = 122;

    // ------------------------
    // Step 1. String Analysis
    // ------------------------

    // Required characters 
    bool hasLowercase = false;
    bool hasUppercase = false;
    bool hasDigit = false;
    unsigned missingCharCnt = 0;

    // list of reps present
    std::vector<unsigned> reps;
    unsigned maxRep = 0;
    unsigned repCnt = 0;        // Reusable repetition counter

    // NOTE: Unrolling the first itteration for rep-char check
    char cPrev;
    char c = s[0];
    if(asciiDigLow <= c && c <= asciiDigHigh) hasDigit = true;
    if(asciiUCLow <= c && c <= asciiUCHigh) hasUppercase = true;
    if(asciiLCLow <= c && c <= asciiLCHigh) hasLowercase = true;
    for(unsigned i = 1; i < len; i++){
        cPrev = c;
        c = s[i];

        if(asciiDigLow <= c && c <= asciiDigHigh) hasDigit = true;
        if(asciiUCLow <= c && c <= asciiUCHigh) hasUppercase = true;
        if(asciiLCLow <= c && c <= asciiLCHigh) hasLowercase = true;

        // Check for repitition
        // If repitition ends add rep total to the list
        if(c == cPrev) repCnt++;
        else {
            if(minReps < repCnt) {
                reps.push_back(repCnt);
                if(maxRep < repCnt) maxRep = repCnt;
            }
            repCnt = 0;
        }
    }
    // NOTE: One last rep check after the loop ends
    if(minReps < repCnt) {
        reps.push_back(repCnt);
        if(maxRep < repCnt) maxRep = repCnt;
    }

    // Compute number of missing req. characters
    if(!hasDigit) missingCharCnt++;
    if(!hasUppercase) missingCharCnt++;
    if(!hasLowercase) missingCharCnt++;

    // -------------------------------
    // Step 2. Compute Minimum Change
    // -------------------------------

    // Keep track of minimum change needed in password
    //    Change == (insertion/deletion/replace)
    unsigned minChange = 0;

    // Insert/replace characters to meet req. chars
    //   - Insert when below min length
    //   - Replace when at/above min length
    //   - Prioritize breaking up larger reps
    //   - Insertion/Replacement effect the reps differently
    for(unsigned n = 0; n < missingCharCnt; n++){
        if(len < minLength){
        if(maxRep <= minReps+2){
                if(0 < reps.size()) reps.pop_back();
            }
            else{
                // Find the curr maxRep, insert, update the new maxRep
                unsigned newMaxRep = 0;
                for(unsigned i = 0; i < reps.size(); i++){
                    unsigned rep = reps[i];
                    if(rep == maxRep){
                        rep -= (minReps + 1);
                        reps[i] = rep;
                    }

                    if(newMaxRep < rep) newMaxRep = rep;
                }
                maxRep = newMaxRep;
            }
            minChange++;
            len++;
        }
        else{
        if(maxRep <= minReps+3){
                if(0 < reps.size()) reps.pop_back();
            }
            else{
                // Find the curr maxRep, insert, update the new maxRep
                unsigned newMaxRep = 0;
                for(unsigned i = 0; i < reps.size(); i++){
                    unsigned rep = reps[i];
                    if(rep == maxRep){
                        rep -= (minReps + 2);
                        reps[i] = rep;
                    }

                    if(newMaxRep < rep) newMaxRep = rep;
                }
                maxRep = newMaxRep;
            }
            minChange++;
        }
    }

    // Take excepss from reps, up untill the sweet spot where 1 replacement removes reps
    unsigned sweetspot = (minReps+1)*2;
    for(int i = 0; i < reps.size(); i++){
        unsigned rep = reps[i];
        if(maxLength < len){
            int excess = (int)rep - (int)sweetspot;
            if(0 < excess){

                // Only take as much as we need to meet min length req
                if(len - excess < maxLength){
                    excess = len - maxLength;
                }

                minChange += excess;
                len -= excess;
                reps[i] = rep - excess;
            }
        }
    }

    // If we still have too many characters, take from the remaining reps
    // Prioritize the smallest ones
    sort(reps.begin(), reps.end());
    for(int i = 0; i < reps.size(); i++){
        unsigned rep = reps[i];
        if(maxLength < len){
            unsigned removal = rep - minReps;

            // Only take as much as we need to meet min length req
            if(len - removal < maxLength){
                removal = len - maxLength;
            }

            minChange += removal;
            len -= removal;
            reps[i] = rep - removal;
        }
    }

    // If there are any repitions left, break them up wityh replacements
    // NOTE: this loop adds 0's if we eliminated all reps by this stage
    for(int i = 0; i < reps.size(); i++){
        unsigned rep = reps[i];
        unsigned numReplacements = (rep + 1)/(minReps+2);
        minChange += numReplacements;
    }

    // insertions to fill remaining length
    if(len < minLength) minChange += minLength - len;

    // deletions to cull remaining length
    if(maxLength < len) minChange += len - maxLength;

    return minChange;
}
