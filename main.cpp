#include "steg.hpp"
#include "encode.hpp"
#include "decode.hpp"

#define ENCODE "encode"
#define DECODE "decode"
#define ENCODE_PARAM_NUM 5
#define DECODE_PARAM_NUM 4

bool isBitSet(char ch, int pos)
{
    //7 6 5 4 3 2 1 0
    //0 0 0 0 0 0 0 1
    ch = ch >> pos;
    if (ch & 1)
        return true;
    return false;
}

int main(int argc, char **argv)
{
    /*
	argv[0] = ./steg
    argv[1] = encode_text/decode_text
                |       \
                |        \
                |         \
                |          \
                |           \
                |           argv[2] = steganographic_image
                |           argv[3] = output_data
	argv[2] = input_image
	argv[3] = intput_data
	argv[4] = output_image
	*/

    if(argc <= 1)
    {
        LOG("not enough params");
        return 1;
    }

    bool encoding = true;
    std::string command = argv[1];

    // making sure there are the right number of params
    if (argc <= 1 && command.find(ENCODE) == std::string::npos &&
        command.find(DECODE) == std::string::npos)
    {
        LOG("Unrecognized command\nPlease use encode/decode");
        return 1;
    }

    encoding = command.find(ENCODE) != std::string::npos;
    int expectedParamNum = (encoding) ? ENCODE_PARAM_NUM : DECODE_PARAM_NUM;

    if (argc != expectedParamNum)
    {
        LOG("Unexpected number of parameters");
        return 1;
    }


    if (encoding)
        encode(argv[2], argv[3], argv[4]);
    else
        decode_text(argv[2], argv[3]);
    return 0;
}