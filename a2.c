#include "a2.h"

int bitwise_xor(int value){
    return value ^ KEY;
}

char *xor_encrypt(char c){
    //generate 7-digit binary string representing result of XOR on c
    char *binstr = (char *)malloc(sizeof(char) * 8);
    binstr[7] = '\0';

    int temp_c = bitwise_xor(c);

    char temp;

    for (int i = 6; i >= 0; i--){
        temp = temp_c % 2+'0';
        binstr[i] = temp;
        temp_c /= 2;
    }

    return binstr;
}

char xor_decrypt(char *s){
    //deciphers what character is represented by s

    //return int
    int temp_int = 0;

    //int to be exponentiated
    int exp_int = 0;

    //starting binary term
    int exp = 0;

    //iteration exponentiation term
    int exp_iter = 0;

    //build the integer
    for (int i = 6; i >= 0; i--){
        exp_int = 1;
        exp_iter = exp;

        //exponentiate
        while(exp_iter > 0){
            // printf("what");
            exp_int *= 2;
            exp_iter--;
        }

        temp_int += (s[i]- 48)* exp_int;
        exp++;
    }
    return (char)bitwise_xor(temp_int);
}

char *gen_code(char *msg){
    //make space for sc
    char *sc_pointer = (char *)malloc(257 * sizeof(char));
    sc_pointer[256]= '\0';

    //keep track of msg len
    int msg_char_len = strlen(msg);

    //initialize SC code
    for(int i = 0; i<256; i++){
        sc_pointer[i] = '_';
    }

    //set ones spots
    int zeros[] = {17,18,19,28,29,30,33,35,44,46,49,50,51,60,61,62,193,194,195,209,211,225,226,227};
    for (int j = 0; j<24; j++){
        sc_pointer[zeros[j]] = '0';
    }

    //set zero spots
    int ones[] = {
        0,1,2,3,4,11,12,13,14,15,16,20,27,31,32,34,36,43,45,47,48,52,59,63,64,65,66,67,68,75,76,77,78,79,
        176,177,178,179,180,192,196,208,210,212,224,228,240,241,242,243,244,255
    };
    for (int h = 0; h<52; h++){
        sc_pointer[ones[h]] = '1';
    }

    //keeping track of free indicies
    int free_ind[180];

    //find the free indicies
    int temp_ind = 0;
    for(int x = 0; x<256; x++){
        if (sc_pointer[x] == '_'){
            free_ind[temp_ind] = x;
            temp_ind++;
        }
    }

    //initialize free inds to zero
    for(int z = 0; z<180; z++){
        sc_pointer[free_ind[z]] = '0';
    }

    int iter_ind = 0;
    //encrypting msg
    for(int p = 0; p<msg_char_len; p++){
        char* xor_char = xor_encrypt(msg[p]);
        for(int m = 0; m<7; m++){
            sc_pointer[free_ind[iter_ind]] = xor_char[m];
            iter_ind++;
        }
        free(xor_char);
    }
    //ending with a termination character
    char* end_char = xor_encrypt('\0');
    for(int y = 0; y<7; y++){
        sc_pointer[free_ind[iter_ind]] = end_char[y];
        iter_ind++;
    }
    free(end_char);

    // print_code(sc_pointer);

    return sc_pointer;
}

char *read_code(char *code){
    //deciphers message in the SC code
    //max char 25 - 175 readable

    char * retstr = (char *)malloc(26 * sizeof(char));
    retstr[25] = '\0';

    char* temp = (char *)malloc(7 * sizeof(char));

    int free_ind[] = {
         5, 6, 7, 8, 9,10,
        21,22,23,24,25,26,
        37,38,39,40,41,42,
        53,54,55,56,57,58,
        69,70,71,72,73,74,
        80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
        96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,
        112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,
        128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
        144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
        160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,
        181,182,183,184,185,186,187,188,189,190,191,
        197,198,199,200,201,202,203,204,205,206,207,
        213,214,215,216,217,218,219,220,221,222,223,
        229,230,231,232,233,234,235,236,237,238,239,
        245,246,247,248,249,250,251,252,253,254
    };
    
    int code_ind = 0;
    int temp_ind = 0;
    int ret_ind = 0;
    for(int i = 0; i<25; i++){
        temp_ind = 0;
        for(int j = 0; j<7;j++){
            temp[temp_ind] = code[free_ind[code_ind]];
            code_ind++;
            temp_ind++;
        }

        char input_char = xor_decrypt(temp);

        retstr[ret_ind] = input_char;

        if(input_char == '\0'){
            free(temp);
            return retstr;
        }
        ret_ind++;
    }

}

char hexequiv(char* split){
    char* hexConstant[] = {"0000","0001","0010","0011","0100","0101","0110","0111","1000","1001","1010","1011","1100","1101","1110","1111"};
    char hexequivalent[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

    for (int k=0; k<16; k++){
        if(strncmp(hexConstant[k],split,4) == 0){
            return hexequivalent[k]; 
        }
    }
}

char *compress(char *code){ 
    char split[4];

    char *hex = (char *)malloc(sizeof(char) * 65);  
    hex[64] = '\0';

    int index = 0;
    int sc_index = 0;

    for (int i = 0; i < 64; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            split[j] = code[sc_index];
            sc_index ++;
        }
        hex[index] = hexequiv(split);
        index ++;
    }

    hex[index] = '\0';

    return hex;
}

char* binaryequiv(char hex){
    char hexConstant[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    char* binaryequivalent[] = {"0000","0001","0010","0011","0100","0101","0110","0111","1000","1001","1010","1011","1100","1101","1110","1111"};
    
    for (int k=0; k<16; k++){
        if(hex == hexConstant[k]){
            return binaryequivalent[k]; 
        }
    }
}

char *decompress(char *code){
    //make space for sc
    char *sc_pointer = (char *)malloc(257 * sizeof(char));
    sc_pointer[256]= '\0';

    //initialize SC code
    for(int p = 0; p<256; p++){
        sc_pointer[p] = '0';
    }

    ///Do whatever :)
    int sc_index = 0;
    char split[4];

    for (int i = 0; i < 64; i++)
    {
        for(int k = 0; k<4; k++){
            split[k] = binaryequiv(code[i])[k];
        }

        for (int j = 0; j < 4; j++)
        {
            sc_pointer[sc_index] = split[j];
            sc_index++;
        }
    }

    sc_pointer[sc_index]= '\0';

    return sc_pointer;
}

int triple_min(int a, int b, int c){
    if(a<b && a<c){
        return a;
    }
    else if(b<c){
        return b;
    }
    else{
        return c;
    }
}

int calc_ld(char *sandy, char *cima){

    //get char lengths
    int s_len = strlen(sandy);
    int c_len = strlen(cima);
    
    int dp_tab[s_len + 1][c_len + 1];

    //set the bounds ie. 0 -> s_len
    for(int i = 0; i<s_len+1; i++){
        for(int j = 0; j<c_len + 1; j++){
            //initialize the case when first char in str is ''
            if(i == 0){
                dp_tab[i][j] = j;
            }
            //initialize if first char in str2 is ''
            else if(j == 0){
                dp_tab[i][j] = i;
            }
            //if char in str1 and str2 is same
            else if(sandy[i-1] == cima[j-1]){
                dp_tab[i][j] = dp_tab[i-1][j-1];
            }
            //if char is diff, find min of the 3 operations
            else{
                dp_tab[i][j] = triple_min(dp_tab[i-1][j-1], dp_tab[i-1][j], dp_tab[i][j-1]) + 1;
            }
        }
    }
    return dp_tab[s_len][c_len];
}