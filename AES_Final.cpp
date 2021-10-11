#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdio>
#include <cstdlib>

using namespace std;

FILE *infile, *outfile; // input file pointer and output file pointer
unsigned char T_Box[4][256][4];
unsigned char T_Box_Inv[4][256][4];
unsigned char multiply(unsigned char a, unsigned char b);
void columnMajor(unsigned char state[4][4], unsigned char plainText[16]);

unsigned char S_Box[256] =
{
    //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, //0
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, //1
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, //2
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, //3
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, //4
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, //5
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, //6
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, //7
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, //8
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, //9
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, //A
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, //B
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, //C
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, //D
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, //E
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16  //F
};

unsigned char S_Box_Inv[256] =
{
    //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

unsigned char Rcon[11] =
{
//   0     1     2     3      4    5     6     7     8    9     10
    0x87, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
};

void print(unsigned char *paddedMessage, int paddedLen)
{
    for(int i=0; i<paddedLen; i++)
        printf("%02X", paddedMessage[i]);
    printf("\n");
}

void KeyExpansionCore(unsigned char *input, unsigned char iter)
{
    //Rotate Left
    unsigned char temp = input[0];
    input[0] = input[1];
    input[1] = input[2];
    input[2] = input[3];
    input[3] = temp;

    //S_Box four Byte
    for(int i = 0; i < 4; i++)
        input[i] = S_Box[input[i]];

    //Rcon
    input[0] ^= Rcon[iter];
}

void KeyExpansion(unsigned char *inputKey, unsigned char *expandedKeys, int numberOfRounds)
{
    int Nk_Byte = (numberOfRounds - 6) * 4;    //128 -> 16, 192 -> 24, 256 -> 32
    //The first 16bytes are original key
    for(int i = 0; i < Nk_Byte; i++)
        expandedKeys[i] = inputKey[i];    //just copy

    //Variables:
    int bytesGenerated = Nk_Byte;    //we've generate the bytes of Nk_Bytes
    int RconInteration = 1;     //Rcon iteration begins at 1
    unsigned char temp[4];      //temp for core

    /*if it is AES-128, there is 10 rounds and 11 * 16 = 176
               AES-192, there is 12 rounds add 13 * 16 = 208
               AES-256, there is 14 rounds add 15 * 16 = 240
    we will run column by column
    */
    while(bytesGenerated < (numberOfRounds + 1) * 16)
    {
        //copy 4 bytes of expandedKeys from last
        for(int i = 0; i < 4; i++)
            temp[i] = expandedKeys[i + bytesGenerated - 4];

        //Perform the core for each 16 byte key, so the first column Round key is different from other's
        if(bytesGenerated % Nk_Byte == 0)
        {
            KeyExpansionCore(temp, RconInteration);
            RconInteration++;
        }
        else if(Nk_Byte == 32 && bytesGenerated % Nk_Byte == 16)
        {
            //Only for AES-256
            //if byteGenerated mod Nk_Byte
            temp[0] = S_Box[temp[0]];
            temp[1] = S_Box[temp[1]];
            temp[2] = S_Box[temp[2]];
            temp[3] = S_Box[temp[3]];
        }
        //XOR temp with [byteGenerated - Nk_Byte], and store in expandedKeys[bytesGenerated]
        for(unsigned char a = 0; a < 4; a++)
        {
            expandedKeys[bytesGenerated] = expandedKeys[bytesGenerated - Nk_Byte] ^ temp[a];
            bytesGenerated++;
        }
    }
}

void SubBytes(unsigned char state[4][4])
{
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j ++)
            state[i][j] =  S_Box[state[i][j]];//The content of state is the S_Box index
}

void ShiftRows(unsigned char state[4][4])
{
    unsigned char temp[4][4];
    //1's row not rotate
    temp[0][0] = state[0][0];
    temp[0][1] = state[0][1];
    temp[0][2] = state[0][2];
    temp[0][3] = state[0][3];

    //2's row left rotate 1 unit
    temp[1][0] = state[1][1];
    temp[1][1] = state[1][2];
    temp[1][2] = state[1][3];
    temp[1][3] = state[1][0];

    //3's row left rotate 2 unit
    temp[2][0] = state[2][2];
    temp[2][1] = state[2][3];
    temp[2][2] = state[2][0];
    temp[2][3] = state[2][1];

    //4's row left rotate 3 unit
    temp[3][0] = state[3][3];
    temp[3][1] = state[3][0];
    temp[3][2] = state[3][1];
    temp[3][3] = state[3][2];

    //state copy from temp
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            state[i][j] = temp[i][j];
}

