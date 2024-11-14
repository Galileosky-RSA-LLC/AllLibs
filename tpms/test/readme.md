# Тест для библиотеки TPMS

## Полезные команды

// получить параметры шины    
TPMSLIBTEST 1,wheelId

// получить параметры всех шин    
TPMSLIBTEST 2

// установить параметры шины    
TPMSLIBTEST 3,wheelId,pres,temp,online,ok,reason

// записать тег шины    
TPMSLIBTEST 4,wheelId,savePoint

// записать тег всех шин    
TPMSLIBTEST 5,savePoint

// очистить тег всех шин    
TPMSLIBTEST 6,savePoint