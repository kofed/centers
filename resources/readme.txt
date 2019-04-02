запуск с файлом изображения ./contours -i=resources/1.jpg
запуск с видео файлом ./contours -v=resources/1.avi
Расчет с третьей координатой:
	1. ./centers -v=resources/1.avi
	2. добавивть вручную третью координату в h.yml
	3. ./centers -v=resources/1.avi -add3d

Запуск теста 
	./centers -i=image.png -t
