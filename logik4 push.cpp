
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

struct Node* root;

// 1. Стандартное добавление (разрешены дубликаты)
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

// 2. Добавление без дубликатов (Задание 3*)
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

    if (data == r->data)
    {
        printf("Элемент %d уже существует и не был добавлен.\n", data);
        return root;
    }

    if (data > r->data)
        CreateTreeNoDuplicates(r, r->left, data);
    else
        CreateTreeNoDuplicates(r, r->right, data);

    return root;
}

// 3. Поиск элемента (Задание 1)
struct Node* SearchTree(struct Node* r, int key)
{
    if (r == NULL || r->data == key)
        return r;

    if (key > r->data)
        return SearchTree(r->left, key);
    else
        return SearchTree(r->right, key);
}

// 4. Подсчёт вхождений с выводом уровней (Задание 2)
int CountOccurrences(struct Node* r, int key, int level)
{
    if (r == NULL)
        return 0;

    int count = 0;

    if (r->data == key)
    {
        printf("Найден элемент %d на уровне %d\n", key, level);
        count = 1;
    }

    if (key > r->data)
        return count + CountOccurrences(r->left, key, level + 1);
    else
        return count + CountOccurrences(r->right, key, level + 1);
}

// Вертикальный вывод дерева с указанием уровней
int treeHeight(struct Node* r)
{
    if (r == NULL)
        return 0;

    int leftHeight = treeHeight(r->left);
    int rightHeight = treeHeight(r->right);

    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

int countNodes(struct Node* r)
{
    if (r == NULL)
        return 0;

    return 1 + countNodes(r->left) + countNodes(r->right);
}

int maxLabelWidth(struct Node* r, int level)
{
    if (r == NULL)
        return 0;

    char label[50];
    snprintf(label, sizeof(label), "%d[L%d]", r->data, level);

    int width = (int)strlen(label);

    int leftWidth = maxLabelWidth(r->left, level + 1);
    int rightWidth = maxLabelWidth(r->right, level + 1);

    if (leftWidth > width) width = leftWidth;
    if (rightWidth > width) width = rightWidth;

    return width;
}

void drawTree(struct Node* r, int level, int start,
    int cellWidth, char** canvas)
{
    if (r == NULL)
        return;

    int leftCount = countNodes(r->left);

    int index = start + leftCount;
    int x = index * cellWidth + cellWidth / 2;
    int y = level * 2;

    char label[50];
    snprintf(label, sizeof(label), "%d[L%d]", r->data, level);

    int len = (int)strlen(label);
    int pos = x - len / 2;

    for (int i = 0; i < len; i++)
        canvas[y][pos + i] = label[i];

    if (r->left != NULL)
    {
        int childIndex = start + countNodes(r->left->left);
        int childX = childIndex * cellWidth + cellWidth / 2;

        canvas[y + 1][(x + childX) / 2] = '/';

        drawTree(r->left, level + 1, start,
            cellWidth, canvas);
    }

    if (r->right != NULL)
    {
        int rightStart = index + 1;
        int childIndex = rightStart + countNodes(r->right->left);
        int childX = childIndex * cellWidth + cellWidth / 2;

        canvas[y + 1][(x + childX) / 2] = '\\';

        drawTree(r->right, level + 1, rightStart,
            cellWidth, canvas);
    }
}

// Вертикальный вывод бинарного дерева
void print_tree_vertical(struct Node* r)
{
    if (r == NULL)
    {
        printf("Дерево пустое.\n");
        return;
    }

    int height = treeHeight(r);
    int nodes = countNodes(r);

    int cellWidth = maxLabelWidth(r, 0) + 2;
    int width = nodes * cellWidth;
    int rows = height * 2 - 1;

    char** canvas = (char**)malloc(rows * sizeof(char*));

    if (canvas == NULL)
    {
        printf("Ошибка выделения памяти.\n");
        return;
    }

    for (int i = 0; i < rows; i++)
    {
        canvas[i] = (char*)malloc(width + 1);

        if (canvas[i] == NULL)
        {
            printf("Ошибка выделения памяти.\n");

            for (int j = 0; j < i; j++)
                free(canvas[j]);

            free(canvas);
            return;
        }

        for (int j = 0; j < width; j++)
            canvas[i][j] = ' ';

        canvas[i][width] = '\0';
    }

    drawTree(r, 0, 0, cellWidth, canvas);

    for (int i = 0; i < rows; i++)
    {
        int end = width - 1;

        while (end >= 0 && canvas[i][end] == ' ')
            end--;

        for (int j = 0; j <= end; j++)
            printf("%c", canvas[i][j]);

        printf("\n");
    }

    for (int i = 0; i < rows; i++)
        free(canvas[i]);

    free(canvas);
}
int main()
{
    setlocale(LC_ALL, "");

    int D, start = 1, mode = 1;
    int searchKey;

    root = NULL;

    printf("Выберите режим построения дерева:\n");
    printf("1 - Стандартный (разрешены дубликаты)\n");
    printf("2 - Без дубликатов\n");
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

    printf("\nПостроенное дерево:\n");
    print_tree_vertical(root);

    printf("\n--- Поиск и подсчёт элементов ---\n");
    printf("Введите значение для поиска: ");
    scanf_s("%d", &searchKey);

    struct Node* found = SearchTree(root, searchKey);

    if (found != NULL)
    {
        printf("\nРезультат поиска: элемент %d НАЙДЕН.\n",
            found->data);
    }
    else
    {
        printf("\nРезультат поиска: элемент %d НЕ найден.\n",
            searchKey);
    }

    printf("\nУровни расположения найденных элементов:\n");

    int count = CountOccurrences(root, searchKey, 0);

    if (count == 0)
    {
        printf("Совпадений не найдено.\n");
    }

    printf("\nКоличество вхождений элемента %d: %d\n",
        searchKey, count);

    printf("\nВведите любое число для выхода: ");
    scanf_s("%d", &D);

    return 0;
}
