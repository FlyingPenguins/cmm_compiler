/Joshua Titley
//FLYING PENGUINS
//CS4300

//Two ways to implement, if we know how many variables we are sending we can get away with 4 transaltion functions, if we do not know
//then we will have to fill in the fprintf statement for each which is easy, but tedious
//
//We will have to impliment a way to pass teh file name to the class, unless we want to open teh file stream with each call, which is probably bad form
//feel free to rip out what you dont want

#include <cstdio>
#include <string.h>
//Initializing functions
void create();
void addData(int Label, char* data);
void firstLine(int mainLoc, int dataSPace);
//SHOULD BE ALL WE NEED FOR IMPLEMENTATION
void writeIns1(char ins);
void writeIns2(char ins, char* dest);
void writeIns3(char ins, char* op1, char* dest);
void writeIns4(char ins, char* op1, char* op2, char* dest);

//DELETE IF NOT NEEDED
//Instruction set
void addI(char* ins, char* op1, char* op2, char* dest);
void addF(char* ins, char* op1, char* op2, char* dest);
void subI(char* ins, char* op1, char* op2, char* dest);
void subF(char* ins, char* op1, char* op2, char* dest);
void mulI(char* ins, char* op1, char* op2, char* dest);
void mulF(char* ins, char* op1, char* op2, char* dest);
void divI(char* ins, char* op1, char* op2, char* dest);
void divF(char* ins, char* op1, char* op2, char* dest);
void residue(char* ins, char* op1, char* op2, char* dest);
void bwOR(char* ins, char* op1, char* op2, char* dest);
void bwAND(char* ins, char* op1, char* op2, char* dest);
void jml(char* ins, char* op1, char* op2, char* dest);
void jmg(char* ins, char* op1, char* op2, char* dest);
void jme(char* ins, char* op1, char* op2, char* dest);
void identity(char* ins, char* op1, char* op2, char* dest);
void byteCopy(char* ins, char* op1, char* op2, char* dest);
void castFloat(char* ins, char* op1, char* op2, char* dest);
void castInt(char* ins, char* op1, char* op2, char* dest);
void bwNOT(char* ins, char* op1, char* op2, char* dest);
void not(char* ins, char* op1, char* op2, char* dest);
void callFunc(char* ins, char* op1, char* op2, char* dest);
void startProg(char* ins, char* op1, char* op2, char* dest);
void jmp(char* ins, char* op1, char* op2, char* dest);
void push(char* ins, char* op1, char* op2, char* dest);
void pop(char* ins, char* op1, char* op2, char* dest);
void newStack(char* ins, char* op1, char* op2, char* dest);
void funcRet(char* ins, char* op1, char* op2, char* dest);
void halt(char* ins, char* op1, char* op2, char* dest);
void noop(char* ins, char* op1, char* op2, char* dest);
/////////////////////////////////////////////////////////////////////
//NEEDS TO BE REPLACED IN ORDER TO INTEGRATE
FILE *file = fopen("test.txt", "a");
//REMOVE THIS DRIVER
void main()
{
    int x = 8;
    char* a = "/2";
    char* b = "/4";
    char* c = "/6";
    char* d = "enter two integers: ";
    char add = 'a';
    char e = 'c';
    char* f = "0";
    char* g = "-11";
    char h = 'h';
    
    create();
    addData(x, d);
    firstLine(28, 5);
    writeIns4(add, a, b, c);
    writeIns3(e, f, g);
    writeIns1(h);

}
////////////////////////////////////////////////////////////////////
//function def

//sets address 0 in the *.q file
void create()
{
     fprintf(file, "000 0\n");
}
void addData(int Label, char* data)
{
    fprintf(file, "%d \"%s\"\n", Label, data);
}
void firstLine(int mainLoc, int dataSpace)
{
    fprintf(file, "$ %d %d\n", mainLoc, dataSpace);
    
}
///////////////////////////////////////////////////////////////////
void writeIns1(char ins)
{
    fprintf(file, "%c\n", ins);
}
void writeIns2(char ins, char* dest)
{
    fprintf(file, "%c %s\n", ins, dest);
}
void writeIns3(char ins, char* op1, char* dest)
{
    fprintf(file, "%c %s %s \n", ins, op1, dest);
}
void writeIns4(char ins, char* op1, char* op2, char* dest)
{
    fprintf(file, "%c %s %s %s\n", ins, op1, op2, dest);
}
/////////////////////////////////////////////////////////////////////////////////////
//NOT NEEDED I DONT THINK BUT IM NOT DELETING IT JUST IN CASE WE HAVE TO GO THIS WAY
/////////////////////////////////////////////////////////////////////////////////////
void addI(char* ins, char* op1, char* op2, char* dest)
{
    fprintf(file, "%c %s %s %s\n", ins, op1, op2, dest);
}
void addF(char* ins, char* op1, char* op2, char* dest)
{
    fprintf(file, "");
}
void subI(char* ins, char* op1, char* op2, char* dest)
{
    fprintf(file, "");
}
void subF(char* ins, char* op1, char* op2, char* dest)
{
    fprintf(file, "");
}
void mulI(char* ins, char* op1, char* op2, char* dest)
{
    fprintf(file, "");
}
void mulF(char* ins, char* op1, char* op2, char* dest)
{
    fprintf(file, "");
}
void divI(char* ins, char* op1, char* op2, char* dest)
{
    fprintf(file, "");
}
void divF(char* ins, char* op1, char* op2, char* dest)
{
    fprintf(file, "");
}
void residue(char* ins, char* op1, char* op2, char* dest)
{
    fprintf(file, "");
}
void bwOR(char* ins, char* op1, char* op2, char* dest)
{
    fprintf(file, "");
}
void bwAND(char* ins, char* op1, char* op2, char* dest)
{
    fprintf(file, "");
}
void jml(char* ins, char* op1, char* op2, char* dest)
{
    fprintf(file, "");
}
void jmg(char* ins, char* op1, char* op2, char* dest)
{
    fprintf(file, "");
}
void jme(char* ins, char* op1, char* op2, char* dest)
{
    fprintf(file, "");
}
void identity(char* ins, char* op1, char* op2, char* dest)
{
    fprintf(file, "");
}
void byteCopy(char* ins, char* op1, char* op2, char* dest)
{
    fprintf(file, "");
}
void castFloat(char* ins, char* op1, char* op2, char* dest)
{
    fprintf(file, "");
}
void castInt(char* ins, char* op1, char* op2, char* dest)
{
    fprintf(file, "");
}
void bwNOT(char* ins, char* op1, char* op2, char* dest)
{
    fprintf(file, "");
}
void not(char* ins, char* op1, char* op2, char* dest)
{
    fprintf(file, "");
}
void callFunc(char* ins, char* op1, char* op2, char* dest)
{
    fprintf(file, "");
}
void startProg(char* ins, char* op1, char* op2, char* dest)
{
    fprintf(file, "");
}
void jmp(char* ins, char* op1, char* op2, char* dest)
{
    fprintf(file, "");
}
void push(char* ins, char* op1, char* op2, char* dest)
{
    fprintf(file, "");
}
void pop(char* ins, char* op1, char* op2, char* dest)
{
    fprintf(file, "");
}
void newStack(char* ins, char* op1, char* op2, char* dest)
{
    fprintf(file, "");
}
void funcRet(char* ins, char* op1, char* op2, char* dest)
{
    fprintf(file, "");
}
void halt(char* ins, char* op1, char* op2, char* dest)
{
    fprintf(file, "");
}
void noop(char* ins, char* op1, char* op2, char* dest)
{
    fprintf(file, "");
}