unsigned char xTime(unsigned char x)
{
    //if the largest bit is 1 then * 0x1b, else just left rotate one bit
    return ((x << 1) ^ ((x >> 7) & 0x1) * 0x1b);
}

unsigned char multiply(unsigned char a, unsigned char b)
{
    unsigned char c = 0;
    unsigned char d = b;
    //to check 8 bits, if it is 1, we do xor
    for (int i = 0; i < 8; i++)
    {
        if (a % 2 == 1)
            c ^= d;
        a /= 2;   //go to check next bit
        d = xTime(d);     //to check this largest
    }
    return c;
}

void MixColumns(unsigned char state[4][4])
{
    unsigned char temp[4];  //temporary for one column
    for(int i = 0; i < 4; i++)
    {
        //to calculate column by column
        temp[0] = state[0][i];
        temp[1] = state[1][i];
        temp[2] = state[2][i];
        temp[3] = state[3][i];
        /*left multiply by
        02 03 01 01
        01 02 03 01
        01 01 02 03
        03 01 01 02
        over GF(2^8)*/
        state[0][i] = multiply(0x02, temp[0]) ^ multiply(0x03, temp[1]) ^ temp[2] ^ temp[3];
        state[1][i] = temp[0] ^ multiply(0x02, temp[1]) ^ multiply(0x03, temp[2]) ^ temp[3];
        state[2][i] = temp[0] ^temp[1] ^ multiply(0x02, temp[2]) ^ multiply(0x03, temp[3]);
        state[3][i] = multiply(0x03, temp[0]) ^ temp[1] ^ temp[2] ^ multiply(0x02, temp[3]);
    }
}

void Inv_SubBytes(unsigned char state[4][4])
{
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j ++)
            state[i][j] =  S_Box_Inv[state[i][j]];//The content of state is the S_Box_Inv index
}

void Inv_ShiftRows(unsigned char state[4][4])
{
    unsigned char temp[4][4];
    //1's row not rotate
    temp[0][0] = state[0][0];
    temp[0][1] = state[0][1];
    temp[0][2] = state[0][2];
    temp[0][3] = state[0][3];

    //2's row right rotate 1 unit
    temp[1][0] = state[1][3];
    temp[1][1] = state[1][0];
    temp[1][2] = state[1][1];
    temp[1][3] = state[1][2];

    //3's row right rotate 2 unit
    temp[2][0] = state[2][2];
    temp[2][1] = state[2][3];
    temp[2][2] = state[2][0];
    temp[2][3] = state[2][1];

    //4's row right rotate 3 unit
    temp[3][0] = state[3][1];
    temp[3][1] = state[3][2];
    temp[3][2] = state[3][3];
    temp[3][3] = state[3][0];

    //state copy from temp
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            state[i][j] = temp[i][j];
}

void Inv_MixColumns(unsigned char state[4][4])
{
    unsigned char temp[4];  //temporary for one column
    for(int i = 0; i < 4; i++)
    {
        //to calculate column by column
        temp[0] = state[0][i];
        temp[1] = state[1][i];
        temp[2] = state[2][i];
        temp[3] = state[3][i];
        /*multiply left by
        0E 0B 0D 09
        09 0E 0B 0D
        0D 09 0E 0B
        0B 0D 09 0E
        over GF(2^8)*/
        state[0][i] = multiply(0x0e, temp[0]) ^ multiply(0x0b, temp[1]) ^ multiply(0x0d, temp[2]) ^ multiply(0x09, temp[3]);
        state[1][i] = multiply(0x09, temp[0]) ^ multiply(0x0e, temp[1]) ^ multiply(0x0b, temp[2]) ^ multiply(0x0d, temp[3]);
        state[2][i] = multiply(0x0d, temp[0]) ^ multiply(0x09, temp[1]) ^ multiply(0x0e, temp[2]) ^ multiply(0x0b, temp[3]);
        state[3][i] = multiply(0x0b, temp[0]) ^ multiply(0x0d, temp[1]) ^ multiply(0x09, temp[2]) ^ multiply(0x0e, temp[3]);
    }
}

