# Single-Cycle-MIPS
We have defined four C++ classes that each implement one of the four major blocks in a single cycle MIPS machine, namely RF (to implement the register file), ALU (to implement the ALU), INSMem (to implement instruction memory), and DataMem (to implement data memory). 


Classes

We have defined four C++ classes that each implement one of the four major blocks in a single cycle MIPS machine, namely RF (to implement the register file), ALU (to implement the ALU), INSMem (to implement instruction memory), and DataMem (to implement data memory). 

1.	RF class: contains 32 32-bit registers defined as a private member. Remember that register $0 is always 0. Your job is to implement the implement the ReadWrite() member function that provides read and write access to the register file.
2.	ALU class: implements the ALU. Your job is to implement ALUOperation() member function that performs the appropriate operation on two 32 bit operands based on ALUOP. See Table 1 for more details.
3.	INSMem class: a Byte addressable memory that contains instructions. The constructor InsMem() initializes the contents of instruction memory from the file imem.txt (this has been done for you). Your job is to implement the member function ReadMemory() that provides read access to instruction memory. An access to the instruction memory class returns 4 bytes of data; i.e., the byte pointed to by the address and the three subsequent bytes. 
4.	DataMem class: is similar to the instruction memory, except that it provides both read and write access. 
                  
Main Function

The main function defines a 32 bit program counter (PC) that is initialized to zero. The MIPS simulation routine is carried out within a while loop. In each iteration of the while loop, you will fetch one instruction from the instruction memory, and based on the instruction, make calls to the register file, ALU and data memory classes (in fact, you might need to make two calls to the register file class, once to read and a second time to write back). Finally you will update the PC so as to fetch the next instruction. When the halt instruction is fetched, you are to break out of the while loop and terminate the simulation.

Make sure that the architectural state is updated correctly after execution of each instruction. The architectural state consists of the Program Counter (PC), the Register File (RF) and the Data Memory (DataMem). We will check the correctness of the architectural state after each instruction. 

Specifically, the OutputRF() function is called at the end of each iteration of the while loop, and will add the new state of the Register File to “RFresult.txt”. Therefore, at the end of the program execution “RFresult.txt” contains all the intermediate states of the Register File. Once the program terminates, the OutputDataMem() function will write the final state of the Data Memory to “dmem.txt”. These functions have been implemented for you. Do not modify them.
