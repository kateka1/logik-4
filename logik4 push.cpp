#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

struct Node {
	int data;
	struct Node* left;
	struct Node* right;
};

struct Node* root;

// 1. Стандартная функция создания дерева из методички (разрешает дубликаты)
struct Node* CreateTree(struct Node* root, struct Node* r, int data)
{
	if (r == NULL)
	{
		r = (struct Node*)malloc(sizeof(struct Node));
		if (r == NULL)
		{
			printf("Ошибка выделения памяти");
			exit(0);
		}

		r->left = NULL;
		r->right = NULL;
		r->data = data;

		if (root == NULL) return r;

		if (data > root->data) root->left = r;
		else root->right = r;
		return r;
	}

	if (data > r->data)
		CreateTree(r, r->left, data);
	else
		CreateTree(r, r->right, data);

	return root;
}

// Задание 3*: Модифицированная функция добавления БЕЗ дубликатов
struct Node* CreateTreeNoDuplicates(struct Node* root, struct Node* r, int data)
{
	if (r == NULL)
	{
		r = (struct Node*)malloc(sizeof(struct Node));
		if (r == NULL)
		{
			printf("Ошибка выделения памяти");
			exit(0);
		}

		r->left = NULL;
		r->right = NULL;
		r->data = data;

		if (root == NULL) return r;

		if (data > root->data) root->left = r;
		else root->right = r;
		return r;
	}

	// Проверка на дубликат: если такое число уже есть, ничего не добавляем
	if (data == r->data)
	{
		printf("  [!] Элемент %d уже существует в дереве и не был добавлен.\n", data);
		return root;
	}

	if (data > r->data)
		CreateTreeNoDuplicates(r, r->left, data);
	else
		CreateTreeNoDuplicates(r, r->right, data);

	return root;
}

// Задание 1: Функция поиска значения в дереве
// Возвращает указатель на найденный узел или NULL, если элемент не найден 676767
struct Node* SearchTree(struct Node* r, int key)
{
	if (r == NULL || r->data == key)
	{
		return r;
	}

	// Двигаемся по той же логике, по которой строилось дерево в CreateTree
	if (key > r->data)
		return SearchTree(r->left, key);
	else
		return SearchTree(r->right, key);
}

// Задание 2: Функция подсчёта числа вхождений заданного элемента в дерево
int CountOccurrences(struct Node* r, int key)
{
	if (r == NULL)
	{
		return 0;
	}

	int count = (r->data == key) ? 1 : 0;

	// В CreateTree при data == r->data элемент отправляется в правую ветку (else -> r->right).
	// Поэтому если key > r->data, элементы могут быть только слева.
	// А если key <= r->data (включая совпадения), продолжаем искать справа.
	if (key > r->data)
		return count + CountOccurrences(r->left, key);
	else
		return count + CountOccurrences(r->right, key);
}

// Функция вывода дерева на экран из методички
void print_tree(struct Node* r, int l)
{
	if (r == NULL)
	{
		return;
	}

	print_tree(r->right, l + 1);
	for (int i = 0; i < l; i++)
	{
		printf("   "); // Увеличено до 3 пробелов для более наглядного отображения уровней
	}

	printf("%d\n", r->data);
	print_tree(r->left, l + 1);
}

int main()
{
	setlocale(LC_ALL, "");
	int D, start = 1, mode = 1;
	int searchKey;

	root = NULL;

	printf("Выберите режим построения дерева:\n");
	printf("1 - Стандартный (разрешены одинаковые элементы — для проверки подсчёта вхождений)\n");
	printf("2 - Без дубликатов (Задание 3* — исключение одинаковых элементов)\n");
	printf("Ваш выбор: ");
	scanf_s("%d", &mode);

	printf("\n-1 - окончание построения дерева\n");
	while (start)
	{
		printf("Введите число: ");
		scanf_s("%d", &D);
		if (D == -1)
		{
			printf("Построение дерева окончено\n\n");
			start = 0;
		}
		else
		{
			if (mode == 2)
				root = CreateTreeNoDuplicates(root, root, D);
			else
				root = CreateTree(root, root, D);
		}
	}

	printf("Построенное дерево:\n");
	print_tree(root, 0);

	// Проверка Задания 1 и Задания 2
	printf("\n--- Поиск и подсчёт элементов ---\n");
	printf("Введите значение для поиска в дереве: ");
	scanf_s("%d", &searchKey);

	// 1. Поиск элемента
	struct Node* found = SearchTree(root, searchKey);
	if (found != NULL)
	{
		printf("Результат поиска: Элемент %d НАЙДЕН в дереве (адрес узла: %p).\n", found->data, (void*)found);
	}
	else
	{
		printf("Результат поиска: Элемент %d НЕ найден в дереве.\n", searchKey);
	}

	// 2. Подсчёт вхождений
	int count = CountOccurrences(root, searchKey);
	printf("Количество вхождений элемента %d в дерево: %d\n", searchKey, count);

	printf("\nВведите любое число для выхода: ");
	scanf_s("%d", &D);
	return 0;
}