void AddRoundKey(unsigned char state[4][4], unsigned char *RoundKey)
{   //Each element do XOR with RoundKey
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            state[j][i] ^= RoundKey[i * 4 + j];	//XOR RoundKey
}

void columnMajor(unsigned char state[4][4], unsigned char plainText[16])
{
    int n = strlen((const char *)plainText);
    //Let plainText do column major store in state
    /*b0  b4  b8  b12
     *b1  b5  b9  b13
     *b2  b6  b10 b14
     *b3  b7  b11 b15*/
    for(int i = 0; i < 16; i++)
        state[i % 4][i / 4] = plainText[i];//if plainText is not larger than 16, add the length of
}

void Generate_Tbox()
{
    unsigned char matrix[4][4] = {
        {2, 1, 1, 3},
        {3, 2, 1, 1},
        {1, 3, 2, 1},
        {1, 1, 3, 2}
    };

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 256; j++)
            for (int k = 0; k < 4; k++)
                T_Box[i][j][k] = multiply(S_Box[j], matrix[i][k]);
}

void Generate_Tbox_Inv()
{
    unsigned char matrix[4][4] = {{0xE, 0x9, 0xD, 0xB},
        {0xB, 0xE, 0x9, 0xD},
        {0xD, 0xB, 0xE, 0x9},
        {0x9, 0xD, 0xB, 0xE}
    };

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 256; j++)
            for (int k = 0; k < 4; k++)
                T_Box_Inv[i][j][k] = multiply(S_Box_Inv[j], matrix[i][k]);
}

void RoundFunction(unsigned char state[4][4], unsigned char *RoundKey)
{
    unsigned char result[4][4];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            result[j][i] = (unsigned char) (T_Box[0][state[0][i]][j]
                                            ^ T_Box[1][state[1][(i + 1) % 4]][j]
                                            ^ T_Box[2][state[2][(i + 2) % 4]][j]
                                            ^ T_Box[3][state[3][(i + 3) % 4]][j]
                                            ^ RoundKey[i * 4 + j]);
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            state[i][j] = result[i][j];
}

void Inv_RoundFunction(unsigned char state[4][4], unsigned char *RoundKey)
{
    unsigned char result[4][4];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            result[j][i] = (unsigned char) (T_Box_Inv[0][state[0][i]][j]
                                            ^ T_Box_Inv[1][state[1][(i + 3) % 4]][j]
                                            ^ T_Box_Inv[2][state[2][(i + 2) % 4]][j]
                                            ^ T_Box_Inv[3][state[3][(i + 1) % 4]][j]
                                            ^ RoundKey[i * 4 + j]);
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            state[i][j] = result[i][j];
}

void Key2InvMixColumns (unsigned char *expandedKeys)
{
    unsigned char temp[4][4];
    for(int i = 0; i < 16; i++)
        temp[i % 4][i / 4] = expandedKeys[i];
    Inv_MixColumns(temp);
    for(int i = 0; i < 16; i++)
        expandedKeys[i] = temp[i % 4][i / 4];
}

void Encrypt(unsigned char *message, unsigned char *inputKey, int numberOfRounds)
{
    unsigned char state[4][4];
    unsigned char expandedKey[240];
    columnMajor(state, message);
    KeyExpansion(inputKey, expandedKey, numberOfRounds);
    AddRoundKey(state, inputKey);

    //AES 10 Round if it is AES-128
    for(int i = 0; i < numberOfRounds - 1; i++)
        RoundFunction(state, expandedKey + (16 * (1 + i)));

    //final Round
    SubBytes(state);
    ShiftRows(state);
    //last is numberOfRounds * 16
    AddRoundKey(state, expandedKey + numberOfRounds * 16);

    //column major to message
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            message[i + j * 4] = state[i][j];
}

