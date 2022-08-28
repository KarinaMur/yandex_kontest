#include <iostream>
#include <string.h>
#include <stdio.h>


using namespace std;
//Структура для хранения желаемой рассадки группы пассажиров.
// "1" -обозначим занятые места, "0"-свободные.
//Данные хранятся в двоичном виде в переменной passenger_seats.
//полностью свободный ряд выглядит как 000-000.
//для запроса :"1 left window" ряд выглядит 100-000.
struct Group
{
    unsigned int passenger_seats=0;//рассадка пассажиров

//в конструкторе в зависимости от
// side  -    стороны(левой или правой) и
// position - положения(у окна или у прохода)
//вычисляем желаемую рассадку.
   Group(int pass_number,char *side,char *position)
   {
//кодируем положение(у окна или у прохода)
       unsigned int pass_pos;

       if(!strcmp(side,"left")) passenger_seats=0b111000;
       else  passenger_seats=0b000111;

       if(!strcmp(position,"window")) pass_pos=0b100001;
       else pass_pos=0b001100;

//если пассажир один,то
// побитово умножаем сторону(левую или правую)
// с его положением(у окна или у прохода),
       if(pass_number==1) passenger_seats&=pass_pos;
//если пассажиров двое-второй будет сидеть посередине(по условию должен сидеть рядом)
       if(pass_number==2) passenger_seats&=(pass_pos|0b010010);
//если пассажиров трое, то они займут три места подряд, рассадка не зависит от положения
    }

};

struct Row
{
    unsigned int seats_in_row=0;//переменная для записи свободных и занятых мест в 2чной системе
    static char board_places[6];//буквенное обозначение мест для вывода на экран

    void set_seats(char* seat)//метод для записи занятых мест 1, свободных-0.
    {
        for(int i=0;i<6;i++)
        {
            if(seat[5-i]=='#')//если место занято- 1.
               seats_in_row|=1<<i;
        }
    }

    void show_row()//печать занятых мест-"#" и свободных мест-"."
    {
        for(int i=0;i<6;i++)
        {
            if(i==3) cout<<"_";
            if((seats_in_row>>(5-i))&1) cout<<"#";
            else cout<<".";
        }
        cout<<endl;
    }

    void show_row_with_pass(unsigned int pass_pos)//печать занятых пассажирами мест-"X",занятых мест-"#" и свободных мест-"."
    {
        for(int i=0;i<6;i++)
        {
            if(i==3) cout<<"_";
            if((seats_in_row>>(5-i))&1)//если место занято
            {
                if((pass_pos>>5-i)&1) cout<<"X";//если занято пассажиром печатаем "X"
                else cout<<"#";
            }

            else cout<<".";
        }
        cout<<endl;
    }

    void print_boarding_places(unsigned int pass_pos,int num_of_row)//печать мест в буквенном формате
    {
        for(int i=0;i<6;i++)
        {
            if((pass_pos>>(5-i))&1)
                cout<<" "<<num_of_row+1<<board_places[i];

        }
        cout<<endl;
    }

    bool boarding(unsigned int pass_pos,int num_of_row)//проверка возможности посадки пассажиров
    {
        if((pass_pos&seats_in_row)==0)//если желаемые места свободны
        {
            seats_in_row|=pass_pos;//добавляем пассажиров в ряд
            cout<<"Passengers can take seats:";
            print_boarding_places(pass_pos,num_of_row);//распечатываем их места
            return true;
        }
        else return false;
    }
};
char Row::board_places[6]={'A','B','C','D','E','F'};


int main()
{

    int n;//количество рядов
    cin>>n;
    getchar();

    Row* rows=new Row[n];//обЪявдяем массив рядов размера n.
    char seat[7];//массив символов для мест в ряду

    for(int i=0; i<n; i++)//в цикле считываем информацию по каждому ряду
    {
        scanf("%c%c%c%*c%c%c%c",&seat[0],&seat[1],&seat[2],&seat[3],&seat[4],&seat[5]);
        seat[6]='\0';
        rows[i].set_seats(seat);//метод для обозначения занятых мест 1, свободных-0.
        getchar();
    }

     int m;//количество групп пассажиров
     cin>>m;
     getchar();



     int pass_number;//количество пассажиров в группе
     char side[10];
     char position[10];

     for(int i=0;i<m;i++)//считываем информацию по группам пассажиров
     {
         cin>>pass_number>>side>>position;
         Group pass_group(pass_number,side,position);//инициализируем группу входящими параметрами

         int j=0;//переменная для хранения номера ряда посадки
         while(j<n&&(!rows[j].boarding(pass_group.pass_side,j)))//пока не закончатся ряды и пока не посадим пассажиров
             {
                 j++;//переходим к следующему ряду
                 if(j==n) cout<<"Cannot fulfill passengers requirements"<<endl;
             }
        if(j!=n){
                 for(int row=0;row<n;row++)
                  {
                      if(row==j) rows[row].show_row_with_pass(pass_group.pass_side);
                      else rows[row].show_row();
                   }
                }

      }



   delete[]rows;


   return 0;
}


