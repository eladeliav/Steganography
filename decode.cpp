#include "decode.hpp"

std::string decode(std::string in_im, std::string out_im)
{
    std::string message = "";
    cv::Mat image = cv::imread(in_im);
    if (image.empty())
    {
        return "IMAGE ERROR";
    }

    //reading input data file
    std::ofstream outFile(out_im);

    if (!outFile.is_open())
    {
        return "input file error";
    }

    //current char
    char ch = 0;
    // current bit
    int bit_count = 0;
    // current pixel
    cv::Vec3b pixel;
    
    for(int row = 0; row < image.rows; row++)
    {
        for(int col = 0; col < image.cols; col++)
        {
            for(int color = 0; color < 3; color++)
            {
                pixel = image.at<cv::Vec3b>(cv::Point(col, row));
                // manipulate char bits according to the LSB of pixel values
				if(isBitSet(pixel.val[color],0))
					ch |= 1;

				// increment bit_count to work on next bit
				bit_count++;

				// bit_count is 8, that means we got our char from the encoded image
				if(bit_count == 8) {

					// NULL char is encountered
					if(ch == '\0')
						goto OUT;

					bit_count = 0;
                    outFile.put(ch);
					//std::cout << ch;
					ch = 0;
				}
				else {
					ch = ch << 1;
				}
                
            }
        }
    }
    OUT:;
    return message;
}