void Decrypt(unsigned char *plainText, unsigned char *inputKey, int numberOfRounds)
{
    unsigned char state[4][4];
    unsigned char expandedKey[240];
    columnMajor(state, plainText);
    KeyExpansion(inputKey, expandedKey, numberOfRounds);

    for(int i = 0; i < numberOfRounds - 1; i++)
        Key2InvMixColumns(expandedKey + (16 * (1 + i)));

    AddRoundKey(state, expandedKey + numberOfRounds * 16);  //xor the state and the last 16 bytes of round key

    //AES 10 Round if it is AES-128
    for(int i = 0; i < numberOfRounds - 1; i++)
        Inv_RoundFunction(state, expandedKey + numberOfRounds * 16 - (16 * (i + 1)));

    //final Round
    Inv_ShiftRows(state);
    Inv_SubBytes(state);
    AddRoundKey(state, expandedKey);

    //column major to plainText
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            plainText[i + j * 4] = state[i][j];
}

void AES_ECB_Encrypt(unsigned char *paddedMessage, int paddedLen, unsigned char *key, int numberOfRounds)
{
    for(int i = 0; i < paddedLen; i += 16)
        Encrypt(paddedMessage + i, key, numberOfRounds);   //we can cypher for each 16bytes

    //cout << "\nCypher Message:" << endl;
    for(int i = 0; i < paddedLen; i++)
    {
        //cout << setw(2) << uppercase << hex << (int)paddedMessage[i] << setfill('0') << " ";  //print all the cypher by hexadecimal
        fprintf(outfile, "%c", paddedMessage[i]);
    }
    //print(paddedMessage, paddedLen);
}

void AES_ECB_Decrypt(unsigned char *paddedMessage, int paddedLen, unsigned char *key, int numberOfRounds)
{
    for(int i = 0; i < paddedLen; i += 16)
        Decrypt(paddedMessage + i, key, numberOfRounds);   //we can cypher for each 16bytes

    //cout << "\nDecrypt Message:" << endl;
    for(int i = 0; i < paddedLen; i++)
    {
        //cout << paddedMessage[i];  //print all the cypher by hexadecimal
        fprintf(outfile, "%c", paddedMessage[i]);
    }
    //print(paddedMessage, paddedLen);
}

void XOR(unsigned char *message, unsigned char *pre_message)
{
    for(int i = 0; i < 16; i++)
        message[i] = message[i] ^ pre_message[i];
}

void AES_CBC_Encrypt(unsigned char *paddedMessage, int paddedLen, unsigned char *initialVector, unsigned char *key, int numberOfRounds)
{
    XOR(paddedMessage, initialVector);
    for(int i = 0; i < paddedLen; i += 16)
    {
        if(i != 0)
            XOR(paddedMessage + i, paddedMessage + (i - 16));
        Encrypt(paddedMessage + i, key, numberOfRounds);   //we can cypher for each 16bytes
    }
    //cout << "\nEncrypt Message:" << endl;
    for(int i = 0; i < paddedLen; i++)
    {
        //cout << setw(2) << uppercase << hex << (int)paddedMessage[i] << setfill('0') << " ";  //print all the cypher by hexadecimal
        fputc(paddedMessage[i], outfile);
    }
    //print(paddedMessage, paddedLen);
}
void AES_CBC_Decrypt(unsigned char *paddedMessage, int paddedLen, unsigned char *initialVector, unsigned char *key, int numberOfRounds)
{
    unsigned char *temp = new unsigned char[paddedLen]; //to temporary the original message
    for(int i = 0; i < paddedLen; i++)
        temp[i] = paddedMessage[i];
    for(int i = 0; i < paddedLen; i += 16)
    {
        Decrypt(paddedMessage + i, key, numberOfRounds);   //we can cypher for each 16bytes
        if(i == 0)
            XOR(paddedMessage, initialVector);
        else
            XOR(paddedMessage + i, temp + i - 16);  //XOR with the previous message (not decrypt)
    }
    //cout << "\nDecrypt Message:" << endl;
    for(int i = 0; i < paddedLen; i++)
    {
        //cout << paddedMessage[i];  //print all the cypher by hexadecimal
        fputc(paddedMessage[i], outfile);
    }
    //print(paddedMessage, paddedLen);
    delete []temp;
}
void XOR_8Byte(unsigned char *message, unsigned char *pre_message)
{
    for(int i = 0; i < 8; i++)
        message[i] = message[i] ^ pre_message[i];
}

