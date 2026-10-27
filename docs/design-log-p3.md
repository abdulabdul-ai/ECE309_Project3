# Project 3 Design Log
**Author:** Mudassir Mohammed · **P2 code used:** instructor reference code for P2 used because I was having commpiling issues for P2.
**Tools used:** Because I was having issues with compiling/running/debugging my code for P2, I used ChatGPT to guide me to set up my workstation, help me install CMake software tools, run accurate scripts in CLI to reach project directory, and compile using WSL (Ubuntu) and run tests. I also used ChatGPT to explain the spec and help me review what the code provided to us meant in human terms and how that connects to the files to be implemented. This helped with understanding the syntax of the program, and lessened the overwhelmingness that came with being given so many files. Additionally, used it for referencing how to convert between data types.


## 1. Hash table

I chose linear probing to avoid nested loops in my code and maintain spacial locality. Additionally, it's cache-friendly.  And even when resizing my hash table, I would only have to move my pointers. 

My hash function is the 64-bit FNV-1a, my function uses all characters and returns the full width value, not just the modulo-reduced value. I initilaized my bucket size to be 16 elements long. I maintain my load-factor to be less than or equal to 0.75 at all costs. And since linear probing slows down sharply as the load factor approaches 1, whenever I predicted inserting into a bucket will make it go over that 0.75 threshold using ((toolCount + 1) / bucket_count in an if statement), I resized and grow my hash table by doubling its size. 

     My 10,000-tool numbers are: 
     Bucket Count: 16384
     Load Factor: 0.61
     Max Probe Length: 30

     Weak Hash's longest chain: 9000 | Original Hash (FNV-1a) probe length: 30

Add is amortized O(1) because a good hash function uniformly distributes items into bucket and we jump directly to the index we want to add to, making it O(1) rather than traversing the entire table to find that index. And in case of linear probing, we only probe a limited distance, then resize (costing O(N)). But since we double the size of table, we can have more direct adding to indexes O(1). Causing us on average to have O(1).

## 2. Pointer stability
Because in my code each element stored inside my buckets in my hash tables are unique_ptr (smart pointer) pointing to the Tool object, not storing the tool object itself. And whenever we resize the table, we simply move the pointers (using std::move) to the new array, and it doesn't modify the Tool object's memory addresss and it stays whereever it was initially allocated. So, when find() returns a pointer address, it survives the resize. 

## 3. Calculator trace

     Stack: LIFO
     Stack: bottom->[]<-top

     Token 1: 2
     Values: [2]
     Ops: []
     Ex: push 2 on values stack

     Token 2: *
     Values: [2]
     Ops: [*]
     Ex: push * on ops stack

     Token 3: (
     Values: [2]
     Ops: [* (]
     Ex: push ( on ops stack

     Token 4: 3
     Values: [2 3]
     Ops: [* (]
     Ex: push 3 on values stack

     Token 5: +
     Values: [2 3]
     Ops: [* ( +]
     Ex: push + on ops stack

     Token 6: 4
     Values: [2 3 4]
     Ops: [* ( +]
     Ex: push 4 on values stack


     Token 7: )
     Values: [2 7]
     Ops: [*]
     Ex: FOUND THE CLOSING PARENTHESIS:
         Step 1: apply the + operator to the top 2 values (3 + 4 = 7 <- store on values stack), causing the top to be the opening parenthesis
         Step 2: Pop the ( operator

     Token 8: -
     Values: [14]
     Ops: [-]
     Ex:  
     Step 1: The top of the ops stack is *, and its precedence is determined to be 2 (greater than -'s precedence of 1), so compute the two numbers in the values stack (2 * 7 = 14), then pop the * operator
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
 They return errors because this programs expects errors from the user such as giving integers which end up in division by zero, or trying to access file directories which don't exist, etc. Rrather than crashing the program, we want the program to manage such errors rather than halt the entire program, when you can continue the rest of the program and inform the user of the failure to retry.

The right time to throw exceptions would be for run-time programmer errors rather than distasteful input from the user, such as calling the wrong accessor (ex: "as_string()" on a number).

## 5. What I would design differently
I would like to challenge myself by choosing chaining for ToolRegistry the next time I get started. I believe I could choose more diverse set of test cases than the ones in the project spec to really ensure my code is working in all types of conditions, not just the ones given in the project spec. 
