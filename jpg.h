#ifndef JPG_H
#define JPG_H
#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cstdio>
#include <tree.h>
#include <utility>
#include <fstream>
#include <string.h>
#include <math.h>
#include <list>
#include <vector>

//Основной класс, хранящий информацию об изображении,
//поля заголовков и прочее.
//Методы отсутствуют, кроме перегруженного конструктора

using namespace std;

//FFFE - структура, хранящая комментарии JPG файла.
typedef struct{
    //определяет, есть ли в изображении коментарий. если = 0, значит нет
    //если не 0, то это длина последнего комментария(может быть несколько)
    unsigned int jpgCommentLenght=0;
    string jpgCommentContent[16]; //массив комментариев
} JPGCOMMENT;

//FFDB - структура хранящая DQT — таблицы квантования
typedef struct
{
    unsigned int jpgDQTLenght[16];//длина i-го заголовка
    unsigned int jpgDQTLVT[16];//длина значений в таблице (0 - 1 байт, 1 - 2 байта)
    unsigned int jpgDQTmaxTableID=0;//количество таблиц или номер последней таблицы в массиве таблиц квантования
    unsigned int jpgDQTtable[16][8][8];//таблицы квантования

} JPGDQT;

//Структура компонент из маркера базового метода кодирования
typedef struct
{
    unsigned int jpgCompID=0;//номер компоненты
    unsigned int jpgHorizontalDecimation=0;//горизонтальное прореживание
    unsigned int jpgVerticalDecimation=0;//вертикальное прореживание
    unsigned int jpgQuantizationTableID=0;//идентификатор таблицы квантования
} JPGCOMPONENT;

//SOF0 - маркер базового метода кодирования
typedef struct
{
    unsigned int jpgSOF0Lenght=0;//длина маркера
    unsigned int jpgSOF0Precision=0;//разрядность значений каналов
    unsigned int jpgSOF0Height=0;//высота изображения
    unsigned int jpgSOF0Width=0;//ширина изображения
    unsigned int jpgSOF0WComponentValue=0;//количество компонент (Обычно 3: Y, Cb, Cr)
    JPGCOMPONENT jpgSOF0WComponents[16];//матрица стуктур компонент

} JPGSOF0;

//FFC4 - таблица Хаффмана
typedef struct
{
    unsigned int jpgDHTLenght=0;//длина маркера
    unsigned int jpgTableType=0;//класс таблицы(0- таблица DC коэффициентов, 1 - AC)
    unsigned int jpgTableID=0;//идентификатор таблицы
    unsigned int jpgTableSize=0;//размер массива, содержащего конечные узлы дерева Хаффмана
    pair<int,string> *jpgDHTtable;//массив, содержащий конечные узлы дерева Хаффмана
    node *jpgDHTtreesRoots;//указатель на корень дерева Хаффмана

} JPGDHT;

typedef struct
{
    JPGDHT  *jpgDHTtables[4];//массив деревьев Хаффмана
} JPGDHTALL;

//FFDA -  маркер секции закодированного изображения
typedef struct
{
    unsigned int jpgSosLenght=0;//длина маркера
    unsigned int jpgSosComponentValue=0;//количество компонент сканирования(озбычно 3: Y, Cb, Cr)
    pair<int,int> *jpgComponentsTable;//контейнер хренения идентификаторово таблиц Хаффмана

    unsigned int nonameBite1=0;
    unsigned int nonameBite2=0;
    unsigned int nonameBite3=0;

} JPGSOS;

//структура хранения отдельных блоков изображения 16х16 пикселей
typedef struct
{
    int jpgYtable[4][8][8];//4 таблицы Y канала
    int jpgCb[8][8];//таблица Cb канала
    int jpgCr[8][8];//таблица Cr канала

    int jpgRtable[8][8];//таблица Red канала
    int jpgGtable[8][8];//таблица Green канала
    int jpgBtable[8][8];//таблица Blue канала


} JPGALLPICTABLES;

//структура хранения всего изображения
typedef struct
{
    list<char> jpgPicture;//изображение в виде набора символов
    vector<JPGALLPICTABLES*> jpgAllPicTables;//изображение в виде массива блоков 16х16 пикселей

} JPGPICTURE;


//класс изображения,кранящий все вышеперечисленные структуры
class JPG
{
public:
    JPG();
    JPG(const char *pFile);
    FILE * pFile;
    JPGCOMMENT comMark;
    JPGDQT dqtMark;
    JPGSOF0 sof0Mark;
    JPGDHTALL dhtAllMark;
    JPGSOS sosMark;
    JPGPICTURE picMark;
    ~JPG();
};

#endif // JPG_H