void AES_CFB8_Encrypt(unsigned char *paddedMessage, int paddedLen, unsigned char *initialVector, unsigned char *key, int numberOfRounds)
{
    unsigned char IVtemp[16], buffer[8];    //buffer is to temp the old IVtemp lowest 8bytes which is not encrypt yet
    for(int i = 0; i < 16; i++)
        IVtemp[i] = initialVector[i];
    for(int i = 0; i < paddedLen; i += 8)
    {
        for(int j = 0; j < 8; j++)
            buffer[j] = IVtemp[j + 8];  //to temp the lowest 8 bytes of IVtemp which is not encrypt yet
        Encrypt(IVtemp, key, numberOfRounds);   //Encrypt the whole IVtemp
        XOR_8Byte(paddedMessage + i, IVtemp);   //highest 8 bytes of IVtemp XOR with the cipher
        for(int j = 0; j < 8; j++)
        {
            IVtemp[j] = buffer[j];  //left shift the old IVtemp lowest 8 bytes to the highest 8 bytes
            IVtemp[j + 8] = paddedMessage[i + j];   //replace the output 8 bytes to the lowest 8 bytes
        }
    }
    //cout << "Encrypt Message:" << endl;
    for(int i = 0; i < paddedLen; i++)
    {
        //cout << setw(2) << uppercase << hex << (int)paddedMessage[i] << setfill('0') << " ";  //print all the cypher by hexadecimal
        fputc(paddedMessage[i], outfile);
    }
    //print(paddedMessage, paddedLen);
}
void AES_CFB8_Decrypt(unsigned char *paddedMessage, int paddedLen, unsigned char *initialVector, unsigned char *key, int numberOfRounds)
{
    unsigned char *paddedOutput = new unsigned char[paddedLen];
    unsigned char IVtemp[16], buffer[8];    //buffer is to temp the old IVtemp lowest 8bytes which is not encrypt yet
    for(int i = 0; i < 16; i++)
        IVtemp[i] = initialVector[i];
    for(int i = 0; i < paddedLen; i += 8)
    {
        for(int j = 0; j < 8; j++)
            buffer[j] = IVtemp[j + 8];  //to temp the lowest 8 bytes of IVtemp which is not encrypt yet
        Encrypt(IVtemp, key, numberOfRounds);   //Encrypt the whole IVtemp
        XOR_8Byte(IVtemp, paddedMessage + i);   //XOR highest 8 bytes of IVtemp with the cipher 8 bytes
        for(int j = 0; j < 8; j++)
        {
            paddedOutput[i + j] = IVtemp[j];    //The highest 8 bytes of IVtemp are the output, and copy to the output buffer
            IVtemp[j] = buffer[j];          //left shift the lowest 8 bytes to the highest 8 bytes
            IVtemp[j + 8] = paddedMessage[i + j];   //replace the cipher 8 bytes to the lowest 8 bytes
        }
    }
    //cout << "\nDecrypt Message:" << endl;
    for(int i = 0; i < paddedLen; i++)
    {
        //cout << paddedOutput[i];  //print all the cypher by hexadecimal
        fputc(paddedOutput[i], outfile);
    }
    //print(paddedOutput, paddedLen);
    delete []paddedOutput;
}

