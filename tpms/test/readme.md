# Тест библиотеки TPMS

## Полезные команды

// Инициализировать все шины значениями по умолчанию
`TPMSLIBTEST 1`

// получить параметры шины  
`TPMSLIBTEST 2,wheelId`

// получить параметры всех шин  
`TPMSLIBTEST 3`

// установить параметры шины  
`TPMSLIBTEST 4,wheelId,pres,temp,isOnline,isOk,reason`

// записать тег шины  
`TPMSLIBTEST 5,wheelId,needSavePoint`

// записать тег всех шин  
`TPMSLIBTEST 6,needSavePoint`

// очистить тег всех шин  
`TPMSLIBTEST 7,needSavePoint`
