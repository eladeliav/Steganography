#include "encode.hpp"

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

    // current character from input file
    char ch;
    // get first char of input file
    inFile.get(ch);
    // current bit to work on
    int bit_count = 0;
    // reached EOF or not
    bool last_null_char = false;
    // check if whole message is encoded
    bool encoded = false;
    // curren pixel
    cv::Vec3b pixel;
    std::string message;
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
                if (last_null_char && bit_count == 8)
                {
                    encoded = true;
                    goto OUT;
                }

                // on last bit but we didn't finish the file, get next char
                if (bit_count == 8)
                {
                    bit_count = 0;
                    message += ch;
                    inFile.get(ch);

                    // if EOF, then we need to encode the NULL char
                    if (inFile.eof())
                    {
                        last_null_char = true;
                        ch = '\0';
                    }
                }
            }
        }
    }

    OUT:;

    // check if we managed to encode
    if (!encoded)
    {
        std::cout << "Failed to encode, try bigger image" << std::endl;
        return;
    }
    // write steg image to system
    //LOG("Successfully encoded image with " << message);
    cv::imwrite(out_im, image);
}