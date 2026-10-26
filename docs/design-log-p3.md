# Project 3 Design Log
**Author:** Mudassir Mohammed · **P2 code used:** instructor reference code for P2 used because I was having commpiling issues while doing P2 and couldn't finish it. 
**Tools used:** Because I was having issues with compiling/running/debugging my code for P2, I used ChatGPT to guide me to set up my workstation and help me install relevant CMake software tools, run accurate scripts in CLI to get VS Code to my project directory and get it to compile using WSL (Ubuntu) and run tests. I also used ChatGPT to explain what the project required of me and help me review what the code provided to us meant in human terms and how that connects to the files that we have to implement. This really helped with understanding the syntax of the program, and lessened the overwhelming feeling that came with being given so many files. Additionally, I used ChatGPT for referencing how to convert between different types for printing values in my Tests. 

<!-- 500–800 words. An argument, not a diary: say what you decided, what
     you rejected, and what evidence backs it up. Delete these comments
     before submitting. -->

## 1. Hash table
<!-- Chaining or open addressing, and why. Your hash function, load-factor
     threshold, and resize policy. Paste your 10,000-tool numbers
     (bucket_count, load_factor, max_probe_length) next to the alternative
     hash's longest chain. Explain briefly why add() is amortized O(1). -->
     For my hash table, I chose linear probing because I wanted to avoid nested loops in my code, and keep it as simple as possible with a single array. Additionally, it's cache-friendly.  And even when resizing my hash table, I would only have to move my pointers. 

My hash function is the 64-bit FNV-1a, my function uses all characters and returns the full width value, not just the modulo-reduced value. I initilaized my bucket size to be 16 elements long. I decided to maintain my load-factor to be less than or equal to 0.75 at all costs. And since linear probing slows down sharply as the load_factor approaches 1, whenever I predicted inserting into a bucket will make it go over that 0.75 threshold using ((toolCount + 1) / bucket_count in an if statement), I resized and grow my hash table by a muliple of 2. 

     My 10,000-tool numbers are: 
     Bucket Count: 16384
     Load Factor: 0.61
     Max Probe Length: 30

     Weak Hash's longest chain: 9000 | Original Hash (FNV-1a) length: 30

     And add is amortized O(1) because instead of traversing through the entire array, we directly map keys to indexes in the array. And only when the index is non-empty, we travel until the specific distance of max_probe_lenghth to find an empty bucket for the key, hence we avoid O(n) case (where it traverses the entire array). And, since we only double our table size each time we appraoch the table getting filled, we get O(1) as an average of all such instances where keys have to be added to the hash tables.

## 2. Pointer stability
<!-- Why does a Tool* returned by find() survive a resize in your design? -->
Because in my code, each element stored inside my buckets in my hash tables are unique_ptr, which is a smart pointer pointing to the Tool object, not the tool object itself. And whenever we resize the hash table, we simply move the pointers (using std::move) to the new array, and doesn't modify the Tool object's memory addresss and it stays whereever it was initially allocated. So, when find() returns a pointer address, it survives the resize. 

## 3. Calculator trace
<!-- Trace the values and ops stacks for 2 * (3 + 4) - 5, one token at a
     time, in your own words. -->

     Stack: LIFO
     Stack: bottom->[]<-top

     Token 1: 2
     Values: [2]
     Ops: []
     Ex: push 2 onto the values stack

     Token 2: *
     Values: [2]
     Ops: [*]
     Ex: push * onto the ops stack

     Token 3: (
     Values: [2]
     Ops: [* (]
     Ex: push ( onto the ops stack

     Token 4: 3
     Values: [2 3]
     Ops: [* (]
     Ex: push 3 onto the values stack

     Token 5: +
     Values: [2 3]
     Ops: [* ( +]
     Ex: push + onto the ops stack

     Token 6: 4
     Values: [2 3 4]
     Ops: [* ( +]
     Ex: push 4 onto the values stack


     Token 7: )
     Values: [2 7]
     Ops: [*]
     Ex: FOUND THE CLOSING PARENTHESIS:
         Step 1: apply the + operator to the top 2 values (3 + 4 = 7<- store on the values stack), causing the top to be the opening parenthesis
         Step 2: Pop the ( operator

     Token 8: -
     Values: [14]
     Ops: [-]
     Ex:  
     Step 1: The top of the ops stack is *, and its precedence is determined to be 2 (greater than -'s precedence of 1), so compute the two numbers in the values stack ( 2 * 7 = 14), then pop the * operator
     Step 2: push - onto the ops stack


     Token 9: 5
     Values: [14 5]
     Ops: [-]
     Ex: Push 5 onto the values stack

     Tokens Ran Out
     Values: [9]
     Ops: []
     Ex:  the top of the ops stack is NOT (, and its precendence is determined to be 1, and theres no other operator with a greater precendence, so apply and compute the two numbers in the values stack, then pop the - operator.And since tokens ran out, single number stored in values is the answer

## 4. Errors as values
<!-- Why do tools, resolve() and calc_evaluate() return errors instead of
     throwing? When IS an exception the right choice? -->
     They return errors because this programs expects errors from the user such as giving integers which end up in division by zero, or trying to access file directories which don't exist, etc. And rather than crashing the program, we want the program to manage such errors rather than halt the entire program, when you can continue the rest of the program and inform the user of the failure.

     The right time to throw exceptions would be for run-time programmer errors rather than distasteful input from the user, such as calling the wrong accessor (ex: "as_string()" on a number).

## 5. What I would design differently
I would like to challenge myself by choosing chaining for ToolRegistry the next time I get started, additionally, I believe I could choose more diverse set of test cases than the ones in the project spec to really ensure my code is working in all types of conditions, not just the ones given in the project spec. 
