#include "encode.hpp"

std::ifstream::pos_type filesize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

void encode(std::string in_im, std::string in_data, std::string out_im)
{
    //reading input image
    cv::Mat image = cv::imread(in_im);
    if (image.empty())
    {
        LOG("image error");
        return;
    }

    //reading input data file
    std::ifstream inFile(in_data);

    if (!inFile.is_open())
    {
        LOG("input file error");
        return;
    }

    if(filesize((in_im.c_str())) < filesize(in_data.c_str()))
    {
        LOG("Image not big enough");
        return;
    }

    // current character from input file
    char ch;
    // get first char of input file
    inFile.get(ch);
    // current bit to work on
    int bit_count = 0;
    // reached EOF or not
    bool reachedEOF = false;
    // check if whole message is encoded
    bool encoded = false;
    // curren pixel
    cv::Vec3b pixel;

    bool encodedFinal = false;
    std::string finalMessage = FINAL_STRING;
    int finalStringIndex = 0;

    for (int row = 0; row < image.rows; row++)
    {
        for (int col = 0; col < image.cols; col++)
        {
            for (int color = 0; color < 3; color++)
            {
                pixel = image.at<cv::Vec3b>(cv::Point(col, row));
                // if the current bit is 1 : change the current LSB of this pixel to 1
                // if it's 0 : change the LSB to 0

                if (isBitSet(ch, 7 - bit_count))
                {
                    pixel.val[color] |= 1; // or with 1
                }
                else
                {
                    pixel.val[color] &= ~1; // and with inverse 1 (11111110)
                }

                //update the image with the changed pixel
                image.at<cv::Vec3b>(cv::Point(col, row)) = pixel;

                // next bit of input data
                bit_count++;

                // if reached end of file and we're on the last bit, then we finished encoding
//                if (reachedEOF && bit_count == 8 && encodedFinal)
//                {
//                    encoded = true;
//                    goto OUT;
//                }

                // on last bit but we didn't finish the file, get next char
                if (bit_count == 8)
                {
                    bit_count = 0;
                    // if EOF, then we need to encode_text the NULL char
                    if (inFile.eof())
                    {
                        reachedEOF = true;
                        if(finalStringIndex == finalMessage.length())
                        {
                            encoded = true;
                            encodedFinal = true;
                            goto OUT;
                        }
                        else
                        {
                            ch = finalMessage[finalStringIndex];
                            finalStringIndex++;
                        }
                    }
                    else
                    {
                        inFile.get(ch);
                    }
                }
            }
        }
    }

    OUT:;

    // check if we managed to encode_text
    if (!encoded)
    {
        std::cout << "Failed to encode_text, try bigger image" << std::endl;
        return;
    }
    // write steg image to system
    LOG("SUCCESSFULLY ENCODED GIVEN DATA AND OUTPUT TO " << out_im);
    cv::imwrite(out_im, image);
}