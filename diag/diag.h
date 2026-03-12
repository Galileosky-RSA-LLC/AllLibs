//! @file
//! @brief Заголовок библиотеки расширенного вывода в диагностику

#if defined DIAG_H
#endinput
#endif
#define DIAG_H

//! @brief Вывести в диагностику текстовые представления битов числа, разделенные символом ";"
//! @details Максимальная длина строки диагностики 95 символов. 
//! @details Например: .number=13,.texts=["A","B","C","D","E"],.textsCount=5,.prefix="Warning:" -> "Warning:A;C;D"
//! @param[in] number число для битового представления
//! @param[in] texts тексты на каждый бит числа. Текст добавляется в диагностику, если соотв. его индексу бит == true
//! @param[in] textsCount количество текстовых строк
//! @param[in] prefix добавляемый префикс строки диагностики
forward stock num2bitDiag(number, const texts[][], textsCount, const prefix{} = "");

//! @brief Вывод в диагностику подмассива в hex и его строковой интерпретации с подписью и разделением на читабельные части
//! @param[in] ar массив для вывода
//! @param[in] arSize размер массива
//! @param[in] arName выводимая подпись, должна оканчиваться \0
//! @param[in] needStr признак необходимости вывода строкового представления подмассива
//! @param[in] pos индекс начала вывода
forward stock diagAr(const ar{}, arSize, const arName{}, bool:needStr = false, pos = 0);
