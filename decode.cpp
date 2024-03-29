#include "decode.hpp"

void decode_text(std::string in_im, std::string out_F)
{
    cv::Mat image = cv::imread(in_im);
    if (image.empty())
    {
        LOG("IMAGE ERROR");
        return;
    }

    std::ofstream outFile(out_F, std::ofstream::binary);

    if (!outFile.is_open())
    {
        LOG("output file error");
        return;
    }
    // file size of encoded data
    int fileSize = 0;
    bool receivedSize = false;
    std::string sizeString;
    //current char
    char ch = 0;
    // current bit
    int bit_count = 0;
    // current pixel
    cv::Vec3b pixel;
    std::string message;
    int currentSize = 0;

    for (int row = 0; row < image.rows; row++)
    {
        for (int col = 0; col < image.cols; col++)
        {
            for (int color = 0; color < 3; color++)
            {
                pixel = image.at<cv::Vec3b>(cv::Point(col, row));
                // manipulate char bits according to the LSB of pixel values
                if (isBitSet(pixel.val[color], 0))
                    ch |= 1;

                // increment bit_count to work on next bit
                bit_count++;
                // bit_count is 8, that means we got our char from the encoded image
                if (bit_count == 8)
                {
                    if(receivedSize && currentSize == fileSize)
                    {
                        goto OUT;
                    }
                    if(receivedSize)
                    {
                        currentSize++;
                        message += ch;

                    }else
                    {
                        sizeString += ch;
                    }
                    if(ch == '$' && !receivedSize)
                    {
                        fileSize = std::stoi(sizeString);
                        LOG("decoded file size: " << fileSize);
                        receivedSize = true;
                    }

                    bit_count = 0;
                    ch = 0;
                } else
                {
                    ch = ch << 1;
                }
            }
        }
    }

    OUT:;
    outFile << message;
    outFile.close();
    LOG("SUCCESSFULLY OUTPUT DECODED DATA TO " << out_F);
}