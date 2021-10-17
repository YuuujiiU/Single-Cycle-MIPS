#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>

using namespace std;

#define ADDU 1
#define SUBU 3
#define AND 4
#define OR  5
#define NOR 7

// Memory size.
// In reality, the memory size should be 2^32, but for this lab and space reasons,
// we keep it as this large number, but the memory is still 32-bit addressable.
#define MemSize 65536


class RF
{
public:
    bitset<32> ReadData1, ReadData2;

    RF()
    {
        Registers.resize(32);
        Registers[0] = bitset<32>(0);
    }

    void ReadWrite(bitset<5> RdReg1, bitset<5> RdReg2, bitset<5> WrtReg, bitset<32> WrtData, bitset<1> WrtEnable)
    {
        // TODO: implement!  


          //to_ulong: Returns an unsigned long with the integer value
          //that has the same set as the bitsit
          //Like 25:21 for Reg1, 20:16 for Reg2 in slide
        unsigned rdReg1Addr = RdReg1.to_ulong();
        ReadData1 = Registers[rdReg1Addr];
        unsigned rdReg2Addr = RdReg2.to_ulong();
        ReadData2 = Registers[rdReg2Addr];

        //Make sure the status of WrtEnable is open
        //Write Into Register
        if (WrtEnable.test(0))
        {
            unsigned WrtRegAddr = WrtReg.to_ulong();
            Registers[WrtRegAddr] = WrtData;
        }

    }

    void OutputRF()
    {
        ofstream rfout;
        rfout.open("RFresult.txt", std::ios_base::app);
        if (rfout.is_open())
        {
            rfout << "A state of RF:" << endl;
            for (int j = 0; j < 32; j++)
            {
                rfout << Registers[j] << endl;
            }

        }
        else cout << "Unable to open file";
        rfout.close();

    }
private:
    vector<bitset<32> >Registers;
};


class ALU
{
public:
    bitset<32> ALUresult;
    bitset<32> ALUOperation(bitset<3> ALUOP, bitset<32> oprand1, bitset<32> oprand2)
    {
        // TODO: implement!

        bitset<32> aluresult;

        if (ALUOP.to_ulong() == ADDU)
        {
            unsigned long Opera1 = oprand1.to_ulong();
            unsigned long Opera2 = oprand2.to_ulong();
            unsigned long Opera3 = Opera1 + Opera2;
            aluresult = bitset<32>(Opera3);
        }
        else if (ALUOP.to_ulong() == SUBU)
        {
            unsigned long Opera1 = oprand1.to_ulong();
            unsigned long Opera2 = oprand2.to_ulong();
            aluresult = bitset<32>(Opera1 - Opera2);
        }
        else if (ALUOP.to_ulong() == AND)
        {
            aluresult = oprand1 & oprand2;
        }
        else if (ALUOP.to_ulong() == OR)
        {
            aluresult = (oprand1.to_ulong() | oprand2.to_ulong());
        }
        else if (ALUOP.to_ulong() == NOR)
        {
            aluresult = ~(oprand1.to_ulong() | oprand2.to_ulong());
        }

        ALUresult = aluresult;


        return ALUresult;

    }
};


class INSMem
{
public:
    bitset<32> Instruction;

    //Initializes the contents of instruction from the file imem.txt
    INSMem()
    {
        IMem.resize(MemSize);
        ifstream imem;
        string line;
        int i = 0;
        imem.open("imem.txt");
        if (imem.is_open())
        {
            while (getline(imem, line))
            {
                IMem[i] = bitset<8>(line);
                i++;
            }

        }
        else cout << "Unable to open file";
        imem.close();

    }

    //To read access to instruction memory
    bitset<32> ReadMemory(bitset<32> ReadAddress)
    {
        // TODO: implement!
        // (Read the byte at the ReadAddress and the following three byte).
        //

        //Convert ReadAddress to ulong
        //Find the begining address for read instrction and read +4 adrress content

        unsigned long addr = ReadAddress.to_ulong();

        string instr;

        for (int i = 0; i < 4; i++)
        {
            instr += IMem[addr++].to_string();
        }

        bitset<32>Instruction(instr);

        return Instruction;

    }

private:
    vector<bitset<8> > IMem;

};

class DataMem
{
public:
    bitset<32> readdata;
    DataMem()
    {
        DMem.resize(MemSize);
        ifstream dmem;
        string line;
        int i = 0;
        dmem.open("dmem.txt");
        if (dmem.is_open())
        {
            while (getline(dmem, line))
            {
                DMem[i] = bitset<8>(line);
                i++;
            }
        }
        else cout << "Unable to open file";
        dmem.close();

    }

    //If it in Register, we can directly read, like above; 
    //If not, we have to go Memory to Read/Write
    bitset<32> MemoryAccess(bitset<32> Address, bitset<32> WriteData, bitset<1> readmem, bitset<1> writemem)
    {

        unsigned ByteAddr = Address.to_ulong();
        int index = (int)ByteAddr;

        // TODO: implement!
        if (readmem.test(0)) {


            std::string readDataStr;

            for (int i = 0; i < 4; i++) {
                std::string byteStr = DMem.at(index++).to_string();
                readDataStr += byteStr;
            }
            readdata = std::bitset<32>(readDataStr);


        }

        else if (writemem.test(0)) {


            std::string writeDataStr = WriteData.to_string();

            for (int i = 0; i < 32; i += 8) {
                std::string byteStr = writeDataStr.substr(i, 8);
                bitset<8> finalbyteStr(byteStr);
                DMem.at(index++) = finalbyteStr;

            }
        }

        return readdata;

    }

