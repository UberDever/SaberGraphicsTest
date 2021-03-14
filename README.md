# SaberGraphicsTest

Фамилия, имя, отчество выполнившего тест: Орловский Максим Юрьевич

Дата выполнения: 26.02.2021

Примерное количество времени, затраченного на выполнение теста: около 10 часов

1.	Напишите функцию, которая принимает на вход знаковое целое число и печатает его двоичное представление,
	не используя библиотечных классов или функций.

2.	Реализуйте функции сериализации и десериализации двусвязного списка в бинарном формате в файл.
	Алгоритмическая сложность решения должна быть меньше квадратичной.
```c++
	// структуру ListNode модифицровать нельзя
	struct ListNode {
		ListNode * prev;
		ListNode * next;
		ListNode * rand; // указатель на произвольный элемент данного списка, либо NULL
		std::string data;
	};

	class List {
	public:
		void Serialize (FILE * file); // сохранение в файл (файл открыт с помощью fopen(path, "wb"))
		void Deserialize (FILE * file); // загрузка из файла (файл открыт с помощью fopen(path, "rb"))
	private:
		ListNode * head;
		ListNode * tail;
		int count;
	};
```

3. Реализуйте функцию расчета усредненных нормалей для вершин меша, заданного индексированными треугольниками.
```c++
//
// Calculate smooth (average) per-vertex normals
//
// [out] normals - output per-vertex normals
// [in] verts - input per-vertex positions
// [in] faces - triangles (triplets of vertex indices)
// [in] nverts - total number of vertices (# of elements in verts and normals arrays)
// [in] nfaces - total number of faces (# of elements in faces array)
//
void calc_mesh_normals(vec3* normals, const vec3* verts, const int* faces, int nverts, int nfaces)
{
  ...
}
```

# Решение

Решение всех заданий находится в папке Assignments в SaberGraphicsTest.

Все алгоритмические задания сделаны с помощью TDD и gtest, их явная демонстрация не представлена. Это также связано с тем, что сложно продемонстрировать второе задание.

Третье задание также находится в Assignments, но для демонстрации был использован OpenGL с загрузкой obj моделей в разных вариантах.

Далее продемонстрировано решение третьего задания в наглядном варианте:

![Showcase](https://github.com/UberDever/SaberGraphicsTest/blob/master/SaberGraphicsTest/textures/Showcase.png)

Общий план с сглаженными кубами и одним однотонным кубом. Наглядно видна разница, при сглаживании нормалей.

![DetailLook](https://github.com/UberDever/SaberGraphicsTest/blob/master/SaberGraphicsTest/textures/DetailLook.png)

Сравнение моего алгоритма сглаживания и алгоритма из Blender 2.8

![DetailLookCylinders](https://github.com/UberDever/SaberGraphicsTest/blob/master/SaberGraphicsTest/textures/DetailLookCylinders.png)

Сравнение, проведенное на циллиндрах со снятыми фасками

![ShadowComparison](https://github.com/UberDever/SaberGraphicsTest/blob/master/SaberGraphicsTest/textures/ShadowComparison.png)

Очень важное сравнение, объясняющее потребность в сглаживании нормалей с весами: при сглаживании нормалей способом обычного усреднения,
тень получается радиальной - что не лучшим образом отражается на внешнем виде модели.

В отличие от сглаживания нормалей с весами - здесь тень прямая и четкая, как и должно быть в реальности.

(Стоит заметить, что источник света находился на одной высоте относительно циллиндров)