void AES_OFB8(unsigned char *paddedMessage, int paddedLen, unsigned char *initialVector, unsigned char *key, int numberOfRounds, int mode)     //mode is encrypt or decrypt
{
    unsigned char IVtemp[16], buffer[8];    //buffer is to temp the old IVtemp lowest 8bytes which is not encrypt yet
    for(int i = 0; i < 16; i++)
        IVtemp[i] = initialVector[i];
    for(int i = 0; i < paddedLen; i += 8)
    {
        for(int j = 0; j < 8; j++)
            buffer[j] = IVtemp[j + 8];  //to temp the lowest 8 bytes of IVtemp which is not encrypt yet
        Encrypt(IVtemp, key, numberOfRounds);
        XOR_8Byte(paddedMessage + i, IVtemp);   //XOR highest 8 bytes of IVtemp with the input 8 bytes
        for(int j = 0; j < 8; j++)
        {
            IVtemp[j + 8] = IVtemp[j];  //replace the lowest 8 bytes of IVtemp to the highest 8 bytes which is encrypted
            IVtemp[j] = buffer[j];      //the lowest 8 bytes of original IVtemp which is not encrypted yet shifts left 8 bytes to the highest
        }
    }
    if(mode)
    {
        //cout << "\nEncrypt Message:" << endl;
        for(int i = 0; i < paddedLen; i++)
        {
            //cout << setw(2) << uppercase << hex << (int)paddedMessage[i] << setfill('0') << " ";  //print all the cypher by hexadecimal
            fputc(paddedMessage[i], outfile);
        }
    }
    else
    {
        //cout << "\nDecrypt Message:" << endl;
        for(int i = 0; i < paddedLen; i++)
        {
            //cout << paddedMessage[i];
            fputc(paddedMessage[i], outfile);
        }
    }
    //print(paddedMessage, paddedLen);
}

void IVadd(unsigned char *IV)
{
    for(int i = 15; i >= 0; i--)
    {
        IV[i]++;
        if (IV[i])
            break;  //check the byte whether overflow. if no,it will break.
    }
}
void AES_CTR(unsigned char *paddedMessage, int paddedLen, unsigned char *initialVector, unsigned char *key, int numberOfRounds, int mode)     //mode is encrypt or decrypt
{
    unsigned char IVtemp[16];   //copy from IVcount and go to encrypt
    unsigned char IVcount[16];  //original initial vector's counter
    for(int i = 0; i < 16; i++)
    {
        IVtemp[i] = initialVector[i];
        IVcount[i] = initialVector[i];
    }
    for(int i = 0; i < paddedLen; i += 16)
    {
        Encrypt(IVtemp, key, numberOfRounds);
        XOR(paddedMessage + i, IVtemp);     //XOR with the IVtemp which is encrypted
        IVadd(IVcount);     //counter + 1
        for(int j = 0; j < 16; j++)
        {
            IVtemp[j] = IVcount[j];     //copy from the counter
        }
    }
    if(mode)
    {
        //cout << "Encrypt Message:" << endl;
        for(int i = 0; i < paddedLen; i++)
        {
            //cout << setw(2) << uppercase << hex << (int)paddedMessage[i] << setfill('0') << " ";
            fputc(paddedMessage[i], outfile);
        }
    }
    else
    {
        //cout << "Decrypt Message:" << endl;
        for(int i = 0; i < paddedLen; i++)
        {
            //cout << paddedMessage[i];
            fputc(paddedMessage[i], outfile);
        }
    }
    //print(paddedMessage, paddedLen);
}

void EDE_Encrypt(unsigned char *paddedMessage, int paddedLen, unsigned char key[3][32], int numberOfRounds)
{
    /*EDE encryption: E(D(E(plainText, key1), key2), key3),
    plain text Encrypt with first key and Decrypt with second key and finally Encrypt with the last key.*/
    for(int i = 0; i < paddedLen; i += 16)
    {
        Encrypt(paddedMessage + i, key[0], numberOfRounds);
        Decrypt(paddedMessage + i, key[1], numberOfRounds);
        Encrypt(paddedMessage + i, key[2], numberOfRounds);
    }
    //cout << "Encrypt message:" << endl;
    //print the cipher and write in the file
    for(int i = 0; i < paddedLen; i++)
    {
        //cout << setw(2) << uppercase << hex << (int)paddedMessage[i] << setfill('0') << " ";
        fputc(paddedMessage[i], outfile);
    }
    //print(paddedMessage, paddedLen);
}

