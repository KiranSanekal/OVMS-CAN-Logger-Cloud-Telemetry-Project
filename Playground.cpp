#include<iostream>

int main()
{
    int x=10;
    int &y=x;
    int*z=&x;
    std::cout<<y<<std::endl;
    std::cout<<z<<std::endl;
    std::cout<<*z<<std::endl;
}