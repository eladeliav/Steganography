#include "encode.hpp"

std::ifstream::pos_type filesize(const char *filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

// if the current bit is 1 : change the current LSB of this pixel to 1
// if it's 0 : change the LSB to 0
void encodeBit(cv::Vec3b& pixel, int colorLayer , char ch, int pos)
{
    if (isBitSet(ch, 7 - pos))
    {
        pixel.val[colorLayer] |= 1; // or with 1
    } else
    {
        pixel.val[colorLayer] &= ~1; // and with inverse 1 (11111110)
    }
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
    //LOG("in_im filesize: " << filesize(in_im.c_str()) << ", in_data filesize: " << filesize(in_data.c_str()));
    if (filesize((in_im.c_str())) < filesize(in_data.c_str()))
    {
        LOG("Image not big enough");
        return;
    }

    // data size
    std::string sizeString = std::to_string(filesize(in_data.c_str()));
    int sizeStringIndex = 0;
    bool encodedSize = false;

    // current character from input file
    char ch = sizeString.at(0);
    // current bit to work on
    int bit_count = 0;
    // check if whole message is encoded
    bool encoded = false;
    // current pixel
    cv::Vec3b pixel;

    for (int row = 0; row < image.rows; row++)
    {
        for (int col = 0; col < image.cols; col++)
        {
            for (int color = 0; color < 3; color++)
            {
                pixel = image.at<cv::Vec3b>(cv::Point(col, row));

                encodeBit(pixel, color, ch, bit_count);

                //update the image with the changed pixel
                image.at<cv::Vec3b>(cv::Point(col, row)) = pixel;

                // next bit of input data
                bit_count++;

                // on last bit but we didn't finish the file, get next char
                if (bit_count == 8)
                {
                    encodedSize = sizeStringIndex == sizeString.length();
                    bit_count = 0;
                    // if EOF, then we need to encode_text the NULL char
                    if (inFile.eof())
                    {
                        encoded = true;
                        goto OUT;
                    }
                    if(encodedSize)
                        inFile.get(ch);
                    else
                        ch = sizeString[++sizeStringIndex];
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
    inFile.close();
}