    void OutputDataMem()
    {
        ofstream dmemout;
        dmemout.open("dmemresult.txt");
        if (dmemout.is_open())
        {
            for (int j = 0; j < 1000; j++)
            {
                dmemout << DMem[j] << endl;
            }

        }
        else cout << "Unable to open file";
        dmemout.close();

    }

private:
    vector<bitset<8> > DMem;

};




int main()
{
    RF myRF;
    ALU myALU;
    INSMem myInsMem;
    DataMem myDataMem;

    bitset<32> program = bitset<32>(0);

    bitset<32> $Rs, $Rd, $Rt, ALUResult, SignImm;
    bitset<5> RsIAddr, RtIAddr;
    bitset<16> ImmIAddr;


    while (1)
    {
        // Fetch
        bitset<32> instruction = myInsMem.ReadMemory(program);
        instruction = myInsMem.ReadMemory(program);

        if (instruction.all()) break;

        // decode(Read RF)


        string opcode = instruction.to_string().substr(0, 6);

        //Execute

        //R-Type
        if (opcode == "000000") {

            bitset<5> AddrRs = bitset<5>(instruction.to_string().substr(6, 5));
            bitset<5> AddrRt = bitset<5>(instruction.to_string().substr(11, 5));
            bitset<5> AddrRd = bitset<5>(instruction.to_string().substr(16, 5));


            myRF.ReadWrite(AddrRs, AddrRt, NULL, NULL, 0);
            $Rs = myRF.ReadData1;
            $Rt = myRF.ReadData2;

            string alu = instruction.to_string().substr(29, 3);
            bitset<3> ALU(alu);
            ALUResult = myALU.ALUOperation(ALU, $Rs, $Rt);

            myRF.ReadWrite(NULL, NULL, AddrRd, ALUResult, 1);
            program = program.to_ulong() + 4;

        }

        else if ((opcode != "000000") && (opcode != "000010"))
        {

            RsIAddr = bitset<5>(instruction.to_string().substr(6, 5));
            RtIAddr = bitset<5>(instruction.to_string().substr(11, 5));
            ImmIAddr = bitset<16>(instruction.to_string().substr(16, 16));




            string ImmStrExtend;

            if (ImmIAddr.to_string().at(0) == '0') {
                ImmStrExtend = std::string(16, '0') + ImmIAddr.to_string();
            }
            else if (ImmIAddr.to_string().at(0) == '1') {
                ImmStrExtend = std::string(16, '1') + ImmIAddr.to_string();
            }

            bitset<32> finalImm(ImmStrExtend);

            myRF.ReadWrite(RsIAddr, NULL, NULL, NULL, 0);
            $Rs = myRF.ReadData1;

            ALUResult = myALU.ALUOperation(bitset<3>(ADDU), $Rs, finalImm);


            if (opcode == "100011") {
                //lw:rs_imm->rt

                //$Rs = myRF.ReadData1;

                //ALUResult = myALU.ALUOperation(bitset<3> (ADDU), $Rs, finalImm);


                bitset<32> operation = myDataMem.MemoryAccess(ALUResult, NULL, 1, 0);
                myRF.ReadWrite(NULL, NULL, RtIAddr, operation, 1);

                program = program.to_ulong() + 4;


            }
            else if (opcode == "101011") {
                // sw:rt->mem(rs+imm)

                myRF.ReadWrite(RtIAddr, NULL, NULL, NULL, 0);
                $Rd = myRF.ReadData1;

                bitset<32> operation = myDataMem.MemoryAccess(ALUResult, $Rd, 0, 1);


                program = program.to_ulong() + 4;



            }

            else if (opcode == "001001") {

                //Type_instruction == "addiu"
                ALUResult = myALU.ALUOperation(bitset<3>(ADDU), $Rs, finalImm);
                myRF.ReadWrite(NULL, NULL, RtIAddr, ALUResult, 1);
                program = program.to_ulong() + 4;
            }

            else if (opcode == "000100") {

                //beq: rs==rt, pc+4+branchaddr
                myRF.ReadWrite(RsIAddr, NULL, NULL, NULL, 0);
                $Rs = myRF.ReadData1;
                myRF.ReadWrite(NULL, RtIAddr, NULL, NULL, 0);
                $Rt = myRF.ReadData2;

                if ($Rs.to_ulong() == $Rt.to_ulong())
                {
                    string finalImmedStr;

                    if (ImmIAddr.test(15))
                    {
                        finalImmedStr = (std::string(14, '1') + ImmIAddr.to_string() + "00");
                    }
                    else
                    {
                        finalImmedStr = (std::string(14, '0') + ImmIAddr.to_string() + "00");
                    }

                    bitset<32> BranchAdddr(finalImmedStr);

                    unsigned long pc_4 = program.to_ulong() + 4 + BranchAdddr.to_ulong();
                    bitset<32> pc(pc_4);
                    program = pc;
                }
                else
                {
                    //unsigned long pc_4 = program.to_ulong() + 4;
                    //program = bitset<32>(pc_4);

                    program = program.to_ulong() + 4;

                }

            }
        }
        else if (opcode == "000010") {
            //J-Type
            bitset<26> JTypeAddress(instruction.to_string().substr(6, 26));

            unsigned long pc_4 = (program.to_ulong() + 4);
            program = bitset<32>(pc_4);

            bitset<4> pc4_3128(program.to_string().substr(0, 4));
            string pcStr = pc4_3128.to_string() + JTypeAddress.to_string() + "00";
            program = bitset<32>(pcStr);
        }
        myRF.OutputRF(); // dump RF;
    }
    myDataMem.OutputDataMem(); // dump data mem

    return 0;

}




