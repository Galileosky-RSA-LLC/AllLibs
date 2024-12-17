#ifndef STRING_H
#define STRING_H
// Библиотека для работы со строками

#define NUM_VALUE_MIN_STR "-2147483648"
#define NUM_VALUE_MIN_STR_LENGTH 11
#define NUM_VALUE_MAX_STR "2147483647"
#define NUM_VALUE_MAX_STR_LENGTH 10
#define BASE64_BLOCK_SIZE 4
#define BASE64_PADDING '='
#define BASE64_ALPHABET_SIZE 64
stock const BASE64_ALPHABET{} ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"


atofi(const str{}, pos, length, separator, precision, &value);
atoi(const str{}, pos, length, &value);
base64Encode(const in{}, inSize, inPos, out{}, outSize, outPos = 0);
isBase64(ch);
base64Decode(const in{}, inSize, inPos, out{}, outSize, outPos = 0);
base64StrLength(inSize);
fromBase64StrMaxSize(base64Size);
fromBase64StrMinSize(base64Size);
decfractoa(num, exp, str{}, pos, length, separator);
getHexString(const ar{}, arStart, arSize, str{}, strSize, strPos = 0);
toAsciiHex(ar{}, arLength, subLength, start = 0);
asciiHexStr2array(const asciiStr{}, asciiStrSize, asciiStrPos, hex{}, hexMaxSize);
asciiHex2num(byte);
isAsciiHex(byte);
itoa(num, str{}, pos, length);
itoaw(num, str{}, pos, length, width);
isDigit(byte);
getDigit(byte);
num2asciiHexHalfByte(num);
numLength(num);
toLowerCase(str{}, strLength, start = 0, ignoreNull = false);
toUpperCase(str{}, strLength, start = 0, ignoreNull = false);
strLen(const str{}, strLength = 0, start = 0);
replaceStr(strDest{}, strDestPos, strDestLength, const strSource{}, strSourceLength, strSourcePos = 0);
unread2space(str{}, strLength, start = 0, ignoreNull = false);
strSplitNums(const str{}, strSize, pos, separator, values[], valuesMaxSize, &valuesActualSize);

#endif // STRING_H