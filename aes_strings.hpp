#ifndef _AES_STRINGS_
#define _AES_STRINGS_

#include <string>

namespace aes_const{

const std::string MENU = "\n***** AES 128 Bit Encryption Program *****\n\n"
                        "[1] Encrypt .txt file.\n"
                        "[2] Test encryption and keygen.\n"
                        "[3] Description of options.\n"
                        "[ANY OTHER KEY] Quit the program\n\n"
                        "Enter your choice: ";

const std::string INFO =
    "\n[1]: Encodes a .txt file in 128 Bit AES.\n"
     "[1]: Enter the name of the input and output files.\n"
     "[1]: .txt extension is automatically added.\n\n"

     "[2]: Test to ensure correct encryption operation.\n\n"
     "State is:\t\tThe cipher key is:\tThe results should be:\n"

     "Values and expected results sourced from the following website:\n"

     "http://www.cs.bc.edu/~straubin/cs381-05/"
     "blockciphers/rijndael_ingles2004.swf\n\n"
     "It should prove that this program works correctly.\n";

const std::string ENTER_KEY =
    "\nEnter 16 characters for the key.\n"
    "Less than 16 will result in blanks being used "
    "which will make the file easier to crack.\n\n"
    "Enter the key here: ";

}

#endif // _AES_STRINGS_