void EDE_Decrypt(unsigned char *paddedMessage, int paddedLen, unsigned char key[3][32], int numberOfRounds)
{
    /*EDE encryption: E(D(E(plainText, key1), key2), key3),
    cipher text Decrypt with last key and Encrypt with second key and finally Decrypt with the first key.*/
    for(int i = 0; i < paddedLen; i += 16)
    {
        Decrypt(paddedMessage + i, key[2], numberOfRounds);
        Encrypt(paddedMessage + i, key[1], numberOfRounds);
        Decrypt(paddedMessage + i, key[0], numberOfRounds);
    }
    //cout << "Decrypt message:" << endl;
    //print the plain text and write in the file
    for(int i = 0; i < paddedLen; i++)
    {
        //cout << paddedMessage[i];
        fputc(paddedMessage[i], outfile);
    }
    //print(paddedMessage, paddedLen);
}

void paddingMessage(unsigned char *paddedMessage, int paddedLen, unsigned char *message, int originalLen)
{
    for(int i = 0; i < paddedLen; i++)
    {
        if(i >= originalLen)
            paddedMessage[i] = 0;   //Remaining position set 0
        else
            paddedMessage[i] = message[i];
    }
}

//Using function, you need to transfer the double pointer, and the location would open correctly
long openFile(unsigned char **message, char *fileName)
{
    long lSize;
    size_t result;
    //open the file, and to check pointer whether is NULL
    infile = fopen(fileName, "rb");
    while(infile == NULL){
        cout << "Open file Erorr...\n";
        fclose(infile);
        cout << "Enter the file name again:\n";
        cin.getline(fileName, 201);
        infile = fopen(fileName, "rb");
    }

    //read the file size
    fseek (infile, 0, SEEK_END);
    lSize = ftell (infile);
    rewind (infile);
    //allocate a memory space
    *message = new unsigned char[lSize];
    //to check that messages put in the array
    if(*message == NULL)
    {
        fputs ("Memory error", stderr);
        exit (2);
    }
    result = fread (*message, 1, lSize, infile);
    //to check that all message are read in
    if(result != lSize)
    {
        fputs ("Reading error", stderr);
        exit (3);
    }
    //Enter the file you want to write in
    cout << "Enter the file name you want to write in:" << endl;
    cin.getline(fileName, 201);
    //open the file
    outfile = fopen(fileName, "wb");
    cout << "file size:" << lSize << endl;
    return lSize;
}

//this function can input any size of key
void getKey(unsigned char *key){
    int i = 0;
    while(1){
        if(i >= 32){
            char c = getchar();
            if(c == '\n'){
                key[32] = '\0';
                break;
            }
        }else{
            key[i] = getc(stdin);
            if(key[i] == '\n'){
                key[i] = '\0';
                break;
            }
            i++;
        }
    }
}

