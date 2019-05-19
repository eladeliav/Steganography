#include "decode.hpp"

std::string decode_text(std::string in_im, std::string out_F)
{
    std::string message = "";
    cv::Mat image = cv::imread(in_im);
    if (image.empty())
    {
        return "IMAGE ERROR";
    }

    std::ofstream outFile(out_F);

    if(!outFile.is_open())
    {
        return "output file error";
    }

    //current char
    char ch = 0;
    // current bit
    int bit_count = 0;
    // current pixel
    cv::Vec3b pixel;
    std::ostringstream buf;
    
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
					if(buf.str().length() == LEN_OF_FINAL_STRING && buf.str() == FINAL_STRING)
						goto OUT;
					else if(buf.str().length() == LEN_OF_FINAL_STRING)
					    buf.str("");

					bit_count = 0;
					//message += ch;
					outFile.put(ch);
					buf << ch;
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
    return "SUCCESSFULLY OUTPUT DECODED DATA TO " + out_F;
}