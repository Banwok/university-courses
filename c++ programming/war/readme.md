# Пьяница

В этой задаче вам предстоит просимулировать карточную игру "Пьяница".

В игре в пьяницу карточная колода раздается поровну двум игрокам. Далее они вскрывают по одной верхней карте, и тот, чья карта старше, забирает себе обе вскрытые карты, которые кладутся под низ его колоды. Тот, кто остается без карт --- проигрывает.

Для простоты будем считать, что все карты различны по номиналу, а также, что самая младшая карта побеждает самую старшую карту ("шестерка берет туза").

Игрок, который забирает себе карты, сначала кладет под низ своей колоды карту первого игрока, затем карту второго игрока (то есть карта второго игрока оказывается внизу колоды).

В игре участвует 10 карт, имеющих значения от 0 до 9, большая карта побеждает меньшую, карта со значением 0 побеждает карту 9.

Реализуйте функцию `SimulateWarGame`, которая принимает значения карт первого и второго игроков (карты перечислены сверху вниз,
т.е. первые элементы массивов будут открыты первыми) и возвращает результат игры.

В структуре с результатом игры помимо игрока-победителя запишите номер хода, на котором была одержана победа. Если игра так и не была
закончена после $`10^6`$ ходов, поле с результатом должно содержать `kNone` (см. заголовочный файл).

### Примечания
* Используйте [std::queue](http://en.cppreference.com/w/cpp/container/queue), чтобы хранить текущую колоду для каждого игрока.
* Контейнер [std::array](http://en.cppreference.com/w/cpp/container/array) является простой оберткой над обычным массивом, поэтому
в отличие от вектора не поддерживает расширение, а его размер должен быть известен на этапе компиляции. Однако в случаях, когда
размер заранее известен и фиксирован, этот контейнер предпочтительнее вектора.