int main()
{
    int keySize, numberOfRound, paddedLen, lSize, isDecrypt, choice;
    char cont = 'Y';
    unsigned char key[32];
    unsigned char key_3[3][32];
    unsigned char IV[] = "1234567890123456";    //initial vector
    char fileName[200];
    unsigned char *inputkey;
    unsigned char *message;
    unsigned char *paddedMessage;
    Generate_Tbox();
    Generate_Tbox_Inv();
    while(cont == 'Y' || cont == 'y')
    {
        memset(key, 0, sizeof(key));
        memset(key_3, 0, sizeof(key_3));
        do{
            cout << "choose to encrypt or decrypt: (0 or 1):";
            cin >> isDecrypt;
        }while(isDecrypt != 0 && isDecrypt != 1);
        //choose the cases of the encrypt method
        do{
            cout << "1.ECB" << endl;
            cout << "2.CBC" << endl;
            cout << "3.CFB-8" << endl;
            cout << "4.OFB-8" << endl;
            cout << "5.CTR" << endl;
            cout << "6.EDE" << endl;
            cout << "Enter the cases of the encrypt method:(1 or 2 or 3 or 4 or 5 or 6) : ";
            cin >> choice;
        }while(choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5 && choice != 6);

        do
        {
            cout << "Choose the key size (only enter 128 or 192 or 256) : ";
            cin >> keySize;
        }
        while(keySize != 128 && keySize != 192 && keySize != 256);
        numberOfRound = (keySize / 32) + 6;     //AES-128:10rounds(192/32 + 6), AES-192:12rounds, AES-256:14rounds
        char c = getchar();
        numberOfRound = (keySize / 32) + 6;     //AES-128:10rounds(192/32 + 6), AES-192:12rounds, AES-256:14rounds
        //enter key, if EDE, we will enter 3 keys, or we just enter one key
        if(choice == 6)
        {
            for(int i = 0; i < 3; i++)
            {
                if(keySize == 128)
                {
                    cout << "Enter the Key (16 characters) : ";
                }
                else if(keySize == 192)
                {
                    cout << "Enter the Key (24 characters) : ";
                }
                else if(keySize == 256)
                {
                    cout << "Enter the Key (32 characters) : ";
                }
                getKey(key_3[i]);   //to input the key
                //cout << endl;
                //cin.getline((char*)key_3[i], 33);
                //cout << key_3[i] << endl;
            }
        }
        else
        {
            if(keySize == 128)
            {
                cout << "Enter the Key (16 characters) : ";
            }
            else if(keySize == 192)
            {
                cout << "Enter the Key (24 characters) : ";
            }
            else if (keySize == 256)
            {
                cout << "Enter the Key (32 characters) : ";
            }
            getKey(key);    //to input the key
            //cout << key << endl;
            //cout << key << endl;
        }
        if(!isDecrypt)
            cout << "Enter the file name you want to cipher" << endl;
        else
            cout << "Enter the file name you want to decrypt" << endl;
        //enter the file name you want to cipher
        cin.getline(fileName, 201);
        lSize = openFile(&message, fileName);
        paddedLen = lSize;
        //if length is 16 times, we don't need to expand the paddedLen
        if(paddedLen % 16 != 0)
        {
            //find the total length(16 * n) which we want to cypher. Where n is the integer
            paddedLen = (paddedLen / 16 + 1) * 16;
        }
        cout << "paddedLen:" << paddedLen << endl;
        paddedMessage = new unsigned char[paddedLen];
        //padding function
        paddingMessage(paddedMessage, paddedLen, message, lSize);

        /*if(isDecrypt == 0)
            print(paddedMessage, paddedLen);
        */
        if(!isDecrypt)
        {
            switch(choice)
            {
            case 1:
                AES_ECB_Encrypt(paddedMessage, paddedLen, key, numberOfRound);
                break;
            case 2:
                AES_CBC_Encrypt(paddedMessage, paddedLen, IV, key, numberOfRound);
                break;
            case 3:
                AES_CFB8_Encrypt(paddedMessage, paddedLen, IV, key, numberOfRound);
                break;
            case 4:
                AES_OFB8(paddedMessage, paddedLen, IV, key, numberOfRound, 1);    //if 1:encrypt
                break;
            case 5:
                AES_CTR(paddedMessage, paddedLen, IV, key, numberOfRound, 1);    //if 1:encrypt
                break;
            case 6:
                EDE_Encrypt(paddedMessage, paddedLen, key_3, numberOfRound);
                break;
            }
            cout << "Encrypt Success!" << endl;
        }
        else
        {
            switch(choice)
            {
            case 1:
                AES_ECB_Decrypt(paddedMessage, paddedLen, key, numberOfRound);
                break;
            case 2:
                AES_CBC_Decrypt(paddedMessage, paddedLen, IV, key, numberOfRound);
                break;
            case 3:
                AES_CFB8_Decrypt(paddedMessage, paddedLen, IV, key, numberOfRound);
                break;
            case 4:
                AES_OFB8(paddedMessage, paddedLen, IV, key, numberOfRound, 0);    //else:decrypt
                break;
            case 5:
                AES_CTR(paddedMessage, paddedLen, IV, key, numberOfRound, 0);    //else:decrypt
                break;
            case 6 :
                EDE_Decrypt(paddedMessage, paddedLen, key_3, numberOfRound);
                break;
            }
            cout << "Decrypt Success!" << endl;
        }
        fclose(infile);
        fclose(outfile);
        delete []message;
        delete []paddedMessage;
        do{
            cout << "Continue?(Y/y or N/n):";
            cin >> cont;
        }while(cont != 'Y' && cont != 'y' && cont != 'N' && cont != 'n');
    }
    system("pause");
    return 0;
}
